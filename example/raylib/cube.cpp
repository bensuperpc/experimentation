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
#include "chunk.hpp"
#include "generator.hpp"
#include "optimizer.hpp"
#include "world.hpp"

int main()
{
    std::ios_base::sync_with_stdio(false);

    world current_world;

    const int screen_width = 1920;
    const int screen_height = 1080;
    const int target_fps = 30;

    bool show_block_grid = true;
    bool show_chunk_grid = true;
    bool show_plain_block = true;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    raylib::Window window(screen_width, screen_height, "Minecube");
    SetTargetFPS(target_fps);

    raylib::Image img = GenImageChecked(256, 256, 32, 32, GREEN, RED);
    raylib::Texture2D textureGrid = LoadTextureFromImage(img);
    SetTextureFilter(textureGrid, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(textureGrid, TEXTURE_WRAP_CLAMP);

    const float cube_size = 2.0f;

    siv::PerlinNoise::seed_type seed = 2510586073u;
    std::cout << "seed: " << seed << std::endl;

    generator current_generator(seed);
    generator new_generator(seed);

    uint32_t chunk_x = 4;
    uint32_t chunk_y = 1;
    uint32_t chunk_z = 4;
    uint32_t chunk_size = chunk_x * chunk_y * chunk_z;
    std::vector<chunk> chunks = std::vector<chunk>(chunk_size);
    new_generator.generate_word(chunks, chunk_x, chunk_y, chunk_z);

    raylib::Camera camera(
        raylib::Vector3(static_cast<float>(64 *  2.0f / 4),
                        (64 / 2) *  2.0f + 24.0f,
                        static_cast<float>(64 *  2.0f / 4)),
        raylib::Vector3(static_cast<float>(64 *  2.0f / 2), 0.0f, static_cast<float>(64 *  2.0f / 2)),
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
    Vector3 block_info_real_pos = {0, 0, 0};
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
            new_generator.reseed(seed);
            new_generator.generate_word(chunks, chunk_x, chunk_y, chunk_z);
        }

        if (IsKeyPressed(KEY_G)) {
            show_block_grid = !show_block_grid;
        }

        if (IsKeyPressed(KEY_P)) {
            show_plain_block = !show_plain_block;
        }

        if (IsKeyPressed(KEY_C)) {
            show_chunk_grid = !show_chunk_grid;
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
        for (size_t ci = 0; ci < chunks.size(); ci++) {
            auto& current_chunk = chunks[ci];
            auto& blocks = current_chunk.get_blocks();
#pragma omp parallel for schedule(auto)
            for (size_t bi = 0; bi < current_chunk.size(); bi++) {
                block& current_block = blocks[bi];
                if (current_block.is_visible && current_block.block_type != block_type::air) {
                    raylib::BoundingBox box = current_block.get_bounding_box();

                    RayCollision box_hit_info = GetRayCollisionBox(ray, box);

                    if (box_hit_info.hit) {
#pragma omp critical
                        collisions.push_back({&current_block, box_hit_info});
                    }
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
            std::cout << "Block position: " << closest_block->x << ", " << closest_block->y << ", " <<
            closest_block->z
                      << std::endl;
                      */
            block_info_pos = closest_block->get_position();
            block_info_real_pos = closest_block->get_real_position();
            block_info_index = closest_block->x + closest_block->z * 16 + closest_block->y * 16 * 16;
            block_info_neighbour = closest_block->neighbors;
            block_info_edges = closest_block->edges;
        } else {
            block_info_pos = {0, 0, 0};
            block_info_real_pos = {0.0, 0.0, 0.0};
            block_info_index = 0;
            block_info_neighbour = 0;
            block_info_edges = 0;
        }

        // Statistics
        size_t skip_by_display = 0;
        size_t skip_by_all_neighbors = 0;
        size_t skip_by_out_of_screen = 0;
        size_t skip_by_surface_only = 0;

        size_t display_block_count = 0;

        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            camera.BeginMode();
            for (size_t ci = 0; ci < chunks.size(); ci++) {
                auto& current_chunk = chunks[ci];
                auto& blocks = current_chunk.get_blocks();

                for (size_t bi = 0; bi < current_chunk.size(); bi++) {
                    block& current_block = blocks[bi];

                    // Draw only blocks if is_visible is true
                    if ((current_block.is_visible == false) || (current_block.block_type == block_type::air)) {
                        skip_by_display++;
                        continue;
                    }

                    // If block is not visible on screen, skip it
                    const raylib::Vector2 blcok_screen_pos = camera.GetWorldToScreen(current_block.get_center());
                    if (blcok_screen_pos.x < 0 || blcok_screen_pos.x > GetScreenWidth() || blcok_screen_pos.y < 0
                        || blcok_screen_pos.y > GetScreenHeight())
                    {
                        skip_by_out_of_screen++;
                        continue;
                    }

                    if (show_plain_block) {
                        current_block.draw();
                        display_block_count++;
                    }
                    if (show_block_grid) {
                        current_block.draw_box();
                    }
                }
                if (show_chunk_grid) {
                    current_chunk.draw_wireframe();
                }
            }
            if (show_block_grid) {
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
            DrawText(("Real XYZ: " + std::to_string(block_info_real_pos.x) + ", "
                      + std::to_string(block_info_real_pos.y) + ", " + std::to_string(block_info_real_pos.z))
                         .c_str(),
                     10,
                     70,
                     20,
                     raylib::Color::Black());
            DrawText(
                ("Neighbours: " + std::to_string(block_info_neighbour)).c_str(), 10, 90, 20, raylib::Color::Black());
            DrawText(("Edges: " + std::to_string(block_info_edges)).c_str(), 10, 110, 20, raylib::Color::Black());

            // Draw statistics
            DrawText(("Blocks on screen: " + std::to_string(display_block_count)).c_str(),
                     10,
                     150,
                     20,
                     raylib::Color::Black());

            // Draw crosshair in the middle of the screen
            DrawLine(
                screen_middle.x - 10, screen_middle.y, screen_middle.x + 10, screen_middle.y, raylib::Color::SkyBlue());
            DrawLine(
                screen_middle.x, screen_middle.y - 10, screen_middle.x, screen_middle.y + 10, raylib::Color::SkyBlue());
        }
        EndDrawing();
        /*
        std::cout << "skip_by_display: " << skip_by_display << std::endl;
        std::cout << "skip_by_all_neighbors: " << skip_by_all_neighbors << std::endl;
        std::cout << "skip_by_out_of_screen: " << skip_by_out_of_screen << std::endl;
        std::cout << "skip_by_surface_only: " << skip_by_surface_only << std::endl;
        size_t total_skipped = skip_by_display + skip_by_all_neighbors + skip_by_out_of_screen +
        skip_by_surface_only; std::cout << "total skipped: " << total_skipped << std::endl; std::cout << "total
        blocks: " << blocks.size() << std::endl; std::cout << "total blocks displayed: " << blocks.size() -
        total_skipped; std::cout << " ("
                  << static_cast<float>(blocks.size() - total_skipped) / static_cast<float>(blocks.size()) *
        100.0f
                  << "%)" << std::endl;
        std::cout << std::endl;
        */

        skip_by_display = 0;
        skip_by_all_neighbors = 0;
        skip_by_out_of_screen = 0;

        display_block_count = 0;
    }

    return 0;
}