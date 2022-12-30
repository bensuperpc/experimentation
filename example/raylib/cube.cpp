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

struct cube
{
    float x;
    float y;
    float z;
    float size_x;
    float size_y;
    float size_z;
    Color color;
    bool display;
};

void generate(
    std::vector<unsigned char>& v, std::vector<cube>& cubes, int width, int height, int depth, siv::PerlinNoise& perlin)
{
#pragma omp parallel for collapse(3) schedule(auto)
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      for (int z = 0; z < depth; z++) {
        const uint8_t value_int =
            static_cast<uint8_t>(perlin.octave3D_01(x / 128.0, y / 128.0, z / 128.0, 16, 0.1) * 255.0);
        v[x * height * depth + y * depth + z] = value_int;
      }
    }
  }

  // cout max and min
  auto minmax = std::minmax_element(v.begin(), v.end());
  std::cout << "min: " << static_cast<int>(*minmax.first) << std::endl;
  std::cout << "max: " << static_cast<int>(*minmax.second) << std::endl;

  const float cube_size = 1.0f;

#pragma omp parallel for collapse(2) schedule(auto)
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      size_t vec_index = y * width + x;
      unsigned char noise_value = v[vec_index] / 16;
      for (int z = 0; z < depth; z++) {
        if (z < noise_value) {
          cubes[vec_index * depth + z] = cube {x * cube_size,
                                               z * cube_size,
                                               y * cube_size,
                                               cube_size,
                                               cube_size,
                                               cube_size,
                                               raylib::Color::Gray(),
                                               true};
        } else {
          cubes[vec_index * depth + z] = cube {x * cube_size,
                                               z * cube_size,
                                               y * cube_size,
                                               cube_size,
                                               cube_size,
                                               cube_size,
                                               raylib::Color::Gray(),
                                               false};
        }
      }
    }
  }

  auto visible = [](const cube& c) { return c.display; };
  auto count = std::count_if(cubes.begin(), cubes.end(), visible);
  std::cout << "Only " << count << " cubes are visible out of " << cubes.size() << std::endl;
}

int main()
{
  const int screen_width = 1920;
  const int screenHeight = 1080;

  const float cube_size = 1.0f;

  raylib::Window window(screen_width, screenHeight, "Minecube");

  siv::PerlinNoise::seed_type seed = 2647393077u;
  std::cout << "seed: " << seed << std::endl;

  siv::PerlinNoise perlin {seed};

  size_t vecX = 128;
  size_t vecY = 128;
  size_t vecZ = 128;

  size_t vec_size = vecX * vecY * vecZ;
  std::vector<unsigned char> v(vec_size, 0);
  std::vector<cube> cubes(vec_size, cube {0, 0, 0, 0, 0, 0, Color {0, 0, 0, 0}, false});
  generate(v, cubes, vecX, vecY, vecZ, perlin);

  raylib::Camera camera(
      raylib::Vector3(static_cast<float>(vecX * cube_size / 4), 64.0f, static_cast<float>(vecX * cube_size / 4)),
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
      generate(v, cubes, vecX, vecY, vecZ, perlin);
    }

    BeginDrawing();
    {
      window.ClearBackground(RAYWHITE);

      camera.BeginMode();
#pragma omp parallel for schedule(auto) num_threads(1)
      for (size_t i = 0; i < cubes.size(); i++) {
        if (!cubes[i].display) {
          continue;
        }

        // Draw only surfaces
        if (cubes[i + 1].display) {
          continue;
        }
#pragma omp critical
        DrawCubeV(raylib::Vector3(cubes[i].x, cubes[i].y, cubes[i].z),
                  raylib::Vector3(cubes[i].size_x, cubes[i].size_y, cubes[i].size_z),
                  cubes[i].color);
#pragma omp critical
        DrawCubeWiresV(raylib::Vector3(cubes[i].x, cubes[i].y, cubes[i].z),
                       raylib::Vector3(cubes[i].size_x, cubes[i].size_y, cubes[i].size_z),
                       raylib::Color::Black());
      }

      camera.EndMode();

      DrawFPS(10, 10);
    }
    EndDrawing();
  }

  return 0;
}