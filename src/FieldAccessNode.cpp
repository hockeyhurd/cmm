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
        ExpressionNode(EnumNodeType::FIELD_ACCESS, location), expr(std::move(expr)), field(fieldName, datatype, -1)
    {
    }

    FieldAccessNode::FieldAccessNode(const Location& location, std::unique_ptr<ExpressionNode>&& expr,
        std::string&& fieldName, const EnumFieldAccessType accessType) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::FIELD_ACCESS, location), expr(std::move(expr)),
        field(std::move(fieldName), std::move(datatype), -1)
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

    Field& FieldAccessNode::getField() CMM_NOEXCEPT
    {
        return field;
    }

    const Field& FieldAccessNode::getField() const CMM_NOEXCEPT
    {
        return field;
    }

    std::string& FieldAccessNode::getFieldName() CMM_NOEXCEPT
    {
        return field.getName();
    }

    const std::string& FieldAccessNode::getFieldName() const CMM_NOEXCEPT
    {
        return field.getName();
    }

#if 0
    CType& FieldAccessNode::getDatatype() CMM_NOEXCEPT /* override */
    {
        return field.getDatatype();
    }

    const CType& FieldAccessNode::getDatatype() const CMM_NOEXCEPT /* override */
    {
        return field.getDatatype();
    }
#endif

    s32 FieldAccessNode::getIndex() const CMM_NOEXCEPT
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

