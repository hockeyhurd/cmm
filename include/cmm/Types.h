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

#if CPP_14
#define CMM_NOEXCEPT noexcept
#define CMM_CONSTEXPR constexpr
#define CMM_CONSTEXPR_FUNC constexpr
#elif CPP_11
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

    enum class EnumCType
    {
        NULL_T = 0, VOID_PTR, BOOL, CHAR, INT8, INT16, INT32, INT64, FLOAT, DOUBLE, STRING, STRUCT
    };

    struct CType
    {
        std::size_t length;

        union
        {
            void* valueVoidPtr;
            char  valueChar;
            bool  valueBool;
            s8    valueS8;
            s16   valueS16;
            s32   valueS32;
            s64   valueS64;
            f32   valueF32;
            f64   valueF64;
            // TODO: Consider making this const
            char*  valueString;
            // TODO: revisit structs
            // char  valueStruct[0];
        };

        CType(void* valueVoidPtr) noexcept;
        CType(const bool valueBool) noexcept;
        CType(const char valueChar) noexcept;
        CType(const s8 valueS8) noexcept;
        CType(const s16 valueS16) noexcept;
        CType(const s32 valueS32) noexcept;
        CType(const s64 valueS64) noexcept;
        CType(const f32 valueF32) noexcept;
        CType(const f64 valueF64) noexcept;
        CType(char* valueString) noexcept;
        // TODO: revisit structs
        // CType(const std::size_t length);
    };

}

#endif //!CMM_TYPES_H

