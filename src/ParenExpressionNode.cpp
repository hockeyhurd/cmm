/**
 * An AST node for paren expression ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-28
 */

#include <cmm/ParenExpressionNode.h>

namespace cmm
{

    ParenExpressionNode::ParenExpressionNode(std::shared_ptr<ExpressionNode> expression) :
        ExpressionNode(NodeType::PAREN_EXPRESSION), expression(expression)
    {
    }

    std::shared_ptr<ExpressionNode> ParenExpressionNode::getExpression()
    {
        return expression;
    }

    const std::shared_ptr<ExpressionNode> ParenExpressionNode::getExpression() const
    {
        return expression;
    }

    std::string ParenExpressionNode::toString() const /* override */
    {
        return "ParenExpressionNode";
    }
}

