#include <cmm/Types.h>
#include <cmm/container/StringView.h>

#include <gtest/gtest.h>

#include <string>
#include <sstream>

using namespace cmm;

TEST(ContainerTest, StringViewConstruct)
{
    const std::string hello = "hello";
    StringView<char> view(hello.c_str(), hello.size());

    ASSERT_EQ(view.size(), hello.size());
    ASSERT_STREQ(view.string(), hello.c_str());
}

TEST(ContainerTest, StringViewCopyConstruct)
{
    const std::string hello = "hello";
    StringView<char> view(hello.c_str(), hello.size());
    StringView<char> view2(view);

    ASSERT_EQ(view.size(), view2.size());
    ASSERT_STREQ(view.string(), view2.string());
}

TEST(ContainerTest, StringViewMoveConstruct)
{
    const std::string hello = "hello";
    StringView<char> view(hello.c_str(), hello.size());
    StringView<char> view2(std::move(view));

    ASSERT_EQ(view.size(), view2.size());
    ASSERT_STREQ(view.string(), view2.string());
}

TEST(ContainerTest, StringViewConstructPartial)
{
    const std::string helloWorld = "Hello, world!";
    const std::string outSubString = "world!";
    const std::size_t shift = 7;
    StringView<char> view(helloWorld.c_str() + shift, helloWorld.size() - shift);

    ASSERT_EQ(view.size(), outSubString.size());
    ASSERT_STREQ(view.string(), outSubString.c_str());
}

TEST(ContainerTest, StringViewCopyAssignment)
{
    const std::string hello = "hello";
    const std::string world = "world";
    StringView<char> view(hello.c_str(), hello.size());
    StringView<char> view2(world.c_str(), world.size());

    ASSERT_EQ(view.size(), view2.size());
    ASSERT_STRNE(view.string(), view2.string());

    view2 = view;
    ASSERT_EQ(view.size(), view2.size());
    ASSERT_STREQ(view.string(), view2.string());
}

TEST(ContainerTest, StringViewMoveAssignment)
{
    const std::string hello = "hello";
    const std::string world = "world";
    StringView<char> view(hello.c_str(), hello.size());
    StringView<char> view2(world.c_str(), world.size());

    ASSERT_EQ(view.size(), view2.size());
    ASSERT_STRNE(view.string(), view2.string());

    view2 = std::move(view);
    ASSERT_EQ(view.size(), view2.size());
    ASSERT_STREQ(view.string(), view2.string());
}

TEST(ContainerTest, StringViewAtEqualsSqBracketOp)
{
    const std::string hello = "hello";
    StringView<char> view(hello.c_str(), hello.size());

    ASSERT_EQ(view.size(), hello.size());
    ASSERT_STREQ(view.string(), hello.c_str());

    for (std::size_t i = 0; i < view.size(); ++i)
    {
        ASSERT_EQ(view.at(i), view[i]);
    }
}

TEST(ContainerTest, StringViewOperatorOut)
{
    const std::string hello = "hello";
    StringView<char> view(hello.c_str(), hello.size());

    ASSERT_EQ(view.size(), hello.size());
    ASSERT_STREQ(view.string(), hello.c_str());

    std::ostringstream os;
    os << view;
    ASSERT_STREQ(os.str().c_str(), view.string());
}

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

