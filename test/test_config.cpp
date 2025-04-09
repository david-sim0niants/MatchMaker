#include "test_config.h"

#include <cstdlib>
#include <iostream>
#include <string>

namespace matchmaker::core::test {

TestConfig::TestConfig() : prng_seed(default_prng_seed)
{
    const char *seed = getenv("SEED");
    try {
        prng_seed = std::stoul(seed);
    } catch (...) {
        std::cerr << "Invalid seed value\n";
    }
}

TestConfig TestConfig::instance;

}
