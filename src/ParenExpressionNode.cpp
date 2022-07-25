/**
 * An AST node for paren expression ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-28
 */

#include <cmm/ParenExpressionNode.h>

namespace cmm
{

    ParenExpressionNode::ParenExpressionNode(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        ExpressionNode(NodeType::PAREN_EXPRESSION), expression(std::move(expression))
    {
    }

    ExpressionNode* ParenExpressionNode::getExpression() CMM_NOEXCEPT
    {
        return expression.get();
    }

    const ExpressionNode* ParenExpressionNode::getExpression() const CMM_NOEXCEPT
    {
        return expression.get();
    }

    std::string ParenExpressionNode::toString() const /* override */
    {
        return "ParenExpressionNode";
    }
}

