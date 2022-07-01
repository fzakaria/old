#include "config.h"
#include "builder.h"

ConfigBuilder Config::fromTOML(toml::v3::table config) { return ConfigBuilder{config}; }

ConfigBuilder Config::build() { return ConfigBuilder{ toml::v3::table() }; }
