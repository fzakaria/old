#pragma once

#include <utility>

#include "config.h"
#include "toml++/toml.h"
class KeyValueBuilder {
  Config::KeyValue key_value;

 public:
  KeyValueBuilder() {}
  KeyValueBuilder(toml::v3::table config);
  operator Config::KeyValue() const { return std::move(key_value); }
  KeyValueBuilder& with(std::string key, std::string value);
  KeyValueBuilder& strict(bool value);
};

class ConfigBuilder {
 private:
  Config config;

 public:
  ConfigBuilder(toml::v3::table config);
  operator Config() const { return std::move(config); }

  ConfigBuilder& strategy(Config::Strategy strategy);
  ConfigBuilder& key_value(Config::KeyValue key_value);
};