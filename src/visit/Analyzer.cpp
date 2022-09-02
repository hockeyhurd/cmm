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

    VisitorResult Analyzer::visit(AddressOfNode& node)
    {
        auto& variable = node.getVariable();
        variable.accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(ArgNode& node)
    {
        auto* expression = node.getExpression();
        expression->accept(this);

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(BinOpNode& node)
    {
        auto* leftNode = node.getLeft();
        auto leftNodeResult = leftNode->accept(this);

        auto* rightNode = node.getRight();
        auto rightNodeResult = rightNode->accept(this);

        auto leftType = deduceType(leftNode);
        auto rightType = deduceType(rightNode);

        if (leftType != rightType)
        {
            if (canPromote(leftType, rightType))
            {
                std::ostringstream builder;
                builder << "Type mismatch between " << toString(leftType)
                        << " and " << toString(rightType)
                        << " but is promotable.";
                reporter.warn(builder.str(), node.getLocation());
            }

            else
            {
                std::ostringstream builder;
                builder << "Type mismatch between " << toString(leftType)
                        << " and " << toString(rightType);
                reporter.error(builder.str(), node.getLocation());
            }
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(BlockNode& node)
    {
        for (auto& statementPtr : node)
        {
            statementPtr->accept(this);
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
        [[maybe_unused]]
        auto& name = node.getName();

        for (auto& func : node)
        {
            func.accept(this);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(FunctionDeclarationStatementNode& node)
    {
        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        [[maybe_unused]]
        auto& name = node.getName();

        for (auto& decl : node)
        {
            decl.accept(this);
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(FunctionDefinitionStatementNode& node)
    {
        localityStack.push(EnumLocality::LOCAL);

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        [[maybe_unused]]
        auto& name = node.getName();

        for (auto& funcDef : node)
        {
            funcDef.accept(this);
        }

        auto& blockNode = node.getBlock();
        blockNode.accept(this);

        localityStack.pop();

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
        switch (node.getDatatype())
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
            optionalVariableNode->accept(this);
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

    VisitorResult Analyzer::visit(TranslationUnitNode& node)
    {
        for (auto& statement : node)
        {
            if (statement != nullptr)
            {
                statement->accept(this);
            }
        }

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(TypeNode& node)
    {
        // TODO: What to do here??
        [[maybe_unused]]
        auto datatype = node.getDatatype();

        return VisitorResult();
    }

    VisitorResult Analyzer::visit(VariableNode& node)
    {
        auto& varName = node.getName();
        auto* varContext = scope.find(varName);

        if (varContext == nullptr)
        {
            reporter.bug("Failed to find variable in current context.", node.getLocation(), true);
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
        scope.add(node.getName(), context);

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

    /* static */
    EnumCType Analyzer::deduceType(ExpressionNode* expression)
    {
        return expression->getDatatype();
    }
}

