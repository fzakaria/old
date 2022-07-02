#pragma once

#include <string>
#include <string_view>

class Strategy {
 public:
  virtual ~Strategy() = default;
  virtual std::string resolve(std::string_view name) const = 0;
};