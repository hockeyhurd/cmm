/**
 * An AST node for declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/DeclarationStatementNode.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

namespace cmm
{
    DeclarationStatementNode::DeclarationStatementNode(std::shared_ptr<TypeNode> type, std::shared_ptr<VariableNode> variable) CMM_NOEXCEPT :
        StatementNode(NodeType::DECLARATION), type(std::move(type)), variable(std::move(variable))
    {
    }

    EnumCType DeclarationStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return type->getDatatype();
    }

    std::string& DeclarationStatementNode::getName() CMM_NOEXCEPT
    {
        return variable->getName();
    }

    const std::string& DeclarationStatementNode::getName() const CMM_NOEXCEPT
    {
        return variable->getName();
    }

    std::string DeclarationStatementNode::toString() const /* override */
    {
        return "DeclarationStatementNode";
    }
}

