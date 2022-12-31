#ifndef BENLIB_STRING_EXTERNAL_EXPERIMENTATION_HPP
#define BENLIB_STRING_EXTERNAL_EXPERIMENTATION_HPP

#include <algorithm>
#include <array>
#include <cstring>
#include <functional>
#include <random>
#include <string>

namespace benlib
{

namespace string
{

namespace experimentation
{

// https://stackoverflow.com/a/444614/10152334
template<typename T = std::mt19937_64>
auto random_generator() -> T
{
    auto constexpr seed_bytes = sizeof(typename T::result_type) * T::state_size;
    auto constexpr seed_len = seed_bytes / sizeof(std::seed_seq::result_type);
    auto seed = std::array<std::seed_seq::result_type, seed_len>();
    auto dev = std::random_device();
    std::generate_n(begin(seed), seed_len, std::ref(dev));
    auto seed_seq = std::seed_seq(begin(seed), end(seed));
    return T {seed_seq};
}

auto random_string(std::size_t len) -> std::string
{
    static constexpr auto chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    thread_local auto rng = random_generator<>();
    auto dist = std::uniform_int_distribution {{}, std::strlen(chars) - 1};
    auto result = std::string(len, '\0');
    std::generate_n(begin(result), len, [&]() { return chars[dist(rng)]; });
    return result;
}

}  // namespace experimentation

}  // namespace string

}  // namespace benlib

#endif  // BENLIB_STRING_EXTERNAL_EXPERIMENTATION_HPP
