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
        chunk(std::vector<block> blocks, int chunk_x, int chunk_y, int chunk_z)
            : blocks(std::move(blocks))
            , chunk_x(chunk_x)
            , chunk_y(chunk_y)
            , chunk_z(chunk_z)
        {
        }

        ~chunk() {}

        [[nodiscard]] raylib::Vector3 get_position() const { return {chunk_x, chunk_y, chunk_z}; }
        [[nodiscard]] raylib::Vector3 get_real_position() const
        {
            return {chunk_x * chunk_size_x, chunk_y * chunk_size_y, chunk_z * chunk_size_z};
        }

        [[nodiscard]] std::vector<block>& get_blocks() { return blocks; }
        void set_blocks(std::vector<block>& blocks) { this->blocks = blocks; }

        //[[nodiscard]] block& get_block(int index) { return blocks[index]; }
        // void set_block(int chuck_index, block &b) { blocks[chuck_index] = b; }

        //[[nodiscard]] block& get_block(int x, int y, int z) { return blocks[z * chunk_size_x + y * chunk_size_x *
        //chunk_size_z + x]; }
        //[[nodiscard]] block& get_block(int x, int y, int z) { return blocks[z * chunk_x + y * chunk_x * chunk_z + x];
        //}

        static constexpr int chunk_size_x = 16;
        static constexpr int chunk_size_y = 64;
        static constexpr int chunk_size_z = 16;
        static constexpr std::vector<block>::size_type chunk_size = chunk_size_x * chunk_size_y * chunk_size_z;

    private:
        std::vector<block> blocks = std::vector<block>(chunk_size, block());

        // Chunk coordinates
        int chunk_x = 0;
        int chunk_y = 0;
        int chunk_z = 0;
};

#endif  // CUBE_CHUNK_HPP