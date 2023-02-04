/**
 * An implementation of the base visitor class for analyzing an AST.
 *
 * @author hockeyhurd
 * @version 2022-08-31
 */

// Our includes
#include <cmm/visit/Analyzer.h>
#include <cmm/EnumTable.h>
#include <cmm/NodeList.h>
#include <cmm/Reporter.h>
#include <cmm/StructTable.h>

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

    Analyzer::Analyzer() CMM_NOEXCEPT : structTable(nullptr)
    {
        localityStack.push(EnumLocality::GLOBAL);
    }

    VisitorResult Analyzer::visit(ArgNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(BinOpNode& node)
    {
        auto* rightNode = node.getRight();

        [[maybe_unused]]
        auto rightNodeResult = rightNode->accept(this);
        const auto& rightType = rightNode->getDatatype();

        // If the right node is a variable or a variable being dereferenced (i.e. a DerefNode),
        // we need to add a (potentially second) DerefNode to wrap it.
        if (isValidNonLitteralRHSNodeType(rightNode->getType()))
        {
            // Add DerefNode
            node.derefNodeRight();

            // Update our pointer to this new pointer.
            // Note: Commenting out so cppcheck doesn't complain, however we may want this as a reminder
            //       in the future.
            // rightNode = node.getRight();
        }

        auto* leftNode = node.getLeft();

        [[maybe_unused]]
        auto leftNodeResult = leftNode->accept(this);
        const bool isAssignment = node.getTypeof() == EnumBinOpNodeType::ASSIGNMENT;
        const bool isLeftVariable = leftNode->getType() == EnumNodeType::VARIABLE;
        const bool isLeftDerefNode = leftNode->getType() == EnumNodeType::DEREF;
        const bool isLeftFieldAccess = leftNode->getType() == EnumNodeType::FIELD_ACCESS;

        if (isAssignment && !isLeftVariable && !isLeftDerefNode && !isLeftFieldAccess)
        {
            reporter.error("Expression is not assignable", leftNode->getLocation());

            return VisitorResult();
        }

        // Establish the Node's datatype by it's left node.
        const auto& leftType = leftNode->getDatatype();
        node.setDatatype(leftType);

        VariableNode* varNode = nullptr;

        if (isAssignment && isLeftVariable)
        {
            varNode = static_cast<VariableNode*>(leftNode);
            auto* varContext = scope.findAnyVariable(varNode->getName());
            varContext->setDirtyBit(true);

            const EnumModifier modifiers = varContext->getModifiers();

            u16 asU16;

            if (varContext->getCType().isPointerType())
            {
                asU16 = static_cast<u16>(EnumModifier::CONST_POINTER);
            }

            else
            {
                asU16 = static_cast<u16>(EnumModifier::CONST_VALUE);
            }

            // Mask out EnumModifier::CONST_POINTER or EnumModifier::CONST_VALUE
            asU16 = ~asU16;
            varContext->setModifiers(static_cast<EnumModifier>(modifiers & asU16));
        }

        else if (isAssignment && isLeftDerefNode)
        {
            // Pop off 1-level of a DerefNode??
            node.popDerefNodeLeft();

            // Update our pointer to this new pointer.
            // Note: Commenting out so cppcheck doesn't complain, however we may want this as a reminder
            //       in the future.
            // leftNode = node.getLeft();
        }

        else if (isAssignment && isLeftFieldAccess)
        {
            // For now, NOOP?
            ;
        }

        // If the left node is a variable and this is NOT an assignment operation,
        // we need to add a DerefNode to wrap it.
        else if (!isAssignment && (isLeftVariable || isLeftFieldAccess))
        {
            // Add DerefNode
            node.derefNodeLeft();

            // Update our pointer to this new pointer.
            // Note: Commenting out so cppcheck doesn't complain, however we may want this as a reminder
            //       in the future.
            // leftNode = node.getLeft();
        }

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

        else if (!isAssignment && leftType.pointers > 0 && rightType.pointers > 0)
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

        // If it is a VariableNode, we need to add a DerefNode in front of it.
        if (expression->getType() == EnumNodeType::VARIABLE)
        {
            node.derefNode();

            // Update our expression pointer for next use below.
            expression = node.getExpression();
        }

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
        // TODO: Someday support multiple TranslationUnitNodes.
        auto result = node.getRoot().accept(this);

        // NULL the structTable pointer to invalidate it.
        structTable = nullptr;

        return result;
    }

    VisitorResult Analyzer::visit(DerefNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        if (!isValidNonLitteralRHSNodeType(expression->getType()))
        {
            reporter.error("Expected a variable or dereference or field access node", node.getLocation());
            return VisitorResult();
        }

        node.resolveDatatype();

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(FieldAccessNode& node)
    {
        auto* expressionNodePtr = node.getExpression();
        const EnumNodeType expressionType = expressionNodePtr->getType();

        if (!isValidNonLitteralRHSNodeType(expressionType))
        {
            reporter.error("Expected a variable or dereference or another field accesss node", node.getLocation());
            return VisitorResult();
        }

        // Visit the sub-expression (typical the struct variable) first to make sure types are fully resolved.
        expressionNodePtr->accept(this);

        // Get the expression's datatype so that we can then try and validate our field.
        const CType& datatype = expressionNodePtr->getDatatype();

        if (datatype.type != EnumCType::STRUCT)
        {
            reporter.error("Expected an expression of type struct", node.getLocation());
            return VisitorResult();
        }

        else if (!datatype.optTypeName.has_value())
        {
            reporter.bug("Missing struct name for the accessing expression. Must be a compiler bug??", node.getLocation(), true);
            return VisitorResult();
        }

        // Check whether DOT or ARROW usage makes sense.
        const auto& fieldName = node.getName();
        const EnumFieldAccessType accessType = node.getFieldAccessType();

        if (datatype.isPointerType() && accessType != EnumFieldAccessType::ARROW)
        {
            std::ostringstream builder;
            builder << "Expression is a pointer type. Expected a dereference or '"
                    << toString(EnumFieldAccessType::ARROW) << "' before accessing field '"
                    << fieldName << "'";

            reporter.error(builder.str(), node.getLocation());
            return VisitorResult();
        }

        else if (!datatype.isPointerType() && accessType != EnumFieldAccessType::DOT)
        {
            std::ostringstream builder;
            builder << "Expression is not a pointer type. Expected a '"
                    << toString(EnumFieldAccessType::DOT) << "' when accessing field '"
                    << fieldName << "'";

            reporter.error(builder.str(), node.getLocation());
            return VisitorResult();
        }

        // Check that the field to be used is a part of the struct.
        // Note: We already checked that optTypeName has a value, so this is safe to access.
        const auto& structName = *datatype.optTypeName;
        const StructData* structData = structTable->get(structName);

        // Check to see that we found the struct and we have access to the definition so that
        // we can then verify the field is inside of the struct.
        if (structData == nullptr || structData->symState != EnumSymState::DEFINED)
        {
            std::ostringstream builder;
            builder << "Could not find struct '" << structName << "'. Make sure this struct is fully defined.";

            reporter.error(builder.str(), node.getLocation());
            return VisitorResult();
        }

        const IField* fieldLookupResult = structData->findField(fieldName);

        // Verify the field is in the struct.
        if (fieldLookupResult == nullptr)
        {
            std::ostringstream builder;
            builder << "Failed to find field '" << fieldName << "' in struct '"
                    << structName << "'";

            reporter.error(builder.str(), node.getLocation());
            return VisitorResult();
        }

        // Sync the StructTable's version of the Field to this node.
        node.set(fieldLookupResult);

        // Update the FieldAccessNode's datatype with the datatype of the field post-lookup.
        const CType& fieldDatatype = fieldLookupResult->getDatatype();
        node.setDatatype(fieldDatatype);

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
            return VisitorResult();
        }

        const auto& stateTypePair = findResult->second;
        const auto& datatype = stateTypePair.second;
        node.setDatatype(datatype);

        for (auto& arg : node)
        {
            arg.accept(this);
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
            const auto& stateTypePair = functionTable[funcName];
            const auto& previousState = stateTypePair.first;
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

        // Not defined
        else
        {
            // functionTable[funcName] = std::make_pair<EnumSymState, CType>(EnumSymState::DECLARED, typeNode.getDatatype());
            functionTable[funcName] = std::make_pair(EnumSymState::DECLARED, typeNode.getDatatype());
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
            const auto& stateTypePair = functionTable[funcName];
            const auto& previousState = stateTypePair.first;
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
            // functionTable[funcName] = std::make_pair<EnumSymState, CType>(EnumSymState::DEFINED, typeNode.getDatatype());
            functionTable[funcName] = std::make_pair(EnumSymState::DEFINED, typeNode.getDatatype());
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
                reporter.warn(builder.str(), blockNode.getEndLocation());
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

        // Is a 'void' function without a return statement.  We will add one implicitly to help with code generation.
        else if (returnStatementPtr == nullptr && datatype.type == EnumCType::VOID)
        {
            const auto& endLocation = node.getBlock().getEndLocation();
            auto returnStatementNode = std::make_unique<ReturnStatementNode>(endLocation);
            node.addReturnStatement(std::move(returnStatementNode));
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

    VisitorResult Analyzer::visit(EnumDefinitionStatementNode& node)
    {
        const auto& enumName = node.getName();
        auto* enumDataPtr = node.getEnumData();

        if (enumDataPtr == nullptr)
        {
            std::ostringstream os;
            os << "Enum '" << enumName << "' doesn't appear to be defined";
            reporter.error(os.str(), node.getLocation());

            return VisitorResult();
        }

        // Next we need to check if there is a naming conflict between other variables or enums.
        const CType datatype(EnumCType::ENUM, 0, std::make_optional<std::string>(enumName));
        const auto currentLocality = localityStack.top();

        for (auto& [name, enumerator] : enumDataPtr->enumeratorMap)
        {
            const auto* lookupContext = scope.findAnyVariable(name);

            if (lookupContext != nullptr)
            {
                // @@@ test this error case.
                std::ostringstream builder;
                builder << "Enumerator '" << name << "' would cause a redefinition of previously declared or defined variable with the same name";
                reporter.error(builder.str(), node.getLocation());
                break;
            }

            VariableContext context(datatype, currentLocality, EnumModifier::CONST_VALUE);
            scope.add(name, context);
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
        auto ifCondExprNodeType = ifCondExpression->getType();

        // Check if the conditional is a simple variable (i.e. "if (a) { ... }"),
        // add a DerefNode before we do other transformations.
        if (ifCondExprNodeType == EnumNodeType::VARIABLE)
        {
            node.wrapIfConditionalWithDerefNode();

            // Now we know this is a DerefNode, but we will make sure by resetting these variables.
            ifCondExpression = node.getIfConditional();
            ifCondExprNodeType = ifCondExpression->getType();
        }

        // Note: static_cast is only safe if it is in fact a BinOpNode pointer.
        auto* binOpNodePtr = static_cast<BinOpNode*>(ifCondExpression);

        // If this isn't a comparison operation, we must wrap it into one.
        // ex. if (1) { ... } should become if (1 != 0) { ... }
        if (ifCondExprNodeType != EnumNodeType::BIN_OP || !binOpNodePtr->isComparisonOp())
        {
            const auto location = ifCondExpression->getLocation();
            const auto& datatype = ifCondExpression->getDatatype();

            // Pointer type: compare to NULL
            if (datatype.pointers > 0)
            {
                auto comparator = std::make_unique<LitteralNode>(location);
                node.wrapIfConditional(EnumBinOpNodeType::CMP_NE, std::move(comparator));
            }

            // Single precision floating point type: compare to 0.0F
            else if (datatype.type == EnumCType::FLOAT)
            {
                auto comparator = std::make_unique<LitteralNode>(location, 0.0F);
                node.wrapIfConditional(EnumBinOpNodeType::CMP_NE, std::move(comparator));
            }

            // Double precision floating point type: compare to 0.0
            else if (datatype.type == EnumCType::DOUBLE)
            {
                auto comparator = std::make_unique<LitteralNode>(location, 0.0);
                node.wrapIfConditional(EnumBinOpNodeType::CMP_NE, std::move(comparator));
            }

            // Int type: compare to 0
            // Note: Should probably consider additional cases (ex. other sized ints).
            else
            {
                auto comparator = std::make_unique<LitteralNode>(location, static_cast<s32>(0));
                node.wrapIfConditional(EnumBinOpNodeType::CMP_NE, std::move(comparator));
            }
        }

        auto* ifStatement = node.getIfStatement();
        ifStatement->accept(this);

        auto* elseStatement = node.getElseStatement();

        if (elseStatement != nullptr)
        {
            elseStatement->accept(this);
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

        // Need to dereference VariableNodes since they can only ever be read from.
        if (expression->getType() == EnumNodeType::VARIABLE)
        {
            node.derefNode();
            expression = node.getExpression();
        }

        const auto& datatype = expression->getDatatype();
        node.setDatatype(datatype);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(ReturnStatementNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        if (isValidNonLitteralRHSNodeType(expression->getType()))
        {
            node.deref();
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(StructDefinitionStatementNode& node)
    {
        const auto currentLocality = localityStack.top();
        // TODO: support static?
        const auto modifier = EnumModifier::NO_MOD;
        const StructOrUnionContext context(currentLocality, modifier);
        const auto& structName = node.getName();
        const auto* structDataPtr = structTable->get(structName);

        if (structDataPtr != nullptr && structDataPtr->symState == EnumSymState::DEFINED)
        {
            std::ostringstream builder;
            builder << "struct " << structName << " is already previously defined. This violates the multiple definition rule";
            reporter.error(builder.str(), node.getLocation());
        }

        else if (structDataPtr == nullptr)
        {
            scope.add(structName, context);

            StructData structData(EnumSymState::DEFINED);
            const auto optionalBadField = node.setupFieldTable(structData.fieldMap);

            if (optionalBadField.has_value())
            {
                std::ostringstream builder;
                builder << "Found duplicate field in struct '" << node.getName() << "' called '"
                        << *optionalBadField << "'";
                reporter.error(builder.str(), node.getLocation());
            }

            auto* addedStructDataPtr = structTable->addOrUpdate(structName, std::move(structData));
            node.setStructData(addedStructDataPtr);
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
        const auto* structDataPtr = structTable->get(structName);

        if (structDataPtr != nullptr)
        {
            std::ostringstream builder;
            builder << "struct " << structName << " is already previously declared or defined";
            reporter.warn(builder.str(), node.getLocation());
        }

        else
        {
            StructData structData(EnumSymState::DECLARED);
            structTable->addOrUpdate(structName, std::move(structData));
            scope.add(structName, context);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(TranslationUnitNode& node)
    {
        structTable = node.getStructTablePtr();

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

            if (node.getOpType() == EnumUnaryOpType::ADDRESS_OF)
            {
                if (expression->getType() != EnumNodeType::VARIABLE)
                {
                    const char* message = "Expected a variable expression prior to attempting to take the address of it";
                    reporter.error(message, node.getLocation());
                }

                else
                {
                    // Note: We don't add a DerefNode because the variable (at least in LLVM) is already a pointer type.
                    // TODO: When if/when we support additional backends, re-consider moving this logic.
                    CType newType = expression->getDatatype();
                    ++newType.pointers;
                    node.setDatatype(newType);
                }
            }

            else if (expression->getType() == EnumNodeType::VARIABLE)
            {
                const CType datatype = expression->getDatatype();
                node.derefNode();
                node.setDatatype(datatype);

                // This line is commented out to ignore a cppcheck "error", but may be needed some day.
                // expression = node.getExpression();
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

        node.setDatatype(varContext->getCType());

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

        auto* lookupContext = scope.findAnyVariable(node.getName());

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

            // Update the node's locality for later use.
            node.setLocality(currentLocality);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(WhileStatementNode& node)
    {
        auto* conditional = node.getConditional();
        conditional->accept(this);

        const auto condExprNodeType = conditional->getType();
        const auto& conditionalExprDatatype = conditional->getDatatype();

        if (condExprNodeType == EnumNodeType::VARIABLE)
        {
            node.derefConditional();
            node.wrapConditionalWithBinOpNode();

            // Now we know this is a DerefNode, but we will make sure by resetting these variables.
            // Note: Commenting out so cppcheck doesn't complain, however we may want this as a reminder
            //       in the future.
            // conditional = node.getConditional();
        }

        if (conditionalExprDatatype.type == EnumCType::VOID || conditionalExprDatatype.type == EnumCType::VOID_PTR)
        {
            reporter.error("Unexpected void or void* expression", node.getLocation());
            return VisitorResult();
        }

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

        const auto& stateTypePair = findResult->second;
        const auto& currentState = stateTypePair.first;
        const bool invResult = currentState == state || (currentState == EnumSymState::DEFINED && state == EnumSymState::DECLARED);
        return !invResult;
    }
}

