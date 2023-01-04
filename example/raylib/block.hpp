#ifndef CUBE_BLOCK_HPP
#define CUBE_BLOCK_HPP

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include <omp.h>

#include "PerlinNoise.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "raymath.h"

// Block type enum

namespace block_type
{
size_t constexpr air = 0;
size_t constexpr grass = 1;
size_t constexpr dirt = 2;
size_t constexpr stone = 3;
size_t constexpr sand = 4;
size_t constexpr water = 5;
size_t constexpr wood = 6;
size_t constexpr leaves = 7;

std::string get_name(size_t block_type)
{
    switch (block_type) {
        case air:
            return "air";
        case grass:
            return "grass";
        case dirt:
            return "dirt";
        case stone:
            return "stone";
        case sand:
            return "sand";
        case water:
            return "water";
        case wood:
            return "wood";
        case leaves:
            return "leaves";
        default:
            return "unknown";
    }
}

}  // namespace block_type

struct Vector3i
{
        int x;
        int y;
        int z;
};

struct Vector2i
{
        int x;
        int y;
};

class block
{
    public:
        explicit block(int x, int y, int z, Color color, size_t block_type, bool is_visible)
            : x(x)
            , y(y)
            , z(z)
            , color(color)
            , block_type(block_type)
            , is_visible(is_visible)
        {
        }

        block() {}

        ~block() {}

        [[nodiscard]] BoundingBox get_bounding_box() const
        {
            raylib::Vector3 cube_begin = get_real_position();
            cube_begin.x -= size_x / 2;
            cube_begin.y -= size_y / 2;
            cube_begin.z -= size_z / 2;
            raylib::Vector3 cube_end = get_real_position();
            cube_end.x += size_x / 2;
            cube_end.y += size_y / 2;
            cube_end.z += size_z / 2;
            
            BoundingBox box = {cube_begin, cube_end};

            return box;
        }

        [[nodiscard]] raylib::Vector3 get_size() const { return {size_x, size_y, size_z}; }

        [[nodiscard]] raylib::Vector3 get_center() const
        {
            Vector3 center = get_real_position();
            center.x += size_x / 2;
            center.y += size_y / 2;
            center.z += size_z / 2;
            return center;
        }

        [[nodiscard]] raylib::Vector3 get_real_position() const { return {x * size_x, y * size_y, z * size_z}; }

        [[nodiscard]] Vector3i get_position() const { return {x, y, z}; }

        // Block coordinates
        int x = 0;
        int y = 0;
        int z = 0;

        static constexpr float size_x = 2.0f;
        static constexpr float size_y = 2.0f;
        static constexpr float size_z = 2.0f;

        Color color = raylib::Color::Gray();
        bool is_visible = true;

        size_t block_type = 0;

        raylib::Texture2D* texture = nullptr;

        // debug
        size_t neighbors = 0;
        size_t edges = 0;
};

#endif  // CUBE_BLOCK_HPP