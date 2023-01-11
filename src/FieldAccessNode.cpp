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
    FieldAccessNode::FieldAccessNode(const Location& location, const std::string& name,
        const CType& datatype, const u32 index) : ExpressionNode(EnumNodeType::FIELD_ACCESS, location),
        field(name, datatype, index)
    {
    }

    FieldAccessNode::FieldAccessNode(const Location& location, std::string&& name,
        CType&& datatype, const u32 index) CMM_NOEXCEPT : ExpressionNode(EnumNodeType::FIELD_ACCESS, location),
        field(std::move(name), std::move(datatype), index)
    {
    }

    Field& FieldAccessNode::getField() CMM_NOEXCEPT
    {
        return field;
    }

    const Field& FieldAccessNode::getField() const CMM_NOEXCEPT
    {
        return field;
    }

    std::string& FieldAccessNode::getName() CMM_NOEXCEPT
    {
        return field.getName();
    }

    const std::string& FieldAccessNode::getName() const CMM_NOEXCEPT
    {
        return field.getName();
    }

    CType& FieldAccessNode::getDatatype() CMM_NOEXCEPT /* override */
    {
        return field.getDatatype();
    }

    const CType& FieldAccessNode::getDatatype() const CMM_NOEXCEPT /* override */
    {
        return field.getDatatype();
    }

    u32 FieldAccessNode::getIndex() const CMM_NOEXCEPT
    {
        return field.getIndex();
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

