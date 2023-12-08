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

void generate(std::vector<unsigned char>& v, const int screen_width, const int screenHeight, siv::PerlinNoise& perlin)
{
#pragma omp parallel for collapse(2) schedule(auto)
    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screen_width; x++) {
            const uint8_t value_int = static_cast<uint8_t>(
                std::clamp(perlin.normalizedOctave2D_01(x / 100.0, y / 100.0, 16, 0.1) * 255.0, 0.0, 255.0));
            v[y * screen_width + x] = value_int;
        }
    }
}

auto main() -> int
{
    const int screen_width = 1920;
    const int screenHeight = 1080;
    const uint32_t targetFPS = 120;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    raylib::Window window(screen_width, screenHeight, "perlin noise");

    SetTargetFPS(targetFPS);

    std::vector<unsigned char> v(screen_width * screenHeight, 0);

    siv::PerlinNoise::seed_type seed = 67890u;
    std::cout << "seed: " << seed << std::endl;

    siv::PerlinNoise perlin {seed};

    generate(v, screen_width, screenHeight, perlin);

    // Create image buffer
    raylib::Image gridImage(screen_width, screenHeight, RAYWHITE);
    Color* pixels = gridImage.LoadColors();
    raylib::Texture gridTexture(gridImage);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            decltype(seed) seed = std::random_device()();
            std::cout << "seed: " << seed << std::endl;
            perlin.reseed(seed);
            generate(v, screen_width, screenHeight, perlin);
        }

        if (IsKeyPressed(KEY_S)) {
            // Update image buffer before saving
            gridImage.Load(gridTexture);
            gridImage.Export("perlin_noise.png");
        }

        // Update image buffer
        for (int y = 0; y < screenHeight; y++) {
            for (int x = 0; x < screen_width; x++) {
                // Black = 255 and white = 0
                unsigned char r = static_cast<unsigned char>(v[y * screen_width + x]);
                unsigned char g = static_cast<unsigned char>(v[y * screen_width + x]);
                unsigned char b = static_cast<unsigned char>(v[y * screen_width + x]);
                pixels[static_cast<uint64_t>(x + y * screen_width)] = Color {r, g, b, 255};
            }
        }

        // Update texture
        gridTexture.Update(pixels);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(gridTexture, 0, 0, WHITE);

        EndDrawing();
    }

    // Free all memory
    gridImage.UnloadColors(pixels);

    return 0;
}
