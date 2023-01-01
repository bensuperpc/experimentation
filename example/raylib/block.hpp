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
        explicit block(int x,
                       int y,
                       int z,
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
            raylib::Vector3 cube_begin = {x * size_x - size_x / 2, y * size_y - size_y / 2, z * size_z - size_z / 2};
            raylib::Vector3 cube_end = {x * size_x + size_x / 2, y * size_y + size_y / 2, z * size_z + size_z / 2};
            BoundingBox box = {cube_begin, cube_end};
            return box;
        }

        void draw() const
        {
            DrawCubeV(raylib::Vector3(x * size_x, y * size_y, z * size_z), {size_x, size_y, size_z}, color);
        }

        void draw_texture() const
        {
            DrawCubeTexture(*texture, raylib::Vector3(x * size_x, y * size_y, z * size_z), size_x, size_y, size_z, raylib::Color::White());
        }

        void draw_wireframe() const
        {
            DrawCubeWiresV(raylib::Vector3(x * size_x, y * size_y, z * size_z), {size_x, size_y, size_z}, raylib::Color::Black());
        }

        void draw_box() const
        {
            DrawCubeWiresV(raylib::Vector3(x * size_x, y * size_y, z * size_z), {size_x, size_y, size_z}, raylib::Color::Black());
        }

        raylib::Vector3 get_size() const { return {size_x, size_y, size_z}; }

        raylib::Vector3 get_center() const { return raylib::Vector3(x * size_x, y * size_y, z * size_z); }

        raylib::Vector3 get_real_position() const { return {x * size_x, y * size_y, z * size_z}; }

        Vector3i get_position() const { return {x, y, z}; }

        // Block coordinates
        int x = 0;
        int y = 0;
        int z = 0;

        float size_x = 2.0f;
        float size_y = 2.0f;
        float size_z = 2.0f;

        Color color = raylib::Color::Gray();
        bool is_visible = true;

        size_t block_type = 0;

        raylib::Texture2D* texture = nullptr;
};

#endif  // CUBE_BLOCK_HPP