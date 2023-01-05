/**
 * An AST node for a return statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-29
 */

// Our includes
#include <cmm/ReturnStatementNode.h>
#include <cmm/CastNode.h>
#include <cmm/DerefNode.h>

namespace cmm
{
    ReturnStatementNode::ReturnStatementNode(const Location& location) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::RETURN_STATEMENT, location), expression(nullptr)
    {
    }

    ReturnStatementNode::ReturnStatementNode(const Location& location, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::RETURN_STATEMENT, location), expression(std::move(expression))
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

    CType* ReturnStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return hasExpression() ? &expression->getDatatype() : nullptr;
    }

    void ReturnStatementNode::cast(const CType& newType)
    {
        if (hasExpression())
        {
            const auto location = expression->getLocation();
            auto tempExpression = std::move(expression);
            expression = std::make_unique<CastNode>(location, newType, std::move(tempExpression));
        }
    }

    void ReturnStatementNode::deref()
    {
        if (hasExpression())
        {
            auto location = expression->getLocation();
            auto tempExpression = std::move(expression);
            expression = std::make_unique<DerefNode>(location, std::move(tempExpression));
        }
    }

    std::string ReturnStatementNode::toString() const /* override */
    {
        return "ReturnStatementNode";
    }
}

