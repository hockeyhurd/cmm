/**
 * An AST node for litteral ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-16
 */

#pragma once

#ifndef CMM_LITTERAL_NODE_H
#define CMM_LITTERAL_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

namespace cmm
{
    // TODO: Consider moving this to sub-classes based on the actual type
    // as we start using this class more often.
    class LitteralNode : public ExpressionNode
    {
    public:

        /**
         * Default constructor for NULL type.
         *
         * @param location the location of this node.
         */
        explicit LitteralNode(const Location& location) CMM_NOEXCEPT;

        /**
         * Default constructor for void pointer type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, void* value) CMM_NOEXCEPT;

        /**
         * Default constructor for bool type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const bool value) CMM_NOEXCEPT;

        /**
         * Default constructor for char type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const char value) CMM_NOEXCEPT;

        /**
         * Default constructor for enum type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const EnumEnum value) CMM_NOEXCEPT;

        /**
         * Default constructor for s8 type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const s8 value) CMM_NOEXCEPT;

        /**
         * Default constructor for s16 type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const s16 value) CMM_NOEXCEPT;

        /**
         * Default constructor for s32 type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const s32 value) CMM_NOEXCEPT;

        /**
         * Default constructor for s64 type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const s64 value) CMM_NOEXCEPT;

        /**
         * Default constructor for f32 type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const f32 value) CMM_NOEXCEPT;

        /**
         * Default constructor for f64 type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const f64 value) CMM_NOEXCEPT;

        /**
         * Default constructor for c-strings type.
         *
         * @param location the location of this node.
         */
        LitteralNode(const Location& location, const char* value) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        LitteralNode(const LitteralNode&) CMM_NOEXCEPT = default;

        /**
         * Move constructor.
         */
        LitteralNode(LitteralNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~LitteralNode();

        /**
         * Copy assignment operator.
         *
         * @return LitteralNode reference.
         */
        LitteralNode& operator= (const LitteralNode&) CMM_NOEXCEPT = default;

        /**
         * Move assignment operator.
         *
         * @return LitteralNode reference.
         */
        LitteralNode& operator= (LitteralNode&&) CMM_NOEXCEPT = default;

        /**
         * Get the underly value.
         *
         * @return CTypeValue.
         */
        CTypeValue getValue() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

        std::string toString() const override;

        /**
         * Gets an appropriate "null" values based upon the CType type.
         *
         * @param location the Location for this LitteralNode.
         * @param type the CType to deduce.
         */
        static LitteralNode getNullForType(const Location& location, const CType& type);

    private:

        CTypeValue value;
    };
}

#endif //!CMM_LITTERAL_NODE_H

