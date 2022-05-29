/**
 * Represents snapshot between two pointers in a lexer token stream.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

#include <cmm/StringView.h>

namespace cmm
{

    StringView::StringView() : str(nullptr), len(0)
    {
    }

    StringView::StringView(const char* str, const std::size_t len) : str(str), len(len)
    {
    }

    StringView::StringView(const char* start, const char* end) : str(start), len(end - start)
    {
    }

    const char* StringView::get() const noexcept
    {
        return str;
    }

    std::size_t StringView::size() const noexcept
    {
        return len;
    }
}

