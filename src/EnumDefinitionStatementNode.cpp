/**
 * An AST node for an enum definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2023-02-03
 */

// Our includes
#include <cmm/EnumDefinitionStatementNode.h>

namespace cmm
{
    EnumDefinitionStatementNode::EnumDefinitionStatementNode(const Location& location, const std::string& name) :
        StatementNode(EnumNodeType::ENUM_DEFINITION, location), name(name)
    {
    }

    EnumDefinitionStatementNode::EnumDefinitionStatementNode(const Location& location, std::string&& name) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::ENUM_DEFINITION, location), name(std::move(name))
    {
    }

    std::string& EnumDefinitionStatementNode::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& EnumDefinitionStatementNode::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    VisitorResult EnumDefinitionStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string EnumDefinitionStatementNode::toString() const /* override */
    {
        return "EnumDefinitionNode";
    }
}

