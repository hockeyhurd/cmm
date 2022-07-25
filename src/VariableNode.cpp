/**
 * An AST node for variable ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/VariableNode.h>

namespace cmm
{
    VariableNode::VariableNode(const std::string& name) : ExpressionNode(NodeType::VARIABLE), name(name)
    {
    }

    VariableNode::VariableNode(std::string&& name) CMM_NOEXCEPT : ExpressionNode(NodeType::VARIABLE), name(std::move(name))
    {
    }

    std::string& VariableNode::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& VariableNode::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    std::string VariableNode::toString() const /* override */
    {
        return "VariableNode";
    }
}

