#include "key_value.h"


KeyValueStrategy::KeyValueStrategy(const toml::v3::table & config) {

}


std::string KeyValueStrategy::resolve(std::string_view name) const {
    return std::string{name};
}