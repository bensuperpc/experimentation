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
#include "world.hpp"

void optimize(std::vector<block>& blocks, const int vecX, const int vecY, const int vecZ, const float cube_size)
{
    // #pragma omp parallel for schedule(auto)
    for (size_t i = 0; i < blocks.size(); i++) {
        block& current_cube = blocks[i];

        /*
        if (current_cube.block_type == block_type::air) {
            current_cube.is_visible = false;
            continue;
        }
        */
        if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
            std::cout << "Block: " << current_cube.x << ", " << current_cube.y << ", " << current_cube.z << " current"
                      << std::endl;
        }

        /*
        int x = i % vecX;z
        int y = (i / vecX) % vecY;
        int z = (i / (vecX * vecY)) % vecZ;
        */

        // If block has all 6 neighbors is displayed, skip it
        size_t neighbors = 0;
        size_t edges = 0;

        // x-1
        size_t i1 = i - 1;
        if (current_cube.x == 0) {
            edges++;
        }
        if (i1 < blocks.size() && current_cube.x > 0) {
            if (blocks[i1].block_type != block_type::air) {
                neighbors++;
                if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
                    std::cout << "Block: " << blocks[i1].x << ", " << blocks[i1].y << ", " << blocks[i1].z << " x-1"
                              << std::endl;
                }
            }
        }

        // x+1
        size_t i2 = i + 1;
        if (current_cube.x == vecX - 1) {
            edges++;
        }
        if (i2 < blocks.size() && current_cube.x < vecX - 1) {
            if (blocks[i2].block_type != block_type::air) {
                neighbors++;
                if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
                    std::cout << "Block: " << blocks[i2].x << ", " << blocks[i2].y << ", " << blocks[i2].z << " x+1"
                              << std::endl;
                }
            }
        }

        // y-1
        size_t i3 = i - vecX;
        if (current_cube.y == 0) {
            edges++;
        }
        if (i3 < blocks.size() && current_cube.y > 0) {
            if (blocks[i3].block_type != block_type::air) {
                neighbors++;
                if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
                    std::cout << "Block: " << blocks[i3].x << ", " << blocks[i3].y << ", " << blocks[i3].z << " y-1"
                              << std::endl;
                }
            }
        }

        // y+1
        size_t i4 = i + vecX;
        if (current_cube.y == vecY - 1) {
            edges++;
        }
        if (i4 < blocks.size() && current_cube.y < vecY - 1) {
            if (blocks[i4].block_type != block_type::air) {
                neighbors++;
                if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
                    std::cout << "Block: " << blocks[i4].x << ", " << blocks[i4].y << ", " << blocks[i4].z << " y+1"
                              << std::endl;
                }
            }
        }

        // z-1
        size_t i5 = i - vecX * vecY;
        if (current_cube.z == 0) {
            edges++;
        }
        if (i5 < blocks.size() && current_cube.z > 0) {
            if (blocks[i5].block_type != block_type::air) {
                neighbors++;
                if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
                    std::cout << "Block: " << blocks[i5].x << ", " << blocks[i5].y << ", " << blocks[i5].z << " z-1"
                              << std::endl;
                }
            }
        }

        // z+1
        size_t i6 = i + vecX * vecY;
        if (current_cube.z == vecZ - 1) {
            edges++;
        }

        if (i6 < blocks.size() && current_cube.z < vecZ - 1) {
            if (blocks[i6].block_type != block_type::air) {
                neighbors++;
                if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
                    std::cout << "Block: " << blocks[i6].x << ", " << blocks[i6].y << ", " << blocks[i6].z << " z+1"
                              << std::endl;
                }
            }
        }
        
        if (neighbors == 6 && edges == 0) {
            current_cube.is_visible = false;
        } else if (neighbors == 5 && edges == 1) {
            current_cube.is_visible = false;
        } else if (neighbors == 4 && edges == 2) {
            current_cube.is_visible = false;
        } else if (neighbors == 3 && edges == 3) {
            current_cube.is_visible = false;
        } else if (neighbors == 2 && edges == 4) {
            current_cube.is_visible = false;
        } else if (neighbors == 1 && edges == 5) {
            current_cube.is_visible = false;
        } else if (neighbors == 0 && edges == 6) {
            current_cube.is_visible = false;
        } else {
            current_cube.is_visible = true;
        }

        // for debug
        current_cube.neighbors = neighbors;
        current_cube.edges = edges;
        if (current_cube.x == 0 && current_cube.y == 15 && current_cube.z == 17) {
            std::cout << "Neighbors: " << neighbors << std::endl;
            std::cout << "Edges: " << edges << std::endl;
            std::cout << std::endl;
        }

    }
}

