#pragma once

#include <random>
#include <type_traits>

namespace matchmaker::misc {

class PRNG {
public:
    using SeedType = std::mt19937::result_type;

    PRNG() : generator(std::random_device{}()) {}

    explicit PRNG(SeedType seed) : generator(seed) {}

    template<typename Integral = int>
    inline std::enable_if_t<std::is_integral_v<Integral>, Integral>
        gen_uniform(Integral min, Integral max)
    {
        return std::uniform_int_distribution<Integral>{min, max}(generator);
    }

    template<typename Integral = int>
    inline std::enable_if_t<std::is_integral_v<Integral>, Integral>
        operator()(Integral min, Integral max)
    {
        return gen_uniform<Integral>(min, max);
    }

    inline std::mt19937 get_generator()
    {
        return generator;
    }

private:
    std::mt19937 generator;
};

}
