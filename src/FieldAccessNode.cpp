/**
 * A class for representing an struct or union member field access in an AST.
 *
 * @author hockeyhurd
 * @version 2023-01-10
 */

// Our includes
#include <cmm/FieldAccessNode.h>

namespace cmm
{
    FieldAccessNode::FieldAccessNode(const Location& location, std::unique_ptr<ExpressionNode>&& expr,
        const std::string& fieldName, const EnumFieldAccessType accessType) :
        ExpressionNode(EnumNodeType::FIELD_ACCESS, location), expr(std::move(expr)),
        fieldName(fieldName), index(-1), accessType(accessType)
    {
    }

    FieldAccessNode::FieldAccessNode(const Location& location, std::unique_ptr<ExpressionNode>&& expr,
        std::string&& fieldName, const EnumFieldAccessType accessType) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::FIELD_ACCESS, location), expr(std::move(expr)),
        fieldName(std::move(fieldName)), index(-1), accessType(accessType)
    {
    }

    ExpressionNode* FieldAccessNode::getExpression() CMM_NOEXCEPT
    {
        return expr.get();
    }

    const ExpressionNode* FieldAccessNode::getExpression() const CMM_NOEXCEPT
    {
        return expr.get();
    }

    std::string& FieldAccessNode::getName() CMM_NOEXCEPT /* override */
    {
        return fieldName;
    }

    const std::string& FieldAccessNode::getName() const CMM_NOEXCEPT /* override */
    {
        return fieldName;
    }

    EnumFieldAccessType FieldAccessNode::getFieldAccessType() const CMM_NOEXCEPT
    {
        return accessType;
    }

    CType& FieldAccessNode::getDatatype() CMM_NOEXCEPT /* override */
    {
        return ExpressionNode::getDatatype();
    }

    const CType& FieldAccessNode::getDatatype() const CMM_NOEXCEPT /* override */
    {
        return ExpressionNode::getDatatype();
    }

    void FieldAccessNode::setDatatype(const CType& datatype) /* override */
    {
        ExpressionNode::setDatatype(datatype);
    }

    s32 FieldAccessNode::getIndex() const CMM_NOEXCEPT /* override */
    {
        return index;
    }

    void FieldAccessNode::setIndex(s32 index) CMM_NOEXCEPT /* override */
    {
        if (index < -1)
        {
            index = -1;
        }

        this->index = index;
    }

    void FieldAccessNode::set(const IField* other) /* override */
    {
        this->fieldName = other->getName();
        ExpressionNode::setDatatype(other->getDatatype());
        this->index = other->getIndex();
    }

    VisitorResult FieldAccessNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string FieldAccessNode::toString() const /* override */
    {
        return "FieldAccessNode";
    }
}

