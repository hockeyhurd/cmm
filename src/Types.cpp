/**
 * File for storing basic type and macro information used everywhere.
 *
 * @author hockeyhurd
 * @version 2022-06-16
 */

// Our includes
#include <cmm/Types.h>
#include <cmm/Token.h>
#include <cmm/container/StringView.h>

// std includes
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace cmm
{

    static std::map<StringView<char>, EnumCType> ctypeMap;

    static void initCTypeMap()
    {
        ctypeMap.emplace(StringView<char>("NULL", strlen("NULL")), EnumCType::NULL_T);
        ctypeMap.emplace(StringView<char>("void", strlen("void")), EnumCType::VOID);
        ctypeMap.emplace(StringView<char>("void*", strlen("void*")), EnumCType::VOID_PTR);
        ctypeMap.emplace(StringView<char>("bool", strlen("bool")), EnumCType::BOOL);
        ctypeMap.emplace(StringView<char>("char", strlen("char")), EnumCType::CHAR);
        ctypeMap.emplace(StringView<char>("short", strlen("short")), EnumCType::INT16);
        ctypeMap.emplace(StringView<char>("int", strlen("int")), EnumCType::INT32);
        ctypeMap.emplace(StringView<char>("long", strlen("long")), EnumCType::INT64);
        ctypeMap.emplace(StringView<char>("float", strlen("float")), EnumCType::FLOAT);
        ctypeMap.emplace(StringView<char>("double", strlen("double")), EnumCType::DOUBLE);
        ctypeMap.emplace(StringView<char>("struct", strlen("struct")), EnumCType::STRUCT);
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

    CType::CType() CMM_NOEXCEPT : type(EnumCType::NULL_T), pointers(0xFFFF), optStructName(std::nullopt)
    {
    }

    CType::CType(const EnumCType type, const u16 pointers, std::optional<std::string> optStructName) CMM_NOEXCEPT :
        type(type), pointers(pointers), optStructName(optStructName)
    {
    }

    CType::CType(const CType& other) : type(other.type), pointers(other.pointers), optStructName(std::nullopt)
    {
        if (other.optStructName.has_value())
        {
            optStructName = std::make_optional(*other.optStructName);
        }
    }

    CType::CType(CType&& other) CMM_NOEXCEPT : type(other.type), pointers(other.pointers),
        optStructName(std::move(other.optStructName))
    {
    }

    CType& CType::operator= (const CType& other)
    {
        if (this != &other)
        {
            type = other.type;
            pointers = other.pointers;
            optStructName = other.optStructName;
        }

        return *this;
    }

    CType& CType::operator= (CType&& other) CMM_NOEXCEPT
    {
        if (this != &other)
        {
            type = other.type;
            pointers = other.pointers;
            optStructName = std::move(other.optStructName);
        }

        return *this;
    }

    bool CType::isPointerType() const CMM_NOEXCEPT
    {
        return pointers > 0;
    }

    bool CType::isFloatingPoint() const CMM_NOEXCEPT
    {
        const bool result = pointers == 0 && (type == EnumCType::FLOAT || type == EnumCType::DOUBLE);
        return result;
    }

    bool CType::isInt() const CMM_NOEXCEPT
    {
        const bool result = pointers == 0 && (type == EnumCType::CHAR ||
            type == EnumCType::INT8  || type == EnumCType::INT16 ||
            type == EnumCType::INT32 || type == EnumCType::INT64);

        return result;
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

    bool isCType(const StringView<char>& str) CMM_NOEXCEPT
    {
        if (ctypeMap.empty())
        {
            initCTypeMap();
        }

        return ctypeMap.find(str) != ctypeMap.cend();
    }

    bool isCType(const std::string& str) CMM_NOEXCEPT
    {
        if (ctypeMap.empty())
        {
            initCTypeMap();
        }

        return ctypeMap.find(StringView<char>(str)) != ctypeMap.cend();
    }

    std::optional<EnumCType> getCType(const StringView<char>& str) CMM_NOEXCEPT
    {
        if (ctypeMap.empty())
        {
            initCTypeMap();
        }

        const auto findResult = ctypeMap.find(str);
        return findResult != ctypeMap.cend() ? std::make_optional(findResult->second) : std::nullopt;
    }

    std::optional<EnumCType> getCType(const std::string& str) CMM_NOEXCEPT
    {
        if (ctypeMap.empty())
        {
            initCTypeMap();
        }

        const auto findResult = ctypeMap.find(StringView<char>(str));
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

    std::optional<EnumBinOpNodeType> isEnumBinOpType(const Token& token) CMM_NOEXCEPT
    {
        if (token.isCharSymbol())
        {
            switch (token.asCharSymbol())
            {
            case CHAR_PLUS:
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::ADD);
            case CHAR_MINUS:
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::SUBTRACT);
            case CHAR_ASTERISK: // multiply
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::MULTIPLY);
            case CHAR_FORWARD_SLASH: // divide
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::DIVIDE);
            default:
                return std::nullopt;
            }
        }

        // Should be unreachable.
        return std::nullopt;
    }

    std::optional<EnumFieldAccessType> isEnumFieldAccessType(const Token& token) CMM_NOEXCEPT
    {
        if (token.isCharSymbol() && token.asCharSymbol() == CHAR_PERIOD)
        {
            return std::make_optional(EnumFieldAccessType::DOT);
        }

        else if (token.isStringSymbol() && token.asStringSymbol() == "->")
        {
            return std::make_optional(EnumFieldAccessType::ARROW);
        }

        // else {}
        return std::nullopt;
    }

    const char* toString(const EnumFieldAccessType accessType) CMM_NOEXCEPT
    {
        switch (accessType)
        {
        case EnumFieldAccessType::DOT:
            return "EnumFieldAccessType::DOT";
        case EnumFieldAccessType::ARROW:
            return "EnumFieldAccessType::ARROW";
        default:
            return "UNKNOWN EnumFieldAccessType";
        }

        // Should be unreachable.
        return "UNKNOWN EnumFieldAccessType";
    }
}

