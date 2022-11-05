/**
 * An AST node for expression ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/ExpressionNode.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

namespace cmm
{

    ExpressionNode::ExpressionNode(const EnumNodeType type, const Location& location) CMM_NOEXCEPT :
        Node(type, location), datatype(EnumCType::VOID)
    {
    }

    ExpressionNode::ExpressionNode(const EnumNodeType type, const Location& location, const CType& datatype) CMM_NOEXCEPT :
        Node(type, location), datatype(datatype)
    {
    }

    /* virtual */
    CType& ExpressionNode::getDatatype() CMM_NOEXCEPT
    {
        return datatype;
    }

    /* virtual */
    const CType& ExpressionNode::getDatatype() const CMM_NOEXCEPT
    {
        return datatype;
    }

    /* virtual */
    void ExpressionNode::setDatatype(const CType& datatype) CMM_NOEXCEPT
    {
        this->datatype = datatype;
    }

    /* virtual */
    std::string ExpressionNode::toString() const /* override */
    {
        return "ExpressiongNode";
    }
}

