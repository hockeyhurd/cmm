/**
 * An AST node for type ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#pragma once

#ifndef CMM_TYPES_NODE_H
#define CMM_TYPES_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>

namespace cmm
{
    class TypeNode : public Node
    {
    public:

        /**
         * Default constructor.
         *
         * @param location the location of this node.
         * @param type the CType.
         */
        TypeNode(const Location& location, const CType& type) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        TypeNode(const TypeNode&) = default;

        /**
         * Move constructor.
         */
        TypeNode(TypeNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~TypeNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return TypeNode reference.
         */
        TypeNode& operator= (const TypeNode&) CMM_NOEXCEPT = default;

        /**
         * Move assignment operator.
         *
         * @return TypeNode reference.
         */
        TypeNode& operator= (TypeNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the datatype.
         *
         * @return reference to CType.
         */
        CType& getDatatype() CMM_NOEXCEPT;

        /**
         * Gets the datatype.
         *
         * @return const reference to CType.
         */
        const CType& getDatatype() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the datatype.
        CType type;
    };
}

#endif //!CMM_TYPES_NODE_H

