#pragma once

#include <string_view>
#include <string>

class Strategy
{
public:
    virtual ~Strategy() {}
    virtual std::string resolve(std::string_view name) const = 0;
};