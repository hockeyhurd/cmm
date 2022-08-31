/**
 * A class for handling variable metadata used primarily during analysis.
 *
 * @author hockeyhurd
 * @version 2022-08-31
 */

#pragma once

#ifndef CMM_VARIABLE_CONTEXT_H
#define CMM_VARIABLE_CONTEXT_H

// Our includes
#include <cmm/Types.h>

namespace cmm
{
    enum class EnumLocality
    {
        GLOBAL = 0, LOCAL, PARAMETER
    };

    class VariableContext
    {
    public:

        /**
         * Constructor.
         *
         * @param type the EnumCType of the variable.
         * @param locality the spacial location of the variable.
         */
        VariableContext(const EnumCType type, const EnumLocality locality) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        VariableContext(const VariableContext&) CMM_NOEXCEPT = default;

        /**
         * Move constructor.
         */
        VariableContext(VariableContext&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~VariableContext() = default;

        /**
         * Copy assignment operator.
         *
         * @return VariableContext reference.
         */
        VariableContext& operator= (const VariableContext&) CMM_NOEXCEPT = default;

        /**
         * Move assignment operator.
         *
         * @return VariableContext reference.
         */
        VariableContext& operator= (VariableContext&&) CMM_NOEXCEPT = default;

        /**
         * Gets the EnumCType of this variable.
         *
         * @return EnumCType.
         */
        EnumCType getType() const CMM_NOEXCEPT;

        /**
         * Gets the EnumLocality of this variable.
         *
         * @return EnumLocality.
         */
        EnumLocality getLocality() const CMM_NOEXCEPT;

    private:

        // The type of the variable.
        EnumCType type;

        // The spacial location of the variable.
        EnumLocality locality;
    };
}

#endif //!CMM_VARIABLE_CONTEXT_H

