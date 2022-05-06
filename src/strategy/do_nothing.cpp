#include <string>
#include <string_view>

#include "strategy.h"

class DoNothingStrategy : public Strategy {
 public:
  std::string resolve(std::string_view name) const override {
    return std::string{name};
  }
};