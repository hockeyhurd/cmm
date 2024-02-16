/**
 * A utility class for parsing commandline-args.
 *
 * @author hockeyhurd
 * @version 2023-05-18
 */

// Our includes
#include <cmm/config/CLIargs.h>
#include <cmm/utils/StringUtils.h>

// std includes
#include <array>
#include <cassert>
#include <optional>
#include <sstream>
#include <unordered_map>

namespace cmm
{
    typedef bool (*OptCallback)(CLIargs&, std::string*, const std::optional<std::string_view>&);

    struct OptData
    {
        std::string_view prefix;
        bool hasNext;
        OptCallback callback;

        OptData(std::string_view prefix, bool hasNext, OptCallback callback = CLIargs::defaultCallback) CMM_NOEXCEPT :
            prefix(prefix), hasNext(hasNext), callback(callback) {}
    };

    CLIargs::CLIargs(const s32 argc, char* argv[]) : args(), buildType(EnumBuildType::BINARY),
        outName("a.out")
    {
        if (argc <= 1)
        {
            return;
        }

        args.reserve(argc - 1);

        for (s32 i = 1; i < argc; ++i)
        {
            args.emplace_back(argv[i]);
        }
    }

    std::size_t CLIargs::count() const CMM_NOEXCEPT
    {
        return args.size();
    }

    bool CLIargs::empty() const CMM_NOEXCEPT
    {
        return args.empty();
    }

    EnumBuildType CLIargs::getBuildType() const CMM_NOEXCEPT
    {
        return buildType;
    }

    std::string& CLIargs::getOutputName() CMM_NOEXCEPT
    {
        return outName;
    }

    const std::string& CLIargs::getOutputName() const CMM_NOEXCEPT
    {
        return outName;
    }

    std::vector<std::string_view> CLIargs::getInputFiles() const
    {
        return inputFiles;
    }

    bool CLIargs::parse(std::string* reason) CMM_NOEXCEPT
    {
        static const std::unordered_map<std::string_view, OptData> optTable =
        {
            { "-o", { "", true, collectOutputName } },
            { "-g", { "", false } },
        };

        for (std::size_t i = 0; i < args.size(); ++i)
        {
            const std::string_view arg = args[i];

            if (isArgACppFile(arg))
            {
                inputFiles.emplace_back(arg);
                continue;
            }

            const auto findResult = optTable.find(arg);

            if (findResult == optTable.cend())
            {
                if (reason != nullptr)
                {
                    std::ostringstream builder;
                    builder << "Unexpected arg '" << arg << "' at index "
                            << (i + 1);
                    *reason = builder.str();
                }

                return false;
            }

            const OptData& optData = findResult->second;
            std::optional<std::string_view> next;

            if (optData.hasNext)
            {
                if (++i >= args.size())
                {
                    if (reason != nullptr)
                    {
                        std::ostringstream builder;
                        builder << "Expected another argument following option '"
                                << arg << "' but found no more arguments";
                        *reason = builder.str();
                    }

                    return false;
                }

                next = std::make_optional(args[i++]);

                if (!next->empty())
                {
                    if (startsWith(*next, '-'))
                    {
                        if (reason != nullptr)
                        {
                            std::ostringstream builder;
                            builder << "The next argument ('" << *next
                                << "') looks like another option";
                            *reason = builder.str();
                        }

                        return false;
                    }

                    else if (isWhitespace(*next))
                    {
                        if (reason != nullptr)
                        {
                            *reason = "Argument is just whitepsace";
                        }

                        return false;
                    }
                }
            }

            const bool result = optData.callback(*this, reason, next);

            if (!result)
            {
                return false;
            }
        }

        return true;
    }

    /* static */
    bool CLIargs::collectOutputName(CLIargs& args, std::string* reason, const std::optional<std::string_view>& value)
    {
        if (!value.has_value())
        {
            if (reason != nullptr)
            {
                *reason = "Expected a value but something went wrong. This is likely a compiler bug...";
            }

            return false;
        }

        else if (value->empty())
        {
            if (reason != nullptr)
            {
                *reason = "Expected a value but found an empty string. This is likely a compiler bug...";
            }

            return false;
        }

        args.outName = *value;

        return true;
    }

    /* static */
    bool CLIargs::isArgACppFile(const std::string_view value)
    {
        CMM_CONSTEXPR std::array<std::string_view, 3> extensions = { ".cpp", ".cxx", ".cc" };
        const auto begIter = value.crbegin();

        for (const std::string_view ext : extensions)
        {
            auto iter = begIter;
            auto extIter = ext.crbegin();

            while (*iter == *extIter)
            {
                ++iter;
                ++extIter;

                if (extIter == ext.crend())
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool CLIargs::collectFileName(CLIargs& args, std::string* reason, const std::optional<std::string_view>& value)
    {
        if (!value.has_value())
        {
            if (reason != nullptr)
            {
                *reason = "Expected a value but something went wrong. This is likely a compiler bug...";
            }

            return false;
        }

        else if (value->empty())
        {
            if (reason != nullptr)
            {
                *reason = "Expected a value but found an empty string. This is likely a compiler bug...";
            }

            return false;
        }

        inputFiles.emplace_back(*value);
        return true;
    }
}

