#ifndef CUBE_GENERATOR_HPP
#define CUBE_GENERATOR_HPP

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
        explicit generator(uint32_t _seed)
            : seed(_seed)
        {
            perlin.reseed(seed);
        }

        generator() = default;

        ~generator() {}

        void reseed(uint32_t seed)
        {
            this->seed = seed;
            perlin.reseed(seed);
        }

        uint32_t get_seed() const
        {
            return seed;
        }

        void generate()
        {
            
        }

    private:
        // default seed
        siv::PerlinNoise::seed_type seed = 2647393077u;
        siv::PerlinNoise perlin {seed};
};

#endif  // CUBE_GENERATOR_HPP