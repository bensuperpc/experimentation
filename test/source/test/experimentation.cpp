#include <string>

#include "experimentation/experimentation.hpp"

#include "gtest/gtest.h"

TEST(experimentation, basic1)
{
    const std::string result = "experimentation";
    const std::string expected = "experimentation";

    EXPECT_EQ(result, expected);
}

auto main(int argc, char** argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
