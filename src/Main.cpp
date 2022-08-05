// Our includes
#include <cmm/Dump.h>
#include <cmm/NodeList.h>
#include <cmm/Parser.h>

// std includes
#include <iostream>

using namespace cmm;

int main()
{
    std::cout << "Hello, world!\n";

    std::string input = "if (x) {} else {}";
    std::string errorMessage;
    Parser parser(input);
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    Dump dump;
    dump.visit(*compUnitPtr);

    return 0;
}

