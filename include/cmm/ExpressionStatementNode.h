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
        ExpressionStatementNode(std::shared_ptr<ExpressionNode> expression);

        /**
         * Copy constructor.
         */
        ExpressionStatementNode(const ExpressionStatementNode&) = default;

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
        ExpressionStatementNode& operator= (const ExpressionStatementNode&) = default;

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
        std::shared_ptr<ExpressionNode> getExpression();

        /**
         * Gets the variable.
         *
         * @return ExpressionNode.
         */
        const std::shared_ptr<ExpressionNode> getExpression() const;

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
        std::shared_ptr<ExpressionNode> expression;
    };
}

#endif //!CMM_EXPRESSION_STATEMENT_H

