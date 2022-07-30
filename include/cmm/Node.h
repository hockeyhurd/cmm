/**
 * A single abstract Node in an AST.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#pragma once

#ifndef CMM_NODE_H
#define CMM_NODE_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <memory>
#include <string>

namespace cmm
{
    // TODO: Consider moving to a seperate file
    enum class NodeType
    {
        UNKNOWN = 0, ARG, COMPILATION_UNIT, BIN_OP, BLOCK,
        FUNCTION_CALL, FUNCTION_DECLARATION_STATEMENT, FUNCTION_DEFINITION_STATEMENT,
        EXPRESSION_STATEMENT, EXPRESSION, IF_ELSE_STATEMENT, PARAMETER, PAREN_EXPRESSION,
        LITTERAL, RETURN_STATEMENT, VARIABLE, VARIABLE_DECLARATION_STATEMENT
    };

    class Node
    {
    protected:

        /**
         * Default constructor that can only be constructed by a derived type.
         */
        Node(const NodeType type) CMM_NOEXCEPT;

    public:

        /**
         * Copy constructor.
         */
        Node(const Node&) CMM_NOEXCEPT = default;

        /**
         * Move constructor.
         */
        Node(Node&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        virtual ~Node() = default;

        /**
         * Copy assignment operator.
         *
         * @return Node reference.
         */
        Node& operator= (const Node&) CMM_NOEXCEPT = default;

        /**
         * Move assignment operator.
         *
         * @return Node reference.
         */
        Node& operator= (Node&&) CMM_NOEXCEPT = default;

        /**
         * Get the type of this node.
         *
         * @return NodeType.
         */
        virtual NodeType getType() const CMM_NOEXCEPT;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(*this));
        }

        /**
         * Gets a string representation of this node. Typically, this is just
         * the name of the class.
         *
         * @return std::string
         */
        virtual std::string toString() const;

    protected:

        // The type of this Node
        NodeType type;
    };
}

#endif //!CMM_NODE_H

