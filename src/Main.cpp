// Our includes
#include <cmm/Dump.h>
#include <cmm/NodeList.h>
#include <cmm/Parser.h>

// std includes
#include <iostream>

using namespace cmm;

int main()
{
    std::string input = "while (true) { i = i + 2; }";
    std::string errorMessage;
    Parser parser(input);
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    Dump dump;
    dump.visit(*compUnitPtr);

    return 0;
}

