/**
 * A class creating lightweight spans, but geared towards strings.
 *
 * @author hockeyhurd
 * @version 2023-01-26
 */

#pragma once

#ifndef CMM_STRING_VIEW_H
#define CMM_STRING_VIEW_H

// Our includes
#include <cmm/Types.h>
#include <cmm/platform/Architecture.h>

// std includes
#include <cstddef>
#include <string>

namespace cmm
{
    template<class T>
    class StringView
    {
    public:

        /**
         * Default constructor for compatibility.
         * Note: Constructing via this constructor yields an INVALID
         *       StringView object.
         */
        StringView() CMM_NOEXCEPT;

        /**
         * Constructor.
         *
         * @param str pointer to the start of a string.
         * @param length std::size_t length of the view.
         */
        StringView(const T* str, const std::size_t length) CMM_NOEXCEPT;

        /**
         * Constructor.
         *
         * @param str std::basic_string<T> string.  The length shall be the size of the string + 1.
         */
        explicit StringView(const std::basic_string<T>& str) CMM_NOEXCEPT;

        /**
         * Constructor.
         *
         * @param str std::basic_string<T> string.
         * @param length std::size_t length of the view.
         */
        StringView(const std::basic_string<T>& str, const std::size_t length) CMM_NOEXCEPT;

        /**
         * Constructor.
         * Note: If the offset is greater than the size of the string, an
         * std::out_of_range exception shall be thrown.
         *
         * @param str std::basic_string<T> string.
         * @param offset std::size_t starting offset of the view.
         * @param length std::size_t length of the view.
         */
        StringView(const std::basic_string<T>& str, const std::size_t offset, const std::size_t length);

        /**
         * Default copy constructor.
         */
        StringView(const StringView<T>&) CMM_NOEXCEPT = default;

