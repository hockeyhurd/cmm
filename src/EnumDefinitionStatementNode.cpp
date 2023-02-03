/**
 * An AST node for an enum definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2023-02-03
 */

// Our includes
#include <cmm/EnumDefinitionStatementNode.h>
#include <cmm/EnumTable.h>

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

    bool EnumDefinitionStatementNode::empty() const CMM_NOEXCEPT
    {
        return enumData == nullptr || enumData->enumeratorSet.empty();
    }

    std::size_t EnumDefinitionStatementNode::size() const CMM_NOEXCEPT
    {
        return enumData != nullptr ? enumData->enumeratorSet.size() : 0;
    }

    EnumData* EnumDefinitionStatementNode::getEnumData() CMM_NOEXCEPT
    {
        return enumData;
    }

    const EnumData* EnumDefinitionStatementNode::getEnumData() const CMM_NOEXCEPT
    {
        return enumData;
    }

    void EnumDefinitionStatementNode::setEnumData(EnumData* enumData) CMM_NOEXCEPT
    {
        this->enumData = enumData;
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

