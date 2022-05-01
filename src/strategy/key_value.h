#include "strategy.h"
#include <string>
#include <string_view>
#include "toml++/toml.h"

class KeyValueStrategy : public Strategy
{
public:
    KeyValueStrategy(const toml::v3::table & config);
    std::string resolve(std::string_view name) const override;
};