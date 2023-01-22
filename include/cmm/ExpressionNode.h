/**
 * An AST node for base expression ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-28
 */

#pragma once

#ifndef CMM_EXPRESSION_H
#define CMM_EXPRESSION_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>

// std includes
#include <string>

namespace cmm
{
    class ExpressionNode : public Node
    {
    protected:

        /**
         * Default constructor.
         */
        ExpressionNode(const EnumNodeType type, const Location& location) CMM_NOEXCEPT;

        /**
         * Default constructor.
         */
        ExpressionNode(const EnumNodeType type, const Location& location, const CType& datatype) CMM_NOEXCEPT;

    public:

        /**
         * Copy constructor.
         */
        ExpressionNode(const ExpressionNode&) = default;

        /**
         * Move constructor.
         */
        ExpressionNode(ExpressionNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        virtual ~ExpressionNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return ExpressionNode reference.
         */
        ExpressionNode& operator= (const ExpressionNode&) = default;

        /**
         * Move assignment operator.
         *
         * @return ExpressionNode reference.
         */
        ExpressionNode& operator= (ExpressionNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the underlying CType.
         *
         * @return CType.
         */
        virtual CType& getDatatype() CMM_NOEXCEPT;

        /**
         * Gets the underlying CType.
         *
         * @return CType.
         */
        virtual const CType& getDatatype() const CMM_NOEXCEPT;

        /**
         * Sets the DataType since it may be 'lazy loaded'.
         *
         * @param type the DataType to set.
         */
        virtual void setDatatype(const CType& datatype);

        virtual VisitorResult accept(Visitor* visitor) override = 0;
        virtual std::string toString() const override;

    protected:

        // The underlying CType.
        CType datatype;

    };
}

#endif //!CMM_EXPRESSION_H

