/**
 * A class creating lightweight spans, but geared towards strings.
 *
 * @author hockeyhurd
 * @version 2023-01-26
 */

// std includes
#include <algorithm>
#include <cstring>
#include <stdexcept>

// Common defines
#define TEMPLATE_T template<class T>
#define CLASS_T StringView<T>

// TODO: Make this less of a magic number:
#define STRNLEN_MAX 0x1000000

namespace cmm
{
    TEMPLATE_T
    CLASS_T::StringView() CMM_NOEXCEPT : str(nullptr), length(0)
    {
    }

    TEMPLATE_T
    CLASS_T::StringView(const T* str, const std::size_t length) CMM_NOEXCEPT : str(str), length(length)
    {
    }

    TEMPLATE_T
    CLASS_T::StringView(const std::basic_string<T>& str) CMM_NOEXCEPT : str(str.c_str()), length(str.size() + 1)
    {
    }

    TEMPLATE_T
    CLASS_T::StringView(const std::basic_string<T>& str, const std::size_t length) CMM_NOEXCEPT :
        str(str.c_str()), length(std::min(length, str.size() + 1))
    {
    }

    TEMPLATE_T
    CLASS_T::StringView(const std::basic_string<T>& str, const std::size_t offset, const std::size_t length)
    {
        if (offset > str.size())
        {
            throw std::out_of_range("Offset exceeds the size of the provided std::basic_string");
        }

        this->str = str.c_str() + offset;
        this->length = std::min(length, str.size() - offset + 1);
    }

    TEMPLATE_T
    const T& CLASS_T::operator[] (std::size_t index) const
    {
        return str[index];
    }

    TEMPLATE_T
    const T* CLASS_T::string() const CMM_NOEXCEPT
    {
        return str;
    }

    TEMPLATE_T
    std::size_t CLASS_T::size() const CMM_NOEXCEPT
    {
        return length;
    }

    TEMPLATE_T
    const T& CLASS_T::at(std::size_t index) const
    {
        if (index >= length)
        {
            throw std::out_of_range("Out of range of StringView");
        }

        return str[index];
    }

    TEMPLATE_T
    std::basic_string<T> CLASS_T::toString() const
    {
        return std::basic_string<T>(str, length);
    }

    TEMPLATE_T
    bool CLASS_T::operator== (const CLASS_T& other) const CMM_NOEXCEPT
    {
        return this == &other || (length == other.length &&
            (str == other.str || std::strncmp(str, other.str, length) == 0));
    }

    TEMPLATE_T
    bool CLASS_T::operator== (const std::string& other) const CMM_NOEXCEPT
    {
        return *this == other.c_str();
    }

    TEMPLATE_T
    bool CLASS_T::operator== (const T* other) const CMM_NOEXCEPT
    {
        return str == other || std::strncmp(str, other, std::min(length, strnlen(other, STRNLEN_MAX))) == 0;
    }

    TEMPLATE_T
    bool CLASS_T::operator!= (const CLASS_T& other) const CMM_NOEXCEPT
    {
        return !(*this == other);
    }

    TEMPLATE_T
    bool CLASS_T::operator!= (const std::string& other) const CMM_NOEXCEPT
    {
        return !(*this == other);
    }

    TEMPLATE_T
    bool CLASS_T::operator!= (const T* other) const CMM_NOEXCEPT
    {
        return !(*this == other);
    }

    TEMPLATE_T
    bool CLASS_T::operator< (const CLASS_T& other) const CMM_NOEXCEPT
    {
        return this != &other && (length < other.length ||
               std::strncmp(str, other.str, std::min(length, other.length)) < 0);
    }

    TEMPLATE_T
    bool CLASS_T::operator< (const std::string& other) const CMM_NOEXCEPT
    {
        return *this < other.c_str();
    }

    TEMPLATE_T
    bool CLASS_T::operator< (const T* other) const CMM_NOEXCEPT
    {
#if 0
        return str != other && (length < other.size() ||
               std::strncmp(str, other, std::min(length, other.length)) < 0);
#else
        if (str != other)
        {
            return false;
        }

        const auto otherSize = strnlen(other, STRNLEN_MAX);
        return length < otherSize || std::strncmp(str, other, std::min(length, otherSize) < 0);
#endif
    }

    TEMPLATE_T
    bool CLASS_T::operator<= (const CLASS_T& other) const CMM_NOEXCEPT
    {
        return *this == other || *this < other;
    }

    TEMPLATE_T
    bool CLASS_T::operator<= (const std::string& other) const CMM_NOEXCEPT
    {
        return *this <= other.c_str();
    }

    TEMPLATE_T
    bool CLASS_T::operator<= (const T* other) const CMM_NOEXCEPT
    {
        return *this == other || *this < other;
    }

    TEMPLATE_T
    bool CLASS_T::operator> (const CLASS_T& other) const CMM_NOEXCEPT
    {
        return !(*this <= other);
    }

    TEMPLATE_T
    bool CLASS_T::operator> (const std::string& other) const CMM_NOEXCEPT
    {
        return !(*this <= other);
    }

    TEMPLATE_T
    bool CLASS_T::operator> (const T* other) const CMM_NOEXCEPT
    {
        return !(*this <= other);
    }

    TEMPLATE_T
    bool CLASS_T::operator>= (const CLASS_T& other) const CMM_NOEXCEPT
    {
        return *this == other || *this > other;
    }

    TEMPLATE_T
    bool CLASS_T::operator>= (const std::string& other) const CMM_NOEXCEPT
    {
        return *this == other || *this > other;
    }

    TEMPLATE_T
    bool CLASS_T::operator>= (const T* other) const CMM_NOEXCEPT
    {
        return *this == other || *this > other;
    }
}

#undef TEMPLATE_T
#undef CLASS_T
#undef STRNLEN_MAX

