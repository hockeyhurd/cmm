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
         * @param location the location of this node.
         * @param type the EnumBinOpNodeType.
         */
        ParenExpressionNode(const Location& location, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

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
         * Gets whether this cast node has ExpressionNode it's trying to cast or not.
         *
         * @return bool.
         */
        bool hasExpression() const CMM_NOEXCEPT;

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
         * Adds a DerefNode infront of the underlying VariableNode.
         * Note: This function does zero checking and assumes the caller
         *       has already validated this operation.
         */
        void derefNode();

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The wrapped ExpressionNode.
        std::unique_ptr<ExpressionNode> expression;
    };
}

#endif //!CMM_PAREN_EXPRESSION_NODE_H

