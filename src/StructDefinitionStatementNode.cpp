/**
 * An AST node for a struct definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-10-26
 */

// Our includes
#include <cmm/StructDefinitionStatementNode.h>
#include <cmm/StructTable.h>
#include <cmm/VariableDeclarationStatementNode.h>

namespace cmm
{
    StructDefinitionStatementNode::StructDefinitionStatementNode(const Location& location, const std::string& name,
        BlockNode&& blockNode) : StatementNode(EnumNodeType::STRUCT_DEFINITION, location),
        name(name), blockNode(std::move(blockNode)), structData(nullptr)
    {
    }

    std::string& StructDefinitionStatementNode::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& StructDefinitionStatementNode::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    BlockNode& StructDefinitionStatementNode::getBlockNode() CMM_NOEXCEPT
    {
        return blockNode;
    }

    const BlockNode& StructDefinitionStatementNode::getBlockNode() const CMM_NOEXCEPT
    {
        return blockNode;
    }

    bool StructDefinitionStatementNode::empty() const CMM_NOEXCEPT
    {
        return blockNode.empty();
    }

    std::size_t StructDefinitionStatementNode::size() const CMM_NOEXCEPT
    {
        return blockNode.size();
    }

    StructData* StructDefinitionStatementNode::getStructData() CMM_NOEXCEPT
    {
        return structData;
    }

    const StructData* StructDefinitionStatementNode::getStructData() const CMM_NOEXCEPT
    {
        return structData;
    }

    void StructDefinitionStatementNode::setStructData(StructData* structData) CMM_NOEXCEPT
    {
        this->structData = structData;
    }

    std::optional<std::string> StructDefinitionStatementNode::setupFieldTable(std::unordered_map<std::string, Field>& fieldMap)
    {
        s32 index = 0;

        for (const auto& statementNodePtr : blockNode)
        {
            const auto* varDeclPtr = static_cast<const VariableDeclarationStatementNode*>(statementNodePtr.get());
            const std::string& name = varDeclPtr->getName();
            const auto findResult = fieldMap.find(name);

            // If we found a duplicate, return this value.
            if (findResult != fieldMap.end())
            {
                return std::make_optional(name);
            }

            fieldMap.emplace(name, Field(name, varDeclPtr->getDatatype(), index++));
        }

        return std::nullopt;
    }

#if 0
    StructDefinitionStatementNode::FieldMapIter StructDefinitionStatementNode::begin() CMM_NOEXCEPT
    {
        return fieldMap.begin();
    }

    StructDefinitionStatementNode::FieldMapConstIter StructDefinitionStatementNode::cbegin() const CMM_NOEXCEPT
    {
        return fieldMap.cbegin();
    }

    StructDefinitionStatementNode::FieldMapIter StructDefinitionStatementNode::end() CMM_NOEXCEPT
    {
        return fieldMap.end();
    }

    StructDefinitionStatementNode::FieldMapConstIter StructDefinitionStatementNode::cend() const CMM_NOEXCEPT
    {
        return fieldMap.cend();
    }
#endif

    VisitorResult StructDefinitionStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string StructDefinitionStatementNode::toString() const /* override */
    {
        return "StructDefinitionStatementNode";
    }
}

