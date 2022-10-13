#include <cmm/Types.h>

#include <gtest/gtest.h>

#include <string>

using namespace cmm;

TEST(MiscTest, TypePromoCharInt)
{
    CType from(EnumCType::CHAR, 0);
    CType to(EnumCType::INT16, 0);

    const auto optResult = canPromote(from, to);
    ASSERT_TRUE(optResult.has_value());
}

TEST(MiscTest, TypePromoIntFloat)
{
    CType from(EnumCType::INT32, 0);
    CType to(EnumCType::FLOAT, 0);

    const auto optResult = canPromote(from, to);
    ASSERT_TRUE(optResult.has_value());
}

TEST(MiscTest, TypePromoLongFloat)
{
    CType from(EnumCType::INT64, 0);
    CType to(EnumCType::FLOAT, 0);

    const auto optResult = canPromote(from, to);
    ASSERT_TRUE(optResult.has_value());
}

TEST(MiscTest, TypePromoFloatDouble)
{
    CType from(EnumCType::FLOAT, 0);
    CType to(EnumCType::DOUBLE, 0);

    const auto optResult = canPromote(from, to);
    ASSERT_TRUE(optResult.has_value());
}

TEST(MiscTest, TypePromoDoubleFloatError)
{
    CType from(EnumCType::DOUBLE, 0);
    CType to(EnumCType::FLOAT, 0);

    const auto optResult = canPromote(from, to);
    ASSERT_FALSE(optResult.has_value());
}

TEST(MiscTest, TypePromoDoubleCHARError)
{
    CType from(EnumCType::DOUBLE, 0);
    CType to(EnumCType::CHAR, 0);

    const auto optResult = canPromote(from, to);
    ASSERT_FALSE(optResult.has_value());
}

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

