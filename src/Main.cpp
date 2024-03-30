// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/Reporter.h>
#include <cmm/config/CLIargs.h>
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/system/ChildProcess.h>
#include <cmm/utils/StringUtils.h>
#include <cmm/visit/Analyzer.h>
#include <cmm/visit/Encode.h>

// std includes
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <sstream>
#include <vector>

using namespace cmm;

static void clangify(const CLIargs& cliArgs, std::queue<std::string>& intermediateFiles);
static std::optional<std::string> readFile(const char* filename, std::string& errorMessage);

s32 main(s32 argc, char* argv[])
{
    auto& reporter = Reporter::instance();

    if (argc <= 1)
    {
        reporter.error("Expected at least one argument (./cmm <file>.c)", Location());

        return EXIT_FAILURE;
    }

    std::string errorMessage;
    CLIargs cliArgs(argc, argv);

    if (!cliArgs.parse(&errorMessage))
    {
        reporter.error(errorMessage, Location());
        return EXIT_FAILURE;
    }

    const std::vector<std::string_view>& inputFiles = cliArgs.getInputFiles();

    if (inputFiles.empty())
    {
        reporter.error("Expected at least one file to process", Location());
        return EXIT_FAILURE;
    }

    std::queue<std::string> intermediateFiles;

    for (const std::string_view file : inputFiles)
    {
        const auto optFileContents = readFile(file.data(), errorMessage);

        if (optFileContents == std::nullopt)
        {
            reporter.error(errorMessage, Location());
            return EXIT_FAILURE;
        }

        std::optional<std::string> intermediateFile = StringUtils::getBaseFileName<std::string>(file);

        if (!intermediateFile.has_value())
        {
            reporter.error("Invalid file (missing .c or .h extension)", Location());
            return EXIT_FAILURE;
        }

        *intermediateFile += ".ll";
        Parser parser(*optFileContents);
        auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

        if (compUnitPtr != nullptr)
        {
            Analyzer analyzer;
            analyzer.visit(*compUnitPtr);

            PlatformLLVM platform;
            std::ofstream ofs(*intermediateFile, std::ios_base::out);
            Encode encoder(&platform, ofs);
            encoder.visit(*compUnitPtr);
            ofs.close();

            intermediateFiles.push(std::move(*intermediateFile));
        }

        else
        {
            reporter.error(errorMessage, Location());
            return EXIT_FAILURE;
        }
    }

    clangify(cliArgs, intermediateFiles);

    return EXIT_SUCCESS;
}

/* static */
void clangify(const CLIargs& cliArgs, std::queue<std::string>& intermediateFiles)
{
    auto& reporter = Reporter::instance();

    // TODO: Until we create our own little filesystem library (because std::filesystem is crap...),
    // we will rely on clang being in the user's path.
    const std::string clangPath = "clang";
    std::vector<std::string> clangArgs;
    clangArgs.reserve(intermediateFiles.size() * 2);

    clangArgs.emplace_back(clangPath);

    while (!intermediateFiles.empty())
    {
        clangArgs.push_back(std::move(intermediateFiles.front()));
        intermediateFiles.pop();
    }

    std::optional<std::string> optOutputName = std::make_optional<std::string>(cliArgs.getOutputName());

    switch (cliArgs.getBuildType())
    {
    case EnumBuildType::ASSEMBLE:
        clangArgs.emplace_back("-S");
        break;
    case EnumBuildType::BINARY:
        if (!optOutputName.has_value() || optOutputName->empty())
        {
            optOutputName = std::make_optional<std::string>("a.out");
        }

        break;
    case EnumBuildType::OBJ:
        clangArgs.emplace_back("-c");
        break;
    default:
    {
        std::ostringstream os;
        os << "Build type: '" << cmm::toString(cliArgs.getBuildType()) << "' is not yet supported.";
        reporter.bug(os.str(), Location(), true);
        break;
    }
    }

    if (optOutputName.has_value())
    {
        clangArgs.emplace_back("-o");
        clangArgs.emplace_back(std::move(*optOutputName));
    }

    for (const auto& str : clangArgs)
    {
        std::cout << str << std::endl;
    }

    cmm::system::ChildProcess childProc(clangPath, std::move(clangArgs));
    childProc.start();
    childProc.wait();
}

/* static */
std::optional<std::string> readFile(const char* filename, std::string& errorMessage)
{
    if (filename == nullptr)
    {
        errorMessage = "'filename' is a nullptr";
        return std::nullopt;
    }

    FILE* file = std::fopen(filename, "r");

    if (file == nullptr)
    {
        errorMessage = "Failed to read file: ";
        errorMessage += filename;
        return std::nullopt;
    }

    std::ostringstream os;
    char temp;

    while ((temp = std::fgetc(file)) != EOF)
    {
        os << temp;
    }

    std::fclose(file);

    return std::make_optional<std::string>(os.str());
}

