#pragma once

#include "config.h"
#include "toml++/toml.h"

#include <utility>
#include <filesystem>

class ConfigBuilder {

    public:
    class KeyValueBuilder {

        Config::KeyValue key_value;

        public:
            operator Config::KeyValue() const { return std::move(key_value); }
            KeyValueBuilder& with(std::string key, std::filesystem::path value);
            KeyValueBuilder& strict(bool value);
    };


    private:
    Config config;

    public:
        ConfigBuilder(toml::v3::table config);
        operator Config() const { return std::move(config); }

        ConfigBuilder& strategy(Config::Strategy strategy);
        ConfigBuilder& key_value(Config::KeyValue key_value);

};