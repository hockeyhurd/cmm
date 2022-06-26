/**
 * File for storing basic type and macro information used everywhere.
 *
 * @author hockeyhurd
 * @version 2022-06-16
 */

// Our includes
#include <cmm/Types.h>

// std includes
#include <map>

namespace cmm
{

    static std::map<std::string, EnumCType> ctypeMap;

    static void initCTypeMap()
    {
        /*ctypeMap = { {"NULL", EnumCType::NULL_T}, {"bool", EnumCType::BOOL}, {"char", EnumCType::CHAR},
            {"short", EnumCType::INT16}, {"int", EnumCType::INT32}, {"long", EnumCType::INT64}, {"float", EnumCType::FLOAT}, {"double", EnumCType::DOUBLE} };*/
        ctypeMap.emplace("NULL", EnumCType::NULL_T);
        ctypeMap.emplace("bool", EnumCType::BOOL);
        ctypeMap.emplace("char", EnumCType::CHAR);
        ctypeMap.emplace("short", EnumCType::INT16);
        ctypeMap.emplace("int", EnumCType::INT32);
        ctypeMap.emplace("long", EnumCType::INT64);
        ctypeMap.emplace("float", EnumCType::FLOAT);
        ctypeMap.emplace("double", EnumCType::DOUBLE);
    }

    bool isCType(const std::string& str) CMM_NOEXCEPT
    {
        if (ctypeMap.empty())
        {
            initCTypeMap();
        }

        return ctypeMap.find(str) != ctypeMap.cend();
    }

    std::optional<EnumCType> getCType(const std::string& str) CMM_NOEXCEPT
    {
        if (ctypeMap.empty())
        {
            initCTypeMap();
        }

        const auto findResult = ctypeMap.find(str);
        return findResult != ctypeMap.cend() ? std::make_optional(findResult->second) : std::nullopt;
    }

    CType::CType(void* valueVoidPtr) CMM_NOEXCEPT : length(sizeof(valueVoidPtr))
    {
        this->valueVoidPtr = valueVoidPtr;
    }

    CType::CType(const bool valueBool) CMM_NOEXCEPT : length(sizeof(valueBool))
    {
        this->valueBool = valueBool;
    }

    CType::CType(const char valueChar) CMM_NOEXCEPT : length(sizeof(valueChar))
    {
        this->valueChar = valueChar;
    }

    CType::CType(const s8 valueS8) CMM_NOEXCEPT : length(sizeof(valueS8))
    {
        this->valueS8 = valueS8;
    }

    CType::CType(const s16 valueS16) CMM_NOEXCEPT : length(sizeof(valueS16))
    {
        this->valueS16 = valueS16;
    }

    CType::CType(const s32 valueS32) CMM_NOEXCEPT : length(sizeof(valueS32))
    {
        this->valueS32 = valueS32;
    }

    CType::CType(const s64 valueS64) CMM_NOEXCEPT : length(sizeof(valueS64))
    {
        this->valueS64 = valueS64;
    }

    CType::CType(const f32 valueF32) CMM_NOEXCEPT : length(sizeof(valueF32))
    {
        this->valueF32 = valueF32;
    }

    CType::CType(const f64 valueF64) CMM_NOEXCEPT : length(sizeof(valueF64))
    {
        this->valueF64 = valueF64;
    }

    CType::CType(char* valueString) CMM_NOEXCEPT : length(sizeof(valueString))
    {
        this->valueString = valueString;
    }

#if 0
    CType::CType(const std::size_t length) CMM_NOEXCEPT : length(length)
    {
        this->valueStruct
    }
#endif
}

