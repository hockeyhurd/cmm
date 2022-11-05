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
#include <cmm/EnumNodeType.h>
#include <cmm/Location.h>
#include <cmm/visit/Visitor.h>

// std includes
#include <memory>
#include <string>

namespace cmm
{
    class Node
    {
    protected:

        /**
         * Default constructor that can only be constructed by a derived type.
         */
        Node(const EnumNodeType type, const Location& location) CMM_NOEXCEPT;

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
         * @return EnumNodeType.
         */
        virtual EnumNodeType getType() const CMM_NOEXCEPT;

        /**
         * Set the type of this node.
         *
         * @param type EnumNodeType.
         */
        virtual void setType(const EnumNodeType type) CMM_NOEXCEPT;

        /**
         * Get the location of this node.
         *
         * @return Location.
         */
        virtual Location& getLocation() CMM_NOEXCEPT;

        /**
         * Get the location of this node.
         *
         * @return Location.
         */
        virtual const Location& getLocation() const CMM_NOEXCEPT;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        virtual VisitorResult accept(Visitor* visitor) = 0;

        /**
         * Gets a string representation of this node. Typically, this is just
         * the name of the class.
         *
         * @return std::string
         */
        virtual std::string toString() const;

    protected:

        // The type of this Node
        EnumNodeType type;

        // The Location of this Node.
        Location location;
    };
}

#endif //!CMM_NODE_H

