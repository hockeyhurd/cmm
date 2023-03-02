/**
 * An AST node for variable ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include "cmm/Types.h"
#include <cmm/VariableNode.h>

namespace cmm
{
    VariableNode::VariableNode(const Location& location, const std::string& name) :
        ExpressionNode(EnumNodeType::VARIABLE, location), name(name), locality(EnumLocality::GLOBAL)
    {
    }

    VariableNode::VariableNode(const Location& location, std::string&& name) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::VARIABLE, location), name(std::move(name)), locality(EnumLocality::GLOBAL)
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

    EnumLocality VariableNode::getLocality() const CMM_NOEXCEPT
    {
        return locality;
    }

    void VariableNode::setLocality(const EnumLocality locality) CMM_NOEXCEPT
    {
        this->locality = locality;
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

