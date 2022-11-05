/**
 * An implementation of the base visitor class for analyzing an AST.
 *
 * @author hockeyhurd
 * @version 2022-08-31
 */

// Our includes
#include <cmm/visit/Analyzer.h>
#include <cmm/NodeList.h>
#include <cmm/Reporter.h>

// std includes
#include <limits>

namespace cmm
{
    /* static */
    Reporter& Analyzer::reporter = Reporter::instance();

    template<class T>
    static bool inRange(const T value)
    {
        return std::numeric_limits<T>::lowest() <= value && value <= std::numeric_limits<T>::max();
    }

    Analyzer::Analyzer() CMM_NOEXCEPT
    {
        localityStack.push(EnumLocality::GLOBAL);
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
    VisitorResult Analyzer::visit(AddressOfNode& node)
    {
        auto* variablePtr = node.getExpression();

        if (variablePtr == nullptr)
        {
            reporter.bug("variablePtr is a nullptr", node.getLocation(), true);
        }

        else if (variablePtr->getType() != EnumNodeType::VARIABLE)
        {
            const char* message = "Expected a variable expression prior to attempting to take the address of it";
            reporter.error(message, node.getLocation());
            return VisitorResult();
        }

        variablePtr->accept(this);

        return VisitorResult();
    }
#pragma GCC diagnostic pop

    VisitorResult Analyzer::visit(ArgNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(BinOpNode& node)
    {
        // TODO: How should we handle the case of: "x = y = 42;"??
        // Ideally, we would like to treat the sub/right-expression as "normal",
        // then re-write the sub/right expression as just another "RVALUE".
        // I guess we'll just need to re-visit this when we get to code generation.

        auto* leftNode = node.getLeft();

        [[maybe_unused]]
        auto leftNodeResult = leftNode->accept(this);
        const bool isAssignment = node.getTypeof() == EnumBinOpNodeType::ASSIGNMENT;

        if (isAssignment && leftNode->getType() != EnumNodeType::VARIABLE)
        {
            reporter.error("Expression is not assignable", leftNode->getLocation());

            return VisitorResult();
        }

        // Establish the Node's datatype by it's left node.
        const auto& leftType = leftNode->getDatatype();
        node.setDatatype(leftType);

        auto* rightNode = node.getRight();

        [[maybe_unused]]
        auto rightNodeResult = rightNode->accept(this);

        const auto& rightType = rightNode->getDatatype();

        if (leftType != rightType)
        {
            // Note: canPromote(fromType, toType)
            // if assignment the rightType must be able to promote to the variable,
            // otherwise we can try both options ex. 1+2.0F fails promotion,
            // when it's valid promotion.
            std::optional<CType> optCastType;
            bool castRight = false;

            if (isAssignment)
            {
                optCastType = canPromote(rightType, leftType);
                castRight = true;
            }

            else
            {
                // Try left to right first (ex. 1+2.0F).
                optCastType = canPromote(leftType, rightType);
                castRight = false;

                // Failed, try the reverse instead (ex. 1.0F+2).
                if (!optCastType.has_value())
                {
                    optCastType = canPromote(rightType, leftType);
                    castRight = true;
                }
            }

            // Promote warn
            if (optCastType.has_value())
            {
                std::ostringstream builder;
                builder << "Type mismatch between '";
                printType(builder, leftType);
                builder << "' and '";
                printType(builder, rightType);
                builder << "', but is promotable to ";
                printType(builder, *optCastType);
                reporter.warn(builder.str(), node.getLocation());

                if (castRight)
                {
                    node.castRight(*optCastType);
                }

                else
                {
                    node.castLeft(*optCastType);
                }
            }

            // See if it's void pointer magic
            else if ((leftType.type == EnumCType::VOID && leftType.pointers > 0 && rightType.pointers > 0) ||
                     (rightType.type == EnumCType::VOID && rightType.pointers > 0 && leftType.pointers > 0))
            {
                // Do nothing, assign anything to void* that is a pointer itself, is apparently valid...
                node.castRight(leftType);
            }

            // Type mismatch, but both are non-void pointers -> warning
            else if (isAssignment && leftType.pointers > 0 && rightType.pointers > 0)
            {
                std::ostringstream builder;
                builder << "Base type mismatch between '";
                printType(builder, leftType);
                builder << "' and '";
                printType(builder, rightType);
                builder << "', but is pointer compatible";
                reporter.warn(builder.str(), node.getLocation());

                node.castRight(leftType);
            }

            // "Catch all" -> error
            else
            {
                std::ostringstream builder;
                builder << "Type mismatch between '";
                printType(builder, leftType);
                builder << "' and '";
                printType(builder, rightType);
                builder << '\'';
                reporter.error(builder.str(), node.getLocation());
            }
        }

        else if (leftType.pointers > 0 && rightType.pointers > 0)
        {
            std::ostringstream builder;
            builder << "Invalid operands to binary expression between '";
            printType(builder, leftType);
            builder << "' and '";
            printType(builder, rightType);
            builder << "'";

            reporter.error(builder.str(), node.getLocation());
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(BlockNode& node)
    {
        scope.push(true);

        for (auto& statementPtr : node)
        {
            statementPtr->accept(this);
        }

        scope.pop();

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(CastNode& node)
    {
        if (!node.hasExpression())
        {
            reporter.bug("Expected expression after cast", node.getLocation(), true);
            return VisitorResult();
        }

        auto* expression = node.getExpression();
        expression->accept(this);

        const auto& from = node.getDatatype();
        const auto& to = expression->getDatatype();

        if (from.pointers == 0 && from.pointers == to.pointers)
        {
            if (canPromote(from, to))
            {
                // Note: Intentionally do nothing (no need to warn).
            }

            else if (canTruncate(from, to))
            {
                // TODO: Consider conditionally reporting this, such as if the user
                // passes a '-Wall' like flag.
                std::ostringstream builder;
                builder << "Attempting to downcast types from '";
                printType(builder, from);
                builder << "' to '";
                printType(builder, to);
                builder << '\'';

                reporter.warn(builder.str(), node.getLocation());
            }
        }

        else if (from.pointers == to.pointers)
        {
            // TODO: Consider conditionally reporting this, such as if the user
            // passes a '-Wall' like flag.
            std::ostringstream builder;
            builder << "Attempting to pointer cast from '";
            printType(builder, from);
            builder << "' to '";
            printType(builder, to);
            builder << '\'';

            reporter.warn(builder.str(), node.getLocation());
        }

        else
        {
            std::ostringstream builder;
            builder << "Cannot cast expression from '";
            printType(builder, from);
            builder << "' to '";
            printType(builder, to);
            builder << "' where arithmetic or pointer is expected";

            reporter.error(builder.str(), node.getLocation());
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(CompilationUnitNode& node)
    {
        auto result = node.getRoot().accept(this);

        return result;
    }

    VisitorResult Analyzer::visit(DerefNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(FunctionCallNode& node)
    {
        const auto& funcName = node.getName();
        const auto findResult = functionTable.find(funcName);

        if (findResult == functionTable.cend())
        {
            std::ostringstream builder;
            builder << "Could not find a declaration or definition for function '"
                    << funcName << "'";
            reporter.error(builder.str(), node.getLocation());
        }

        for (auto& func : node)
        {
            func.accept(this);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(FunctionDeclarationStatementNode& node)
    {
        if (localityStack.top() != EnumLocality::GLOBAL)
        {
            std::ostringstream builder;
            builder << "Function '" << node.getName() << "' is being declared outside of global scope. Function declarations must exist in global scope only.";
            reporter.error(builder.str(), node.getLocation());
            return VisitorResult();
        }

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        const auto& funcName = node.getName();

        if (!validateFunction(funcName, EnumSymState::DECLARED))
        {
            const auto previousState = functionTable[funcName];
            std::ostringstream builder;

            if (previousState == EnumSymState::DECLARED)
            {
                builder << "Function '" << funcName << "' was already previously declared and does not need to be declared here as well.";
                reporter.warn(builder.str(), node.getLocation());
            }

            else if (previousState == EnumSymState::DEFINED)
            {
                builder << "Function '" << funcName << "' was already previously defined and does not need to be declared here as well.";
                reporter.warn(builder.str(), node.getLocation());
            }

            else
            {
                reporter.bug("Unknown EnumSymState will not be handled.  Is this an internal bug? (Analyzer::visit(FunctionDeclarationStatementNode&))", node.getLocation(), false);
            }
        }

        // If in global scope, we must make sure there is NOT a variable with the same name (i.e. redefinition error).
        else if (scope.findVariable(node.getName()) != nullptr)
        {
            std::ostringstream builder;
            builder << "Function '" << node.getName() << "' matches a variable definition";
            reporter.error(builder.str(), node.getLocation());
        }

        else
        {
            functionTable[funcName] = EnumSymState::DECLARED;
        }

        for (auto& paramNode : node)
        {
            paramNode.accept(this);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(FunctionDefinitionStatementNode& node)
    {
        if (localityStack.top() != EnumLocality::GLOBAL)
        {
            std::ostringstream builder;
            builder << "Function '" << node.getName() << "' is being declared outside of global scope. Function definitions must exist in global scope only.";
            reporter.error(builder.str(), node.getLocation());
            return VisitorResult();
        }

        scope.push(true);

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        const auto& funcName = node.getName();

        if (!validateFunction(funcName, EnumSymState::DECLARED))
        {
            const auto previousState = functionTable[funcName];
            std::ostringstream builder;

            if (previousState == EnumSymState::DEFINED)
            {
                builder << "Function '" << funcName << "' was already previously defined and does not need to be defined here as well (multiple definitions).";
                reporter.error(builder.str(), node.getLocation());
            }

            else
            {
                reporter.bug("Unknown EnumSymState will not be handled.  Is this an internal bug? (Analyzer::visit(FunctionDefinitionStatementNode&))", node.getLocation(), false);
            }
        }

        else
        {
            functionTable[funcName] = EnumSymState::DEFINED;
        }

        localityStack.push(EnumLocality::PARAMETER);

        for (auto& paramNode : node)
        {
            paramNode.accept(this);
        }

        localityStack.pop();
        localityStack.push(EnumLocality::LOCAL);

        auto& blockNode = node.getBlock();
        blockNode.accept(this);

        localityStack.pop();
        scope.pop();

        auto* returnStatementPtr = node.getReturnStatement();

        // Cases to check:
        // 1. void with optional 'return;' statement. Error if return statement exists and is non-void.
        // 2. non-void with optional return statement -> warning
        // 3. non-void with return statement that does not match or is not promotable or is not truncatable -> warning.

        auto& datatype = node.getDatatype();

        if (datatype.type != EnumCType::VOID)
        {
            // Case #2
            if (returnStatementPtr == nullptr || !returnStatementPtr->hasExpression())
            {
                std::ostringstream builder;
                builder << "Missing return statement in non-void function '" << node.getName() << "'";
                reporter.warn(builder.str(), blockNode.getLocation());
            }

            else if (*returnStatementPtr->getDatatype() != datatype)
            {
                const auto* returnType = returnStatementPtr->getDatatype();
                auto optCastType = canPromote(*returnType, datatype);

                // Case #3 promotable
                if (optCastType.has_value())
                {
                    const char* toTypeStr = toString(node.getDatatype().type);
                    std::ostringstream builder;
                    builder << "Return type mismatch. Expected '" << toTypeStr
                            << "', but found '" << toString(returnType->type)
                            << "'. This will be promoted to '" << toTypeStr << '\'';
                    reporter.warn(builder.str(), returnStatementPtr->getLocation());

                    returnStatementPtr->cast(*optCastType);
                }

                else if ((optCastType = canTruncate(*returnType, node.getDatatype())).has_value())
                {
                    const char* toTypeStr = toString(node.getDatatype().type);
                    std::ostringstream builder;
                    builder << "Return type mismatch. Expected '" << toTypeStr
                            << "', but found '" << toString(returnType->type)
                            << "'. This will be truncated to '" << toTypeStr << '\'';
                    reporter.warn(builder.str(), returnStatementPtr->getLocation());

                    // TODO: Can't be tested until CastNodes are added to the parser.
                    returnStatementPtr->cast(*optCastType);
                }

                // Case #3 not promotable
                else
                {
                    const char* toTypeStr = toString(node.getDatatype().type);
                    std::ostringstream builder;
                    builder << "Return type mismatch. Expected '" << toTypeStr
                            << "', but found '" << toString(returnType->type);
                    reporter.error(builder.str(), returnStatementPtr->getLocation());
                }
            }
        }

        // void function returns a non-void value case #1.
        else if (returnStatementPtr != nullptr && returnStatementPtr->getDatatype()->type != EnumCType::VOID)
        {
            std::ostringstream builder;
            builder << "Function '" << node.getName() << "' should not return a non-void value";
            reporter.error(builder.str(), returnStatementPtr->getLocation());
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(ExpressionStatementNode& node)
    {
        node.getExpression()->accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(IfElseStatementNode& node)
    {
        auto* ifCondExpression = node.getIfConditional();
        ifCondExpression->accept(this);

        auto* ifStatement = node.getIfStatement();
        ifStatement->accept(this);

        auto* elseStatement = node.getElseStatement();

        if (elseStatement != nullptr)
        {
            ifStatement->accept(this);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(LitteralNode& node)
    {
        switch (node.getDatatype().type)
        {
        case EnumCType::NULL_T:
            break;
        case EnumCType::VOID:
            break;
        case EnumCType::VOID_PTR:
            break;
        case EnumCType::STRING:
            break;
        case EnumCType::BOOL:
            break;
        case EnumCType::CHAR:
        {
            if (!inRange(node.getValue().valueChar))
            {
                std::ostringstream os;
                os << "value '" << static_cast<int>(node.getValue().valueChar)
                   << "'" << " is outside of the expected range for a char."; 
                reporter.warn(os.str(), node.getLocation());
            }
        }
            break;
        case EnumCType::INT8:
        {
            if (!inRange(node.getValue().valueS8))
            {
                std::ostringstream os;
                os << "value '" << static_cast<int>(node.getValue().valueS8)
                   << "'" << " is outside of the expected range for a 8-bit signed int."; 
                reporter.warn(os.str(), node.getLocation());
            }
        }
            break;
        case EnumCType::INT16:
        {
            if (!inRange(node.getValue().valueS16))
            {
                std::ostringstream os;
                os << "value '" << static_cast<int>(node.getValue().valueS16)
                   << "'" << " is outside of the expected range for a 16-bit signed int."; 
                reporter.warn(os.str(), node.getLocation());
            }
        }
            break;
        case EnumCType::INT32:
        {
            if (!inRange(node.getValue().valueS32))
            {
                std::ostringstream os;
                os << "value '" << node.getValue().valueS32
                   << "'" << " is outside of the expected range for a 32-bit signed int."; 
                reporter.warn(os.str(), node.getLocation());
            }
        }
            break;
        case EnumCType::INT64:
        {
            if (!inRange(node.getValue().valueS64))
            {
                std::ostringstream os;
                os << "value '" << node.getValue().valueS64
                   << "'" << " is outside of the expected range for a 64-bit signed int."; 
                reporter.warn(os.str(), node.getLocation());
            }
        }
            break;
        case EnumCType::FLOAT:
        {
            if (!inRange(node.getValue().valueF32))
            {
                std::ostringstream os;
                os << "value '" << node.getValue().valueF32
                   << "'" << " is outside of the expected range for a 32-bit float."; 
                reporter.warn(os.str(), node.getLocation());
            }
        }
            break;
        case EnumCType::DOUBLE:
        {
            if (!inRange(node.getValue().valueF64))
            {
                std::ostringstream os;
                os << "value '" << node.getValue().valueF64
                   << "'" << " is outside of the expected range for a 64-bit double."; 
                reporter.warn(os.str(), node.getLocation());
            }
        }
            break;
        case EnumCType::STRUCT:
            break;
        default:
            break;
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(ParameterNode& node)
    {
        auto& typeNode = node.getDatatype();
        typeNode.accept(this);

        auto& optionalVariableNode = node.getVariable();

        if (optionalVariableNode.has_value())
        {
            const auto& name = optionalVariableNode->getName();
            const auto currentLocality = localityStack.top();
            VariableContext context(typeNode.getDatatype(), currentLocality, EnumModifier::NO_MOD);
            auto* findVariable = scope.findVariable(name);

            if (findVariable != nullptr)
            {
                std::ostringstream builder;
                builder << "Variable '" << name << "' of type ";
                printType(builder, typeNode.getDatatype());
                builder << " was already used as a parameter";

                reporter.error(builder.str(), node.getLocation());
            }

            else
            {
                scope.add(name, context);
                optionalVariableNode->accept(this);
            }
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(ParenExpressionNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(ReturnStatementNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(StructDefinitionStatementNode& node)
    {
        const auto currentLocality = localityStack.top();
        // TODO: support static?
        const auto modifier = EnumModifier::NO_MOD;
        const StructOrUnionContext context(currentLocality, modifier);
        const auto& structName = node.getName();
        const auto optStructState = structTable.get(structName);

        if (optStructState.has_value() && *optStructState == EnumSymState::DEFINED)
        {
            std::ostringstream builder;
            builder << "struct " << structName << " is already previously defined. This violates the multiple definition rule";
            reporter.error(builder.str(), node.getLocation());
        }

        else if (!optStructState.has_value())
        {
            structTable.addOrUpdate(structName, EnumSymState::DEFINED);
            scope.add(structName, context);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(StructFwdDeclarationStatementNode& node)
    {
        const auto currentLocality = localityStack.top();
        // TODO: support static?
        const auto modifier = EnumModifier::NO_MOD;
        const StructOrUnionContext context(currentLocality, modifier);
        const auto& structName = node.getName();
        const auto optStructState = structTable.get(structName);

        if (optStructState.has_value())
        {
            std::ostringstream builder;
            builder << "struct " << structName << " is already previously declared or defined";
            reporter.warn(builder.str(), node.getLocation());
        }

        else
        {
            structTable.addOrUpdate(structName, EnumSymState::DECLARED);
            scope.add(structName, context);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(TranslationUnitNode& node)
    {
        for (auto& statement : node)
        {
            if (statement != nullptr)
            {
                statement->accept(this);
            }

            else
            {
                static const char* errorMessage = "Un-expected statement with a nullptr.";
                reporter.bug(errorMessage, node.getLocation(), true);
            }
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(TypeNode& node)
    {
        // Note: Parser should have verified the type, however with some structs
        // or typedefs, it may not have been able to fully verify and deferred to here.
        // TODO: Update this logic once we get there.
        [[maybe_unused]]
        const auto& datatype = node.getDatatype();

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(UnaryOpNode& node)
    {
        if (node.hasExpression())
        {
            auto* expression = node.getExpression();
            expression->accept(this);

            // if (node.getOpType() == EnumUnaryOpType::ADDRESS_OF && expression->getType() != EnumNodeType::VARIABLE)
            if (node.getOpType() == EnumUnaryOpType::ADDRESS_OF)
            {
                if (expression->getType() != EnumNodeType::VARIABLE)
                {
                    const char* message = "Expected a variable expression prior to attempting to take the address of it";
                    reporter.error(message, node.getLocation());
                }

                else
                {
                    CType newType = expression->getDatatype();
                    ++newType.pointers;
                    node.setDatatype(newType);
                }
            }
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(VariableNode& node)
    {
        const auto& varName = node.getName();
        const auto* varContext = scope.findAnyVariable(varName);

        if (varContext == nullptr)
        {
            std::ostringstream builder;
            builder << "Failed to find variable '" << varName << "' in current context.";
            reporter.error(builder.str(), node.getLocation());
            return VisitorResult();
        }

        node.setDatatype(varContext->getType());

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(VariableDeclarationStatementNode& node)
    {
        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        auto currentLocality = localityStack.top();
        VariableContext context(node.getDatatype(), currentLocality, EnumModifier::NO_MOD);

        // Before we add it to the current scope, we should check if this declaration would be a duplicate
        // and conditionally report this case.  Note: This condition must strictly be in the current frame
        // of reference and not a parent, since this would be allowed.
        // Also, if we are in global scope, we must check this isn't a function too.

        auto* lookupContext = scope.findVariable(node.getName());

        if (lookupContext != nullptr)
        {
            std::ostringstream builder;
            builder << "Redefinition of variable '" << node.getName() << '\'';
            reporter.error(builder.str(), node.getLocation());
        }

        else if (currentLocality == EnumLocality::GLOBAL && functionTable.find(node.getName()) != functionTable.cend())
        {
            std::ostringstream builder;
            builder << "Redefinition of variable '" << node.getName() << "\' from a function to a variable";
            reporter.error(builder.str(), node.getLocation());
        }

        else
        {
            scope.add(node.getName(), context);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(WhileStatementNode& node)
    {
        auto* conditional = node.getConditional();
        conditional->accept(this);

        auto* statement = node.getStatement();
        statement->accept(this);

        return VisitorResult();
    }

    bool Analyzer::validateFunction(const std::string& name, const EnumSymState state)
    {
        const auto findResult = functionTable.find(name);

        if (findResult == functionTable.cend())
        {
            return true;
        }

        const auto currentState = findResult->second;
        const bool invResult = currentState == state || (currentState == EnumSymState::DEFINED && state == EnumSymState::DECLARED);
        return !invResult;
    }
}

