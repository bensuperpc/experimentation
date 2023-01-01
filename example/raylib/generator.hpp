#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include <omp.h>

#include "PerlinNoise.hpp"
// Raylib
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "raymath.h"

// Cube lib
#include "block.hpp"

class generator
{
    public:
        explicit generator(int world_size_x, int world_size_y, int world_size_z)
            : world_size_x(world_size_x)
            , world_size_y(world_size_y)
            , world_size_z(world_size_z)
        {
        }

        ~generator() {}

        int world_size_x = 128;
        int world_size_y = 128;
        int world_size_z = 128;
};