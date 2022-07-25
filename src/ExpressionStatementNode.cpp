/**
 * An AST node for declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/ExpressionStatementNode.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

namespace cmm
{
    ExpressionStatementNode::ExpressionStatementNode(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        StatementNode(NodeType::EXPRESSION_STATEMENT), expression(std::move(expression))
    {
    }

    ExpressionNode* ExpressionStatementNode::getExpression() CMM_NOEXCEPT
    {
        return expression.get();
    }

    const ExpressionNode* ExpressionStatementNode::getExpression() const CMM_NOEXCEPT
    {
        return expression.get();
    }

    std::string ExpressionStatementNode::toString() const /* override */
    {
        return "ExpressionStatementNode";
    }
}

