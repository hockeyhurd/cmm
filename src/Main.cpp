// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/config/CLIargs.h>
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/visit/Analyzer.h>
#include <cmm/visit/Encode.h>

// std includes
#include <cstdio>
#include <iostream>
#include <optional>
#include <sstream>

using namespace cmm;

static void decomposeArgs(s32 argc, char* argv[], std::vector<std::string>& filesToCompile);
static std::optional<std::string> readFile(const char* filename, std::string& errorMessage);

int main(s32 argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << "Expected at least one argument (./cmm <file>.c)\n";
        return EXIT_FAILURE;
    }

    std::vector<std::string> filesToCompile;
    decomposeArgs(argc, argv, filesToCompile);
    std::string errorMessage;
    const auto optFileContents = readFile(argv[1], errorMessage);

    if (optFileContents == std::nullopt)
    {
        std::cout << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    Parser parser(*optFileContents);
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    if (compUnitPtr != nullptr)
    {
        // Dump dump;
        // dump.visit(*compUnitPtr);

        Analyzer analyzer;
        analyzer.visit(*compUnitPtr);

        PlatformLLVM platform;
        std::ostringstream os;
        Encode encoder(&platform, os);
        encoder.visit(*compUnitPtr);

        std::cout << os.str() << std::endl;
    }

    else
    {
        std::cout << errorMessage << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* static */
void decomposeArgs(s32 argc, char* argv[], std::vector<std::string>& filesToCompile)
{
    CLIargs cliArgs(argc, argv);
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

