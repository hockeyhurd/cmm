/**
 * File for storing basic type and macro information used everywhere.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

#pragma once

#ifndef CMM_TYPES_H
#define CMM_TYPES_H

#if defined(_DEBUG) || defined(DEBUG_BUILD)
#define DebugMode 1u
#else
#define DebugMode 0u
#endif

#ifdef __cplusplus
#define TYPES_CPP 1
#else
#define TYPES_CPP 0
#endif

#ifdef __APPLE__
#define OS_APPLE 1
#define OS_WIN 0
#define OS_UNIX 1
#elif WIN32 || _WIN64
#define OS_APPLE 0
#define OS_WIN 1
#define OS_UNIX 0
#elif __unix__
#define OS_APPLE 0
#define OS_WIN 0
#define OS_UNIX 1
#endif

#include <cstdlib>
#include <cstdint>
#include <optional>
#include <string>
// #include <cfloat>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>

#if defined(__GNUC__)
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

#if (__cplusplus == 199711L) // C++98
#define CPP_98 1
#define CPP_11 0
#define CPP_14 0
#define CPP_17 0
#define CPP_20 0
#define CPP_VER 1998
#elif (__cplusplus == 201103L) // C++11
#define CPP_98 1
#define CPP_11 1
#define CPP_14 0
#define CPP_17 0
#define CPP_20 0
#define CPP_VER 2011
#elif (__cplusplus == 201402L) // C++14
#define CPP_98 1
#define CPP_11 1
#define CPP_14 1
#define CPP_17 0
#define CPP_20 0
#define CPP_VER 2014
#elif (__cplusplus == 201703L || __cplusplus > 201402L) // C++17
#define CPP_98 1
#define CPP_11 1
#define CPP_14 1
#define CPP_17 1
#define CPP_20 0
#define CPP_VER 2017
#elif (__cplusplus == 202002L) // C++20
#define CPP_98 1
#define CPP_11 1
#define CPP_14 1
#define CPP_17 1
#define CPP_20 1
#define CPP_VER 2020
#else // C++ (pre-98)
#define CPP_98 0
#define CPP_11 0
#define CPP_14 0
#define CPP_17 0
#define CPP_20 0
#define CPP_VER 1997
#endif

#if CPP_VER >= 2014
#define CMM_NOEXCEPT noexcept
#define CMM_CONSTEXPR constexpr
#define CMM_CONSTEXPR_FUNC constexpr
#elif CPP_VER >= 2011
#define CMM_NOEXCEPT noexcept
#define CMM_CONSTEXPR constexpr
#define CMM_CONSTEXPR_FUNC
#else
#define CMM_NOEXCEPT
#define CMM_CONSTEXPR
#define CMM_CONSTEXPR_FUNC
#endif

namespace cmm 
{

    using  u8 =  std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using  s8 =  std::int8_t;
    using s16 = std::int16_t;
    using s32 = std::int32_t;
    using s64 = std::int64_t;

    using f32 = float;
    using f64 = double;

    const f32 f32_PI = 3.14159265358979323846264338327950288F;
    const f64 f64_PI = 3.14159265358979323846264338327950288;

    const f32 f32_2_PI = 6.28318530717958647692528676655900576F;
    const f64 f64_2_PI = 6.28318530717958647692528676655900576;

    const f32 f32_HALF_PI = 1.57079632679489661923132169163975144F;
    const f64 f64_HALF_PI = 1.57079632679489661923132169163975144;

    const f32 f32_QUARTER_PI = 0.78539816339744830961566084581987572F;
    const f64 f64_QUARTER_PI = 0.78539816339744830961566084581987572;

    const f32 f32_EIGHTH_PI = 0.39269908169872415480783042290993786F;
    const f64 f64_EIGHTH_PI = 0.39269908169872415480783042290993786;

    const f32 f32_e = 2.71828182845904523536F;
    const f64 f64_e = 2.71828182845904523536;

    // Forward declarations
    class Token;

    enum class EnumLocality : u16
    {
        GLOBAL = 0, INTERNAL, LOCAL, PARAMETER
    };

    constexpr const char* toString(const EnumLocality locality)
    {
        switch (locality)
        {
        case EnumLocality::GLOBAL:
            return "GLOBAL";
        case EnumLocality::INTERNAL:
            return "INTERNAL";
        case EnumLocality::LOCAL:
            return "LOCAL";
        case EnumLocality::PARAMETER:
            return "PARAMETER";
        default:
            return "UNKNOWN";
        }

        return nullptr;
    }

    enum class EnumLRValue : u16
    {
        UNKNOWN = 0, LVALUE, RVALUE
    };

    constexpr const char* toString(const EnumLRValue valueType)
    {
        switch (valueType)
        {
        case EnumLRValue::LVALUE:
            return "LVALUE";
        case EnumLRValue::RVALUE:
            return "RVALUE";
        case EnumLRValue::UNKNOWN:
        default:
            return "UNKNOWN";
        }

        return nullptr;
    }

    // Note: modifier 'extern' is implicit, so we don't include that here regardles
    // if the modifier is explicitly used.
    enum EnumModifier : u16
    {
        NO_MOD = 0, STATIC = 1, CONST_POINTER = 2, CONST_VALUE = 4, ALL_VALUES = 7
    };

    constexpr bool isValidModifier(const u16 value)
    {
        constexpr u16 invAllValues = ~EnumModifier::ALL_VALUES;

        return (value & invAllValues) == 0;
    }

    enum class EnumSymState
    {
        DECLARED = 0, DEFINED
    };

    enum class EnumCType : u16
    {
        NULL_T = 0, VOID, VOID_PTR, BOOL, CHAR, ENUM, INT8, INT16, INT32, INT64,
        FLOAT, DOUBLE, STRING, STRUCT
    };

    enum EnumEnum
    {
        // Note: We purposely don't give it a value because we will treat as a
        // 'normal' int.
    };

    struct CType
    {
        EnumCType type;
        u16 pointers;
        std::optional<std::string> optTypeName;

        /**
         * Needed for std::pair... do NOT use otherwise.
         */
        CType() CMM_NOEXCEPT;
        explicit CType(const EnumCType type, const u16 pointers = 0,
            std::optional<std::string>&& optTypeName = std::nullopt) CMM_NOEXCEPT;
        CType(const CType& other);
        CType(CType&& other) CMM_NOEXCEPT;

        CType& operator= (const CType& other);
        CType& operator= (CType&& other) CMM_NOEXCEPT;

        bool isEnum() const CMM_NOEXCEPT;
        bool isFloatingPoint() const CMM_NOEXCEPT;
        bool isInt() const CMM_NOEXCEPT;
        bool isPointerType() const CMM_NOEXCEPT;

        bool operator== (const CType& other) const CMM_NOEXCEPT;
        bool operator!= (const CType& other) const CMM_NOEXCEPT;
    };

    struct CTypeValue
    {
        std::size_t length;

        union
        {
            void* valueVoidPtr;
            char  valueChar;
            bool  valueBool;
            EnumEnum valueEnum;
            s8    valueS8;
            s16   valueS16;
            s32   valueS32;
            s64   valueS64;
            f32   valueF32;
            f64   valueF64;
            // TODO: Consider making this const
            char*  valueString;
        };

        explicit CTypeValue(void* valueVoidPtr) CMM_NOEXCEPT;
        explicit CTypeValue(const bool valueBool) CMM_NOEXCEPT;
        explicit CTypeValue(const char valueChar) CMM_NOEXCEPT;
        explicit CTypeValue(const EnumEnum valueEnum) CMM_NOEXCEPT;
        explicit CTypeValue(const s8 valueS8) CMM_NOEXCEPT;
        explicit CTypeValue(const s16 valueS16) CMM_NOEXCEPT;
        explicit CTypeValue(const s32 valueS32) CMM_NOEXCEPT;
        explicit CTypeValue(const s64 valueS64) CMM_NOEXCEPT;
        explicit CTypeValue(const f32 valueF32) CMM_NOEXCEPT;
        explicit CTypeValue(const f64 valueF64) CMM_NOEXCEPT;
        explicit CTypeValue(char* valueString) CMM_NOEXCEPT;
    };

    std::optional<CType> canPromote(const CType& from, const CType& to);
    std::optional<CType> canTruncate(const CType& from, const CType& to);
    bool isCType(const std::string& str) CMM_NOEXCEPT;
    std::optional<EnumCType> getCType(const std::string& str) CMM_NOEXCEPT;

    constexpr const char* toString(const EnumCType type)
    {
        switch (type)
        {
        case EnumCType::NULL_T:
            return "NULL";
        case EnumCType::VOID:
            return "void";
        case EnumCType::VOID_PTR:
            return "void*";
        case EnumCType::BOOL:
            return "bool";
        case EnumCType::CHAR:
            return "char";
        case EnumCType::ENUM:
            return "enum";
        case EnumCType::INT8:
            return "int8";
        case EnumCType::INT16:
            return "int16";
        case EnumCType::INT32:
            return "int32";
        case EnumCType::INT64:
            return "int64";
        case EnumCType::FLOAT:
            return "float";
        case EnumCType::DOUBLE:
            return "double";
        case EnumCType::STRING:
            return "string";
        case EnumCType::STRUCT:
            return "struct";
        default:
            return "Unknown type";
        }

        return "Unknown type";
    }

    enum class EnumBinOpNodeType
    {
        ASSIGNMENT = 0, ADD, SUBTRACT, MULTIPLY, DIVIDE,
        CMP_EQ, CMP_NE, CMP_GE, CMP_GT, CMP_LE, CMP_LT
    };

    std::optional<EnumBinOpNodeType> isEnumBinOpType(const Token& token) CMM_NOEXCEPT;

    enum class EnumCastType
    {
        NOP = 0, NARROWING, WIDENING
    };

    const char* toString(const EnumCastType type) CMM_NOEXCEPT;

    enum class EnumFieldAccessType
    {
        DOT = 0, ARROW
    };

    std::optional<EnumFieldAccessType> isEnumFieldAccessType(const Token& token) CMM_NOEXCEPT;
    const char* toString(const EnumFieldAccessType accessType) CMM_NOEXCEPT;

    template<class Stream, class T, class N>
    void printRepeat(Stream& stream, const T& value, const N count)
    {
        for (N i = 0; i < count; ++i)
        {
            stream << value;
        }
    }

    template<class Stream>
    void printType(Stream& stream, const CType& type)
    {
        stream << toString(type.type);

        if (type.isEnum() && type.optTypeName.has_value())
        {
            stream << " " << *type.optTypeName;
        }

        printRepeat(stream, '*', type.pointers);
    }

    template <class T>
    void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

}

namespace std
{
    template<>
    struct hash<cmm::CType>
    {
        std::size_t operator() (const cmm::CType& type) const
        {
            std::size_t result = (cmm::s32) type.type;
            cmm::hash_combine(result, type.pointers);
            cmm::hash_combine(result, type.optTypeName);

            return result;
        }
    };
}

#endif //!CMM_TYPES_H