void generate(std::vector<block>& blocks,
              const int vecX,
              const int vecY,
              const int vecZ,
              siv::PerlinNoise& perlin,
              const float cube_size)
{
    // Generate noise 2D noise map (0-255)
    std::vector<unsigned char> v(vecX * vecY, 0);

    // #pragma omp parallel for collapse(2) schedule(auto)
    for (int x = 0; x < vecX; x++) {
        for (int y = 0; y < vecY; y++) {
            const uint8_t value_int = static_cast<uint8_t>(perlin.octave2D_01(x / 256.0, y / 256.0, 16, 0.2) * 255.0);
            v[y * vecY + x] = value_int;
        }
    }

    // cout max and min
    auto minmax = std::minmax_element(v.begin(), v.end());
    std::cout << "min: " << static_cast<int>(*minmax.first) << std::endl;
    std::cout << "max: " << static_cast<int>(*minmax.second) << std::endl;

    // Generate blocks
    // #pragma omp parallel for collapse(2) schedule(auto)
    for (int x = 0; x < vecX; x++) {
        for (int y = 0; y < vecY; y++) {
            size_t vec_index = x * vecX + y;
            // Noise value is divided by 4 to make it smaller and it is used as the height of the block (z)
            unsigned char noise_value = v[vec_index] / 4;
            for (int z = 0; z < vecZ; z++) {
                block& current_block = blocks[y * vecX * vecZ + z * vecY + x];

                current_block.x = x;
                current_block.y = z;
                current_block.z = y;

                current_block.size_x = cube_size;
                current_block.size_y = cube_size;
                current_block.size_z = cube_size;
                current_block.color = raylib::Color::Gray();

                // If the noise value is greater than the current block, make it air
                if (z > noise_value) {
                    current_block.is_visible = false;
                    current_block.block_type = block_type::air;
                    continue;
                }

                current_block.is_visible = true;
                current_block.block_type = block_type::stone;
            }
        }
    }
    optimize(blocks, vecX, vecY, vecZ, cube_size);
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    world current_world;

    const int screen_width = 1920;
    const int screen_height = 1080;
    const int target_fps = 30;

    bool show_grid = true;
    bool show_plain_block = true;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    raylib::Window window(screen_width, screen_height, "Minecube");
    SetTargetFPS(target_fps);

    raylib::Image img = GenImageChecked(256, 256, 32, 32, GREEN, RED);
    raylib::Texture2D textureGrid = LoadTextureFromImage(img);
    SetTextureFilter(textureGrid, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(textureGrid, TEXTURE_WRAP_CLAMP);

    const float cube_size = 2.0f;

    siv::PerlinNoise::seed_type seed = 2647393077u;
    std::cout << "seed: " << seed << std::endl;

    siv::PerlinNoise perlin {seed};

    size_t vecX = 64;
    size_t vecY = 64;
    size_t vecZ = 64;

    size_t vec_size = vecX * vecY * vecZ;
    std::vector<block> blocks = std::vector<block>(vec_size);

    generate(blocks, vecX, vecY, vecZ, perlin, cube_size);

    for (size_t i = 0; i < vec_size; i++) {
        blocks[i].texture = &textureGrid;
    }

    raylib::Camera camera(
        raylib::Vector3(
            static_cast<float>(vecX * cube_size / 4), (vecX / 2) * cube_size, static_cast<float>(vecX * cube_size / 4)),
        raylib::Vector3(static_cast<float>(vecX * cube_size / 2), 0.0f, static_cast<float>(vecX * cube_size / 2)),
        raylib::Vector3(0.0f, 1.0f, 0.0f),
        60.0f,
        CAMERA_PERSPECTIVE);

    camera.SetMode(CAMERA_FIRST_PERSON);

    // Ray and closest_collision
    raylib::Ray ray;
    std::vector<std::pair<block*, RayCollision>> collisions;

    RayCollision closest_collision = {0};
    block* closest_block = nullptr;

    // Block info
    Vector3i block_info_pos = {0, 0, 0};
    size_t block_info_index = 0;
    size_t block_info_neighbour = 0;
    size_t block_info_edges = 0;

    while (!window.ShouldClose()) {
        // If window is not focused or minimized, don't update to save resources
        // !IsWindowFocused()
        if (IsWindowMinimized()) {
            continue;
        }

        camera.Update();

        closest_collision = {0};
        closest_collision.hit = false;
        closest_collision.distance = std::numeric_limits<float>::max();
        if (closest_block != nullptr) {
            closest_block->color = raylib::Color::Gray();
        }
        collisions.clear();

        raylib::Vector2 mouse_pos = GetMousePosition();
        raylib::Vector2 screen_middle = {static_cast<float>(screen_width / 2), static_cast<float>(screen_height / 2)};

        ray = ray.GetMouse(screen_middle, camera);

        if (IsKeyPressed(KEY_R)) {
            decltype(seed) seed = std::random_device()();
            std::cout << "seed: " << seed << std::endl;
            perlin.reseed(seed);
            generate(blocks, vecX, vecY, vecZ, perlin, cube_size);
        }

        if (IsKeyPressed(KEY_G)) {
            show_grid = !show_grid;
        }

        if (IsKeyPressed(KEY_H)) {
            show_plain_block = !show_plain_block;
        }

        if (IsKeyPressed(KEY_F5)) {
            // Take screenshot
            TakeScreenshot("screenshot.png");
        }

        /*
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          if (closest_block != nullptr) {
            closest_block->is_visible = false;
          }
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
          if (closest_block != nullptr) {
            closest_block->is_visible = true;
          }
        }
        */

// TODO: optimize to check only the blocks around the player
#pragma omp parallel for schedule(auto)
        for (size_t i = 0; i < blocks.size(); i++) {
            block& current_cube = blocks[i];
            if (current_cube.is_visible && current_cube.block_type != block_type::air) {
                raylib::BoundingBox box = current_cube.get_bounding_box();

                RayCollision box_hit_info = GetRayCollisionBox(ray, box);

                if (box_hit_info.hit) {
#pragma omp critical
                    collisions.push_back({&current_cube, box_hit_info});
                }
            }
        }

        if (!collisions.empty()) {
            std::sort(collisions.begin(),
                      collisions.end(),
                      [](const auto& a, const auto& b) { return a.second.distance < b.second.distance; });
            closest_collision = collisions[0].second;
            closest_block = collisions[0].first;
            closest_block->color = raylib::Color::Red();

            // Print block position
            /*
            std::cout << "Block position: " << closest_block->x << ", " << closest_block->y << ", " << closest_block->z
                      << std::endl;
                      */
            block_info_pos = closest_block->get_position();
            block_info_index = closest_block->x + closest_block->z * vecX + closest_block->y * vecX * vecZ;
            block_info_neighbour = closest_block->neighbors;
            block_info_edges = closest_block->edges;
        } else {
            block_info_pos = {0, 0, 0};
            block_info_index = 0;
            block_info_neighbour = 0;
            block_info_edges = 0;
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
                if ((current_cube.is_visible == false) || (current_cube.block_type == block_type::air)) {
                    skip_by_display++;
                    continue;
                }

                /*
                // Calculate block position
                int x = i % vecX;
                int y = (i / vecX) % vecY;
                int z = (i / (vecX * vecY)) % vecZ;

                // Calculate block position in world
                float x_pos = x * cube_size;
                float y_pos = y * cube_size;
                float z_pos = z * cube_size;
                */

                // Draw only surfaces
                // if (blocks[std::min(static_cast<int32_t>(blocks.size()) - 1, static_cast<int32_t>(i) +
                // 1)].is_visible) {
                //     skip_by_surface_only++;
                //     continue;
                // }

                // If block is not visible on screen, skip it
                const raylib::Vector2 cube_screen_pos = camera.GetWorldToScreen(current_cube.get_center());
                if (cube_screen_pos.x < 0 || cube_screen_pos.x > GetScreenWidth() || cube_screen_pos.y < 0
                    || cube_screen_pos.y > GetScreenHeight())
                {
                    skip_by_out_of_screen++;
                    continue;
                }
                if (show_plain_block) {
#pragma omp critical
                    current_cube.draw();
                }
                if (show_grid) {
#pragma omp critical
                    current_cube.draw_box();
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

            // Draw FPS
            DrawFPS(10, 10);

            // Draw block info
            std::string block_info = "XYZ: " + std::to_string(block_info_pos.x) + ", "
                + std::to_string(block_info_pos.y) + ", " + std::to_string(block_info_pos.z);
            DrawText(block_info.c_str(), 10, 30, 20, raylib::Color::Black());
            DrawText(("Index: " + std::to_string(block_info_index)).c_str(), 10, 50, 20, raylib::Color::Black());
            DrawText(
                ("Neighbours: " + std::to_string(block_info_neighbour)).c_str(), 10, 70, 20, raylib::Color::Black());
            DrawText(("Edges: " + std::to_string(block_info_edges)).c_str(), 10, 90, 20, raylib::Color::Black());

            // Draw crosshair in the middle of the screen
            DrawLine(
                screen_middle.x - 10, screen_middle.y, screen_middle.x + 10, screen_middle.y, raylib::Color::SkyBlue());
            DrawLine(
                screen_middle.x, screen_middle.y - 10, screen_middle.x, screen_middle.y + 10, raylib::Color::SkyBlue());
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
        std::cout << " ("
                  << static_cast<float>(blocks.size() - total_skipped) / static_cast<float>(blocks.size()) * 100.0f
                  << "%)" << std::endl;
        std::cout << std::endl;

        skip_by_display = 0;
        skip_by_all_neighbors = 0;
        skip_by_out_of_screen = 0;
    }

    return 0;
}