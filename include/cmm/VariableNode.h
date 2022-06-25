/**
 * An AST node for variable ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#pragma once

#ifndef CMM_VARIABLE_NODE_H
#define CMM_VARIABLE_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>

// std includes
// #include <optional>
#include <string>

namespace cmm
{
    class Token;

    class VariableNode : public Node
    {
    public:

        /**
         * Default constructor.
         *
         * @param name the std::string name of the variable.
         */
        explicit VariableNode(const std::string& name);

        /**
         * Default constructor.
         *
         * @param name the std::string name of the variable.
         */
        explicit VariableNode(std::string&& name) noexcept;

        /**
         * Copy constructor.
         */
        VariableNode(const VariableNode&) = default;

        /**
         * Move constructor.
         */
        VariableNode(VariableNode&&) noexcept = default;

        /**
         * Destructor
         */
        ~VariableNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return VariableNode reference.
         */
        VariableNode& operator= (const VariableNode&) = default;

        /**
         * Move assignment operator.
         *
         * @return VariableNode reference.
         */
        VariableNode& operator= (VariableNode&&) noexcept = default;

        /**
         * Gets the name of this variable.
         *
         * @return std::string.
         */
        std::string& getName();

        /**
         * Gets the name of this variable.
         *
         * @return std::string.
         */
        const std::string& getName() const;

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

        // The name of the variable.
        std::string name;
    };
}

#endif //!CMM_VARIABLE_NODE_H

