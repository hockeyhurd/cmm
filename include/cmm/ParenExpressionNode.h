/**
 * An AST node for paren expression ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-28
 */

#pragma once

#ifndef CMM_PAREN_EXPRESSION_NODE_H
#define CMM_PAREN_EXPRESSION_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

namespace cmm
{
    class ParenExpressionNode : public ExpressionNode
    {
    public:

        /**
         * Default constructor with binary op type.
         *
         * @param type the EnumBinOpNodeType.
         */
        ParenExpressionNode(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        ParenExpressionNode(const ParenExpressionNode&) = delete;

        /**
         * Move constructor.
         */
        ParenExpressionNode(ParenExpressionNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~ParenExpressionNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return ParenExpressionNode reference.
         */
        ParenExpressionNode& operator= (const ParenExpressionNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return ParenExpressionNode reference.
         */
        ParenExpressionNode& operator= (ParenExpressionNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the ExpressionNode.
         *
         * @return ExpressionNode pointer.
         */
        ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the ExpressionNode.
         *
         * @return const ExpressionNode pointer.
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

        // The wrapped ExpressionNode.
        std::unique_ptr<ExpressionNode> expression;
    };
}

#endif //!CMM_PAREN_EXPRESSION_NODE_H

