/**
 * A utility class for parsing commandline-args.
 *
 * @author hockeyhurd
 * @version 2023-05-18
 */

#pragma once

#ifndef CMM_CLI_ARGS_H
#define CMM_CLI_ARGS_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cmm
{
    struct OptData;

    class CLIargs
    {
    public:
        friend struct OptData;

        /**
         * Constructor.
         */
        CLIargs(const s32 argc, char* argv[]);
        CLIargs(const CLIargs&) = delete;
        CLIargs(CLIargs&&) CMM_NOEXCEPT = delete;
        ~CLIargs() = default;

        CLIargs& operator= (const CLIargs&) = delete;
        CLIargs& operator= (CLIargs&&) CMM_NOEXCEPT = delete;

        std::size_t count() const CMM_NOEXCEPT;
        bool empty() const CMM_NOEXCEPT;

        bool isDebug() const CMM_NOEXCEPT;
        EnumBuildType getBuildType() const CMM_NOEXCEPT;
        std::string& getOutputName() CMM_NOEXCEPT;
        const std::string& getOutputName() const CMM_NOEXCEPT;

        std::vector<std::string_view> getInputFiles() const;

        bool parse(std::string* reason) CMM_NOEXCEPT;

    private:

        inline bool defaultCallback(std::string*, const std::optional<std::string_view>&) { return true; }
        bool collectDebugMode(std::string* reason, const std::optional<std::string_view>& value);
        bool collectOutputName(std::string* reason, const std::optional<std::string_view>& value);
        bool isArgCFile(const std::string_view str);
        bool collectFileName(CLIargs& args, std::string* reason, const std::optional<std::string_view>& value);

    private:

        std::vector<std::string_view> args;
        std::vector<std::string_view> inputFiles;

        EnumBuildType buildType;
        bool debugMode;
        std::string outName;

    };
}

#endif //!CMM_CLI_ARGS_H

