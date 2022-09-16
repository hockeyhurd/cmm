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
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

TEST(AnalyzerTest, AnalyzerFunctionDeclaredInsideAnotherFunctionError)
{
    reporter.reset();

    const std::string input = "int func() { int func2(); } return 0;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
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
    ASSERT_EQ(reporter.getErrorCount(), 1);
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

    ASSERT_EQ(reporter.getErrorCount(), 1);
}

// TODO: Disable until we refactor EnumCTypes to be more robust.
TEST(AnalyzerTest, DISABLED_AnalyzerVarDerefAssignmentViaIncrement)
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
    ASSERT_EQ(reporter.getErrorCount(), 1);
}

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

