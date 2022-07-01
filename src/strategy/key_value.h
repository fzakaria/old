#include <string>
#include <string_view>

#include "strategy.h"
#include "src/config/config.h"

class KeyValueStrategy : public Strategy {
  const Config::KeyValue config;

 public:
  KeyValueStrategy(Config::KeyValue config);
  std::string resolve(std::string_view name) const override;
  bool is_strict() const;
};