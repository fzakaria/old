#include "key_value.h"
#include <iostream>
#include <stdexcept>

KeyValueStrategy::KeyValueStrategy(toml::v3::table config) : config(config) {
}

std::string KeyValueStrategy::resolve(std::string_view name) const {
    if (this->config.contains(name) && this->config[name].is_string()) {
        std::cerr << "replacing " << name << " with " << this->config[name] << std::endl;
    } else {
        std::cerr << "could not find replacement for [" << name << "]" << std::endl;
        if (this->is_strict()) {
            throw std::domain_error("Strict mode enabled, failing startup.");
        }
    }
    return this->config[name].value<std::string>().value_or(std::string{name});
};


bool KeyValueStrategy::is_strict() const {
    return config["strict"].as_boolean()->value_or(false);
}