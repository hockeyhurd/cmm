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

    ArgNode::ArgNode(std::unique_ptr<ExpressionNode>&& value) CMM_NOEXCEPT : Node(NodeType::ARG), value(std::move(value))
    {
    }

    ExpressionNode* ArgNode::getValue() CMM_NOEXCEPT
    {
        return value.get();
    }

    const ExpressionNode* ArgNode::getValue() const CMM_NOEXCEPT
    {
        return value.get();
    }

    std::string ArgNode::toString() const /* override */
    {
        return "ArgNode";
    }
}

