/**
 * An AST node for representing the address of operator node.
 *
 * @author hockeyhurd
 * @version 2022-08-25
 */

// Our includes
#include <cmm/AddressOfNode.h>
#include <cmm/ExpressionNode.h>

namespace cmm
{
    AddressOfNode::AddressOfNode(const Location& location, VariableNode&& variable) CMM_NOEXCEPT :
        UnaryOpNode(NodeType::ADDRESS_OF, location, EnumUnaryOpType::ADDRESS_OF, nullptr),
        variable(std::move(variable))
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

    bool AddressOfNode::hasExpression() const CMM_NOEXCEPT /* override */
    {
        // Always true since our VaraibleNode is a non-pointer type.
        return true;
    }

    ExpressionNode* AddressOfNode::getExpression() CMM_NOEXCEPT /* override */
    {
        return &variable;
    }

    const ExpressionNode* AddressOfNode::getExpression() const CMM_NOEXCEPT /* override */
    {
        return &variable;
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

