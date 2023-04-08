#ifndef PASSWORD_EXPERIMENTATION_HPP
#define PASSWORD_EXPERIMENTATION_HPP

#include <string_view> // std::string_view
#include <iostream>
#include <string> // std::string
#include <random> // std::random_device, std::mt19937_64, std::uniform_int_distribution, std::seed_seq
#include <cassert> // assert

namespace experimentation
{

template<typename T = uint64_t, typename Generator>
inline auto randomize(const T min, const T max) -> T {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    
    Generator rng(std::random_device{}());

    using dist_type = typename std::conditional<std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>>::type;
    dist_type uni(min, max);
    return uni(rng);
}

template<typename T = uint64_t>
inline auto fast_randomize_64(const T min, const T max) -> T {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    
    thread_local static std::mt19937_64 rng(std::random_device{}());

    using dist_type = typename std::conditional<std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>>::type;
    dist_type uni(min, max);
    return uni(rng);
}

template<typename T = uint64_t, const T min, const T max>
inline auto fast_randomize_tp_64() -> T {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    
    thread_local static std::mt19937_64 rng(std::random_device{}());

    using dist_type = typename std::conditional<std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>>::type;
    dist_type uni(min, max);
    return uni(rng);
}

}  // namespace experimentation
#endif  // PASSWORD_EXPERIMENTATION_HPP