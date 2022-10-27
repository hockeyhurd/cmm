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

TEST(AnalyzerTest, AnalyzerMultipleVariableDeclarationDefinitionValid)
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
    ASSERT_EQ(reporter.getErrorCount(), 0);
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
    ASSERT_EQ(reporter.getWarningCount(), 1);
    ASSERT_EQ(reporter.getErrorCount(), 0);
}

TEST(AnalyzerTest, AnalyzerExplicitDowncastIntToLongNoWarning)
{
    reporter.reset();

    // Weird code, but perfectly valid.
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
    ASSERT_GT(reporter.getErrorCount(), 1);
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

s32 main(s32 argc, char* argv[])
{
    reporter.setEnablePrint(false);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

