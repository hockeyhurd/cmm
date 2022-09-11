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
    const std::string input = "int main() { int x; 4 = x; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerCharAssignmentWarning)
{
    const std::string input = "int main() { char a; a = 256; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getWarningCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerMultipleVariableDeclarationDefinitionError)
{
    const std::string input = "int main() { char a; a = 'c'; char a; return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerMultipleVariableDeclarationDefinitionValid)
{
    const std::string input = "int main() { char a; a = 'c'; { char a; a = 'b'; } return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 0);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerFunctionAndVariableDeclarationsWithSameNameError)
{
    const std::string input = "int func; float func(int);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerFunctionAndVariableDeclarationsWithSameNameError2)
{
    const std::string input = "int func(); float func;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerFunctionDeclaredInsideAnotherFunctionError)
{
    const std::string input = "int func() { int func2(); } return 0;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerFunctionDefinedInsideAnotherFunctionError)
{
    const std::string input = "int func() { int func2() { return 0; } return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerFunctionCallAnUndeclaredFunctionError)
{
    const std::string input = "int main() { func2(); return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerVoidFunctionReturnsNonVoidTypeError)
{
    const std::string input = "void func() { return 0; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);

    reporter.reset();
}

TEST(AnalyzerTest, AnalyzerVarAssignmentViaAddressOfIntError)
{
    const std::string input = "int* a; a = &42;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);

    ASSERT_EQ(reporter.getErrorCount(), 1);
    reporter.reset();
}

// TODO: Disable until we refactor EnumCTypes to be more robust.
TEST(AnalyzerTest, DISABLED_AnalyzerVarDerefAssignmentViaIncrement)
{
    const std::string input = "int* a; int b; b = 42; a = b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    Analyzer analyzer;
    analyzer.visit(*compUnitPtr);
    ASSERT_EQ(reporter.getErrorCount(), 1);
    reporter.reset();
}

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

