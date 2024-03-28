// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/Reporter.h>
#include <cmm/config/CLIargs.h>
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/system/ChildProcess.h>
#include <cmm/visit/Analyzer.h>
#include <cmm/visit/Encode.h>

// std includes
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

using namespace cmm;

static std::optional<std::string> readFile(const char* filename, std::string& errorMessage);

s32 main(s32 argc, char* argv[])
{
    auto& reporter = Reporter::instance();

    if (argc <= 1)
    {
        reporter.error("Expected at least one argument (./cmm <file>.c)", Location(0, 0));

        return EXIT_FAILURE;
    }

    std::string errorMessage;
    CLIargs cliArgs(argc, argv);

    if (!cliArgs.parse(&errorMessage))
    {
        reporter.error(errorMessage, Location(0, 0));
        return EXIT_FAILURE;
    }

    const std::vector<std::string_view>& inputFiles = cliArgs.getInputFiles();

    if (inputFiles.empty())
    {
        reporter.error("Expected at least one file to process", Location(0, 0));
        return EXIT_FAILURE;
    }

    // TODO: Support more than one file.
    const auto optFileContents = readFile(inputFiles[0].data(), errorMessage);

    if (optFileContents == std::nullopt)
    {
        reporter.error(errorMessage, Location(0, 0));
        return EXIT_FAILURE;
    }

    Parser parser(*optFileContents);
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    if (compUnitPtr != nullptr)
    {
        Analyzer analyzer;
        analyzer.visit(*compUnitPtr);

        PlatformLLVM platform;
        std::ofstream ofs(cliArgs.getOutputName(), std::ios_base::out);
        Encode encoder(&platform, ofs);
        encoder.visit(*compUnitPtr);
        ofs.close();

        // TODO: Remove these hardcoded paths...
        const std::string clangPath = "/usr/bin/clang";
        std::vector<std::string> clangArgs = { clangPath, cliArgs.getOutputName() };

        cmm::system::ChildProcess childProc(clangPath, std::move(clangArgs));
        childProc.start();
        childProc.wait();
    }

    else
    {
        reporter.error(errorMessage, Location(0, 0));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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

