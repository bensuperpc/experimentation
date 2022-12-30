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

void generate(std::vector<cube>& cubes,
              const int width,
              const int height,
              const int depth,
              siv::PerlinNoise& perlin,
              const float cube_size)
{
  std::vector<unsigned char> v(width * height * depth, 0);
#pragma omp parallel for collapse(3) schedule(auto)
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      for (int z = 0; z < depth; z++) {
        const uint8_t value_int =
            static_cast<uint8_t>(perlin.octave3D_01(x / 256.0, y / 256.0, z / 256.0, 16, 0.2) * 255.0);
        v[x * height * depth + y * depth + z] = value_int;
      }
    }
  }

  // cout max and min
  auto minmax = std::minmax_element(v.begin(), v.end());
  std::cout << "min: " << static_cast<int>(*minmax.first) << std::endl;
  std::cout << "max: " << static_cast<int>(*minmax.second) << std::endl;

#pragma omp parallel for collapse(2) schedule(auto)
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      size_t vec_index = y * width + x;
      unsigned char noise_value = v[vec_index] / 4;
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
  const int screen_width = 2560;
  const int screen_height = 1440;
  const int target_fps = 30;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  raylib::Window window(screen_width, screen_height, "Minecube");
  SetTargetFPS(target_fps);

  const float cube_size = 1.0f;

  siv::PerlinNoise::seed_type seed = 2647393077u;
  std::cout << "seed: " << seed << std::endl;

  siv::PerlinNoise perlin {seed};

  size_t vecX = 128;
  size_t vecY = 128;
  size_t vecZ = 128;

  size_t vec_size = vecX * vecY * vecZ;
  std::vector<cube> cubes(vec_size, cube {0, 0, 0, 0, 0, 0, Color {0, 0, 0, 0}, false});
  generate(cubes, vecX, vecY, vecZ, perlin, cube_size);

  raylib::Camera camera(
      raylib::Vector3(static_cast<float>(vecX * cube_size / 4), 64.0f, static_cast<float>(vecX * cube_size / 4)),
      raylib::Vector3(static_cast<float>(vecX * cube_size / 2), 0.0f, static_cast<float>(vecX * cube_size / 2)),
      raylib::Vector3(0.0f, 1.0f, 0.0f),
      60.0f,
      CAMERA_PERSPECTIVE);

  camera.SetMode(CAMERA_ORBITAL);

  while (!window.ShouldClose()) {
    camera.Update();

    if (IsKeyPressed(KEY_R)) {
      decltype(seed) seed = std::random_device()();
      std::cout << "seed: " << seed << std::endl;
      perlin.reseed(seed);
      generate(cubes, vecX, vecY, vecZ, perlin, cube_size);
    }

    BeginDrawing();
    {
      window.ClearBackground(RAYWHITE);

      camera.BeginMode();

      // Statistics
      size_t skip_by_display = 0;
      size_t skip_by_all_neighbors = 0;
      size_t skip_by_out_of_screen = 0;

#pragma omp parallel for schedule(auto) num_threads(1)
      for (size_t i = 0; i < cubes.size(); i++) {
        cube& current_cube = cubes[i];

        // Draw only cubes if display is true
        if (!current_cube.display) {
          skip_by_display++;
          continue;
        }

        // Draw only surfaces
        // if (cubes[i + 1].display) {
        //  continue;
        //}

        // int x = i % vecX;
        // int y = (i / vecX) % vecY;
        // int z = i / (vecX * vecY);

        // If cube has all 6 neighbors is displayed, skip it
        const cube& c1 = cubes[std::max(0, static_cast<int32_t>(i) - 1)];
        const cube& c2 = cubes[std::min(static_cast<int32_t>(cubes.size()) - 1, static_cast<int32_t>(i) + 1)];
        const cube& c3 = cubes[std::max(0, static_cast<int32_t>(i) - static_cast<int32_t>(vecX))];
        const cube& c4 = cubes[std::min(static_cast<int32_t>(cubes.size()) - 1,
                                        static_cast<int32_t>(i) + static_cast<int32_t>(vecX))];
        const cube& c5 = cubes[std::max(0, static_cast<int32_t>(i) - static_cast<int32_t>(vecX * vecY))];
        const cube& c6 = cubes[std::min(static_cast<int32_t>(cubes.size()) - 1,
                                        static_cast<int32_t>(i) + static_cast<int32_t>(vecX * vecY))];

        if (c1.display && c2.display && c3.display && c4.display && c5.display && c6.display) {
          skip_by_all_neighbors++;
          continue;
        }

        // If cube is not visible on screen, skip it
        const Vector3 cube_center = {current_cube.x + current_cube.size_x / 2,
                                     current_cube.y + current_cube.size_y / 2,
                                     current_cube.z + current_cube.size_z / 2};

        const Vector2 cube_screen_pos = camera.GetWorldToScreen(cube_center);

        if (cube_screen_pos.x < 0 || cube_screen_pos.x > GetScreenWidth() || cube_screen_pos.y < 0
            || cube_screen_pos.y > GetScreenHeight())
        {
          skip_by_out_of_screen++;
          continue;
        }

#pragma omp critical
        DrawCubeV(raylib::Vector3(current_cube.x, current_cube.y, current_cube.z),
                  raylib::Vector3(current_cube.size_x, current_cube.size_y, current_cube.size_z),
                  current_cube.color);
#pragma omp critical
        DrawCubeWiresV(raylib::Vector3(current_cube.x, current_cube.y, current_cube.z),
                       raylib::Vector3(current_cube.size_x, current_cube.size_y, current_cube.size_z),
                       raylib::Color::Black());
      }

      std::cout << "skip_by_display: " << skip_by_display << std::endl;
      std::cout << "skip_by_all_neighbors: " << skip_by_all_neighbors << std::endl;
      std::cout << "skip_by_out_of_screen: " << skip_by_out_of_screen << std::endl;
      std::cout << "total skipped: " << skip_by_display + skip_by_all_neighbors + skip_by_out_of_screen << std::endl;
      std::cout << "total cubes: " << cubes.size() << std::endl;
      std::cout << "total cubes displayed: "
                << cubes.size() - skip_by_display - skip_by_all_neighbors - skip_by_out_of_screen;
      std::cout << " ("
                << static_cast<float>(cubes.size() - skip_by_display - skip_by_all_neighbors - skip_by_out_of_screen)
              / static_cast<float>(cubes.size()) * 100.0f
                << "%)" << std::endl;
      std::cout << std::endl;

      skip_by_display = 0;
      skip_by_all_neighbors = 0;
      skip_by_out_of_screen = 0;

      camera.EndMode();

      DrawFPS(10, 10);
    }
    EndDrawing();
  }

  return 0;
}