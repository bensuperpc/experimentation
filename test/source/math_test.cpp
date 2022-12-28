#include <string>

#include "experimentation/math.hpp"

#include "gtest/gtest.h"

namespace math = benlib::math::experimentation;

TEST(experimentation, basic1)
{
  using T1 = uint32_t;
  using T2 = uint32_t;
  using T3 = uint64_t;

  T1 a = 64;
  T2 b = 32;

  T3 result = math::concate<T1, T2, T3>(a, b);

  EXPECT_EQ(result, 274877906976);

  std::pair<T1, T2> pair = math::split<T3, T1, T2>(result);

  EXPECT_EQ(pair.first, a);
  EXPECT_EQ(pair.second, b);
}

TEST(experimentation, basic2)
{
  using T1 = uint16_t;
  using T2 = uint16_t;
  using T3 = uint32_t;

  T1 a = 64;
  T2 b = 32;

  T3 result = math::concate<T1, T2, T3>(a, b);

  EXPECT_EQ(result, 4194336);

  std::pair<T1, T2> pair = math::split<T3, T1, T2>(result);

  EXPECT_EQ(pair.first, a);
  EXPECT_EQ(pair.second, b);
}
