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

    ArgNode::ArgNode(const Location& location, std::unique_ptr<ExpressionNode>&& value) CMM_NOEXCEPT : Node(NodeType::ARG, location), value(std::move(value))
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

    std::string ArgNode::toString() const /* override */
    {
        return "ArgNode";
    }
}

