/**
 * An AST node for a function declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

#include <cmm/FunctionDeclarationStatementNode.h>

namespace cmm
{
    FunctionDeclarationStatementNode::FunctionDeclarationStatementNode(TypeNode type,
        const std::string& funcName) :
        StatementNode(NodeType::FUNCTION_DECLARATION_STATEMENT), type(type), funcName(funcName)
    {
    }

    FunctionDeclarationStatementNode::FunctionDeclarationStatementNode(TypeNode type,
        std::string&& funcName) CMM_NOEXCEPT :
        StatementNode(NodeType::FUNCTION_DECLARATION_STATEMENT), type(type), funcName(std::move(funcName))
    {
    }

    EnumCType FunctionDeclarationStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return type.getDatatype();
    }

    std::string& FunctionDeclarationStatementNode::getName() CMM_NOEXCEPT
    {
        return funcName;
    }

    const std::string& FunctionDeclarationStatementNode::getName() const CMM_NOEXCEPT
    {
        return funcName;
    }

    std::string FunctionDeclarationStatementNode::toString() const /* override */
    {
        return "FunctionDeclarationStatementNode";
    }
}

