#include "gtest/gtest.h"
#include "src/strategy/key_value.h"
#include "toml++/toml.h"

TEST(KeyValueStrategyTest, StrictModeTestingEmpty) {
  auto strategy = KeyValueStrategy(toml::table{});
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetFalse) {
  auto strategy = KeyValueStrategy(toml::parse(
    R"(
      strict = false
    )"
  ));
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetTrue) {
  auto strategy = KeyValueStrategy(toml::parse(
    R"(
      strict = true
    )"
  ));
  ASSERT_TRUE(strategy.is_strict());
}


TEST(KeyValueStrategyTest, StrictModeTestingSetInvalidString) {
  auto strategy = KeyValueStrategy(toml::parse(
    R"(
      strict = "hi"
    )"
  ));
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetInvalidNumber) {
  auto strategy = KeyValueStrategy(toml::parse(
    R"(
      strict = 123
    )"
  ));
  ASSERT_TRUE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, ReplaceEmpty) {
  auto name = "hello world";
  auto strategy = KeyValueStrategy(toml::table{});
  ASSERT_EQ(strategy.resolve(name), "hello world");
}