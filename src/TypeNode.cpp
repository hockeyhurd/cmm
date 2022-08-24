/**
 * An AST node for type ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/TypeNode.h>

namespace cmm
{
    TypeNode::TypeNode(const EnumCType type) CMM_NOEXCEPT :
        Node(NodeType::VARIABLE), type(type)
    {
    }

    EnumCType TypeNode::getDatatype() const CMM_NOEXCEPT
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

