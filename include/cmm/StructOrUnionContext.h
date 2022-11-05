/**
 * A class for handling struct and union metadata used primarily during analysis.
 *
 * @author hockeyhurd
 * @version 2022-10-17
 */

#pragma once

#ifndef CMM_STRUCT_UNION_CONTEXT_H
#define CMM_STRUCT_UNION_CONTEXT_H

// Our includes
#include <cmm/Types.h>

namespace cmm
{
    class StructOrUnionContext
    {
    public:

        /**
         * Constructor.
         *
         * @param locality the spacial location of the variable.
         * @param modifiers the modifiers used on the variable.
         */
        StructOrUnionContext(const EnumLocality locality, const EnumModifier modifiers) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        StructOrUnionContext(const StructOrUnionContext&) CMM_NOEXCEPT = default;

        /**
         * Move constructor.
         */
        StructOrUnionContext(StructOrUnionContext&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~StructOrUnionContext() = default;

        /**
         * Copy assignment operator.
         *
         * @return StructOrUnionContext reference.
         */
        StructOrUnionContext& operator= (const StructOrUnionContext&) CMM_NOEXCEPT = default;

        /**
         * Move assignment operator.
         *
         * @return StructOrUnionContext reference.
         */
        StructOrUnionContext& operator= (StructOrUnionContext&&) CMM_NOEXCEPT = default;

        /**
         * Gets the EnumLocality of this variable.
         *
         * @return EnumLocality.
         */
        EnumLocality getLocality() const CMM_NOEXCEPT;

        /**
         * Gets the EnumModifier used with this variable.
         *
         * @return EnumModifier.
         */
        EnumModifier getModifiers() const CMM_NOEXCEPT;

    private:

        // The spacial location of the variable.
        EnumLocality locality;

        // The modifiers of the variable.
        EnumModifier modifiers;
    };
}

#endif //!CMM_STRUCT_UNION_CONTEXT_H


