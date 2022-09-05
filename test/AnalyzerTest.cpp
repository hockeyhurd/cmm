#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/Reporter.h>
#include <cmm/visit/Analyzer.h>

#include <gtest/gtest.h>

#include <string>

using namespace cmm;

static Reporter& reporter = Reporter::instance();

TEST(AnalyzerTest, AnalyzerWarnCharAssignment)
{
    const std::string input = "int main() { char a; a = 256; }";
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
    const std::string input = "int main() { char a; a = 'c'; char a; }";
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
    const std::string input = "int main() { char a; a = 'c'; { char a; a = 'b'; } }";
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
    const std::string input = "int func() { int func2(); }";
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

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

