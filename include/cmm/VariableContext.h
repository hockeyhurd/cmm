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
    class VariableContext
    {
    public:

        /**
         * Constructor.
         *
         * @param type the CType of the variable.
         * @param locality the spacial location of the variable.
         * @param modifiers the modifiers used on the variable.
         */
        VariableContext(const CType& type, const EnumLocality locality, const EnumModifier modifiers) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        VariableContext(const VariableContext&) = default;

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
         * Gets the CType of this variable.
         *
         * @return CType.
         */
        CType& getType() CMM_NOEXCEPT;

        /**
         * Gets the CType of this variable.
         *
         * @return CType.
         */
        const CType& getCType() const CMM_NOEXCEPT;

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

        /**
         * Sets the EnumModifier used with this variable.
         *
         * @param modifiers The new modifier value.
         */
        void setModifiers(const EnumModifier modifiers) CMM_NOEXCEPT;

        /**
         * Gets the state of the dirty bit.
         *
         * @return bool true if variable was recently modified, otherwise false.
         */
        bool getDirtyBit() const CMM_NOEXCEPT;

        /**
         * Allows for setting the dirty bit's value.
         *
         * @param dirtyBit bool.
         */
        void setDirtyBit(const bool dirtyBit) CMM_NOEXCEPT;

    private:

        // The type of the variable.
        CType type;

        // The spacial location of the variable.
        EnumLocality locality;

        // The modifiers of the variable.
        EnumModifier modifiers;

        // Used for tracking when a variable has changed and the compiler should
        // consider re-reading from memory for the latest value of this variable.
        bool dirtyBit;
    };
}

#endif //!CMM_VARIABLE_CONTEXT_H

