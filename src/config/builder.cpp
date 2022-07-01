#include "builder.h"

static Config::Strategy fromString(std::string value) {
  if (value == "do_nothing") {
    return Config::Strategy::DoNothing;
  }
  if (value == "key_value") {
    return Config::Strategy::KeyValue;
  }
  return Config::Strategy::DoNothing;
}

ConfigBuilder::ConfigBuilder(toml::v3::table config) {
  auto type_str = config["strategy"].value_or("do_nothing");
  auto type = fromString(type_str);
  strategy(type);

  auto type_config = config[type_str].as_table();

  if (type == Config::Strategy::KeyValue) {
    ConfigBuilder::KeyValueBuilder key_value_builder =
        ConfigBuilder::KeyValueBuilder().strict(
            config["strict"].value_or(false));
    for (auto it = type_config->begin(); it != type_config->end(); it++) {
      if (it->first == "strict") {
        continue;
      }
      if (!it->second.is_string()) {
        continue;
      }
      auto value = it->second.value<std::string>().value();
      key_value_builder.with(std::string{it->first.str()}, value);
    }
    key_value(key_value_builder);
  }
}

ConfigBuilder& ConfigBuilder::strategy(Config::Strategy strategy) {
  config._strategy = strategy;
  return *this;
}

ConfigBuilder& ConfigBuilder::key_value(Config::KeyValue key_value) {
  config._key_value = key_value;
  return *this;
}

ConfigBuilder::KeyValueBuilder& ConfigBuilder::KeyValueBuilder::with(
    std::string key, std::filesystem::path value) {
  key_value._mapping.insert({key, value});
  return *this;
}

ConfigBuilder::KeyValueBuilder& ConfigBuilder::KeyValueBuilder::strict(
    bool value) {
  key_value._strict = value;
  return *this;
}