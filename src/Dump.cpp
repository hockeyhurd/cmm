/**
 * An implementation of the base visitor class for dumping an AST.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

// Our includes
#include <cmm/Dump.h>
#include <cmm/NodeList.h>

// std includes
#include <iostream>

namespace cmm
{
    template<class T>
    static void printNode(const T& node)
    {
        std::cout << '[' << node.toString() << "]: ";
    }

    Dump::Dump() CMM_NOEXCEPT : indent(0)
    {
    }

    VisitorResult Dump::visit(ArgNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        auto* expression = node.getExpression();
        expression->accept(this);

        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(BinOpNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        auto* leftNode = node.getLeft();
        leftNode->accept(this);

        auto* rightNode = node.getRight();
        rightNode->accept(this);
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(BlockNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        std::cout << "{\n";

        increaseIntentation();

        for (auto iter = node.begin(); iter != node.end(); ++iter)
        {
            auto& statementPtr = *iter;
            statementPtr->accept(this);
        }

        decreaseIntentation();

        printIndentation();
        std::cout << "}\n";
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(CompilationUnitNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        auto result = node.getRoot() ? node.getRoot()->accept(this) : VisitorResult();
        decreaseIntentation();
        printNewLine();

        return result;
    }

    VisitorResult Dump::visit(FunctionCallNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        std::cout << "name: " << node.getName();
        printNewLine();

        printIndentation();
        std::cout << "args: [\n";

        for (auto iter = node.begin(); iter != node.end(); ++iter)
        {
            increaseIntentation();
            iter->accept(this);
            decreaseIntentation();
        }

        printIndentation();
        std::cout << "]\n";
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(FunctionDeclarationStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        printIndentation();
        std::cout << "name: " << node.getName();
        printNewLine();

        printIndentation();
        std::cout << "params: [";
        printNewLine();

        for (auto iter = node.begin(); iter != node.end(); ++iter)
        {
            increaseIntentation();
            iter->accept(this);
            decreaseIntentation();
        }

        printIndentation();
        std::cout << ']';
        printNewLine();

        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(FunctionDefinitionStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        printIndentation();
        std::cout << "name: " << node.getName();
        printNewLine();

        printIndentation();
        std::cout << "params: [";
        printNewLine();

        for (auto iter = node.begin(); iter != node.end(); ++iter)
        {
            increaseIntentation();
            iter->accept(this);
            decreaseIntentation();
        }

        printIndentation();
        std::cout << ']';
        printNewLine();

        auto& blockNode = node.getBlock();
        blockNode.accept(this);

        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(ExpressionNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(ExpressionStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        node.getExpression()->accept(this);
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(IfElseStatementNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(LitteralNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        std::cout << toString(node.getValueType()) << ": ";

        switch (node.getValueType())
        {
        case EnumCType::NULL_T:
            std::cout << "NULL";
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

        printNewLine();
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(ParameterNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();

        auto& typeNode = node.getDatatype();
        typeNode.accept(this);

        auto& optionalVariableNode = node.getVariable();

        if (optionalVariableNode.has_value())
        {
            optionalVariableNode->accept(this);
        }

        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(ParenExpressionNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(ReturnStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        auto* expression = node.getExpression();
        expression->accept(this);
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(TypeNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        std::cout << toString(node.getDatatype());
        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(VariableNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        std::cout << node.getName();
        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(VariableDeclarationStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        printIndentation();
        std::cout << "name: " << node.getName();
        printNewLine();

        decreaseIntentation();

        return VisitorResult();
    }

    void Dump::increaseIntentation(const s32 amount) CMM_NOEXCEPT
    {
        indent += amount;
    }

    void Dump::decreaseIntentation(const s32 amount) CMM_NOEXCEPT
    {
        indent = std::max(0, indent - amount);
    }

    void Dump::printIndentation() const
    {
        for (s32 i = 0; i < indent; ++i)
        {
            std::cout << ' ';
        }
    }

    void Dump::printNewLine() const
    {
        std::cout << std::endl;
    }

}

