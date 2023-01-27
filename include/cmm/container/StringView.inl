/**
 * A class creating lightweight spans, but geared towards strings.
 *
 * @author hockeyhurd
 * @version 2023-01-26
 */

#define TEMPLATE_T template<class T>
#define CLASS_T StringView<T>

namespace cmm
{
    TEMPLATE_T
    CLASS_T::StringViewconst T* str, const std::size_t length) CMM_NOEXCEPT : str(str), length(length)
    {
    }

    TEMPLATE_T
    const T& CLASS_T::operator[] (std::size_t index) const
    {
        return str[index];
    }

    TEMPLATE_T
    const T& CLASS_T::at(std::size_t index) const
    {
        if (index >= length)
        {
            throw std::out_of_range();
        }

        return str[index];
    }
}

#undef TEMPLATE_T
#undef CLASS_T

