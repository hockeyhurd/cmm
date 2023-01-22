/**
 * An implementation of the base visitor class for dumping an AST.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

// Our includes
#include <cmm/NodeList.h>
#include <cmm/visit/Dump.h>

// std includes
#include <algorithm>
#include <iostream>

namespace cmm
{
    template<class T>
    static void printNode(const T& node)
    {
        std::cout << '[' << node.toString() << "]: ";
    }

    static void printDatatype(const CType& type)
    {
        std::cout << toString(type.type);

        if (type.type == EnumCType::STRUCT)
        {
            std::cout << type.optStructName.value();
        }

        printRepeat(std::cout, '*', type.pointers);
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

    VisitorResult Dump::visit(CastNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();

        if (node.hasExpression())
        {
            auto* expression = node.getExpression();
            expression->accept(this);
        }

        else
        {
            std::cout << "NULL\n";
        }

        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(CompilationUnitNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        auto result = node.getRoot().accept(this);
        decreaseIntentation();
        printNewLine();

        return result;
    }

    VisitorResult Dump::visit(DerefNode& node)
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

    VisitorResult Dump::visit(FieldAccessNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        auto* expression = node.getExpression();
        expression->accept(this);

        printIndentation();
        std::cout << "datatype: ";
        printType(std::cout, node.getDatatype());
        printNewLine();

        printIndentation();
        std::cout << "field name: " << node.getName();
        printNewLine();

        decreaseIntentation();
        decreaseIntentation();

        return VisitorResult();
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
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        std::cout << "if (\n";

        increaseIntentation();
        auto* ifCondExpression = node.getIfConditional();
        ifCondExpression->accept(this);
        printIndentation();
        std::cout << ")\n";
        decreaseIntentation();

        increaseIntentation();
        auto* ifStatement = node.getIfStatement();
        ifStatement->accept(this);
        decreaseIntentation();

        auto* elseStatement = node.getElseStatement();

        if (elseStatement != nullptr)
        {
            printIndentation();
            std::cout << "else\n";
            increaseIntentation();
            ifStatement->accept(this);
            decreaseIntentation();
        }

        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(LitteralNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        const auto& datatype = node.getDatatype();
        std::cout << toString(datatype.type) << ": ";

        switch (datatype.type)
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
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        auto* expression = node.getExpression();
        expression->accept(this);
        decreaseIntentation();

        return VisitorResult();
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

    VisitorResult Dump::visit(StructDefinitionStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        printIndentation();
        std::cout << "Name: " << node.getName();
        printNewLine();

        printIndentation();
        std::cout << "Fields:";
        printNewLine();

        increaseIntentation();
        node.getBlockNode().accept(this);
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(StructFwdDeclarationStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printDatatype(node.getDatatype());
        decreaseIntentation();

        return VisitorResult();
    }

    VisitorResult Dump::visit(TranslationUnitNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        std::for_each(node.begin(), node.end(), [this](std::unique_ptr<StatementNode>& statement) { if (statement != nullptr) statement->accept(this); });
        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(TypeNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        const auto& datatype = node.getDatatype();
        std::cout << toString(datatype.type);
        printRepeat(std::cout, '*', datatype.pointers);
        decreaseIntentation();
        printNewLine();

        return VisitorResult();
    }

    VisitorResult Dump::visit(UnaryOpNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();
        printIndentation();
        std::cout << "UnaryOpType: " << toString(node.getOpType());
        printNewLine();

        if (node.hasExpression())
        {
            auto* expression = node.getExpression();
            expression->accept(this);
        }

        else
        {
            printIndentation();
            std::cout << "<empty>";
            printNewLine();
        }

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

    VisitorResult Dump::visit(WhileStatementNode& node)
    {
        printIndentation();
        printNode(node);
        printNewLine();

        increaseIntentation();

        printIndentation();
        std::cout << "condition:\n";
        auto* conditional = node.getConditional();
        conditional->accept(this);
        printNewLine();

        printIndentation();
        std::cout << "statement:\n";
        auto* statement = node.getStatement();
        statement->accept(this);
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
        printRepeat(std::cout, ' ', indent);
    }

    void Dump::printNewLine() const
    {
        std::cout << std::endl;
    }

}

