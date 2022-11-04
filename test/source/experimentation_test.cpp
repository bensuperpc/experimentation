#include <string>

#include "experimentation/experimentation.hpp"

#include "gtest/gtest.h"

TEST(experimentation, basic1)
{
  const std::string result = "experimentation";
  const std::string expected = "experimentation";

  EXPECT_EQ(result, expected);
}
