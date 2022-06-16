/**
 * File for storing basic type and macro information used everywhere.
 *
 * @author hockeyhurd
 * @version 2022-06-16
 */

#include <cmm/Types.h>

namespace cmm
{

    CType::CType(void* valueVoidPtr) noexcept : length(sizeof(valueVoidPtr))
    {
        value.valueVoidPtr = valueVoidPtr;
    }

    CType::CType(const char valueChar) noexcept : length(sizeof(valueChar))
    {
        value.valueChar = valueChar;
    }

    CType::CType(const s8 valueS8) noexcept : length(sizeof(valueS8))
    {
        value.valueS8 = valueS8;
    }

    CType::CType(const s16 valueS16) noexcept : length(sizeof(valueS16))
    {
        value.valueS16 = valueS16;
    }

    CType::CType(const s32 valueS32) noexcept : length(sizeof(valueS32))
    {
        value.valueS32 = valueS32;
    }

    CType::CType(const s64 valueS64) noexcept : length(sizeof(valueS64))
    {
        value.valueS64 = valueS64;
    }

    CType::CType(char* valueString) noexcept : length(sizeof(valueString))
    {
        value.valueString = valueString;
    }

#if 0
    CType::CType(const std::size_t length) noexcept : length(length)
    {
        value.valueStruct
    }
#endif
}

