/**
 * Represents snapshot between two pointers in a lexer token stream.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

#pragma once
#ifndef CMM_STRING_VIEW_H
#define CMM_STRING_VIEW_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <cstdint>

namespace cmm
{
    class StringView
    {
    public:
        StringView();
        StringView(const char* str, const std::size_t len);
        StringView(const char* start, const char* end);
        StringView(const StringView&) = default;
        StringView(StringView&&) CMM_NOEXCEPT = default;
        ~StringView() = default;
        
        StringView& operator= (const StringView&) = default;
        StringView& operator= (StringView&&) CMM_NOEXCEPT = default;

        const char* get() const CMM_NOEXCEPT;
        std::size_t size() const CMM_NOEXCEPT;

    private:
        const char* str;
        std::size_t len;
    };
}

#endif //!CMM_STRING_VIEW_H