        /**
         * Default move constructor.
         * Note: Implemented same as copy constructor currently.
         */
        StringView(StringView<T>&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        ~StringView() = default;

        /**
         * Default copy assignment operator.
         */
        StringView<T>& operator= (const StringView<T>&) CMM_NOEXCEPT = default;

        /**
         * Default copy assignment operator.
         * Note: Implemented same as copy assignment operator currently.
         */
        StringView<T>& operator= (StringView<T>&&) CMM_NOEXCEPT = default;

        /**
         * Gets a reference to the underlying char if in range, else is undefined behavior.
         *
         * @param index the index of the character to get.
         * @return const reference to the underlying char if in range.
         */
        const T& operator[] (std::size_t index) const;

        /**
         * Gets the underlying pointer to the start of the string.
         * @Note: There is no gurantee this is null-terminated.
         *
         * @return const pointer to string of type T.
         */
        const T* string() const CMM_NOEXCEPT;

        /**
         * Gets the length of the StringView.
         *
         * @return std::size_t length.
         */
        std::size_t size() const CMM_NOEXCEPT;

        /**
         * Gets a reference to the underlying char if in range, else will throw an exception.
         *
         * @param index the index of the character to get.
         * @return const reference to the underlying char if in range.
         */
        const T& at(std::size_t index) const;

        /**
         * Creates a copy as a std::basic_string.
         *
         * @return std::basic_string<T> copy.
         */
        std::basic_string<T> toString() const;

        /**
         * Checks if this StringView is equal to another.
         *
         * @param other the other StringView to compare.
         * @return bool true if equal.
         */
        bool operator== (const StringView<T>& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is equal to a std::string.
         *
         * @param other the other std::string to compare.
         * @return bool true if equal.
         */
        bool operator== (const std::string& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is equal to a c-string.
         *
         * @param other the other std::string to compare.
         * @return bool true if equal.
         */
        bool operator== (const T* other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is not equal to another.
         *
         * @param other the other StringView to compare.
         * @return bool true if not equal.
         */
        bool operator!= (const StringView<T>& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is not equal to a std::string.
         *
         * @param other the other StringView to compare.
         * @return bool true if not equal.
         */
        bool operator!= (const std::string& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is not equal to a c-string.
         *
         * @param other the other StringView to compare.
         * @return bool true if not equal.
         */
        bool operator!= (const T* other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is less than another StringView.
         *
         * @param other the other StringView to compare.
         * @return bool true if less than another StringView.
         */
        bool operator< (const StringView<T>& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is less than to a std::string.
         *
         * @param other the other StringView to compare.
         * @return bool true if less than to a std::string.
         */
        bool operator< (const std::string& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is less than to a c-string.
         *
         * @param other the other StringView to compare.
         * @return bool true if less than to a c-string StringView.
         */
        bool operator< (const T* other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is less than or equal to another StringView.
         *
         * @param other the other StringView to compare.
         * @return bool true if less than or equal to another StringView.
         */
        bool operator<= (const StringView<T>& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is less than or equal to a std::string.
         *
         * @param other the other StringView to compare.
         * @return bool true if less than or equal to a std::string.
         */
        bool operator<= (const std::string& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is less than or equal to a c-string.
         *
         * @param other the other StringView to compare.
         * @return bool true if less than or equal to a c-string.
         */
        bool operator<= (const T* other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is greater than another StringView.
         *
         * @param other the other StringView to compare.
         * @return bool true if greater than another StringView.
         */
        bool operator> (const StringView<T>& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is greater than a std::string.
         *
         * @param other the other StringView to compare.
         * @return bool true if greater than a std::string.
         */
        bool operator> (const std::string& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is greater than a c-string.
         *
         * @param other the other StringView to compare.
         * @return bool true if greater than a c-string.
         */
        bool operator> (const T* other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is greater than or equal to another StringView.
         *
         * @param other the other StringView to compare.
         * @return bool true if greater than or equal to another StringView.
         */
        bool operator>= (const StringView<T>& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is greater than or equal to a std::string.
         *
         * @param other the other StringView to compare.
         * @return bool true if greater than or equal to a std::string.
         */
        bool operator>= (const std::string& other) const CMM_NOEXCEPT;

        /**
         * Checks if this StringView is greater than or equal to a c-string.
         *
         * @param other the other StringView to compare.
         * @return bool true if greater than or equal to a c-string.
         */
        bool operator>= (const T* other) const CMM_NOEXCEPT;

    private:

        // The pointer to the start of the string.
        const T* str;

        // The length of the view.
        std::size_t length;
    };
}

template<class OutStream, class T>
OutStream& operator<< (OutStream& os, const cmm::StringView<T>& str)
{
    const auto len = str.size();

    if (len > 0)
    {
        for (std::size_t i = 0; i < len; ++i)
        {
            os << str[i];
        }

        // Add a null-termination character for compatibility.
        os << '\0';
    }

    return os;
}

template<class T>
struct std::hash<cmm::StringView<T>>
{
    std::size_t operator() (const cmm::StringView<T>& view) const CMM_NOEXCEPT
    {
#if CMM_ARCH_NATIVE_BIT_SIZE == 32
        std::size_t result = static_cast<std::size_t>(3323198485u);
        CMM_CONSTEXPR std::size_t constVal = 0x5BD1E995;
        CMM_CONSTEXPR std::size_t constShiftVal = 15;
#elif CMM_ARCH_NATIVE_BIT_SIZE == 64
        std::size_t result = static_cast<std::size_t>(525201411107845655u);
        CMM_CONSTEXPR std::size_t constVal = 0x5BD1E9955BD1E995;
        CMM_CONSTEXPR std::size_t constShiftVal = 47;
#else
#error "Architecture currently not supported"
#endif

        for (std::size_t i = 0; i < view.size(); ++i)
        {
            result ^= view[i];
            result *= constVal;
            result ^= result >> constShiftVal;
        }

        return result;
    }
};

#ifndef CMM_STRING_VIEW_INL
#define CMM_STRING_VIEW_INL
#include <cmm/container/StringView.inl>
#endif //!CMM_STRING_VIEW_INL

#endif //!CMM_STRING_VIEW_H

