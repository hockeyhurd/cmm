#include <cmm/Types.h>
#include <cmm/config/CLIargs.h>

#include <gtest/gtest.h>

#include <array>
#include <string>

using namespace cmm;

TEST(CLITest, NoArgsNoProbs)
{
    std::array<const char*, 1> args = { "cliTest" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_TRUE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_TRUE(cliArgs.parse(&reason));
    ASSERT_TRUE(reason.empty());
}

TEST(CLITest, InvalidArgCausesFailure)
{
    std::array<const char*, 2> args = { "cliTest", "asdf" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());
}

TEST(CLITest, InvalidEmptyArgCausesFailure)
{
    std::array<const char*, 2> args = { "cliTest", "" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());
}

TEST(CLITest, InvalidWhitespace1CausesFailure)
{
    std::array<const char*, 3> args = { "cliTest", "-o", " " };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());
}

TEST(CLITest, InvalidWhitespace2CausesFailure)
{
    std::array<const char*, 3> args = { "cliTest", "-o", "\t" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());
}

TEST(CLITest, InvalidWhitespace3CausesFailure)
{
    std::array<const char*, 3> args = { "cliTest", "-o", "\n" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());
}

TEST(CLITest, InvalidWhitespace4CausesFailure)
{
    std::array<const char*, 3> args = { "cliTest", "-o", "\r\n" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());
}

TEST(CLITest, SimpleDebugOption)
{
    std::array<const char*, 2> args = { "cliTest", "-g" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_TRUE(cliArgs.parse(&reason));
    ASSERT_TRUE(reason.empty());
}

TEST(CLITest, ExpectNameAfterOption)
{
    std::array<const char*, 3> args = { "cliTest", "-o", "hello" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_TRUE(cliArgs.parse(&reason));
    ASSERT_TRUE(reason.empty());

    ASSERT_STREQ(cliArgs.getOutputName().c_str(), args[2]);
}

TEST(CLITest, ExpectNameAfterOptionIsMissingFailure)
{
    std::array<const char*, 2> args = { "cliTest", "-o" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());
}

TEST(CLITest, ExpectNameAfterOptionIsEmptyFailure)
{
    std::array<const char*, 3> args = { "cliTest", "-o", "" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());

    ASSERT_STRNE(cliArgs.getOutputName().c_str(), args[2]);
}

TEST(CLITest, ExpectNameAfterOptionLooksLikeAnOptionFailure)
{
    std::array<const char*, 3> args = { "cliTest", "-o", "-g" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_FALSE(cliArgs.parse(&reason));
    ASSERT_FALSE(reason.empty());

    ASSERT_STRNE(cliArgs.getOutputName().c_str(), args[2]);
}

TEST(CLITest, ExpectFile)
{
    std::array<const char*, 4> args = { "cliTest", "hello.cpp", "asdf.cc", "sda2.cxx" };
    CLIargs cliArgs(args.size(), const_cast<char**>(args.data()));

    ASSERT_FALSE(cliArgs.empty());
    ASSERT_EQ(cliArgs.count(), args.size() - 1);

    std::string reason;
    ASSERT_TRUE(cliArgs.parse(&reason));
    ASSERT_TRUE(reason.empty());

    const auto& inputFiles = cliArgs.getInputFiles();

    for (std::size_t i = 0; i < args.size() - 1; ++i)
    {
        ASSERT_STREQ(inputFiles[i].data(), args[i + 1]);
    }
}

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

