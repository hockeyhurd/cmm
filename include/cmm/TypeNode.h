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
         * @param type the EnumCType.
         */
        TypeNode(const Location& location, const EnumCType type) CMM_NOEXCEPT;

        /**
         * Default constructor.
         *
         * @param location the location of this node.
         * @param type the EnumCType.
         * @param pointerInderectionCount the amount of pointer inderection or dimensions.
         */
        TypeNode(const Location& location, const EnumCType type, const u32 pointerInderectionCount) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        TypeNode(const TypeNode&) CMM_NOEXCEPT = default;

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
         * @return EnumCType.
         */
        EnumCType getDatatype() const CMM_NOEXCEPT;

        /**
         * The number of pointer inderection.
         */
        u32 getDimensions() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the datatype.
        EnumCType type;

        // a.k.a. the number of dimensions.
        u32 pointerInderectionCount;
    };
}

#endif //!CMM_TYPES_NODE_H

