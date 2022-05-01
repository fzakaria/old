#include "strategy.h"
#include "do_nothing.h"
#include "key_value.h"
#include <memory>
#include <string_view>
#include "toml++/toml.h"

std::unique_ptr<Strategy> CreateStrategy(std::string_view type, toml::v3::table config) {
    if (type == "do_nothing") {
       return std::make_unique<DoNothingStrategy>(); 
    }

    if (type == "key_value") {
        return std::make_unique<KeyValueStrategy>(config);
    }

    // default case
    std::cerr << "Unknown strategy type [" << type << "]. Defaulting to do_nothing strategy." << std::endl;
    return std::make_unique<DoNothingStrategy>();
}