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

    ExpressionNode::ExpressionNode(const NodeType type, const Location& location) CMM_NOEXCEPT : Node(type, location)
    {
    }

    /* virtual */
    std::string ExpressionNode::toString() const /* override */
    {
        return "ExpressiongNode";
    }
}

