#include <memory>
#include <string_view>

#include "do_nothing.h"
#include "key_value.h"
#include "src/config/config.h"
#include "strategy.h"

std::unique_ptr<Strategy> CreateStrategy(Config config) {
  if (config.strategy() == Config::Strategy::DoNothing) {
    return std::make_unique<DoNothingStrategy>();
  }

  if (config.strategy() == Config::Strategy::KeyValue) {
    return std::make_unique<KeyValueStrategy>(config.key_value());
  }

  // default case
  std::cerr << "Unknown strategy type. Defaulting to do_nothing strategy."
            << std::endl;
  return std::make_unique<DoNothingStrategy>();
}