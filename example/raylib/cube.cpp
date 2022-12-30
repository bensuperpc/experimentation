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

/*
void generate(std::vector<unsigned char>& v, int width, int height, int depth, siv::PerlinNoise& perlin)
{
#pragma omp parallel for collapse(3) schedule(auto)
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                const uint8_t value_int =
                        static_cast<uint8_t>(perlin.octave3D_01(x / 100.0, y / 100.0, z / 100.0, 16, 0.1) * 255.0);
                v[x * height * depth + y * depth + z] = value_int;
            }
        }
    }
}
*/

struct cube
{
    float x;
    float y;
    float z;
    float size_x;
    float size_y;
    float size_z;
    Color color;
};


void generate(std::vector<unsigned char>& v, std::vector<cube>& c, int width, int height, siv::PerlinNoise& perlin)
{
#pragma omp parallel for collapse(2) schedule(auto)
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      const uint8_t value_int = static_cast<uint8_t>(perlin.octave2D_01(x / 100.0, y / 100.0, 16, 0.1) * 255.0);
      v[x * height + y] = value_int;
    }
  }
}

int main()
{
  const int screen_width = 1920;
  const int screenHeight = 1080;

  const float cube_size = 1.0f;

  raylib::Window window(screen_width, screenHeight, "raylib [core] example - 3d camera first person");

  siv::PerlinNoise::seed_type seed = 12345u;
  std::cout << "seed: " << seed << std::endl;

  siv::PerlinNoise perlin {seed};

  size_t vecX = 64;
  size_t vecY = 64;
  /*
  size_t vecZ = 1;
  std::vector<unsigned char> v(vecX * vecY * vecZ, 0);
  generate(v, vecX, vecY, vecZ, perlin);
  */
  std::vector<unsigned char> v(vecX * vecY, 0);
  std::vector<cube> c(vecX * vecY, cube{0, 0, 0, 0, 0, 0, Color{0, 0, 0, 0}});
  generate(v, c, vecX, vecY, perlin);

  // cout max and min
  auto minmax = std::minmax_element(v.begin(), v.end());
  std::cout << "min: " << static_cast<int>(*minmax.first) << std::endl;
  std::cout << "max: " << static_cast<int>(*minmax.second) << std::endl;

  raylib::Camera camera(raylib::Vector3(static_cast<float>(vecX * cube_size / 4), 24.0f, static_cast<float>(vecX * cube_size/ 4)),
                        raylib::Vector3(static_cast<float>(vecX * cube_size / 2), 0.0f, static_cast<float>(vecX * cube_size / 2)),
                        raylib::Vector3(0.0f, 1.0f, 0.0f),
                        60.0f,
                        CAMERA_PERSPECTIVE);

  camera.SetMode(CAMERA_ORBITAL);

  SetTargetFPS(30);

  while (!window.ShouldClose()) {
    camera.Update();

    if (IsKeyPressed(KEY_R)) {
      decltype(seed) seed = std::random_device()();
      std::cout << "seed: " << seed << std::endl;
      perlin.reseed(seed);
      generate(v, c, vecX, vecY, perlin);

      auto minmax = std::minmax_element(v.begin(), v.end());
      std::cout << "min: " << static_cast<int>(*minmax.first) << std::endl;
      std::cout << "max: " << static_cast<int>(*minmax.second) << std::endl;
    }

    BeginDrawing();
    {
      window.ClearBackground(RAYWHITE);

      camera.BeginMode();

      for (int y = 0; y < vecY; y++) {
        for (int x = 0; x < vecX; x++) {
          // Noise value is equal to the height of the column at this position
          // 0 = lowest, no cube
          // 255 = highest, 255 cubes high
          size_t vec_index = y * vecX + x;
          unsigned char noise_value = v[vec_index] / 16;


          for (int z = 0; z < noise_value; z++) {
            DrawCube(
                Vector3(x * cube_size, z * cube_size, y * cube_size), cube_size, cube_size, cube_size, raylib::Color::Gray());
            DrawCubeWires(Vector3(x * cube_size, z * cube_size, y * cube_size),
                          cube_size,
                          cube_size,
                          cube_size,
                          raylib::Color::DarkGray());
          }
        }
      }

      camera.EndMode();

      DrawFPS(10, 10);
    }
    EndDrawing();
  }

  return 0;
}