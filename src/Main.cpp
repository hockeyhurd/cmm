// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/visit/Dump.h>

// std includes
#include <iostream>

using namespace cmm;

int main()
{
    std::string input = "*a = &b;";
    std::string errorMessage;
    Parser parser(input);
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    Dump dump;
    dump.visit(*compUnitPtr);

    return 0;
}

