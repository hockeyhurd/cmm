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
#include <cmm/Node.h>

// std includes
#include <optional>

namespace cmm
{
    class Token;

    enum class EnumBinOpNodeType
    {
        ASSIGNMENT = 0, ADD, SUBTRACT, MULTIPLY, DIVIDE
    };

    std::optional<EnumBinOpNodeType> isEnumBinOpType(const Token& token) noexcept;

    // TODO: Consider moving this to sub-classes based on the actual type
    // as we start using this class more often.
    class BinOpNode : public Node
    {
    public:

        /**
         * Default constructor with binary op type.
         *
         * @param type the EnumBinOpNodeType.
         */
        BinOpNode(const EnumBinOpNodeType type, std::shared_ptr<Node> left,
                  std::shared_ptr<Node> right);

        /**
         * Copy constructor.
         */
        BinOpNode(const BinOpNode&) = delete;

        /**
         * Move constructor.
         */
        BinOpNode(BinOpNode&&) noexcept = default;

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
        BinOpNode& operator= (BinOpNode&&) noexcept = default;

        /**
         * Get the type of this binary op node.
         *
         * @return EnumCType.
         */
        EnumBinOpNodeType getTypeof() const noexcept;

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

        // The type of our bin op
        EnumBinOpNodeType type;

        // The operands
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    };
}

#endif //!CMM_BIN_OP_NODE_H

