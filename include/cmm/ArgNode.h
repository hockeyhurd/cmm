/**
 * An AST node for an argument that is contained in a function call, node.
 *
 * @author hockeyhurd
 * @version 2022-07-24
 */

#pragma once

#ifndef CMM_ARG_H
#define CMM_ARG_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

// std includes
#include <memory>
#include <string>

namespace cmm
{
    class ExpressionNode;

    class ArgNode : public ExpressionNode
    {
    public:

        /**
         * Constructor with a value.
         *
         * @param location the location of this node.
         * @param value the value used as an argument.
         */
        ArgNode(const Location& location, std::unique_ptr<ExpressionNode>&& value) CMM_NOEXCEPT;

        /**
         * Copy constructor
         */
        ArgNode(const ArgNode&) = delete;

        /**
         * Move constructor
         */
        ArgNode(ArgNode&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor
         */
        ~ArgNode() = default;

        /**
         * Copy assignment operator.
         */
        ArgNode& operator= (const ArgNode&) = delete;

        /**
         * Move assignment operator.
         */
        ArgNode& operator= (ArgNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the expression in this argument.
         *
         * @return EnumCType.
         */
        ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the expression in this argument.
         *
         * @return EnumCType.
         */
        const ExpressionNode* getExpression() const CMM_NOEXCEPT;

        /**
         * Gets the underlying CType.
         *
         * @return CType.
         */
        CType& getDatatype() CMM_NOEXCEPT override;

        /**
         * Gets the underlying CType.
         *
         * @return CType.
         */
        const CType& getDatatype() const CMM_NOEXCEPT override;

        /**
         * Sets the DataType since it may be 'lazy loaded'.
         *
         * @param type the DataType to set.
         */
        void setDatatype(const CType& datatype) CMM_NOEXCEPT override;

        /**
         * Adds a DerefNode to the underlying Expression.
         * Note: This function does zero checking and assumes the caller
         *       has already validated this operation.
         */
        void derefNode();

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The value used in this arg
        std::unique_ptr<ExpressionNode> value;

    };
}

#endif //!CMM_ARG_H

