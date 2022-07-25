/**
 * An AST node for expression statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-28
 */

#pragma once

#ifndef CMM_EXPRESSION_STATEMENT_H
#define CMM_EXPRESSION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>
#include <cmm/StatementNode.h>

// std includes
#include <string>

namespace cmm
{
    class ExpressionStatementNode : public StatementNode
    {
    public:

        /**
         * Default constructor.
         *
         * @param expression the ExpressionNode.
         */
        ExpressionStatementNode(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        ExpressionStatementNode(const ExpressionStatementNode&) = delete;

        /**
         * Move constructor.
         */
        ExpressionStatementNode(ExpressionStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~ExpressionStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return ExpressionStatementNode reference.
         */
        ExpressionStatementNode& operator= (const ExpressionStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return ExpressionStatementNode reference.
         */
        ExpressionStatementNode& operator= (ExpressionStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the variable.
         *
         * @return ExpressionNode.
         */
        ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the variable.
         *
         * @return ExpressionNode.
         */
        const ExpressionNode* getExpression() const CMM_NOEXCEPT;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(*this));
        }

        std::string toString() const override;

    private:

        // The wrapped ExpressionNode
        std::unique_ptr<ExpressionNode> expression;
    };
}

#endif //!CMM_EXPRESSION_STATEMENT_H

