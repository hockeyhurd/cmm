/**
 * An AST node for litteral types.
 *
 * @author hockeyhurd
 * @version 2022-06-16
 */

#pragma once

#ifndef CMM_LITTERAL_H
#define CMM_LITTERAL_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <memory>

namespace cmm
{
    class LitteralNode : public std::enable_shared_from_this<LitteralNode>
    {
    public:

        /**
         * Default constructor for void pointer type.
         */
        LitteralNode(void* value) noexcept;

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
        LitteralNode(char* value); // TODO: Consider making this 'noexcept'.

        /**
         * Copy constructor.
         */
        LitteralNode(const LitteralNode&) = default;

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
        LitteralNode& operator= (const LitteralNode&) = default;

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
        EnumCType getCType() const noexcept;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(shared_from_this()));
        }

    private:

        CType value;
        EnumCType type;
    };
}

#endif //!CMM_LITTERAL_H

