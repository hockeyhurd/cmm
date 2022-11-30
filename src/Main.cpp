// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/visit/Analyzer.h>
#include <cmm/visit/Dump.h>
#include <cmm/visit/Encode.h>

// std includes
// #include <fstream>
#include <iostream>
#include <sstream>

using namespace cmm;

int main()
{
    std::string input = "int main() { int a; a = 10; int b; b = 32; int c; c = a + b; return c; }";
    std::string errorMessage;
    Parser parser(input);
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    if (compUnitPtr != nullptr)
    {
        Dump dump;
        dump.visit(*compUnitPtr);

        Analyzer analyzer;
        analyzer.visit(*compUnitPtr);

        PlatformLLVM platform;
        std::ostringstream os;
        Encode encoder(&platform, os);
        encoder.visit(*compUnitPtr);

        std::cout << "Output:\n" << os.str() << std::endl;
    }

    return 0;
}

