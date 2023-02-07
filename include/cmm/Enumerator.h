/**
 * A class for representing an enum's enumerator.
 *
 * @author hockeyhurd
 * @version 2023-02-04
 */

#pragma once

#ifndef CMM_ENUMERATOR_H
#define CMM_ENUMERATOR_H

// Our includes
#include <cmm/Types.h>

namespace cmm
{
    class Enumerator
    {
    public:

        /**
         * Constructor with copy semantics.
         *
         * @param name std::string name of the enumerator.
         * @param index the index of the enumerator within it's definition.
         */
        Enumerator(const std::string& name, const s32 index);

        /**
         * Constructor with move semantics.
         *
         * @param name std::string name of the enumerator.
         * @param index the index of the enumerator within it's definition.
         */
        Enumerator(std::string&& name, const s32 index) CMM_NOEXCEPT;

        /**
         * Constructor with copy semantics.
         *
         * @param name std::string name of the enumerator.
         * @param index the index of the enumerator within it's definition.
         * @param value the value of the enumerator if defined.
         */
        Enumerator(const std::string& name, const s32 index, const s32 value);

        /**
         * Constructor with move semantics.
         *
         * @param name std::string name of the enumerator.
         * @param index the index of the enumerator within it's definition.
         * @param value the value of the enumerator if defined.
         */
        Enumerator(std::string&& name, const s32 index, const s32 value) CMM_NOEXCEPT;

        /**
         * Default copy constructor.
         */
        Enumerator(const Enumerator&) = default;

        /**
         * Default move constructor.
         */
        Enumerator(Enumerator&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        ~Enumerator() = default;

        /**
         * Default copy assignment operator.
         */
        Enumerator& operator= (const Enumerator&) = default;

        /**
         * Default move assignment operator.
         */
        Enumerator& operator= (Enumerator&&) CMM_NOEXCEPT = default;

        /**
         * Gets the std::string name of this Enumerator.
         *
         * @return std::string reference.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the std::string name of this Enumerator.
         *
         * @return std::string const reference.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets the index of the Enumerator within its definition.
         *
         * @return s32 index.
         */
        s32 getIndex() const CMM_NOEXCEPT;

        /**
         * Gets the index of the Enumerator within its definition.
         *
         * @param index the s32 index of the enumerator.
         */
        void setIndex(s32 index) CMM_NOEXCEPT;

        /**
         * Gets the index of the Enumerator within its definition.
         *
         * @return s32 value.
         */
        s32 getValue() const CMM_NOEXCEPT;

        /**
         * Gets the index of the Enumerator within its definition.
         *
         * @param index the s32 value of the enumerator.
         */
        void setValue(s32 value) CMM_NOEXCEPT;

    private:

        // The name of the Enumerator.
        std::string name;

        // The index of the Enumerator within the definition.
        // Note: A value of less than OR equal to '-1' indicates
        // that the Enumerator's index has not been defined/set (yet).
        s32 index;

        // The value of the Enumerator. If unspecified, the value will be
        // set to the index.
        s32 value;
    };
}

#endif //!CMM_ENUMERATOR_H

