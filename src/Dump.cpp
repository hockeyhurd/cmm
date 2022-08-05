/**
 * An implementation of the base visitor class for dumping an AST.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

// Our includes
#include <cmm/Dump.h>
#include <cmm/NodeList.h>

namespace cmm
{
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
        return node.getRoot()->accept(this);
    }

    VisitorResult Dump::visit(FunctionCallNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(FunctionDeclarationStatementNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
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
        CMM_UNIMPLEMENTED_EXCEPTION();
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
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(VariableNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

    VisitorResult Dump::visit(VariableDeclarationStatementNode& node)
    {
        CMM_UNIMPLEMENTED_EXCEPTION();
    }

}

