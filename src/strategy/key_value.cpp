#include "key_value.h"

#include <iostream>
#include <stdexcept>

#include "glog/logging.h"

KeyValueStrategy::KeyValueStrategy(Config::KeyValue config) : config(config) {}

std::string KeyValueStrategy::resolve(std::string_view name) const {
  auto it = config.mapping().find(std::string{name});

  if (it == config.mapping().end()) {
    LOG(INFO) << "could not find replacement for [" << name << "]";
    if (this->is_strict()) {
      throw std::domain_error("Strict mode enabled, failing startup.");
    }
    return std::string{name};
  }

  auto value = it->second;
  LOG(INFO) << "replacing [" << name << "] with [" << name << "]";
  return value;
};

bool KeyValueStrategy::is_strict() const { return config.is_strict(); }