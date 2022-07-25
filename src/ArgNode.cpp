/**
 * An AST node for an argument that is contained in a function call, node.
 *
 * @author hockeyhurd
 * @version 2022-07-24
 */

// Our includes
#include <cmm/ArgNode.h>

namespace cmm
{

    ArgNode::ArgNode(VariableNode&& variable) CMM_NOEXCEPT : Node(NodeType::ARG), variable(std::move(variable))
    {
    }

    std::string& ArgNode::getName() CMM_NOEXCEPT
    {
        return variable.getName();
    }

    const std::string& ArgNode::getName() const CMM_NOEXCEPT
    {
        return variable.getName();
    }

    std::string ArgNode::toString() const /* override */
    {
        return "ArgNode";
    }
}

