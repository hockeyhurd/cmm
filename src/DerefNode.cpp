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
        ExpressionNode(NodeType::DEREF), expr(std::move(expr)), rootType(NodeType::UNKNOWN)
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

    NodeType DerefNode::getRootType() const CMM_NOEXCEPT
    {
        if (rootType == NodeType::UNKNOWN)
        {
            auto* lastExpr = expr.get();

            while (lastExpr != nullptr && lastExpr->getType() == NodeType::DEREF)
            {
                auto* derefNode = static_cast<DerefNode*>(lastExpr);
                lastExpr = derefNode->getExpression();
            }

            rootType = lastExpr->getType();
        }

        return rootType;
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

