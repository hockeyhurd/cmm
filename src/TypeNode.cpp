/**
 * An AST node for type ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/TypeNode.h>

namespace cmm
{
    TypeNode::TypeNode(const Location& location, const EnumCType type) CMM_NOEXCEPT :
        Node(NodeType::VARIABLE, location), type(type), pointerInderectionCount(0)
    {
    }

    TypeNode::TypeNode(const Location& location, const EnumCType type, const u32 pointerInderectionCount) CMM_NOEXCEPT :
        Node(NodeType::VARIABLE, location), type(type), pointerInderectionCount(pointerInderectionCount)
    {
    }

    EnumCType TypeNode::getDatatype() const CMM_NOEXCEPT
    {
        return type;
    }

    u32 TypeNode::getDimensions() const CMM_NOEXCEPT
    {
        return pointerInderectionCount;
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

