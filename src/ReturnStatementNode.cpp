/**
 * An AST node for a variable declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-29
 */

// Our includes
#include <cmm/ReturnStatementNode.h>

namespace cmm
{
    ReturnStatementNode::ReturnStatementNode() CMM_NOEXCEPT : StatementNode(NodeType::RETURN_STATEMENT),
        expression(nullptr)
    {
    }

    ReturnStatementNode::ReturnStatementNode(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        StatementNode(NodeType::RETURN_STATEMENT), expression(std::move(expression))
    {
    }

    bool ReturnStatementNode::hasExpression() const CMM_NOEXCEPT
    {
        return expression != nullptr;
    }

    ExpressionNode* ReturnStatementNode::getExpression() CMM_NOEXCEPT
    {
        return expression.get();
    }

    const ExpressionNode* ReturnStatementNode::getExpression() const CMM_NOEXCEPT
    {
        return expression.get();
    }

    std::string ReturnStatementNode::toString() const /* override */
    {
        return "ReturnStatementNode";
    }
}

