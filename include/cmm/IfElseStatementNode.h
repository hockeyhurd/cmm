/**
 * An AST node for a if-else statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-29
 */

#pragma once

#ifndef CMM_IF_ELSE_STATEMENT_H
#define CMM_IF_ELSE_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/StatementNode.h>

// std includes
#include <memory>
#include <string>

namespace cmm
{
    class ExpressionNode;

    class IfElseStatementNode : public StatementNode
    {
    public:

        /**
         * Constructor with an if conditional and statement.
         *
         * @param ifConditionalExpression the conditional expression.
         * @param ifStatement the statement after the if conditional.
         */
        IfElseStatementNode(std::unique_ptr<ExpressionNode>&& ifConditionalExpression,
            std::unique_ptr<StatementNode>&& ifStatement) CMM_NOEXCEPT;

        /**
         * Constructor with an if conditional and statement.
         *
         * @param ifConditionalExpression the conditional expression.
         * @param ifStatement the statement after the if conditional.
         * @param elseStatement the statement after the else keyword.
         */
        IfElseStatementNode(std::unique_ptr<ExpressionNode>&& ifConditionalExpression,
            std::unique_ptr<StatementNode>&& ifStatement,
            std::unique_ptr<StatementNode>&& elseStatement) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        IfElseStatementNode(const IfElseStatementNode&) = delete;

        /**
         * Move constructor.
         */
        IfElseStatementNode(IfElseStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~IfElseStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return IfElseStatementNode reference.
         */
        IfElseStatementNode& operator= (const IfElseStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return IfElseStatementNode reference.
         */
        IfElseStatementNode& operator= (IfElseStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the if conditional expression.
         *
         * @return pointer to ExpressionNode.
         */
        ExpressionNode* getIfConditional() CMM_NOEXCEPT;

        /**
         * Gets the if conditional expression.
         *
         * @return const pointer to ExpressionNode.
         */
        const ExpressionNode* getIfConditional() const CMM_NOEXCEPT;

        /**
         * Gets the if statement following the conditional.
         *
         * @return pointer to StatementNode.
         */
        StatementNode* getIfStatement() CMM_NOEXCEPT;

        /**
         * Gets the if statement following the conditional.
         *
         * @return const pointer to StatementNode.
         */
        const StatementNode* getIfStatement() const CMM_NOEXCEPT;

        /**
         * Gets whether an else statement block exists or not.
         *
         * @return bool.
         */
        bool hasElseStatement() const CMM_NOEXCEPT;

        /**
         * Gets the else statement.
         *
         * @return pointer to StatementNode.
         */
        StatementNode* getElseStatement() CMM_NOEXCEPT;

        /**
         * Gets the else statement.
         *
         * @return const pointer to StatementNode.
         */
        const StatementNode* getElseStatement() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

        std::string toString() const override;

    private:

        // The 'if' conditional expression.
        std::unique_ptr<ExpressionNode> ifConditionalExpression;

        // The 'if' statement (i.e. block, empty statement, etc.).
        std::unique_ptr<StatementNode> ifStatement;

        // The 'else' statement (i.e. block, empty statement, etc.).
        std::unique_ptr<StatementNode> elseStatement;
    };
}

#endif //!CMM_IF_ELSE_STATEMENT_H

