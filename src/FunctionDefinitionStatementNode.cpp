/**
 * An AST node for a function definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

// Our includes
#include <cmm/FunctionDefinitionStatementNode.h>
#include <cmm/ReturnStatementNode.h>

namespace cmm
{
    FunctionDefinitionStatementNode::FunctionDefinitionStatementNode(const Location& location, TypeNode type,
        const std::string& funcName, BlockNode&& block, ParamList&& params) :
        StatementNode(NodeType::FUNCTION_DEFINITION_STATEMENT, location), type(type), funcName(funcName),
        block(std::move(block)), params(std::move(params)), returnStatementPtr(nullptr), returnStatementPtrChecked(false)
    {
    }

    FunctionDefinitionStatementNode::FunctionDefinitionStatementNode(const Location& location, TypeNode type,
        std::string&& funcName, BlockNode&& block, ParamList&& params) CMM_NOEXCEPT :
        StatementNode(NodeType::FUNCTION_DEFINITION_STATEMENT, location), type(type), funcName(std::move(funcName)),
        block(std::move(block)), params(std::move(params)), returnStatementPtr(nullptr), returnStatementPtrChecked(false)
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

    bool FunctionDefinitionStatementNode::hasParams() const CMM_NOEXCEPT
    {
        return !params.empty();
    }

    std::size_t FunctionDefinitionStatementNode::paramCount() const CMM_NOEXCEPT
    {
        return params.size();
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

    ReturnStatementNode* FunctionDefinitionStatementNode::getReturnStatement() CMM_NOEXCEPT
    {
        if (returnStatementPtr == nullptr)
        {
            if (!returnStatementPtrChecked && !block.empty())
            {
                // For now, assume the return statement is simply the last node in the block??
                auto endIter = block.end();

                // Minus 1 due to sentinel value.
                --endIter;

                auto& uniquePtrOfEndNode = *endIter;

                // Note: we intentionally use dynamic_cast to check if it is actually a ReturnStatementNode,
                // which would set this to nullptr anyways if it isn't.
                returnStatementPtr = dynamic_cast<ReturnStatementNode*>(uniquePtrOfEndNode.get());
            }

            returnStatementPtrChecked = true;
        }

        return returnStatementPtr;
    }

    const ReturnStatementNode* FunctionDefinitionStatementNode::getReturnStatement() const CMM_NOEXCEPT
    {
        if (returnStatementPtr == nullptr)
        {
            if (!returnStatementPtrChecked && !block.empty())
            {
                // For now, assume the return statement is simply the last node in the block??
                auto endIter = block.cend();

                // Minus 1 due to sentinel value.
                --endIter;

                const auto& uniquePtrOfEndNode = *endIter;

                // Note: we intentionally use dynamic_cast to check if it is actually a ReturnStatementNode,
                // which would set this to nullptr anyways if it isn't.
                returnStatementPtr = dynamic_cast<ReturnStatementNode*>(uniquePtrOfEndNode.get());
            }

            returnStatementPtrChecked = true;
        }

        return returnStatementPtr;
    }

    std::string FunctionDefinitionStatementNode::toString() const /* override */
    {
        return "FunctionDefinitionStatementNode";
    }
}

