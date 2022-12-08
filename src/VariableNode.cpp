/**
 * An AST node for variable ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/VariableNode.h>

namespace cmm
{
    VariableNode::VariableNode(const Location& location, const std::string& name) :
        ExpressionNode(EnumNodeType::VARIABLE, location), name(name)
    {
    }

    VariableNode::VariableNode(const Location& location, std::string&& name) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::VARIABLE, location), name(std::move(name))
    {
    }

    std::string& VariableNode::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& VariableNode::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    VisitorResult VariableNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string VariableNode::toString() const /* override */
    {
        return "VariableNode";
    }
}

