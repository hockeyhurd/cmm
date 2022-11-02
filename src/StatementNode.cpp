/**
 * An AST node for statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/StatementNode.h>

namespace cmm
{
    StatementNode::StatementNode(const EnumNodeType type, const Location& location) CMM_NOEXCEPT : Node(type, location)
    {
    }

    /* virtual */
    std::string StatementNode::toString() const /* override */
    {
        return "StatementNode";
    }
}

