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
// #include <algorithm>

namespace cmm
{
    /* static */
    Reporter& Analyzer::reporter = Reporter::instance();

    Analyzer::Analyzer() CMM_NOEXCEPT
    {
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
        leftNode->accept(this);

        auto* rightNode = node.getRight();
        rightNode->accept(this);

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
        // TODO: Do we need to do anything here??
#if 0
        switch (node.getValueType())
        {
        case EnumCType::NULL_T:
            std::cout << "NULL";
            break;
        case EnumCType::VOID:
            std::cout << "void";
            break;
        case EnumCType::VOID_PTR:
            std::cout << "void*";
            break;
        case EnumCType::BOOL:
            std::cout << (node.getValue().valueBool ? "true" : "false");
            break;
        case EnumCType::CHAR:
            std::cout << static_cast<s32>(node.getValue().valueChar);
            break;
        case EnumCType::INT8:
            std::cout << static_cast<s32>(node.getValue().valueS8);
            break;
        case EnumCType::INT16:
            std::cout << node.getValue().valueS16;
            break;
        case EnumCType::INT32:
            std::cout << node.getValue().valueS32;
            break;
        case EnumCType::INT64:
            std::cout << node.getValue().valueS64;
            break;
        case EnumCType::FLOAT:
            std::cout << node.getValue().valueF32;
            break;
        case EnumCType::DOUBLE:
            std::cout << node.getValue().valueF64;
            break;
        case EnumCType::STRING:
            std::cout << node.getValue().valueString;
            break;
        case EnumCType::STRUCT:
            std::cout << "struct";
            break;
        default:
            std::cout << "Unknown type";
            break;
        }
#endif

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
        // TODO: What to do here??
        [[maybe_unused]]
        auto& name = node.getName();
        return VisitorResult();
    }

    VisitorResult Analyzer::visit(VariableDeclarationStatementNode& node)
    {
        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

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
}

