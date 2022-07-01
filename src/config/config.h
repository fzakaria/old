#pragma once

#include <iostream>
#include <unordered_map>

#include "toml++/toml.h"

class ConfigBuilder;
class KeyValueBuilder;

/**
 * @brief A single Config object that holds all the various types of
 * configurations that are possible. The values that are set are largely
 * dictated by the strategy type employed.
 */
class Config {
 public:
  enum class Strategy { DoNothing, KeyValue };

  class KeyValue {
    std::unordered_map<std::string, std::string> _mapping;
    bool _strict = false;

   public:
    friend class KeyValueBuilder;
    friend std::ostream& operator<<(std::ostream& os,
                                    const Config::KeyValue& obj);

    const std::unordered_map<std::string, std::string>& mapping() const {
      return _mapping;
    }
    static KeyValueBuilder fromTOML(toml::v3::table config);
    bool is_strict() const { return _strict; }
    toml::v3::table asTOML() const;
  };

 private:
  Strategy _strategy;
  KeyValue _key_value;

 public:
  friend class ConfigBuilder;
  friend std::ostream& operator<<(std::ostream& os, const Config& obj);

  static ConfigBuilder build();
  static ConfigBuilder fromTOML(toml::v3::table config);

  Strategy strategy() const { return _strategy; }
  const KeyValue& key_value() const { return _key_value; }
  toml::v3::table asTOML() const;
};