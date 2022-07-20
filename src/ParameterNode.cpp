/**
 * An AST node for a parameter ast node.
 *
 * @author hockeyhurd
 * @version 2022-07-20
 */

// Our includes
#include <cmm/ParameterNode.h>

namespace cmm
{

    ParameterNode::ParameterNode(TypeNode&& type, VariableNode&& variable) CMM_NOEXCEPT :
        Node(NodeType::PARAMETER), type(std::move(type)), variable(std::move(variable))
    {
    }

    TypeNode& ParameterNode::getDatatype() CMM_NOEXCEPT
    {
        return type;
    }

    const TypeNode& ParameterNode::getDatatype() const CMM_NOEXCEPT
    {
        return type;
    }

    VariableNode& ParameterNode::getVariable() CMM_NOEXCEPT
    {
        return variable;
    }

    const VariableNode& ParameterNode::getVariable() const CMM_NOEXCEPT
    {
        return variable;
    }

    std::string ParameterNode::toString() const /* override */
    {
        return "ParameterNode";
    }
}

