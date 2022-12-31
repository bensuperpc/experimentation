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
#include "block.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "raymath.h"

void generate(std::vector<block>& blocks,
              const int width,
              const int height,
              const int depth,
              siv::PerlinNoise& perlin,
              const float cube_size)
{
  std::vector<unsigned char> v(width * height * depth, 0);

  // Generate noise
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

// Generate blocks
#pragma omp parallel for collapse(2) schedule(auto)
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      size_t vec_index = y * width + x;
      unsigned char noise_value = v[vec_index] / 4;
      for (int z = 0; z < depth; z++) {
        block& current_block = blocks[vec_index * depth + z];
        current_block.x = x * cube_size;
        current_block.y = z * cube_size;
        current_block.z = y * cube_size;
        current_block.size_x = cube_size;
        current_block.size_y = cube_size;
        current_block.size_z = cube_size;
        current_block.color = raylib::Color::Gray();

        current_block.block_x = x;
        current_block.block_y = y;
        current_block.block_z = z;

        if (z < noise_value) {
          current_block.is_visible = true;
        } else {
          current_block.is_visible = false;
        }
      }
    }
  }

  auto visible = [](const block& c) { return c.is_visible; };
  auto count = std::count_if(blocks.begin(), blocks.end(), visible);
  std::cout << "Only " << count << " blocks are visible out of " << blocks.size() << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  const int screen_width = 1920;
  const int screen_height = 1080;
  const int target_fps = 30;

  bool show_grid = true;
  bool optimisation = true;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  raylib::Window window(screen_width, screen_height, "Minecube");
  SetTargetFPS(target_fps);

  const float cube_size = 2.0f;

  siv::PerlinNoise::seed_type seed = 2647393077u;
  std::cout << "seed: " << seed << std::endl;

  siv::PerlinNoise perlin {seed};

  size_t vecX = 96;
  size_t vecY = 96;
  size_t vecZ = 96;

  size_t vec_size = vecX * vecY * vecZ;
  std::vector<block> blocks(vec_size, block {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Color {0, 0, 0, 0}, false});
  generate(blocks, vecX, vecY, vecZ, perlin, cube_size);

  raylib::Camera camera(
      raylib::Vector3(static_cast<float>(vecX * cube_size / 4), (vecX / 2) * cube_size * 0.7f, static_cast<float>(vecX * cube_size / 4)),
      raylib::Vector3(static_cast<float>(vecX * cube_size / 2), 0.0f, static_cast<float>(vecX * cube_size / 2)),
      raylib::Vector3(0.0f, 1.0f, 0.0f),
      60.0f,
      CAMERA_PERSPECTIVE);

  camera.SetMode(CAMERA_FIRST_PERSON);

  // Ray and closest_collision
  Ray ray = {0};
  std::vector<std::pair<block*, RayCollision>> collisions;

  RayCollision closest_collision = {0};
  block *closest_block = nullptr;

  Vector2 screen_middle = {static_cast<float>(screen_width / 2), static_cast<float>(screen_height / 2)};

  while (!window.ShouldClose()) {
    camera.Update();

    closest_collision = {0};
    closest_collision.hit = false;
    closest_collision.distance = std::numeric_limits<float>::max();
    if (closest_block != nullptr) {
      closest_block->color = raylib::Color::Gray();
    }
    collisions.clear();
  
    Vector2 mouse_pos = GetMousePosition();
    ray = GetMouseRay(screen_middle, camera);

    if (IsKeyPressed(KEY_R)) {
      decltype(seed) seed = std::random_device()();
      std::cout << "seed: " << seed << std::endl;
      perlin.reseed(seed);
      generate(blocks, vecX, vecY, vecZ, perlin, cube_size);
    }

    if (IsKeyPressed(KEY_G)) {
      show_grid = !show_grid;
    }

    if (IsKeyPressed(KEY_O)) {
      optimisation = !optimisation;
    }

    if (IsKeyPressed(KEY_F5)) {
      // Take screenshot
      TakeScreenshot("screenshot.png");
    }

#pragma omp parallel for schedule(auto)
    for (size_t i = 0; i < blocks.size(); i++) {
      block& current_cube = blocks[i];
      if (current_cube.is_visible) {
        Vector3 cube_begin = {current_cube.x - current_cube.size_x / 2,
                              current_cube.y - current_cube.size_y / 2,
                              current_cube.z - current_cube.size_z / 2};
        Vector3 cube_end = {current_cube.x + current_cube.size_x / 2,
                            current_cube.y + current_cube.size_y / 2,
                            current_cube.z + current_cube.size_z / 2};
        BoundingBox box = {cube_begin, cube_end};

        RayCollision box_hit_info = GetRayCollisionBox(ray, box);

        if (box_hit_info.hit) {
          #pragma omp critical
          collisions.push_back({&current_cube, box_hit_info});
        }
      }
    }

    if (!collisions.empty()) {
      std::sort(collisions.begin(), collisions.end(), [](const auto& a, const auto& b) {
        return a.second.distance < b.second.distance;
      });
      closest_collision = collisions[0].second;
      closest_block = collisions[0].first;
      closest_block->color = raylib::Color::Red();
      std::cout << "Hit block: " << closest_collision.hit << " at distance: " << closest_collision.distance << std::endl;
    }

    // Statistics
    size_t skip_by_display = 0;
    size_t skip_by_all_neighbors = 0;
    size_t skip_by_out_of_screen = 0;
    size_t skip_by_surface_only = 0;

    BeginDrawing();
    {
      window.ClearBackground(RAYWHITE);

      camera.BeginMode();
#pragma omp parallel for schedule(auto) num_threads(1)
      for (size_t i = 0; i < blocks.size(); i++) {
        block& current_cube = blocks[i];
        // Draw only blocks if is_visible is true
        if (!current_cube.is_visible) {
          skip_by_display++;
          continue;
        }

        // Draw only surfaces
        // if (blocks[std::min(static_cast<int32_t>(blocks.size()) - 1, static_cast<int32_t>(i) + 1)].is_visible) {
        //  skip_by_surface_only++;
        //  continue;
        //}

        if (optimisation) {
          // If block has all 6 neighbors is displayed, skip it
          size_t neighbors = 0;

          size_t c1i = i - 1;
          if (c1i >= 0 && c1i < blocks.size()) {
            if (blocks[c1i].is_visible) {
              neighbors++;
            }
          }

          size_t c2i = i + 1;
          if (c2i >= 0 && c2i < blocks.size()) {
            if (blocks[c2i].is_visible) {
              neighbors++;
            }
          }

          size_t c3i = i - vecX;
          if (c3i >= 0 && c3i < blocks.size()) {
            if (blocks[c3i].is_visible) {
              neighbors++;
            }
          }

          size_t c4i = i + vecX;
          if (c4i >= 0 && c4i < blocks.size()) {
            if (blocks[c4i].is_visible) {
              neighbors++;
            }
          }

          size_t c5i = i - vecX * vecY;
          if (c5i >= 0 && c5i < blocks.size()) {
            if (blocks[c5i].is_visible) {
              neighbors++;
            }
          }

          size_t c6i = i + vecX * vecY;
          if (c6i >= 0 && c6i < blocks.size()) {
            if (blocks[c6i].is_visible) {
              neighbors++;
            }
          }

          if (neighbors == 6) {
            // check if block is on external surface of the world
            /*
            if (current_cube.x == 0 || current_cube.x == vecX * cube_size || current_cube.y == 0
                || current_cube.y == vecY * cube_size || current_cube.z == 0 || current_cube.z == vecZ * cube_size)
            {
            } else {
              skip_by_all_neighbors++;
              continue;
            }
            */
            skip_by_all_neighbors++;
            continue;
          }
        }

        // If block is not visible on screen, skip it
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
        if (show_grid) {
          DrawCubeWiresV(raylib::Vector3(current_cube.x, current_cube.y, current_cube.z),
                         raylib::Vector3(current_cube.size_x, current_cube.size_y, current_cube.size_z),
                         raylib::Color::Black());
        }
      }
      if (show_grid) {
        DrawGrid(256, 1.0f);
      }

      if (closest_collision.hit) {
        DrawCube(closest_collision.point, 0.3f, 0.3f, 0.3f, raylib::Color::Yellow());
        DrawCubeWires(closest_collision.point, 0.3f, 0.3f, 0.3f, raylib::Color::Black());

        Vector3 normalEnd;
        normalEnd.x = closest_collision.point.x + closest_collision.normal.x;
        normalEnd.y = closest_collision.point.y + closest_collision.normal.y;
        normalEnd.z = closest_collision.point.z + closest_collision.normal.z;

        DrawLine3D(closest_collision.point, normalEnd, raylib::Color::Blue());
      }

      camera.EndMode();

      DrawFPS(10, 10);

      // Draw crosshair in the middle of the screen
      DrawLine(screen_middle.x - 10, screen_middle.y, screen_middle.x + 10, screen_middle.y, raylib::Color::SkyBlue());
      DrawLine(screen_middle.x, screen_middle.y - 10, screen_middle.x, screen_middle.y + 10, raylib::Color::SkyBlue());
    }
    EndDrawing();

    std::cout << "skip_by_display: " << skip_by_display << std::endl;
    std::cout << "skip_by_all_neighbors: " << skip_by_all_neighbors << std::endl;
    std::cout << "skip_by_out_of_screen: " << skip_by_out_of_screen << std::endl;
    std::cout << "skip_by_surface_only: " << skip_by_surface_only << std::endl;
    size_t total_skipped = skip_by_display + skip_by_all_neighbors + skip_by_out_of_screen + skip_by_surface_only;
    std::cout << "total skipped: " << total_skipped << std::endl;
    std::cout << "total blocks: " << blocks.size() << std::endl;
    std::cout << "total blocks displayed: " << blocks.size() - total_skipped;
    std::cout << " (" << static_cast<float>(blocks.size() - total_skipped) / static_cast<float>(blocks.size()) * 100.0f
              << "%)" << std::endl;
    std::cout << std::endl;

    skip_by_display = 0;
    skip_by_all_neighbors = 0;
    skip_by_out_of_screen = 0;
  }

  return 0;
}