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
#include <string>

namespace cmm
{
    class ReturnStatementNode : public StatementNode
    {
    public:

        /**
         * Default constructor without a defined expression to return.
         *
         * @param location the location of this node.
         */
        explicit ReturnStatementNode(const Location& location) CMM_NOEXCEPT;

        /**
         * Constructor with a defined expression to return.
         *
         * @param location the location of this node.
         * @param expression the returned expression.
         */
        ReturnStatementNode(const Location& location, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

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

        /**
         * Gets the underlying CType from the Expression (if non-nullptr).
         *
         * @return pointer to optional CType of the underlying expression.
         */
        CType* getDatatype() const CMM_NOEXCEPT;

        /**
         * Attempts to cast the right ExpressionNode.
         *
         * @param newType the type of the sub-expression.
         */
        void cast(const CType& newType);

        /**
         * Adds a DerefNode to the return expression.
         * Note: This assumes the caller has already performed checks.
         */
        void deref();

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The optional expression contained in the returned expression.
        std::unique_ptr<ExpressionNode> expression;
    };
}

#endif //!CMM_RETURN_STATEMENT_H

