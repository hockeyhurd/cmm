/**
 * A single abstract Node in an AST.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Node.h>

namespace cmm
{

    Node::Node(const NodeType type) CMM_NOEXCEPT : type(type)
    {
    }

    /* virtual */
    NodeType Node::getType() const CMM_NOEXCEPT
    {
        return type;
    }

    /* virtual */
    std::string Node::toString() const
    {
        return "Node";
    }
}

