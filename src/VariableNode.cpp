/**
 * An AST node for variable ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/VariableNode.h>

namespace cmm
{
    VariableNode::VariableNode(const std::string& name) : Node(NodeType::VARIABLE), name(name)
    {
    }

    VariableNode::VariableNode(std::string&& name) CMM_NOEXCEPT : Node(NodeType::VARIABLE), name(std::move(name))
    {
    }

    std::string& VariableNode::getName()
    {
        return name;
    }

    const std::string& VariableNode::getName() const
    {
        return name;
    }

    std::string VariableNode::toString() const /* override */
    {
        return "VariableNode";
    }
}

