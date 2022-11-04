#ifndef BENLIB_MATH_EXPERIMENTATION_HPP
#define BENLIB_MATH_EXPERIMENTATION_HPP

#include <cstdint>
#include <type_traits>

namespace benlib
{

namespace math
{

namespace experimentation
{

// Schwarzschild radius of a black hole
// https://en.wikipedia.org/wiki/Schwarzschild_radius

template<typename T1 = uint64_t, typename T2 = uint64_t, typename T3 = double>
constexpr auto schwarzschild_radius(const T1& mass, const T2& c = 299792458, const T3& G = 6.67408e-11) -> uint64_t
{
  // Check if T is floating point or integer
  static_assert(std::is_floating_point<T1>::value || std::is_integral<T1>::value,
                "T1 must be floating point or integer");
  static_assert(std::is_floating_point<T2>::value || std::is_integral<T2>::value,
                "T2 must be floating point or integer");
  static_assert(std::is_floating_point<T3>::value || std::is_integral<T3>::value,
                "T3 must be floating point or integer");

  return (2 * G * mass) / (c * c);
}

}  // namespace experimentation

}  // namespace math

}  // namespace benlib

#endif  // BENLIB_MATH_EXPERIMENTATION_HPP
