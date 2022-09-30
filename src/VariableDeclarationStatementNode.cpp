/**
 * An AST node for a variable declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/VariableDeclarationStatementNode.h>

namespace cmm
{
    VariableDeclarationStatementNode::VariableDeclarationStatementNode(const Location& location, TypeNode type,
        VariableNode&& variable) CMM_NOEXCEPT :
        StatementNode(NodeType::VARIABLE_DECLARATION_STATEMENT, location), type(type), variable(std::move(variable))
    {
    }

    TypeNode& VariableDeclarationStatementNode::getTypeNode() CMM_NOEXCEPT
    {
        return type;
    }

    const TypeNode& VariableDeclarationStatementNode::getTypeNode() const CMM_NOEXCEPT
    {
        return type;
    }

    CType& VariableDeclarationStatementNode::getDatatype() CMM_NOEXCEPT
    {
        return type.getDatatype();
    }

    const CType& VariableDeclarationStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return type.getDatatype();
    }

    std::string& VariableDeclarationStatementNode::getName() CMM_NOEXCEPT
    {
        return variable.getName();
    }

    const std::string& VariableDeclarationStatementNode::getName() const CMM_NOEXCEPT
    {
        return variable.getName();
    }

    VisitorResult VariableDeclarationStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string VariableDeclarationStatementNode::toString() const /* override */
    {
        return "VariableDeclarationStatementNode";
    }
}

