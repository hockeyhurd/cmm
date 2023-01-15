/**
 * An interface for representing a struct or union's field.
 *
 * @author hockeyhurd
 * @version 2023-01-14
 */

#pragma once

#ifndef CMM_IFIELD_H
#define CMM_IFIELD_H

// Our includes
#include <cmm/Types.h>

namespace cmm
{
    class IField
    {
    protected:

        /**
         * Default constructor.
         */
        IField() = default;

        /**
         * Default copy constructor.
         */
        IField(const IField&) = default;

        /**
         * Default move constructor.
         */
        IField(IField&&) CMM_NOEXCEPT = default;

        /**
         * Default copy assignment operator.
         */
        IField& operator= (const IField&) = default;

        /**
         * Default move assignment operator.
         */
        IField& operator= (IField&&) CMM_NOEXCEPT = default;

    public:

        /**
         * Default destructor.
         */
        virtual ~IField() = default;

        /**
         * Gets the std::string name of this IField.
         *
         * @return std::string reference.
         */
        virtual std::string& getName() CMM_NOEXCEPT = 0;

        /**
         * Gets the std::string name of this IField.
         *
         * @return std::string const reference.
         */
        virtual const std::string& getName() const CMM_NOEXCEPT = 0;

        /**
         * Gets the CType of this IField.
         *
         * @return CType reference.
         */
        virtual CType& getDatatype() CMM_NOEXCEPT = 0;

        /**
         * Gets the CType of this IField.
         *
         * @return CType const reference.
         */
        virtual const CType& getDatatype() const CMM_NOEXCEPT = 0;

        /**
         * Sets the CType of this IField.
         *
         * @param datatype the CType to set.
         */
        virtual void setDatatype(const CType& datatype) = 0;

        /**
         * Gets the index of the IField within its struct.
         *
         * @return s32 index.
         */
        virtual s32 getIndex() const CMM_NOEXCEPT = 0;

        /**
         * Gets the index of the IField within its struct.
         *
         * @param index the s32 index of the field.
         */
        virtual void setIndex(s32 index) CMM_NOEXCEPT = 0;

        /**
         * Sets this IField based upon another's IField.
         *
         * @param other the other IField to set from.
         *        Note: This parameter shall ALWAYS be a valid pointer.
         */
        virtual void set(const IField* other) = 0;
    };
}

#endif //!CMM_IFIELD_H

