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
    // TODO: Still needed??
    enum class NodeType
    {
        UNKNOWN = 0, COMPILATION_UNIT, BIN_OP, LITTERAL, VARIABLE
    };

    class Node : public std::enable_shared_from_this<Node>
    {
    protected:

        /**
         * Default constructor that can only be constructed by a derived type.
         */
        Node(const NodeType type) noexcept;

    public:

        /**
         * Copy constructor.
         */
        Node(const Node&) noexcept = default;

        /**
         * Move constructor.
         */
        Node(Node&&) noexcept = default;

        /**
         * Destructor
         */
        virtual ~Node() = default;

        /**
         * Copy assignment operator.
         *
         * @return Node reference.
         */
        Node& operator= (const Node&) noexcept = default;

        /**
         * Move assignment operator.
         *
         * @return Node reference.
         */
        Node& operator= (Node&&) noexcept = default;

        /**
         * Get the type of this node.
         *
         * @return NodeType.
         */
        virtual NodeType getType() const noexcept;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(shared_from_this()));
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

