/**
 * A class for representing cast expressions in ast.
 *
 * @author hockeyhurd
 * @version 2022-09-09
 */

// Our includes
#include <cmm/CastNode.h>

namespace cmm
{
    CastNode::CastNode(const Location& location, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        ExpressionNode(NodeType::CAST, location), expression(std::move(expression))
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

    VisitorResult CastNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string CastNode::toString() const
    {
        return "CastNode";
    }
}

