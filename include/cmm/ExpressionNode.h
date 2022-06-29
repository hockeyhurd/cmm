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
        ExpressionNode(const NodeType type) CMM_NOEXCEPT;

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
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(*this));
        }

        virtual std::string toString() const override;

    };
}

#endif //!CMM_EXPRESSION_H

