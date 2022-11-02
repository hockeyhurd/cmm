/**
 * An AST node for representing a while loop.
 *
 * @author hockeyhurd
 * @version 2022-08-21
 */

// Our includes
#include <cmm/WhileStatementNode.h>
#include <cmm/ExpressionNode.h>

namespace cmm
{
    WhileStatementNode::WhileStatementNode(const Location& location, std::unique_ptr<ExpressionNode>&& conditional,
        std::unique_ptr<StatementNode>&& statement) CMM_NOEXCEPT : StatementNode(EnumNodeType::WHILE_STATEMENT, location),
        conditional(std::move(conditional)), statement(std::move(statement))
    {
    }

    ExpressionNode* WhileStatementNode::getConditional() CMM_NOEXCEPT
    {
        return conditional.get();
    }

    const ExpressionNode* WhileStatementNode::getConditional() const CMM_NOEXCEPT
    {
        return conditional.get();
    }

    StatementNode* WhileStatementNode::getStatement() CMM_NOEXCEPT
    {
        return statement.get();
    }

    const StatementNode* WhileStatementNode::getStatement() const CMM_NOEXCEPT
    {
        return statement.get();
    }

    VisitorResult WhileStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string WhileStatementNode::toString() const /* override */
    {
        return "WhileStatementNode";
    }
}

