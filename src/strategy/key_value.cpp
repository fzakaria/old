#include "key_value.h"
#include <iostream>
#include <stdexcept>
#include "glog/logging.h"

KeyValueStrategy::KeyValueStrategy(toml::v3::table config) : config(config) {
}

std::string KeyValueStrategy::resolve(std::string_view name) const {
    if (!this->config.contains(name) || !this->config[name].is_string()) {
        LOG(INFO) << "could not find replacement for [" << name << "]";
        if (this->is_strict()) {
            throw std::domain_error("Strict mode enabled, failing startup.");
        }
    }

    LOG(INFO) << "replacing [" << name << "] with [" << this->config[name] << "]";
    return this->config[name].value<std::string>().value_or(std::string{name});
};


bool KeyValueStrategy::is_strict() const {
    return config["strict"].value_or(false);
}