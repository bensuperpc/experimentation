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

class block
{
    public:
        explicit block(
            float x, float y, float z, float size_x, float size_y, float size_z, Color color, bool is_visible)
            : x(x)
            , y(y)
            , z(z)
            , size_x(size_x)
            , size_y(size_y)
            , size_z(size_z)
            , color(color)
            , is_visible(is_visible)
        {
        }

        ~block() {}

        BoundingBox get_bounding_box() const
        {
            Vector3 cube_begin = {x - size_x / 2, y - size_y / 2, z - size_z / 2};
            Vector3 cube_end = {x + size_x / 2, y + size_y / 2, z + size_z / 2};
            BoundingBox box = {cube_begin, cube_end};
            return box;
        }

        Vector3 get_block_center() const { return {x + size_x / 2, y + size_y / 2, z + size_z / 2}; }

        void draw() const { DrawCubeV({x, y, z}, {size_x, size_y, size_z}, color); }

        void draw_box() const { DrawCubeWiresV({x, y, z}, {size_x, size_y, size_z}, raylib::Color::Black()); }

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
};