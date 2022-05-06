#pragma once

#include <string>
#include <string_view>

class Strategy {
 public:
  virtual ~Strategy() {}
  virtual std::string resolve(std::string_view name) const = 0;
};