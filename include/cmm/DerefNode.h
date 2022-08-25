/**
 * An AST node for dereferencing pointer as an ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

#pragma once

#ifndef CMM_DEREF_NODE_H
#define CMM_DEREF_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

// std includes
#include <string>

namespace cmm
{
    class DerefNode : public ExpressionNode
    {
    public:

        /**
         * Constructor.
         *
         * @param expr the expression to be dereferenced.
         */
        explicit DerefNode(std::unique_ptr<ExpressionNode>&& expr) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        DerefNode(const DerefNode&) = delete;

        /**
         * Move constructor.
         */
        DerefNode(DerefNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~DerefNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return DerefNode reference.
         */
        DerefNode& operator= (const DerefNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return DerefNode reference.
         */
        DerefNode& operator= (DerefNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the expression to be dereferenced.
         *
         * @return std::string.
         */
        ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the expression to be dereferenced.
         *
         * @return std::string.
         */
        const ExpressionNode* getExpression() const CMM_NOEXCEPT;

        /**
         * Attempts to pointer chase until we get the underlying type.
         * This is needed because there could be several other DerefNodes before
         * we get to say a VariableNode.
         *
         * @return underlying NodeType of wrapped ExpressionNode.
         */
        NodeType getRootType() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The expression to be dereferenced.
        std::unique_ptr<ExpressionNode> expr;

        // Cached root NodeType.
        mutable NodeType rootType;
    };
}

#endif //!CMM_DEREF_NODE_H

