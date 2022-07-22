#pragma once

#include <utility>

#include "config.h"
#include "toml++/toml.h"
class KeyValueBuilder {
  Config::KeyValue key_value;

 public:
  KeyValueBuilder() = default;
  KeyValueBuilder(toml::v3::table config);
  operator Config::KeyValue() const { return key_value; }
  KeyValueBuilder& with(const std::string& key, const std::string& value);
  KeyValueBuilder& strict(bool value);
};

class ConfigBuilder {
 private:
  Config config;

 public:
  ConfigBuilder(toml::v3::table config);
  operator Config() const { return config; }

  ConfigBuilder& strategy(Config::Strategy strategy);
  ConfigBuilder& key_value(Config::KeyValue key_value);
};