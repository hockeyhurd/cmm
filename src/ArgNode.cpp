/**
 * An AST node for an argument that is contained in a function call, node.
 *
 * @author hockeyhurd
 * @version 2022-07-24
 */

// Our includes
#include <cmm/ArgNode.h>
#include <cmm/ExpressionNode.h>

namespace cmm
{

    ArgNode::ArgNode(const Location& location, std::unique_ptr<ExpressionNode>&& value) CMM_NOEXCEPT : ExpressionNode(EnumNodeType::ARG, location), value(std::move(value))
    {
    }

    ExpressionNode* ArgNode::getExpression() CMM_NOEXCEPT
    {
        return value.get();
    }

    const ExpressionNode* ArgNode::getExpression() const CMM_NOEXCEPT
    {
        return value.get();
    }

    CType& ArgNode::getDatatype() CMM_NOEXCEPT /* override */
    {
        return value->getDatatype();
    }

    const CType& ArgNode::getDatatype() const CMM_NOEXCEPT /* override */
    {
        return value->getDatatype();
    }

    void ArgNode::setDatatype(const CType& datatype) CMM_NOEXCEPT /* override */
    {
        value->setDatatype(datatype);
    }

    VisitorResult ArgNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string ArgNode::toString() const /* override */
    {
        return "ArgNode";
    }
}

