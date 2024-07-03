/**
 * A class for creating temporary files.
 *
 * @author hockeyhurd
 * @version 2024-07-03
 */

#pragma once

#ifndef CMM_SYSTEM_TEMP_FILE_H
#define CMM_SYSTEM_TEMP_FILE_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <cstdio>
#include <optional>
#include <string>

namespace cmm
{
namespace system
{
    class TempFile
    {
    private:

        /**
         * Constructor.
         */
        TempFile();

    public:

        TempFile(const TempFile&) = delete;
        TempFile(TempFile&&) CMM_NOEXCEPT;
        ~TempFile();

        TempFile& operator= (const TempFile&) = delete;
        TempFile& operator= (TempFile&&) CMM_NOEXCEPT;

        static std::optional<TempFile> createTemp();
        s32 getFD() const CMM_NOEXCEPT;

        std::string readAll() const;
        void rewind();

    private:

        // See documentation here: https://man7.org/linux/man-pages/man3/fileno.3.html
        static CMM_CONSTEXPR s32 badFD = -1;
        std::FILE* file;
    };
}
}

#endif //!CMM_SYSTEM_TEMP_FILE_H


