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
        this->valueVoidPtr = valueVoidPtr;
    }

    CType::CType(const bool valueBool) noexcept : length(sizeof(valueBool))
    {
        this->valueBool = valueBool;
    }

    CType::CType(const char valueChar) noexcept : length(sizeof(valueChar))
    {
        this->valueChar = valueChar;
    }

    CType::CType(const s8 valueS8) noexcept : length(sizeof(valueS8))
    {
        this->valueS8 = valueS8;
    }

    CType::CType(const s16 valueS16) noexcept : length(sizeof(valueS16))
    {
        this->valueS16 = valueS16;
    }

    CType::CType(const s32 valueS32) noexcept : length(sizeof(valueS32))
    {
        this->valueS32 = valueS32;
    }

    CType::CType(const s64 valueS64) noexcept : length(sizeof(valueS64))
    {
        this->valueS64 = valueS64;
    }

    CType::CType(const f32 valueF32) noexcept : length(sizeof(valueF32))
    {
        this->valueF32 = valueF32;
    }

    CType::CType(const f64 valueF64) noexcept : length(sizeof(valueF64))
    {
        this->valueF64 = valueF64;
    }

    CType::CType(char* valueString) noexcept : length(sizeof(valueString))
    {
        this->valueString = valueString;
    }

#if 0
    CType::CType(const std::size_t length) noexcept : length(length)
    {
        this->valueStruct
    }
#endif
}

