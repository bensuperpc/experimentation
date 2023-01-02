#ifndef CUBE_CHUNK_HPP
#define CUBE_CHUNK_HPP

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

class chunk
{
    public:
        explicit chunk()
        {
        }

        ~chunk() {}

    static constexpr int chunk_size_x = 16;
    static constexpr int chunk_size_y = 64;
    static constexpr int chunk_size_z = 16;
    static constexpr int chunk_size = chunk_size_x * chunk_size_y * chunk_size_z;

    std::vector<block> blocks = std::vector<block>(chunk_size);

};

#endif  // CUBE_CHUNK_HPP