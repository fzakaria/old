#include "config.h"

#include "builder.h"

static std::string toString(Config::Strategy strategy) {
  switch (strategy) {
    case Config::Strategy::DoNothing:
      return "do_nothing";
    case Config::Strategy::KeyValue:
      return "key_value";
    default:
      return "do_nothing";
  }
}

ConfigBuilder Config::fromTOML(toml::v3::table config) {
  return ConfigBuilder{config};
}

toml::v3::table Config::asTOML() const {
  auto table = toml::table{{"key_value", key_value().asTOML()},
                           {"strategy", toString(strategy())}};
  return table;
}

std::ostream& operator<<(std::ostream& os, const Config& obj) {
  return os << obj.asTOML();
}

ConfigBuilder Config::build() { return ConfigBuilder{toml::v3::table()}; }

KeyValueBuilder Config::KeyValue::fromTOML(toml::v3::table config) {
  return KeyValueBuilder{config};
}

toml::v3::table Config::KeyValue::asTOML() const {
  auto table = toml::table{{"strict", is_strict()}};
  table.insert(mapping().begin(), mapping().end());
  return table;
}

std::ostream& operator<<(std::ostream& os, const Config::KeyValue& obj) {
  return os << obj.asTOML();
}