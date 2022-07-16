/**
 * An AST node for declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/VariableDeclarationStatementNode.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

namespace cmm
{
    VariableDeclarationStatementNode::VariableDeclarationStatementNode(std::unique_ptr<TypeNode>&& type, std::unique_ptr<VariableNode>&& variable) CMM_NOEXCEPT :
        StatementNode(NodeType::DECLARATION_STATEMENT), type(std::move(type)), variable(std::move(variable))
    {
    }

    EnumCType VariableDeclarationStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return type->getDatatype();
    }

    std::string& VariableDeclarationStatementNode::getName() CMM_NOEXCEPT
    {
        return variable->getName();
    }

    const std::string& VariableDeclarationStatementNode::getName() const CMM_NOEXCEPT
    {
        return variable->getName();
    }

    std::string VariableDeclarationStatementNode::toString() const /* override */
    {
        return "VariableDeclarationStatementNode";
    }
}
