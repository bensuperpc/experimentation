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
}  // namespace block_type

class block
{
    public:
        explicit block(float x,
                       float y,
                       float z,
                       float size_x,
                       float size_y,
                       float size_z,
                       Color color,
                       size_t block_type,
                       bool is_visible)
            : x(x)
            , y(y)
            , z(z)
            , size_x(size_x)
            , size_y(size_y)
            , size_z(size_z)
            , color(color)
            , block_type(block_type)
            , is_visible(is_visible)
        {
        }

        ~block() {}

        BoundingBox get_bounding_box() const
        {
            raylib::Vector3 cube_begin = {x - size_x / 2, y - size_y / 2, z - size_z / 2};
            raylib::Vector3 cube_end = {x + size_x / 2, y + size_y / 2, z + size_z / 2};
            BoundingBox box = {cube_begin, cube_end};
            return box;
        }

        void draw() const { DrawCubeV({x, y, z}, {size_x, size_y, size_z}, color); }
        void draw_texture() const { DrawCubeTexture(*texture, {x, y, z}, size_x, size_y, size_z, raylib::Color::White()); }

        void draw_box() const { DrawCubeWiresV({x, y, z}, {size_x, size_y, size_z}, raylib::Color::Black()); }

        raylib::Vector3 get_position() const { return {x, y, z}; }
        raylib::Vector3 get_size() const { return {size_x, size_y, size_z}; }
        raylib::Vector3 get_center() const { return {x + size_x / 2, y + size_y / 2, z + size_z / 2}; }

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float size_x = 1.0f;
        float size_y = 1.0f;
        float size_z = 1.0f;

        Color color = raylib::Color::Gray();
        bool is_visible = true;

        size_t block_type = 0;

        size_t block_x = 0;
        size_t block_y = 0;
        size_t block_z = 0;

        raylib::Texture2D* texture = nullptr;
};