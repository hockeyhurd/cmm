#include <cmm/Types.h>
#include <cmm/StructTable.h>

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

TEST(MiscTest, StructTableAddAndCheck)
{
    const std::string name = "A";
    StructTable table;

    ASSERT_TRUE(table.empty());
    ASSERT_EQ(table.size(), 0);
    ASSERT_FALSE(table.has(name));

    table.addOrUpdate(name, EnumSymState::DECLARED);
    ASSERT_FALSE(table.empty());
    ASSERT_EQ(table.size(), 1);
    ASSERT_TRUE(table.has(name));
    ASSERT_EQ(table.get(name)->symState, EnumSymState::DECLARED);

    table.addOrUpdate(name, EnumSymState::DEFINED);
    ASSERT_FALSE(table.empty());
    ASSERT_EQ(table.size(), 1);
    ASSERT_TRUE(table.has(name));
    ASSERT_EQ(table.get(name)->symState, EnumSymState::DEFINED);
}

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

