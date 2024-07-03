/**
 * A class for creating temporary files.
 *
 * @author hockeyhurd
 * @version 2024-07-03
 */

// Our includes
#include <cmm/system/TempFile.h>
#include <cmm/Reporter.h>

// std includes
#include <sstream>

namespace cmm
{
namespace system
{
    TempFile::TempFile() : file(std::tmpfile())
    {
    }

    TempFile::TempFile(TempFile&& other) CMM_NOEXCEPT : file(other.file)
    {
        other.file = nullptr;
    }

    TempFile::~TempFile()
    {
        if (file != nullptr)
        {
            std::fclose(file);
            file = nullptr;
        }
    }

    TempFile& TempFile::operator= (TempFile&& other) CMM_NOEXCEPT
    {
        if (file != nullptr)
        {
            std::fclose(file);
            file = nullptr;
        }

        file = other.file;
        other.file = nullptr;

        return *this;
    }

    /* static */
    std::optional<TempFile> TempFile::createTemp()
    {
        TempFile file;

        if (file.file == nullptr)
        {
            auto& reporter = Reporter::instance();
            reporter.error("Unable to create a temporary file. Aborting...", Location::nullLocation());
        }

        return std::make_optional(std::move(file));
    }

    s32 TempFile::getFD() const CMM_NOEXCEPT
    {
        s32 fd = TempFile::badFD;

        if (file != nullptr)
        {
            fd = fileno(file);

            if (fd == TempFile::badFD)
            {
                auto& reporter = Reporter::instance();
                reporter.error("Unable to get file descriptor from temporary file. Aborting...", Location::nullLocation());
            }
        }

        return fd;
    }

    std::string TempFile::readAll() const
    {
        if (file == nullptr)
        {
            return "";
        }

        std::ostringstream os;
        char temp;

        while ((temp = std::fgetc(file)) != EOF)
        {
            os << temp;
        }

        const auto stringifiedOutput = os.str();
        return stringifiedOutput;
    }

    void TempFile::rewind()
    {
        if (file != nullptr)
        {
            std::rewind(file);
        }
    }
}
}

