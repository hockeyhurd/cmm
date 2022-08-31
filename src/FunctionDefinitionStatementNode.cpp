/**
 * An AST node for a function definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

#include <cmm/FunctionDefinitionStatementNode.h>

namespace cmm
{
    FunctionDefinitionStatementNode::FunctionDefinitionStatementNode(const Location& location, TypeNode type,
        const std::string& funcName, BlockNode&& block, ParamList&& params) :
        StatementNode(NodeType::FUNCTION_DEFINITION_STATEMENT, location), type(type), funcName(funcName),
        block(std::move(block)), params(std::move(params))
    {
    }

    FunctionDefinitionStatementNode::FunctionDefinitionStatementNode(const Location& location, TypeNode type,
        std::string&& funcName, BlockNode&& block, ParamList&& params) CMM_NOEXCEPT :
        StatementNode(NodeType::FUNCTION_DEFINITION_STATEMENT, location), type(type), funcName(std::move(funcName)),
        block(std::move(block)), params(std::move(params))
    {
    }

    TypeNode& FunctionDefinitionStatementNode::getTypeNode() CMM_NOEXCEPT
    {
        return type;
    }

    const TypeNode& FunctionDefinitionStatementNode::getTypeNode() const CMM_NOEXCEPT
    {
        return type;
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

    FunctionDefinitionStatementNode::ParamListIter FunctionDefinitionStatementNode::begin() CMM_NOEXCEPT
    {
        return params.begin();
    }

    const FunctionDefinitionStatementNode::ParamListConseIter
    FunctionDefinitionStatementNode::cbegin() const CMM_NOEXCEPT
    {
        return params.cbegin();
    }

    FunctionDefinitionStatementNode::ParamListIter FunctionDefinitionStatementNode::end() CMM_NOEXCEPT
    {
        return params.end();
    }

    const FunctionDefinitionStatementNode::ParamListConseIter
    FunctionDefinitionStatementNode::cend() const CMM_NOEXCEPT
    {
        return params.cend();
    }

    std::string FunctionDefinitionStatementNode::toString() const /* override */
    {
        return "FunctionDefinitionStatementNode";
    }
}

