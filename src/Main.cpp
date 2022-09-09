// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/visit/Analyzer.h>
#include <cmm/visit/Dump.h>

// std includes
#include <iostream>

using namespace cmm;

int main()
{
    std::string input = "int main() { int a; int b; b = 42; a = b; }";
    std::string errorMessage;
    Parser parser(input);
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    if (compUnitPtr != nullptr)
    {
        Dump dump;
        dump.visit(*compUnitPtr);

        Analyzer analyzer;
        analyzer.visit(*compUnitPtr);
    }

    return 0;
}

