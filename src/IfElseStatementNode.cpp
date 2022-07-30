/**
 * An AST node for a if-else statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-29
 */

// Our includes
#include <cmm/IfElseStatementNode.h>
#include <cmm/ExpressionNode.h>

namespace cmm
{
    IfElseStatementNode::IfElseStatementNode(std::unique_ptr<ExpressionNode>&& ifConditionalExpression,
        std::unique_ptr<StatementNode>&& ifStatement) CMM_NOEXCEPT : StatementNode(NodeType::IF_ELSE_STATEMENT),
        ifConditionalExpression(std::move(ifConditionalExpression)), ifStatement(std::move(ifStatement)),
        elseStatement(nullptr)
    {
    }

    IfElseStatementNode::IfElseStatementNode(std::unique_ptr<ExpressionNode>&& ifConditionalExpression,
        std::unique_ptr<StatementNode>&& ifStatement, std::unique_ptr<StatementNode>&& elseStatement) CMM_NOEXCEPT :
        StatementNode(NodeType::IF_ELSE_STATEMENT), ifConditionalExpression(std::move(ifConditionalExpression)),
        ifStatement(std::move(ifStatement)), elseStatement(std::move(elseStatement))
    {
    }

    ExpressionNode* IfElseStatementNode::getIfConditional() CMM_NOEXCEPT
    {
        return ifConditionalExpression.get();
    }

    const ExpressionNode* IfElseStatementNode::getIfConditional() const CMM_NOEXCEPT
    {
        return ifConditionalExpression.get();
    }

    StatementNode* IfElseStatementNode::getIfStatement() CMM_NOEXCEPT
    {
        return ifStatement.get();
    }

    const StatementNode* IfElseStatementNode::getIfStatement() const CMM_NOEXCEPT
    {
        return ifStatement.get();
    }

    bool IfElseStatementNode::hasElseStatement() const CMM_NOEXCEPT
    {
        return elseStatement != nullptr;
    }

    StatementNode* IfElseStatementNode::getElseStatement() CMM_NOEXCEPT
    {
        return elseStatement.get();
    }

    const StatementNode* IfElseStatementNode::getElseStatement() const CMM_NOEXCEPT
    {
        return elseStatement.get();
    }

    std::string IfElseStatementNode::toString() const /* override */
    {
        return "IfElseStatementNode";
    }
}

