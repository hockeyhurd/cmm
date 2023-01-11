/**
 * A class for representing an struct or union member field access in an AST.
 *
 * @author hockeyhurd
 * @version 2023-01-10
 */

#pragma once

#ifndef CMM_FIELD_ACCESS_NODE_H
#define CMM_FIELD_ACCESS_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>
#include <cmm/Field.h>

namespace cmm
{
    class FieldAccessNode : public ExpressionNode
    {
    public:

        /**
         * Constructor with copy semantics.
         *
         * @param location the Location of this node.
         * @param name std::string name of the field.
         * @param datatype the CType for the field.
         * @param index the index of the field within it's struct.
         */
        FieldAccessNode(const Location& location, const std::string& name, const CType& datatype, const u32 index);

        /**
         * Constructor with move semantics.
         *
         * @param location the Location of this node.
         * @param name std::string name of the field.
         * @param datatype the CType for the field.
         * @param index the index of the field within it's struct.
         */
        FieldAccessNode(const Location& location, std::string&& name, CType&& datatype, const u32 index) CMM_NOEXCEPT;

        /**
         * Default copy constructor.
         */
        FieldAccessNode(const FieldAccessNode&) = default;

        /**
         * Default move constructor.
         */
        FieldAccessNode(FieldAccessNode&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        ~FieldAccessNode() = default;

        /**
         * Default copy assignment operator.
         */
        FieldAccessNode& operator= (const FieldAccessNode&) = default;

        /**
         * Default move assignment operator.
         */
        FieldAccessNode& operator= (FieldAccessNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the underlying Field object.
         *
         * @return reference to the wrapped Field.
         */
        Field& getField() CMM_NOEXCEPT;

        /**
         * Gets the underlying Field object.
         *
         * @return const reference to the wrapped Field.
         */
        const Field& getField() const CMM_NOEXCEPT;

        /**
         * Gets the std::string name of this FieldAccessNode.
         *
         * @return std::string reference.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the std::string name of this FieldAccessNode.
         *
         * @return std::string const reference.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets the CType of this FieldAccessNode.
         *
         * @return CType reference.
         */
        CType& getDatatype() CMM_NOEXCEPT override;

        /**
         * Gets the CType of this FieldAccessNode.
         *
         * @return CType const reference.
         */
        const CType& getDatatype() const CMM_NOEXCEPT override;

        /**
         * Gets the index of the FieldAccessNode within its struct.
         *
         * @return u32 index.
         */
        u32 getIndex() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The field we are wrapping.
        Field field;
    };
}

#endif //!CMM_FIELD_ACCESS_NODE_H

