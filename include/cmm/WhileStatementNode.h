/**
 * An AST node for representing a while loop.
 *
 * @author hockeyhurd
 * @version 2022-08-21
 */

#pragma once

#ifndef CMM_WHILE_STATEMENT_NODE_H
#define CMM_WHILE_STATEMENT_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/StatementNode.h>

// std includes
#include <memory>

namespace cmm
{
    // Forward declarations:
    class ExpressionNode;

    class WhileStatementNode : public StatementNode
    {
    public:

        /**
         * Constructor.
         *
         * @param conditional the conditional ExpressionNode.
         * @param statement the loop's block statement.
         */
        WhileStatementNode(std::unique_ptr<ExpressionNode>&& conditional, std::unique_ptr<StatementNode>&& statement) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        WhileStatementNode(const WhileStatementNode&) = delete;

        /**
         * Move constructor.
         */
        WhileStatementNode(WhileStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~WhileStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return WhileStatementNode reference.
         */
        WhileStatementNode& operator= (const WhileStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return WhileStatementNode reference.
         */
        WhileStatementNode& operator= (WhileStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the ExpressionNode for the conditional.
         *
         * @return pointer to ExpressionNode.
         */
        ExpressionNode* getConditional() CMM_NOEXCEPT;

        /**
         * Gets the ExpressionNode for the conditional.
         *
         * @return const pointer to ExpressionNode.
         */
        const ExpressionNode* getConditional() const CMM_NOEXCEPT;

        /**
         * Get the StatementNode for the while loop to run.
         *
         * @return pointer to StatementNode.
         */
        StatementNode* getStatement() CMM_NOEXCEPT;

        /**
         * Get the StatementNode for the while loop to run.
         *
         * @return const pointer to StatementNode.
         */
        const StatementNode* getStatement() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;

        std::string toString() const override;

    private:

        // The conditional expression
        std::unique_ptr<ExpressionNode> conditional;

        // The looping statement.
        std::unique_ptr<StatementNode> statement;
    };
}

#endif //!CMM_WHILE_STATEMENT_NODE_H

