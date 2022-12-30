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
    

    block(float x, float y, float z, float size_x, float size_y, float size_z, Color color, bool is_visible)
        : x(x), y(y), z(z), size_x(size_x), size_y(size_y), size_z(size_z), color(color), display(is_visible)
    {
    }

    ~block()
    {
        
    }

    //Vector3 position = {0.0f, 0.0f, 0.0f};
    //Vector3 size = {1.0f, 1.0f, 1.0f};
    /*
    float & x = position.x;
    float & y = position.y;
    float & z = position.z;
    float & size_x = size.x;
    float & size_y = size.y;
    float & size_z = size.z;
    */
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float size_x = 1.0f;
    float size_y = 1.0f;
    float size_z = 1.0f;

    Color color = raylib::Color::Gray();
    bool display = true;

    /*
    Color color = raylib::Color::Gray();
    uint64_t block_type = 0;
    bool is_visible = true;
    */
};