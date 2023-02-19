/**
 * A list of forward declarations for all nodes in this compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#pragma once

#ifndef CMM_NODE_LIST_FWD_H
#define CMM_NODE_LIST_FWD_H

namespace cmm
{
    class ArgNode;
    class BinOpNode;
    class BlockNode;
    class CastNode;
    class CompilationUnitNode;
    class DerefNode;
    class EnumDefinitionStatementNode;
    class EnumUsageNode;
    class ExpressionNode;
    class ExpressionStatementNode;
    class FieldAccessNode;
    class FunctionCallNode;
    class FunctionDeclarationStatementNode;
    class FunctionDefinitionStatementNode;
    class IfElseStatementNode;
    class LitteralNode;
    class Node;
    class ParameterNode;
    class ParenExpressionNode;
    class ReturnStatementNode;
    class StatementNode;
    class StructDefinitionStatementNode;
    class StructFwdDeclarationStatementNode;
    class TranslationUnitNode;
    class TypeNode;
    class UnaryOpNode;
    class VariableDeclarationStatementNode;
    class VariableNode;
    class WhileStatementNode;
}

#endif //!CMM_NODE_LIST_FWD_H

