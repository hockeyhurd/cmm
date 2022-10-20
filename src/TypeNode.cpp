/**
 * An AST node for type ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/TypeNode.h>

namespace cmm
{
    // For now we assume it's a variable and the parser will override
    // by using setNodeType later??
    TypeNode::TypeNode(const Location& location, const CType& type) CMM_NOEXCEPT :
        Node(NodeType::VARIABLE, location), type(type)
    {
    }

    CType& TypeNode::getDatatype() CMM_NOEXCEPT
    {
        return type;
    }

    const CType& TypeNode::getDatatype() const CMM_NOEXCEPT
    {
        return type;
    }

    VisitorResult TypeNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string TypeNode::toString() const /* override */
    {
        return "TypeNode";
    }
}

