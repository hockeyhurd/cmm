// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/config/CLIargs.h>
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/visit/Analyzer.h>
#include <cmm/visit/Encode.h>

// std includes
#include <fstream>
#include <sstream>

using namespace cmm;

int main(s32 argc, char* argv[])
{
    std::string errorMessage;
    Parser parser("");
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
    }

    return 0;
}

