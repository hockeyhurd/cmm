/**
 * A utility file for obtaining architecture specific information.
 *
 * @author hockeyhurd
 * @version 2023-01-08
 */

#pragma once

#ifndef CMM_ARCHITECTURE_H
#define CMM_ARCHITECTURE_H

// Our includes
#include <cmm/Types.h>

namespace cmm
{
    CMM_CONSTEXPR_FUNC const char* getLLVMPointerSizeAsCString() CMM_NOEXCEPT
    {
#if UINTPTR_MAX == 0xFFFF
        return "i16";
#elif UINTPTR_MAX == 0xFFFFFFFF
        return "i32";
#elif UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu
        return "i64";
#else
#error "Could not determine pointer size
        return "null";
#endif
    }

    CMM_CONSTEXPR_FUNC std::size_t getPointerSizeInBytes() CMM_NOEXCEPT
    {
#if UINTPTR_MAX == 0xFFFF
        return 2;
#elif UINTPTR_MAX == 0xFFFFFFFF
        return 4;
#elif UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu
        return 8;
#else
#error "Could not determine pointer size
        return 0;
#endif
    }
}

#endif //!CMM_ARCHITECTURE_H

