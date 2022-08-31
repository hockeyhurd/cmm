/**
 * An AST node for representing the address of operator node.
 *
 * @author hockeyhurd
 * @version 2022-08-25
 */

// Our includes
#include <cmm/AddressOfNode.h>

namespace cmm
{
    AddressOfNode::AddressOfNode(const Location& location, VariableNode&& variable) CMM_NOEXCEPT :
        ExpressionNode(NodeType::ADDRESS_OF, location), variable(std::move(variable))
    {
    }

    VariableNode& AddressOfNode::getVariable() CMM_NOEXCEPT
    {
        return variable;
    }

    const VariableNode& AddressOfNode::getVariable() const CMM_NOEXCEPT
    {
        return variable;
    }

    VisitorResult AddressOfNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string AddressOfNode::toString() const /* override */
    {
        return "AddressOfNode";
    }
}

