/**
 * A class for representing cast expressions in ast.
 *
 * @author hockeyhurd
 * @version 2022-09-09
 */

#pragma once

#ifndef CMM_CAST_NODE_H
#define CMM_CAST_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

// std includes
#include <memory>
#include <string>

namespace cmm
{
    class CastNode : public ExpressionNode
    {
    public:

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param expression the expression to be casted.
         */
        CastNode(const Location& location, const CType& newType, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        CastNode(const CastNode&) = delete;

        /**
         * Move constructor.
         */
        CastNode(CastNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~CastNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return CastNode reference.
         */
        CastNode& operator= (const CastNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return CastNode reference.
         */
        CastNode& operator= (CastNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets whether this cast node has ExpressionNode it's trying to cast or not.
         *
         * @return bool.
         */
        bool hasExpression() const CMM_NOEXCEPT;

        /**
         * Gets the ExpressionNode being casted.
         *
         * @return pointer to ExpressionNode being casted if not a nullptr, else returns a nullptr.
         */
        ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the ExpressionNode being casted.
         *
         * @return const pointer to ExpressionNode being casted if not a nullptr, else returns a nullptr.
         */
        const ExpressionNode* getExpression() const CMM_NOEXCEPT;

        /**
         * Replaces the set ExpressionNode with a new expression.
         *
         * @param expression the new ExpressionNode.
         */
        void setExpression(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT;

        /**
         * Adds a DerefNode to the underlying Expression.
         * Note: This function does zero checking and assumes the caller
         *       has already validated this operation.
         */
        void derefNode();

        /**
         * Gets the EnumCastType of this CastNode.
         *
         * @return EnumCastType.
         */
        EnumCastType getCastType() const CMM_NOEXCEPT;

        /**
         * Sets the EnumCastType for this CastNode.
         *
         * @param castType the EnumCastType to set.
         */
        void setCastType(const EnumCastType castType) CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The expression we are casting.
        std::unique_ptr<ExpressionNode> expression;

        // The type of cast.
        EnumCastType castType;
    };
}

#endif //!CMM_CAST_NODE_H

