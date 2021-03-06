#include "src/strategy/key_value.h"

#include <stdexcept>

#include "gtest/gtest.h"
#include "src/config/builder.h"
#include "src/config/config.h"

TEST(KeyValueStrategyTest, StrictModeTestingEmpty) {
  Config::KeyValue config = Config::KeyValue::fromTOML(toml::parse(
      R"(
    )"));
  auto strategy = KeyValueStrategy(config);
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetFalse) {
  Config::KeyValue config = Config::KeyValue::fromTOML(toml::parse(
      R"(
      [key_value]
      strict = false
    )"));
  auto strategy = KeyValueStrategy(config);
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetTrue) {
  Config::KeyValue config = Config::KeyValue::fromTOML(toml::parse(
      R"(
      strict = true
    )"));
  auto strategy = KeyValueStrategy(config);
  ASSERT_TRUE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetInvalidString) {
  Config::KeyValue config = Config::KeyValue::fromTOML(toml::parse(
      R"(
      strict = "hi"
    )"));
  auto strategy = KeyValueStrategy(config);
  ASSERT_FALSE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, StrictModeTestingSetInvalidNumber) {
  Config::KeyValue config = Config::KeyValue::fromTOML(toml::parse(
      R"(
      strict = 123
    )"));
  auto strategy = KeyValueStrategy(config);
  ASSERT_TRUE(strategy.is_strict());
}

TEST(KeyValueStrategyTest, ReplaceEmpty) {
  auto name = "hello world";
  auto config = Config::KeyValue();
  auto strategy = KeyValueStrategy(config);
  ASSERT_EQ(strategy.resolve(name), "hello world");
}

TEST(KeyValueStrategyTest, ReplaceEmptyStrict) {
  auto name = "hello world";
  auto config = KeyValueBuilder().strict(true);
  auto strategy = KeyValueStrategy(config);
  EXPECT_THROW(strategy.resolve(name), std::domain_error);
}

TEST(KeyValueStrategyTest, ReplaceSimple) {
  auto name = "libruby-2.7.so.2.7";
  Config::KeyValue config = Config::KeyValue::fromTOML(toml::parse(
      R"(
      "libruby-2.7.so.2.7" = "/lib/x86_64-linux-gnu/libruby-2.7.so.2.7"
    )"));
  auto strategy = KeyValueStrategy(config);
  ASSERT_EQ(strategy.resolve(name), "/lib/x86_64-linux-gnu/libruby-2.7.so.2.7");
}

TEST(KeyValueStrategyTest, ReplaceWrongType) {
  auto name = "libruby-2.7.so.2.7";
  Config::KeyValue config = Config::KeyValue::fromTOML(toml::parse(
      R"(
      "libruby-2.7.so.2.7" = 123
    )"));
  auto strategy = KeyValueStrategy(config);
  ASSERT_EQ(strategy.resolve(name), name);
}