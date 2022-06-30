/**
 * An AST node for statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/StatementNode.h>

namespace cmm
{
    StatementNode::StatementNode(const NodeType type) CMM_NOEXCEPT : Node(type)
    {
    }

    /* virtual */
    std::string StatementNode::toString() const /* override */
    {
        return "StatementNode";
    }
}

