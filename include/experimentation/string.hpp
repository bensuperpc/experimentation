#ifndef BENLIB_STRING_EXPERIMENTATION_HPP
#define BENLIB_STRING_EXPERIMENTATION_HPP

#include <string>
#include <string_view>

#include "experimentation/string_external.hpp"

namespace benlib
{

namespace string
{

namespace experimentation
{

bool is_palindrome_for(std::string_view str)
{
  const auto size = str.size();
  for (std::size_t i = 0; i < size / 2; ++i) {
    if (str[i] != str[size - i - 1]) {
      return false;
    }
  }
  return true;
}

bool is_palindrome_std_v1(std::string_view str)
{
  std::string copy_str = std::string {str};
  std::reverse(copy_str.begin(), copy_str.end());
  return copy_str == str;
}

bool is_palindrome_std_v2(std::string_view str)
{
  return std::equal(str.begin(), str.begin() + str.size() / 2, str.rbegin());
}

}  // namespace experimentation

}  // namespace string

}  // namespace benlib

#endif  // BENLIB_STRING_EXPERIMENTATION_HPP
