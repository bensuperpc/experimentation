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
        chunk() {}

        ~chunk() {}

        [[nodiscard]] raylib::Vector3 get_position() const { return {chunk_x, chunk_y, chunk_z}; }
        [[nodiscard]] raylib::Vector3 get_real_position() const
        {
            return {chunk_x * chunk_size_x, chunk_y * chunk_size_y, chunk_z * chunk_size_z};
        }

    private:
        const int chunk_size_x = 16;
        const int chunk_size_y = 64;
        const int chunk_size_z = 16;
        const std::vector<block>::size_type chunk_size = chunk_size_x * chunk_size_y * chunk_size_z;

        std::vector<block> blocks = std::vector<block>(chunk_size, block());

        // Chunk coordinates
        int chunk_x = 0;
        int chunk_y = 0;
        int chunk_z = 0;
};

#endif  // CUBE_CHUNK_HPP