/**
 * An AST node for dereferencing pointer as an ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

// Our includes
#include <cmm/DerefNode.h>

namespace cmm
{
    DerefNode::DerefNode(std::unique_ptr<ExpressionNode>&& expr) CMM_NOEXCEPT :
        ExpressionNode(NodeType::DEREF), expr(std::move(expr))
    {
    }

    ExpressionNode* DerefNode::getExpression() CMM_NOEXCEPT
    {
        return expr.get();
    }

    const ExpressionNode* DerefNode::getExpression() const CMM_NOEXCEPT
    {
        return expr.get();
    }

    VisitorResult DerefNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string DerefNode::toString() const /* override */
    {
        return "DerefNode";
    }
}

