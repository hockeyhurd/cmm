// Our includes
#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/visit/Analyzer.h>
// #include <cmm/visit/Dump.h>
#include <cmm/visit/Encode.h>

// std includes
// #include <fstream>
#include <iostream>
#include <sstream>

using namespace cmm;

int main()
{
    // std::string input = "struct Vec2 { int x; int y; }; int sum(int x, int y) { return x + y; } int main() { int a; a = 10; int b; b = 32; int c; c = sum(a, b); return c; }";
    // std::string input = "int main() { int a; a = 10; if (a + 1) { a = 20; } else { a = 30; } return a; }";
    // std::string input = "void func(); int main() { float a; a = 10.0F; float b; b = 16.0F; float c; c = (2.0F * -b) + a; int result; result = (int) c; return result; }";
    // std::string input = "int main() { int a; a = 42; int* ptr; ptr = &a; int result; result = *ptr; return result; }";
    // std::string input = "void func(int* value) { *value = *value * 2; } int main() { int x; x = 2; func(&x); return x; }";
    // std::string input = "int main() { int x; x = 2; int y; y = 3; int result; result = x-- + --y; return result; }";
    // std::string input = "struct Vec2 { int x; int y; }; int main() { struct Vec2 v2; v2.x = 10; v2.y = 32; int result; result = v2.x + v2.y; return result; }";
    // std::string input = "struct Vec2 { int x; int y; }; struct Vec3 { struct Vec2 v2; int z; }; int main() { struct Vec3 v3; v3.v2.x = 10; v3.v2.y = 12; v3.z = 20; int result; result = v3.v2.x + v3.v2.y + v3.z; return result; }";
    // std::string input = "enum A { X, Y }; int main() { int z; z = (int) Y; return z; }";
    // std::string input = "enum A { X, Y }; int main() { enum A a; a = Y; return (int) a; }";
    std::string input = "int func(char c) { int z; z = (int) c; return z; } double func2(float f) { double z; z = (double) f; return z; } float func3(double d) { float z; z = (float) d; return z; } float func4(int i) { float z; z = (float) i; return z; } int func5(int c) { int z; z = (int) c; return z; }";
    std::string errorMessage;
    Parser parser(input);
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

        std::cout << "Output:\n" << os.str() << std::endl;
    }

    return 0;
}

