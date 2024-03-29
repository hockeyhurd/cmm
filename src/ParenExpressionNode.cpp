/**
 * An AST node for paren expression ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-28
 */

#include <cmm/ParenExpressionNode.h>
#include <cmm/DerefNode.h>

namespace cmm
{

    ParenExpressionNode::ParenExpressionNode(const Location& location, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::PAREN_EXPRESSION, location), expression(std::move(expression))
    {
    }

    bool ParenExpressionNode::hasExpression() const CMM_NOEXCEPT
    {
        return expression != nullptr;
    }

    ExpressionNode* ParenExpressionNode::getExpression() CMM_NOEXCEPT
    {
        return expression.get();
    }

    const ExpressionNode* ParenExpressionNode::getExpression() const CMM_NOEXCEPT
    {
        return expression.get();
    }

    void ParenExpressionNode::derefNode()
    {
        const auto location = expression->getLocation();
        auto temp = std::move(expression);
        expression = std::make_unique<DerefNode>(location, std::move(temp));
    }

    VisitorResult ParenExpressionNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string ParenExpressionNode::toString() const /* override */
    {
        return "ParenExpressionNode";
    }
}

