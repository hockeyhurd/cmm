/**
 * An AST node for binary operator ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-22
 */

#pragma once

#ifndef CMM_BIN_OP_NODE_H
#define CMM_BIN_OP_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

// std includes
#include <optional>

namespace cmm
{
    class Token;

    class BinOpNode : public ExpressionNode
    {
    public:

        /**
         * Default constructor with binary op type.
         *
         * @param location the location of this node.
         * @param type the EnumBinOpNodeType.
         * @param left the left ExpressionNode.
         * @param right the right ExpressionNode.
         */
        BinOpNode(const Location& location, const EnumBinOpNodeType type,
                  std::unique_ptr<ExpressionNode>&& left,
                  std::unique_ptr<ExpressionNode>&& right) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        BinOpNode(const BinOpNode&) = delete;

        /**
         * Move constructor.
         */
        BinOpNode(BinOpNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~BinOpNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return BinOpNode reference.
         */
        BinOpNode& operator= (const BinOpNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return BinOpNode reference.
         */
        BinOpNode& operator= (BinOpNode&&) CMM_NOEXCEPT = default;

        /**
         * Get the type of this binary op node.
         *
         * @return EnumBinOpNodeType.
         */
        EnumBinOpNodeType getTypeof() const CMM_NOEXCEPT;

        /**
         * Gets whether the EnumBinOpNodeType is a comparison operation or not.
         *
         * @return bool.
         */
        bool isComparisonOp() const CMM_NOEXCEPT;

        /**
         * Gets the left node.
         *
         * @return ExpressionNode pointer.
         */
        ExpressionNode* getLeft() CMM_NOEXCEPT;

        /**
         * Gets the left node.
         *
         * @return const ExpressionNode pointer.
         */
        const ExpressionNode* getLeft() const CMM_NOEXCEPT;

        /**
         * Gets the right node.
         *
         * @return ExpressionNode pointer.
         */
        ExpressionNode* getRight() CMM_NOEXCEPT;

        /**
         * Gets the right node.
         *
         * @return const ExpressionNode pointer.
         */
        const ExpressionNode* getRight() const CMM_NOEXCEPT;

        /**
         * Attempts to cast the left ExpressionNode.
         *
         * @param newType the type of the sub-expression.
         */
        void castLeft(const CType& newType);

        /**
         * Attempts to cast the right ExpressionNode.
         *
         * @param newType the type of the sub-expression.
         */
        void castRight(const CType& newType);

        /**
         * Adds a DerefNode to the left node.
         * Note: This function does zero checking and assumes the caller
         *       has already validated this operation.
         */
        void derefNodeLeft();

        /**
         * Adds a DerefNode to the right node.
         * Note: This function does zero checking and assumes the caller
         *       has already validated this operation.
         */
        void derefNodeRight();

        /**
         * Removes a DerefNode to the left node.
         * Note: This function does zero checking and assumes the caller
         *       has already validated this operation.
         */
        void popDerefNodeLeft();

        /**
         * Removes a DerefNode to the right node.
         * Note: This function does zero checking and assumes the caller
         *       has already validated this operation.
         */
        void popDerefNodeRight();

        /**
         * Replaces the left ExpressionNode with a new expression.
         *
         * @param left the new left ExpressionNode.
         */
        void setLeftNode(std::unique_ptr<ExpressionNode>&& left) CMM_NOEXCEPT;

        /**
         * Replaces the right ExpressionNode with a new expression.
         *
         * @param right the new right ExpressionNode.
         */
        void setRightNode(std::unique_ptr<ExpressionNode>&& right) CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The type of our bin op
        EnumBinOpNodeType type;

        // The operands
        std::unique_ptr<ExpressionNode> left;
        std::unique_ptr<ExpressionNode> right;
    };
}

#endif //!CMM_BIN_OP_NODE_H

