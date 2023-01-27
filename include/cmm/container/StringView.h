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

// std includes
#include <cstddef>

namespace cmm
{
    template<class T>
    class StringView
    {
    public:

        /**
         * Constructor.
         *
         * @param str pointer to the start of a string.
         * @param length std::size_t length of the view.
         */
        StringView(const T* str, const std::size_t length) CMM_NOEXCEPT;

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

#ifndef CMM_STRING_VIEW_INL
#define CMM_STRING_VIEW_INL
#include <cmm/container/StringView.inl>
#endif //!CMM_STRING_VIEW_INL

#endif //!CMM_STRING_VIEW_H

