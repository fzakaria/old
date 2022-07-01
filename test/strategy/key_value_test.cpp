#include "src/strategy/key_value.h"

#include <stdexcept>

#include "gtest/gtest.h"
#include "src/config/config.h"
#include "src/config/builder.h"

TEST(KeyValueStrategyTest, StrictModeTestingEmpty) {

  auto config = Config::KeyValue();
  auto strategy = KeyValueStrategy(config);
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetFalse) {
  auto config = ConfigBuilder::KeyValueBuilder().strict(false);
  auto strategy = KeyValueStrategy(config);
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetTrue) {
  auto strategy = KeyValueStrategy(toml::parse(
      R"(
      strict = true
    )"));
  ASSERT_TRUE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetInvalidString) {
  auto strategy = KeyValueStrategy(toml::parse(
      R"(
      strict = "hi"
    )"));
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetInvalidNumber) {
  auto strategy = KeyValueStrategy(toml::parse(
      R"(
      strict = 123
    )"));
  ASSERT_TRUE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, ReplaceEmpty) {
  auto name = "hello world";
  auto strategy = KeyValueStrategy(toml::table{});
  ASSERT_EQ(strategy.resolve(name), "hello world");
}

TEST(KeyValueStrategyTest, ReplaceEmptyStrict) {
  auto name = "hello world";
  auto strategy = KeyValueStrategy(toml::parse(
      R"(
      strict = true
    )"));
  EXPECT_THROW(strategy.resolve(name), std::domain_error);
}

TEST(KeyValueStrategyTest, ReplaceSimple) {
  auto name = "libruby-2.7.so.2.7";
  auto strategy = KeyValueStrategy(toml::parse(
      R"(
      "libruby-2.7.so.2.7" = "/lib/x86_64-linux-gnu/libruby-2.7.so.2.7"
    )"));
  ASSERT_EQ(strategy.resolve(name), "/lib/x86_64-linux-gnu/libruby-2.7.so.2.7");
}

TEST(KeyValueStrategyTest, ReplaceWrongType) {
  auto name = "libruby-2.7.so.2.7";
  auto strategy = KeyValueStrategy(toml::parse(
      R"(
      "libruby-2.7.so.2.7" = 123
    )"));
  ASSERT_EQ(strategy.resolve(name), name);
}