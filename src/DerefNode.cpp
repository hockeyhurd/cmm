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
    DerefNode::DerefNode(const Location& location, std::unique_ptr<ExpressionNode>&& expr) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::DEREF, location), expr(std::move(expr)), rootType(EnumNodeType::UNKNOWN),
        modType(std::nullopt)
    {
    }

    DerefNode::DerefNode(const Location& location, std::unique_ptr<ExpressionNode>&& expr, const bool pExplicit) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::DEREF, location), expr(std::move(expr)), rootType(EnumNodeType::UNKNOWN),
        modType(this->expr->getDatatype())
    {
    }

    bool DerefNode::hasExpression() const CMM_NOEXCEPT
    {
        return expr != nullptr;
    }

    ExpressionNode* DerefNode::getExpression() CMM_NOEXCEPT
    {
        return expr.get();
    }

    const ExpressionNode* DerefNode::getExpression() const CMM_NOEXCEPT
    {
        return expr.get();
    }

    EnumNodeType DerefNode::getRootType() const CMM_NOEXCEPT
    {
        if (rootType == EnumNodeType::UNKNOWN)
        {
            auto* lastExpr = expr.get();

            while (lastExpr != nullptr && lastExpr->getType() == EnumNodeType::DEREF)
            {
                auto* derefNode = static_cast<DerefNode*>(lastExpr);
                lastExpr = derefNode->getExpression();
            }

            rootType = lastExpr->getType();
        }

        return rootType;
    }

    void DerefNode::resolveDatatype() CMM_NOEXCEPT
    {
        if (modType.has_value())
        {
            modType = expr->getDatatype();

            // We subtract one since this is "popping off" one level of indirection.
            --modType->pointers;
        }
    }

    std::unique_ptr<ExpressionNode> DerefNode::release() CMM_NOEXCEPT
    {
        return std::move(expr);
    }

    CType& DerefNode::getDatatype() CMM_NOEXCEPT /* override */
    {
        if (modType.has_value())
        {
            return *modType;
        }

        return expr->getDatatype();
    }

    const CType& DerefNode::getDatatype() const CMM_NOEXCEPT /* override */
    {
        if (modType.has_value())
        {
            return *modType;
        }

        return expr->getDatatype();
    }

    void DerefNode::setDatatype(const CType& datatype) CMM_NOEXCEPT /* override */
    {
        if (modType.has_value())
        {
            modType = datatype;
        }

        else
        {
            expr->setDatatype(datatype);
        }
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

