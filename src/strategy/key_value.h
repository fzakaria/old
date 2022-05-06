#include <string>
#include <string_view>

#include "strategy.h"
#include "toml++/toml.h"

class KeyValueStrategy : public Strategy {
  const toml::v3::table config;

 public:
  KeyValueStrategy(toml::v3::table config);
  std::string resolve(std::string_view name) const override;
  bool is_strict() const;
};