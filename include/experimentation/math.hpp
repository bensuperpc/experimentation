#ifndef BENLIB_MATH_EXPERIMENTATION_HPP
#define BENLIB_MATH_EXPERIMENTATION_HPP

#include <bitset>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

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

  return static_cast<uint64_t>(2 * G * mass) / (c * c);
}

// Check if triangle is right angled
// https://en.wikipedia.org/wiki/Pythagorean_theorem

template<typename T1 = double, typename T2 = double, typename T3 = double>
constexpr auto is_right_angled(const T1& a, const T2& b, const T3& c) -> bool
{
  // Check if T is floating point or integer
  static_assert(std::is_floating_point<T1>::value || std::is_integral<T1>::value,
                "T1 must be floating point or integer");
  static_assert(std::is_floating_point<T2>::value || std::is_integral<T2>::value,
                "T2 must be floating point or integer");
  static_assert(std::is_floating_point<T3>::value || std::is_integral<T3>::value,
                "T3 must be floating point or integer");

  return (a * a) + (b * b) == (c * c);
}

template<typename T1 = uint32_t, typename T2 = uint32_t, typename T3 = uint64_t>
constexpr auto concate(const T1& a, const T2& b) -> T3
{
  static_assert(std::is_unsigned<T1>::value && std::is_unsigned<T2>::value && std::is_unsigned<T3>::value,
                "T1, T2, and T3 must be unsigned integer types");
  static_assert(sizeof(T1) + sizeof(T2) == sizeof(T3), "T3 must be able to hold T1 and T2");

  return (static_cast<T3>(a) << (sizeof(T1) * 8)) | b;
}

template<typename T1 = uint64_t, typename T2 = uint32_t, typename T3 = uint32_t>
constexpr auto split(const T1& a) -> std::pair<T2, T3>
{
  static_assert(std::is_unsigned<T1>::value && std::is_unsigned<T2>::value && std::is_unsigned<T3>::value,
                "T1, T2, and T3 must be unsigned integer types");

  static_assert(sizeof(T1) == sizeof(T2) + sizeof(T3), "T1 must be able to hold T2 and T3");

  return {static_cast<T2>(a >> sizeof(T2) * 8), a & std::numeric_limits<T3>::max()};
  // return {static_cast<T2>(a >> sizeof(T2) * 8), a & T3(-1)};
}

template<typename T1 = uint64_t>
constexpr auto to_binary(const T1& a) -> std::string
{
  std::bitset<sizeof(T1) * 8> bits(a);
  return bits.to_string();
}

}  // namespace experimentation

}  // namespace math

}  // namespace benlib

#endif  // BENLIB_MATH_EXPERIMENTATION_HPP
