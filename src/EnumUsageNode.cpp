/**
 * An AST node for an enumerator usage ast nodes.
 *
 * @author hockeyhurd
 * @version 2023-02-18
 */

// Our includes
#include <cmm/EnumUsageNode.h>
#include <cmm/Enumerator.h>

namespace cmm
{
    EnumUsageNode::EnumUsageNode(const Location& location, const std::string& name) :
        ExpressionNode(EnumNodeType::ENUM_USAGE, location, CType(EnumCType::ENUM)), name(name), enumerator(nullptr)
    {
    }

    EnumUsageNode::EnumUsageNode(const Location& location, std::string&& name) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::ENUM_USAGE, location, CType(EnumCType::ENUM)), name(std::move(name)), enumerator(nullptr)
    {
    }

    std::string& EnumUsageNode::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& EnumUsageNode::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    std::string EnumUsageNode::toString() const /* override */
    {
        return "EnumUsageNode";
    }

    Enumerator* EnumUsageNode::getEnumerator() CMM_NOEXCEPT
    {
        return enumerator;
    }

    void EnumUsageNode::setEnumerator(Enumerator* enumerator) CMM_NOEXCEPT
    {
        this->enumerator = enumerator;
    }

    VisitorResult EnumUsageNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    const Enumerator* EnumUsageNode::getEnumerator() const CMM_NOEXCEPT
    {
        return enumerator;
    }
}

