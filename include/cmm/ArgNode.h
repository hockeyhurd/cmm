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
#include <cmm/Node.h>
#include <cmm/VariableNode.h>

// std includes
#include <string>

namespace cmm
{
    class ArgNode : public Node
    {
    public:

        /**
         * Default constructor
         */
        ArgNode(VariableNode&& variable) CMM_NOEXCEPT;

        /**
         * Copy constructor
         */
        ArgNode(const ArgNode&) = default;

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
        ArgNode& operator= (const ArgNode&) = default;

        /**
         * Move assignment operator.
         */
        ArgNode& operator= (ArgNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the name of this argument.
         *
         * @return std::string.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of this argument.
         *
         * @return std::string.
         */
        const std::string& getName() const CMM_NOEXCEPT;

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

        // The variable used in this arg
        VariableNode variable;

    };
}

#endif //!CMM_ARG_H

