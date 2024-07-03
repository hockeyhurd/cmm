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

    std::optional<s32> TempFile::getFD() const CMM_NOEXCEPT
    {
        std::optional<s32> result(std::nullopt);

        if (file == nullptr)
        {
            return result;
        }

        const s32 fd = fileno(file);

        if (fd == TempFile::BAD_FD)
        {
            auto& reporter = Reporter::instance();
            reporter.error("Unable to get file descriptor from temporary file. Aborting...", Location::nullLocation());
            return result;
        }

        result = std::make_optional(fd);

        return result;
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

