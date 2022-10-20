/**
 * A single abstract Node in an AST.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Node.h>

namespace cmm
{

    Node::Node(const NodeType type, const Location& location) CMM_NOEXCEPT : type(type), location(location)
    {
    }

    /* virtual */
    NodeType Node::getType() const CMM_NOEXCEPT
    {
        return type;
    }

    /* virtual */
    void Node::setType(const NodeType type) CMM_NOEXCEPT
    {
        this->type = type;
    }

    /* virtual */
    Location& Node::getLocation() CMM_NOEXCEPT
    {
        return location;
    }

    /* virtual */
    const Location& Node::getLocation() const CMM_NOEXCEPT
    {
        return location;
    }

    /* virtual */
    std::string Node::toString() const
    {
        return "Node";
    }
}

