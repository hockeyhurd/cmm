/**
 * A class for representing cast expressions in ast.
 *
 * @author hockeyhurd
 * @version 2022-09-09
 */

// Our includes
#include <cmm/CastNode.h>
#include <cmm/DerefNode.h>

namespace cmm
{
    CastNode::CastNode(const Location& location, const CType& newType, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::CAST, location, newType), expression(std::move(expression))
    {
    }

    bool CastNode::hasExpression() const CMM_NOEXCEPT
    {
        return expression != nullptr;
    }

    ExpressionNode* CastNode::getExpression() CMM_NOEXCEPT
    {
        return expression.get();
    }

    const ExpressionNode* CastNode::getExpression() const CMM_NOEXCEPT
    {
        return expression.get();
    }

    void CastNode::setExpression(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT
    {
        this->expression = std::move(expression);
    }

    void CastNode::derefNode()
    {
        const auto location = expression->getLocation();
        auto temp = std::move(expression);
        expression = std::make_unique<DerefNode>(location, std::move(temp));
    }

    VisitorResult CastNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string CastNode::toString() const
    {
        return "CastNode";
    }
}

