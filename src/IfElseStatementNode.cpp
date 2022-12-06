/**
 * An AST node for a if-else statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-29
 */

// Our includes
#include <cmm/IfElseStatementNode.h>
#include <cmm/BinOpNode.h>
#include <cmm/DerefNode.h>
#include <cmm/ExpressionNode.h>

namespace cmm
{
    IfElseStatementNode::IfElseStatementNode(const Location& location, std::unique_ptr<ExpressionNode>&& ifConditionalExpression,
        std::unique_ptr<StatementNode>&& ifStatement) CMM_NOEXCEPT : StatementNode(EnumNodeType::IF_ELSE_STATEMENT, location),
        ifConditionalExpression(std::move(ifConditionalExpression)), ifStatement(std::move(ifStatement)),
        elseStatement(nullptr)
    {
    }

    IfElseStatementNode::IfElseStatementNode(const Location& location, std::unique_ptr<ExpressionNode>&& ifConditionalExpression,
        std::unique_ptr<StatementNode>&& ifStatement, std::unique_ptr<StatementNode>&& elseStatement) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::IF_ELSE_STATEMENT, location), ifConditionalExpression(std::move(ifConditionalExpression)),
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

    void IfElseStatementNode::wrapIfConditional(const EnumBinOpNodeType binOpType, std::unique_ptr<ExpressionNode>&& comparator)
    {
        auto tempLHS = std::move(ifConditionalExpression);
        const auto location = tempLHS->getLocation();
        ifConditionalExpression = std::make_unique<BinOpNode>(location, binOpType, std::move(tempLHS), std::move(comparator));
    }

    void IfElseStatementNode::wrapIfConditionalWithDerefNode()
    {
        const auto location = ifConditionalExpression->getLocation();
        auto temp = std::move(ifConditionalExpression);
        ifConditionalExpression = std::make_unique<DerefNode>(location, std::move(temp));
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

