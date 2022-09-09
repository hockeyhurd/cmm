/**
 * An AST node for a return statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-29
 */

// Our includes
#include <cmm/ReturnStatementNode.h>

namespace cmm
{
    ReturnStatementNode::ReturnStatementNode(const Location& location) CMM_NOEXCEPT :
        StatementNode(NodeType::RETURN_STATEMENT, location), expression(nullptr)
    {
    }

    ReturnStatementNode::ReturnStatementNode(const Location& location, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        StatementNode(NodeType::RETURN_STATEMENT, location), expression(std::move(expression))
    {
    }

    bool ReturnStatementNode::hasExpression() const CMM_NOEXCEPT
    {
        return expression != nullptr;
    }

    ExpressionNode* ReturnStatementNode::getExpression() CMM_NOEXCEPT
    {
        return expression.get();
    }

    const ExpressionNode* ReturnStatementNode::getExpression() const CMM_NOEXCEPT
    {
        return expression.get();
    }

    std::optional<EnumCType> ReturnStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return hasExpression() ? std::make_optional(expression->getDatatype()) : std::nullopt;
    }

    std::string ReturnStatementNode::toString() const /* override */
    {
        return "ReturnStatementNode";
    }
}

