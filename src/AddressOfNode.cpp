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
        UnaryOpNode(EnumNodeType::ADDRESS_OF, location, EnumUnaryOpType::ADDRESS_OF, nullptr)
    {
    }

    void AddressOfNode::setExpression(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT /* override */
    {
        // Do nothing unless it's a VariableNode
        if (expression->getType() == EnumNodeType::VARIABLE)
        {
            UnaryOpNode::setExpression(std::move(expression));
        }
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

