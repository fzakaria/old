#include "builder.h"

static Config::Strategy fromString(const std::string & value) {
  if (value == "do_nothing") {
    return Config::Strategy::DoNothing;
  }
  if (value == "key_value") {
    return Config::Strategy::KeyValue;
  }
  return Config::Strategy::DoNothing;
}

ConfigBuilder::ConfigBuilder(toml::v3::table config) {
  const auto *type_str = config["strategy"].value_or("do_nothing");
  auto type = fromString(type_str);
  strategy(type);

  auto *key_value_config = config["key_value"].as_table();
  if (key_value_config != nullptr) {
    key_value(Config::KeyValue::fromTOML(*key_value_config));
  }
}

ConfigBuilder& ConfigBuilder::strategy(Config::Strategy strategy) {
  config._strategy = strategy;
  return *this;
}

ConfigBuilder& ConfigBuilder::key_value(Config::KeyValue key_value) {
  config._key_value = std::move(key_value);
  return *this;
}

KeyValueBuilder::KeyValueBuilder(toml::v3::table config) {
  strict(config["strict"].value_or(false));
  for (auto && [key, value] : config) {
    if (key == "strict") {
      continue;
    }
    if (!value.is_string()) {
      continue;
    }
    auto path = value.value<std::string>().value();
    with(std::string{key.str()}, path);
  }
}

KeyValueBuilder& KeyValueBuilder::with(const std::string & key, const std::string & value) {
  key_value._mapping.insert({key, value});
  return *this;
}

KeyValueBuilder& KeyValueBuilder::strict(bool value) {
  key_value._strict = value;
  return *this;
}