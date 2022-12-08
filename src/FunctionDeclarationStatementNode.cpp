/**
 * An AST node for a function declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

#include <cmm/FunctionDeclarationStatementNode.h>

namespace cmm
{
    FunctionDeclarationStatementNode::FunctionDeclarationStatementNode(const Location& location, TypeNode type,
        const std::string& funcName, ParamList&& params) :
        StatementNode(EnumNodeType::FUNCTION_DECLARATION_STATEMENT, location), type(type), funcName(funcName),
        params(std::move(params))
    {
    }

    FunctionDeclarationStatementNode::FunctionDeclarationStatementNode(const Location& location, TypeNode type,
        std::string&& funcName, ParamList&& params) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::FUNCTION_DECLARATION_STATEMENT, location), type(type), funcName(std::move(funcName)),
        params(std::move(params))
    {
    }

    TypeNode& FunctionDeclarationStatementNode::getTypeNode() CMM_NOEXCEPT
    {
        return type;
    }

    const TypeNode& FunctionDeclarationStatementNode::getTypeNode() const CMM_NOEXCEPT
    {
        return type;
    }

    CType& FunctionDeclarationStatementNode::getDatatype() CMM_NOEXCEPT
    {
        return type.getDatatype();
    }

    const CType& FunctionDeclarationStatementNode::getDatatype() const CMM_NOEXCEPT
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

    FunctionDeclarationStatementNode::ParamListIter FunctionDeclarationStatementNode::begin() CMM_NOEXCEPT
    {
        return params.begin();
    }

    const FunctionDeclarationStatementNode::ParamListConseIter
    FunctionDeclarationStatementNode::cbegin() const CMM_NOEXCEPT
    {
        return params.cbegin();
    }

    FunctionDeclarationStatementNode::ParamListIter FunctionDeclarationStatementNode::end() CMM_NOEXCEPT
    {
        return params.end();
    }

    const FunctionDeclarationStatementNode::ParamListConseIter
    FunctionDeclarationStatementNode::cend() const CMM_NOEXCEPT
    {
        return params.cend();
    }
    
    VisitorResult FunctionDeclarationStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string FunctionDeclarationStatementNode::toString() const /* override */
    {
        return "FunctionDeclarationStatementNode";
    }
}

