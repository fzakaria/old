#pragma once

#include "toml++/toml.h"
#include <unordered_map>
#include <filesystem>

class ConfigBuilder;

/**
 * @brief A single Config object that holds all the various types of configurations that are possible.
 * The values that are set are largely dictated by the strategy type employed.
 */
class Config {
    
    public:
        enum class Strategy {
            DoNothing,
            KeyValue
        };

    class KeyValue {
        std::unordered_map<std::string, std::filesystem::path> _mapping;
        bool _strict;
        public:
            friend class ConfigBuilder;
            const std::unordered_map<std::string, std::filesystem::path>& mapping() const { return _mapping; }
            bool is_strict() const { return _strict; }

    };

    private:

    Strategy _strategy;
    KeyValue _key_value;

    public:
        friend class ConfigBuilder;

        static ConfigBuilder build();
        static ConfigBuilder fromTOML(toml::v3::table config);

        Strategy strategy() const { return _strategy; }
        const KeyValue& key_value() const { return _key_value; }
};