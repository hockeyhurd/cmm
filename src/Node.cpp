/**
 * A single abstract Node in an AST.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Node.h>

namespace cmm
{

    Node::Node(const NodeType type) noexcept : type(type)
    {
    }

    /* virtual */
    NodeType Node::getType() const noexcept
    {
        return type;
    }

    /* virtual */
    std::string Node::toString() const
    {
        return "Node";
    }
}

