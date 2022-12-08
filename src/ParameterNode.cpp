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
    ParameterNode::ParameterNode(const Location& location, TypeNode&& type) CMM_NOEXCEPT :
        Node(EnumNodeType::PARAMETER, location), type(std::move(type)), variable(std::nullopt)
    {
    }

    ParameterNode::ParameterNode(const Location& location, TypeNode&& type, VariableNode&& variable) CMM_NOEXCEPT :
        Node(EnumNodeType::PARAMETER, location), type(std::move(type)), variable(std::move(variable))
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

    std::optional<VariableNode>& ParameterNode::getVariable() CMM_NOEXCEPT
    {
        return variable;
    }

    const std::optional<VariableNode>& ParameterNode::getVariable() const CMM_NOEXCEPT
    {
        return variable;
    }

    std::string ParameterNode::toString() const /* override */
    {
        return "ParameterNode";
    }
}

