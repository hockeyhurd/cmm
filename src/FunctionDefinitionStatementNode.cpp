/**
 * An AST node for a function declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

#include <cmm/FunctionDefinitionStatementNode.h>

namespace cmm
{
    FunctionDefinitionStatementNode::FunctionDefinitionStatementNode(TypeNode type,
        const std::string& funcName, BlockNode&& block) :
        StatementNode(NodeType::FUNCTION_DEFINITION_STATEMENT), type(type), funcName(funcName),
        block(std::move(block))
    {
    }

    FunctionDefinitionStatementNode::FunctionDefinitionStatementNode(TypeNode type,
        std::string&& funcName, BlockNode&& block) CMM_NOEXCEPT :
        StatementNode(NodeType::FUNCTION_DEFINITION_STATEMENT), type(type), funcName(std::move(funcName)),
        block(std::move(block))
    {
    }

    EnumCType FunctionDefinitionStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return type.getDatatype();
    }

    std::string& FunctionDefinitionStatementNode::getName() CMM_NOEXCEPT
    {
        return funcName;
    }

    const std::string& FunctionDefinitionStatementNode::getName() const CMM_NOEXCEPT
    {
        return funcName;
    }

    BlockNode& FunctionDefinitionStatementNode::getBlock() CMM_NOEXCEPT
    {
        return block;
    }

    const BlockNode& FunctionDefinitionStatementNode::getBlock() const CMM_NOEXCEPT
    {
        return block;
    }

    std::string FunctionDefinitionStatementNode::toString() const /* override */
    {
        return "FunctionDefinitionStatementNode";
    }
}

