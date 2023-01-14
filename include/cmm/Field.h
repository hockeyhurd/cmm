/**
 * A class for representing a struct or union's field.
 *
 * @author hockeyhurd
 * @version 2023-01-08
 */

#pragma once

#ifndef CMM_FIELD_H
#define CMM_FIELD_H

// Our includes
#include <cmm/Types.h>

namespace cmm
{
    class Field
    {
    public:

        /**
         * Constructor with copy semantics.
         *
         * @param name std::string name of the field.
         * @param datatype the CType for the field.
         * @param index the index of the field within it's struct.
         */
        Field(const std::string& name, const CType& datatype, const s32 index);

        /**
         * Constructor with move semantics.
         *
         * @param name std::string name of the field.
         * @param datatype the CType for the field.
         * @param index the index of the field within it's struct.
         */
        Field(std::string&& name, CType&& datatype, const s32 index) CMM_NOEXCEPT;

        /**
         * Default copy constructor.
         */
        Field(const Field&) = default;

        /**
         * Default move constructor.
         */
        Field(Field&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        ~Field() = default;

        /**
         * Default copy assignment operator.
         */
        Field& operator= (const Field&) = default;

        /**
         * Default move assignment operator.
         */
        Field& operator= (Field&&) CMM_NOEXCEPT = default;

        /**
         * Gets the std::string name of this Field.
         *
         * @return std::string reference.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the std::string name of this Field.
         *
         * @return std::string const reference.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets the CType of this Field.
         *
         * @return CType reference.
         */
        CType& getDatatype() CMM_NOEXCEPT;

        /**
         * Gets the CType of this Field.
         *
         * @return CType const reference.
         */
        const CType& getDatatype() const CMM_NOEXCEPT;

        /**
         * Gets the index of the Field within its struct.
         *
         * @return s32 index.
         */
        s32 getIndex() const CMM_NOEXCEPT;

    private:

        std::string name;
        CType datatype;
        s32 index;
    };
}

#endif //!CMM_FIELD_H

