/**
 * A class for representing unary operator expressions in ast.
 *
 * @author hockeyhurd
 * @version 2022-09-09
 */

#pragma once

#ifndef CMM_UNARY_OP_NODE_H
#define CMM_UNARY_OP_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

// std includes
#include <memory>
#include <optional>
#include <string>

namespace cmm
{
    // Forward declarations:
    class Token;

    enum class EnumUnaryOpType
    {
        ADDRESS_OF = 0, NEGATIVE, POSITIVE, DECREMENT, INCREMENT
    };

    const char* toString(const EnumUnaryOpType opType) CMM_NOEXCEPT;
    std::optional<EnumUnaryOpType> getOpType(const std::string& str);
    std::optional<EnumUnaryOpType> getOpType(const Token& token);

    class UnaryOpNode : public ExpressionNode
    {
    protected:

        /**
         * Constructor.
         *
         * @param type the EnumNodeType of this node.
         * @param location the location of this node.
         * @param opType the unary op type of this node.
         * @param expression the expression to be casted.
         * @param prefix flag for whether this is a prefix (true) or postfix (false).
         */
        UnaryOpNode(const EnumNodeType type, const Location& location, const EnumUnaryOpType opType,
            std::unique_ptr<ExpressionNode>&& expression, const bool prefix) CMM_NOEXCEPT;

    public:

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param opType the unary op type of this node.
         * @param expression the expression to be casted.
         * @param prefix flag for whether this is a prefix (true) or postfix (false).
         */
        UnaryOpNode(const Location& location, const EnumUnaryOpType opType,
            std::unique_ptr<ExpressionNode>&& expression, const bool prefix) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        UnaryOpNode(const UnaryOpNode&) = delete;

        /**
         * Move constructor.
         */
        UnaryOpNode(UnaryOpNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        virtual ~UnaryOpNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return UnaryOpNode reference.
         */
        UnaryOpNode& operator= (const UnaryOpNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return UnaryOpNode reference.
         */
        UnaryOpNode& operator= (UnaryOpNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the unary op type.
         *
         * @return EnumUnaryOpType.
         */
        virtual EnumUnaryOpType getOpType() const CMM_NOEXCEPT;

        /**
         * Gets whether this cast node has ExpressionNode it's trying to cast or not.
         *
         * @return bool.
         */
        virtual bool hasExpression() const CMM_NOEXCEPT;

        /**
         * Gets the ExpressionNode being casted.
         *
         * @return pointer to ExpressionNode being casted if not a nullptr, else returns a nullptr.
         */
        virtual ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the ExpressionNode being casted.
         *
         * @return const pointer to ExpressionNode being casted if not a nullptr, else returns a nullptr.
         */
        virtual const ExpressionNode* getExpression() const CMM_NOEXCEPT;

        /**
         * Overrides the ExpressionNode with a new value.
         *
         * @param expression the ExpressionNode to replace.
         */
        virtual void setExpression(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

        /**
         * Gets the EnumNodeType of the wrapped ExpressionNode if it's a non-nullptr, else std::nullopt.
         *
         * @return std::optional<EnumNodeType>
         */
        virtual std::optional<EnumNodeType> getExpressionNodeType() const CMM_NOEXCEPT;

        /**
         * Gets whether this UnaryOpNode is in prefix notation vs postfixed.
         *
         * @return bool true if prefix notation, else false for postfix.
         */
        virtual bool isPrefix() const CMM_NOEXCEPT;

        /**
         * Gets whether this UnaryOpNode is in postfixed notation vs prefixed.
         *
         * @return bool true if postfix notation, else false for prefix.
         */
        virtual bool isPostfix() const CMM_NOEXCEPT;

        /**
         * Adds a DerefNode to the underlying expression, which is expected to be a VariableNode.
         * Note: This function assumes the caller has already performed the necessary checks
         *       to satisfy this assumption.
         */
        virtual void derefNode();

        virtual VisitorResult accept(Visitor* visitor) override;
        virtual std::string toString() const override;

    protected:

        // The type of unary op
        EnumUnaryOpType opType;

        // The expression we are casting.
        std::unique_ptr<ExpressionNode> expression;

        // Flag for whether the unary operator is a prefix or postfix notation.
        bool prefix;
    };
}

#endif //!CMM_UNARY_OP_NODE_H

