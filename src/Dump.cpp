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
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(BinOpNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(BlockNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
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
        CMM_UNIMPLEMENTED_EXCEPTION();
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
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(ExpressionNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(ExpressionStatementNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(IfElseStatementNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(LitteralNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
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
        CMM_UNIMPLEMENTED_EXCEPTION();
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
        CMM_UNIMPLEMENTED_EXCEPTION();
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

