#include "strategy.h"
#include "do_nothing.h"
#include <memory>
#include <string_view>

std::unique_ptr<Strategy> StrategyFactory(std::string_view type) {
    if (type == "do_nothing") {
       return std::make_unique<DoNothingStrategy>(); 
    }

    // default case
    std::cerr << "Unknown strategy type [" << type << "]. Defaulting to do_nothing strategy." << std::endl;
    return std::make_unique<DoNothingStrategy>();
}