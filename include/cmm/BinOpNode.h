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

    enum class EnumBinOpNodeType
    {
        ASSIGNMENT = 0, ADD, SUBTRACT, MULTIPLY, DIVIDE
    };

    std::optional<EnumBinOpNodeType> isEnumBinOpType(const Token& token) CMM_NOEXCEPT;

    // TODO: Consider moving this to sub-classes based on the actual type
    // as we start using this class more often.
    class BinOpNode : public ExpressionNode
    {
    public:

        /**
         * Default constructor with binary op type.
         *
         * @param type the EnumBinOpNodeType.
         */
        BinOpNode(const EnumBinOpNodeType type, std::unique_ptr<ExpressionNode>&& left,
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
         * @return EnumCType.
         */
        EnumBinOpNodeType getTypeof() const CMM_NOEXCEPT;

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

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

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

