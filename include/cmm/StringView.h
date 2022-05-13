/**
 * Represents a single token fed to the lexer.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

#pragma once
#ifndef CMM_STRING_VIEW_H
#define CMM_STRING_VIEW_H

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
        StringView(StringView&&) noexcept = default;
        ~StringView() = default;
        
        StringView& operator= (const StringView&) = default;
        StringView& operator= (StringView&&) noexcept = default;

        const char* get() const noexcept;
        std::size_t size() const noexcept;

    private:
        const char* str;
        std::size_t len;
    };
}

#endif //!CMM_STRING_VIEW_H

