/**
 * An AST node for a return statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-29
 */

#pragma once

#ifndef CMM_RETURN_STATEMENT_H
#define CMM_RETURN_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>
#include <cmm/StatementNode.h>

// std includes
#include <memory>
#include <optional>
#include <string>

namespace cmm
{
    class ReturnStatementNode : public StatementNode
    {
    public:

        /**
         * Default constructor without a defined expression to return.
         */
        ReturnStatementNode() CMM_NOEXCEPT;

        /**
         * Constructor with a defined expression to return.
         *
         * @param expression the returned expression.
         */
        ReturnStatementNode(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        ReturnStatementNode(const ReturnStatementNode&) = delete;

        /**
         * Move constructor.
         */
        ReturnStatementNode(ReturnStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~ReturnStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return ReturnStatementNode reference.
         */
        ReturnStatementNode& operator= (const ReturnStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return ReturnStatementNode reference.
         */
        ReturnStatementNode& operator= (ReturnStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets whether an expression exists in this return statement.
         *
         * @return bool.
         */
        bool hasExpression() const CMM_NOEXCEPT;

        /**
         * Gets the returned expression.
         *
         * @return pointer to ExpressionNode in the returned expression.
         */
        ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the returned expression.
         *
         * @return const pointer to ExpressionNode in the returned expression.
         */
        const ExpressionNode* getExpression() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

        std::string toString() const override;

    private:

        // The optional expression contained in the returned expression.
        std::unique_ptr<ExpressionNode> expression;
    };
}

#endif //!CMM_RETURN_STATEMENT_H

