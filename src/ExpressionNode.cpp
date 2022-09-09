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

    ExpressionNode::ExpressionNode(const NodeType type, const Location& location) CMM_NOEXCEPT :
        Node(type, location), datatype(EnumCType::VOID)
    {
    }

    ExpressionNode::ExpressionNode(const NodeType type, const Location& location, const EnumCType datatype) CMM_NOEXCEPT :
        Node(type, location), datatype(datatype)
    {
    }

    /* virtual */
    EnumCType ExpressionNode::getDatatype() const CMM_NOEXCEPT
    {
        return datatype;
    }

    /* virtual */
    void  ExpressionNode::setDatatype(const EnumCType datatype) CMM_NOEXCEPT
    {
        this->datatype = datatype;
    }

    /* virtual */
    std::string ExpressionNode::toString() const /* override */
    {
        return "ExpressiongNode";
    }
}

