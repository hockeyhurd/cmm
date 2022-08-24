/**
 * An AST node for variable ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/VariableNode.h>

namespace cmm
{
    VariableNode::VariableNode(const std::string& name) : ExpressionNode(NodeType::VARIABLE),
        name(name), derefCount(0)
    {
    }

    VariableNode::VariableNode(std::string&& name) CMM_NOEXCEPT : ExpressionNode(NodeType::VARIABLE),
        name(std::move(name)), derefCount(0)
    {
    }

    VariableNode::VariableNode(const std::string& name, const u32 derefCount) : ExpressionNode(NodeType::VARIABLE),
        name(name), derefCount(derefCount)
    {
    }

    VariableNode::VariableNode(std::string&& name, const u32 derefCount) CMM_NOEXCEPT : ExpressionNode(NodeType::VARIABLE),
        name(std::move(name)), derefCount(derefCount)
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

    u32 VariableNode::getDereferenceCount() const CMM_NOEXCEPT
    {
        return derefCount;
    }

    VisitorResult VariableNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string VariableNode::toString() const /* override */
    {
        return "VariableNode";
    }
}

