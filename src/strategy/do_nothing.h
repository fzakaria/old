#include "strategy.h"
#include <string>
#include <string_view>

class DoNothingStrategy : public Strategy
{
public:
    std::string resolve(std::string_view name) const override
    {
        return std::string{name};
    }
};