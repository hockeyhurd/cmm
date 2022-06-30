/**
 * An AST node for declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#include <cmm/ExpressionStatementNode.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

namespace cmm
{
    ExpressionStatementNode::ExpressionStatementNode(std::shared_ptr<ExpressionNode> expression) :
        StatementNode(NodeType::EXPRESSION_STATEMENT), expression(expression)
    {
    }

    std::shared_ptr<ExpressionNode> ExpressionStatementNode::getExpression()
    {
        return expression;
    }

    const std::shared_ptr<ExpressionNode> ExpressionStatementNode::getExpression() const
    {
        return expression;
    }

    std::string ExpressionStatementNode::toString() const /* override */
    {
        return "ExpressionStatementNode";
    }
}

