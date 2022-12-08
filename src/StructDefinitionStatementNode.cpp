/**
 * An AST node for a struct definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-10-26
 */

#include <cmm/StructDefinitionStatementNode.h>

namespace cmm
{
    StructDefinitionStatementNode::StructDefinitionStatementNode(const Location& location, const std::string& name, BlockNode&& blockNode) :
        StatementNode(EnumNodeType::STRUCT_DEFINITION, location), name(name), blockNode(std::move(blockNode))
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

    VisitorResult StructDefinitionStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string StructDefinitionStatementNode::toString() const /* override */
    {
        return "StructDefinitionStatementNode";
    }
}

