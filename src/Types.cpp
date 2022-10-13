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
#include <unordered_map>
#include <unordered_set>

namespace cmm
{

    static std::map<std::string, EnumCType> ctypeMap;

    static void initCTypeMap()
    {
        ctypeMap.emplace("NULL", EnumCType::NULL_T);
        ctypeMap.emplace("void", EnumCType::VOID);
        ctypeMap.emplace("void*", EnumCType::VOID_PTR);
        ctypeMap.emplace("bool", EnumCType::BOOL);
        ctypeMap.emplace("char", EnumCType::CHAR);
        ctypeMap.emplace("short", EnumCType::INT16);
        ctypeMap.emplace("int", EnumCType::INT32);
        ctypeMap.emplace("long", EnumCType::INT64);
        ctypeMap.emplace("float", EnumCType::FLOAT);
        ctypeMap.emplace("double", EnumCType::DOUBLE);
        ctypeMap.emplace("struct", EnumCType::STRUCT);
    }

    static std::optional<CType> promoOrTruncateLookup(const CType& from, const CType& to, std::unordered_map<EnumCType, std::unordered_set<EnumCType>>& theMap)
    {
        if (from.pointers != to.pointers)
        {
            return std::nullopt;
        }

        const auto firstFindResult = theMap.find(from.type);

        if (firstFindResult == theMap.cend())
        {
            return std::nullopt;
        }

        const auto& set = firstFindResult->second;
        const auto secondFindResult = set.find(to.type);

        if (secondFindResult != set.cend())
        {
            return std::make_optional<CType>(to);
        }

        // else {}
        return std::nullopt;
    }

    CType::CType(const EnumCType type, const u16 pointers, std::optional<std::string> optStructName) CMM_NOEXCEPT :
        type(type), pointers(pointers), optStructName(optStructName)
    {
    }

    bool CType::operator== (const CType& other) const CMM_NOEXCEPT
    {
        return type == other.type && pointers == other.pointers;
    }

    bool CType::operator!= (const CType& other) const CMM_NOEXCEPT
    {
        return !(*this == other);
    }

    std::optional<CType> canPromote(const CType& from, const CType& to)
    {
        static std::unordered_map<EnumCType, std::unordered_set<EnumCType>> promoMap;

        // One time map init
        if (promoMap.empty())
        {
            {
                auto& set = promoMap[EnumCType::CHAR];
                set.emplace(EnumCType::CHAR);
                set.emplace(EnumCType::INT8);
                set.emplace(EnumCType::INT16);
                set.emplace(EnumCType::INT32);
                set.emplace(EnumCType::INT64);
                set.emplace(EnumCType::FLOAT);
                set.emplace(EnumCType::DOUBLE);
            }

            {
                auto& set = promoMap[EnumCType::INT8];
                set.emplace(EnumCType::CHAR);
                set.emplace(EnumCType::INT16);
                set.emplace(EnumCType::INT32);
                set.emplace(EnumCType::INT64);
                set.emplace(EnumCType::FLOAT);
                set.emplace(EnumCType::DOUBLE);
            }

            {
                auto& set = promoMap[EnumCType::INT16];
                set.emplace(EnumCType::INT16);
                set.emplace(EnumCType::INT32);
                set.emplace(EnumCType::INT64);
                set.emplace(EnumCType::FLOAT);
                set.emplace(EnumCType::DOUBLE);
            }

            {
                auto& set = promoMap[EnumCType::INT32];
                set.emplace(EnumCType::INT32);
                set.emplace(EnumCType::INT64);
                set.emplace(EnumCType::FLOAT);
                set.emplace(EnumCType::DOUBLE);
            }

            {
                auto& set = promoMap[EnumCType::INT64];
                set.emplace(EnumCType::INT64);
                set.emplace(EnumCType::FLOAT);
                set.emplace(EnumCType::DOUBLE);
            }

            {
                auto& set = promoMap[EnumCType::FLOAT];
                set.emplace(EnumCType::DOUBLE);
            }
        }

        return promoOrTruncateLookup(from, to, promoMap);
    }

    std::optional<CType> canTruncate(const CType& from, const CType& to)
    {
        static bool init = false;
        static std::unordered_map<EnumCType, std::unordered_set<EnumCType>> truncateMap;

        // One time map init
        if (!init && truncateMap.empty())
        {
            init = true;
            // TODO: Fill in
        }

        return promoOrTruncateLookup(from, to, truncateMap);
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

    CTypeValue::CTypeValue(void* valueVoidPtr) CMM_NOEXCEPT : length(sizeof(valueVoidPtr))
    {
        this->valueVoidPtr = valueVoidPtr;
    }

    CTypeValue::CTypeValue(const bool valueBool) CMM_NOEXCEPT : length(sizeof(valueBool))
    {
        this->valueBool = valueBool;
    }

    CTypeValue::CTypeValue(const char valueChar) CMM_NOEXCEPT : length(sizeof(valueChar))
    {
        this->valueChar = valueChar;
    }

    CTypeValue::CTypeValue(const s8 valueS8) CMM_NOEXCEPT : length(sizeof(valueS8))
    {
        this->valueS8 = valueS8;
    }

    CTypeValue::CTypeValue(const s16 valueS16) CMM_NOEXCEPT : length(sizeof(valueS16))
    {
        this->valueS16 = valueS16;
    }

    CTypeValue::CTypeValue(const s32 valueS32) CMM_NOEXCEPT : length(sizeof(valueS32))
    {
        this->valueS32 = valueS32;
    }

    CTypeValue::CTypeValue(const s64 valueS64) CMM_NOEXCEPT : length(sizeof(valueS64))
    {
        this->valueS64 = valueS64;
    }

    CTypeValue::CTypeValue(const f32 valueF32) CMM_NOEXCEPT : length(sizeof(valueF32))
    {
        this->valueF32 = valueF32;
    }

    CTypeValue::CTypeValue(const f64 valueF64) CMM_NOEXCEPT : length(sizeof(valueF64))
    {
        this->valueF64 = valueF64;
    }

    CTypeValue::CTypeValue(char* valueString) CMM_NOEXCEPT : length(sizeof(valueString))
    {
        this->valueString = valueString;
    }
}

