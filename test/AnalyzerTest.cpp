#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/Reporter.h>
#include <cmm/visit/Analyzer.h>

#include <gtest/gtest.h>

#include <string>

using namespace cmm;

static Reporter& reporter = Reporter::instance();

TEST(AnalyzerTest, AnalyzerExpressionNotAssignableError)
{
    reporter.reset();

    const std::string input = "int main() { int x; 4 = x; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerCharAssignIntError)
{
    reporter.reset();

    const std::string input = "int main() { char a; a = 256; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerIntAssignCharWarning)
{
    reporter.reset();

    const std::string input = "int main() { int a; a = 'c'; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 1);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerMultipleVariableDeclarationDefinitionError)
{
    reporter.reset();

    const std::string input = "int main() { char a; a = 'c'; char a; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerMultipleVariableDeclarationDefinitionError2)
{
    reporter.reset();

    const std::string input = "int main() { char a; a = 'c'; { char a; a = 'b'; } return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerFunctionAndVariableDeclarationsWithSameNameError)
{
    reporter.reset();

    const std::string input = "int func; float func(int);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerFunctionAndVariableDeclarationsWithSameNameError2)
{
    reporter.reset();

    const std::string input = "int func(); float func;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerFunctionDeclaredInsideAnotherFunctionError)
{
    reporter.reset();

    const std::string input = "int func() { int func2(); return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerFunctionDefinedInsideAnotherFunctionError)
{
    reporter.reset();

    const std::string input = "int func() { int func2() { return 0; } return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerFunctionCallAnUndeclaredFunctionError)
{
    reporter.reset();

    const std::string input = "int main() { func2(); return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerVoidFunctionReturnsNonVoidTypeError)
{
    reporter.reset();

    const std::string input = "void func() { return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerIntFunctionReturnsCharWarning)
{
    reporter.reset();

    const std::string input = "int func() { return 'c'; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 1);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerCharFunctionReturnsIntError)
{
    reporter.reset();

    const std::string input = "char func() { return 42; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerFunctionContainsParametersWithSameNameError)
{
    reporter.reset();

    const std::string input = "int func(int x, int x) { return 42; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerVarDoubleAssignmentViaIntValid)
{
    reporter.reset();

    const std::string input = "int x; int y; x = y = 42;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerVarQuadAssignmentViaIntValid)
{
    reporter.reset();

    const std::string input = "int a; int b; int c; int d; a = b = c = d = 42;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerVarAssignmentViaAddressOfIntError)
{
    reporter.reset();

    const std::string input = "int* a; a = &42;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);

    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerVarDerefAssignmentViaIncrement)
{
    reporter.reset();

    const std::string input = "int* a; int b; b = 42; a = b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerExplicitDowncastLongToIntWarning)
{
    reporter.reset();

    const std::string input = "long x; x = 42; int y; y = (int) x;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 2);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerExplicitDowncastIntToLongNoWarning)
{
    reporter.reset();

    const std::string input = "int x; x = 42; long y; y = (long) x;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastVoidPtrAssignDoubleWarning)
{
    reporter.reset();

    const std::string input = "double d; d = 42.0; void* ptr; ptr = d;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerImplicitCastVoidPtrAssignDoublePointerValid)
{
    reporter.reset();

    const std::string input = "double d; d = 42.0; void* ptr; ptr = &d;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastDoublePointerAssignVoidPtrValid)
{
    reporter.reset();

    const std::string input = "double x; void* ptr; ptr = &x; double* xPtr; xPtr = ptr;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastDoubleAssignVoidPtrError)
{
    reporter.reset();

    const std::string input = "double x; void* ptr; ptr = &x; double xPtr; xPtr = ptr;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerImplicitCastIntPtrAssignFloatPtrWarning)
{
    reporter.reset();

    const std::string input = "float x; x = 42.0F; int* ptr; ptr = &x;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 1);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastVoidPtrAssignDoubleVoidPtrValid)
{
    reporter.reset();

    const std::string input = "void* ptr; void** ptrPtr; ptr = ptrPtr;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastDoubleVoidPtrAssignVoidPtrValid)
{
    reporter.reset();

    const std::string input = "void* ptr; void** ptrPtr; ptrPtr = ptr;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastIntAddFloatWarning)
{
    reporter.reset();

    const std::string input = "1 + 2.0F;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 1);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastFloatAddIntWarning)
{
    reporter.reset();

    const std::string input = "1.0F + 2;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 1);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastIntPointerAddIntWarning)
{
    reporter.reset();

    const std::string input = "int x; x = 5; int* xPtr; xPtr = &x; int* result; result = xPtr + 5;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerLoadValueFromPointerAndReturnValid)
{
    reporter.reset();

    const std::string input = "int a; a = 42; int* ptr; ptr = &a; int result; result = *ptr;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerImplicitCastIntPointerAddIntPointerError)
{
    reporter.reset();

    const std::string input = "int x; x = 5; int y; y = 10; int* xPtr; xPtr = &x; int* yPtr; yPtr = &y; int* result; result = xPtr + yPtr;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerUnnecessaryExplicitCastValid)
{
    reporter.reset();

    const std::string input = "int main() { int z; z = (int) 5; return z; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerParenExpressionNode)
{
    reporter.reset();

    const std::string input = "double func() { double a; a = 10.0; double b; b = 16.0; double c; c = (2.0 * b) + a; return c; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerStructDeclarationsThenStructDefinitionValid)
{
    reporter.reset();

    const std::string input = "struct A; struct A {};";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerMultipleStructDeclarationsWarning)
{
    reporter.reset();

    const std::string input = "struct A; struct A;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_GT(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerStructDefinitionThenStructDeclarationWarning)
{
    reporter.reset();

    const std::string input = "struct A {}; struct A;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_GT(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerMultipleStructDefinitionError)
{
    reporter.reset();

    const std::string input = "struct A {}; struct A {};";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerParamMatchesLocalVariableError)
{
    reporter.reset();

    const std::string input = "int func(int x) { int x; x = 42; return x; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerFieldAccessValid)
{
    reporter.reset();

    const std::string input = "struct Vec2 { int x; int y; }; int main() { struct Vec2 v2; v2.x = 10; v2.y = 32; int sum; sum = v2.x + v2.y; return sum; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerFieldAccessAccessorIsNotAStructError)
{
    reporter.reset();

    const std::string input = "struct Vec2 { int x; int y; }; int main() { int z; z = 10; z.x = 5; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerFieldAccessExpectedArrowOpError)
{
    reporter.reset();

    const std::string input = "struct Vec2 { int x; int y; }; int main() { struct Vec2* v2; v2.x; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerFieldAccessStructUndefinedError)
{
    reporter.reset();

    const std::string input = "struct Vec2 { int x; int y; }; int main() { struct Vec3 v3; v3.x; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerFieldAccessUsingAMissingFieldError)
{
    reporter.reset();

    const std::string input = "struct Vec2 { int x; int y; }; int main() { struct Vec2 v2; v2.z; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeEnumeratorNameConflictionWithVariableError)
{
    reporter.reset();

    const std::string input = "int X; enum A { Y, X };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeVariableNameConflictionWithEnumeratorError)
{
    reporter.reset();

    const std::string input = "enum A { X, Y }; int X;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeThenAssignToIntVarValid)
{
    reporter.reset();

    const std::string input = "enum A { X, Y }; int Z; Z = (int) X;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeThenAssignToVarValid)
{
    reporter.reset();

    const std::string input = "enum A { X, Y }; enum A Z; Z = Y;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeWithAssignmentCharWarning)
{
    reporter.reset();

    const std::string input = "enum A { X = 'a', Y };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 1);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeWithAssignmentDoubleError)
{
    reporter.reset();

    const std::string input = "enum A { X = -2.5, Y };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeWithAssignmentStringError)
{
    reporter.reset();

    const std::string input = "enum A { X = \"Hello, world!\", Y };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeRedefinitionError)
{
    reporter.reset();

    const std::string input = "enum A { X, Y }; enum A { X, Y };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerEnumDefinitionStatementNodeEnumeratorNameConflictionError)
{
    reporter.reset();

    const std::string input = "enum A { X, Y }; enum B { X, Z };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerAssignEnumVariableToAnotherWarning)
{
    reporter.reset();

    const std::string input = "enum A { X, Y }; enum B { Z, W }; enum A e; e = Z;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 0);
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

s32 main(s32 argc, char* argv[])
{
    reporter.setEnablePrint(false);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

