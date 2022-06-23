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
#include <cmm/Node.h>

namespace cmm
{
    // TODO: Consider moving this to sub-classes based on the actual type
    // as we start using this class more often.
    class LitteralNode : public Node
    {
    public:

        /**
         * Default constructor for void pointer type.
         */
        LitteralNode(void* value) noexcept;

        /**
         * Default constructor for bool type.
         */
        LitteralNode(const bool value) noexcept;

        /**
         * Default constructor for char type.
         */
        LitteralNode(const char value) noexcept;

        /**
         * Default constructor for s8 type.
         */
        LitteralNode(const s8 value) noexcept;

        /**
         * Default constructor for s16 type.
         */
        LitteralNode(const s16 value) noexcept;

        /**
         * Default constructor for s32 type.
         */
        LitteralNode(const s32 value) noexcept;

        /**
         * Default constructor for s64 type.
         */
        LitteralNode(const s64 value) noexcept;

        /**
         * Default constructor for f32 type.
         */
        LitteralNode(const f32 value) noexcept;

        /**
         * Default constructor for f64 type.
         */
        LitteralNode(const f64 value) noexcept;

        /**
         * Default constructor for c-strings type.
         */
        LitteralNode(const char* value) noexcept;

        /**
         * Copy constructor.
         */
        LitteralNode(const LitteralNode&) noexcept = default;

        /**
         * Move constructor.
         */
        LitteralNode(LitteralNode&&) noexcept = default;

        /**
         * Destructor
         */
        ~LitteralNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return LitteralNode reference.
         */
        LitteralNode& operator= (const LitteralNode&) noexcept = default;

        /**
         * Move assignment operator.
         *
         * @return LitteralNode reference.
         */
        LitteralNode& operator= (LitteralNode&&) noexcept = default;

        /**
         * Get the type of this litteral.
         *
         * @return EnumCType.
         */
        EnumCType getTypeof() const noexcept;

        /**
         * Get the underly value.
         *
         * @return CType.
         */
        CType getValue() const noexcept;

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

        CType value;
        EnumCType type;
    };
}

#endif //!CMM_LITTERAL_NODE_H

