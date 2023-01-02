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

template<typename T = int>
void optimize(std::vector<block>& blocks,
              const T begin_x,
              const T begin_y,
              const T begin_z,
              const uint32_t size_x,
              const uint32_t size_y,
              const uint32_t size_z)
{
    constexpr bool debug = true;
    constexpr int debug_x = 21;
    constexpr int debug_y = 31;
    constexpr int debug_z = 21;

    const T end_x = static_cast<T>(begin_x + size_x);
    const T end_y = static_cast<T>(begin_y + size_y);
    const T end_z = static_cast<T>(begin_z + size_z);
    /*
    Block (xyz): 21, 31, 21 current
    Block (xyz): 20, 31, 21 index: 64852 block_type: stone x-1
    Adding neighbor
    Block (xyz): 22, 31, 21 index: 64854 block_type: stone x+1
    Adding neighbor
    Block (xyz): 21, 30, 21 block_type: stone y-1
    Adding neighbor
    Block (xyz): 21, 32, 21 index: 66901 block_type: air y+1
    Adding edge
    Block (xyz): 21, 31, 20 index: 64789 block_type: stone z-1
    Adding neighbor
    Block (xyz): 21, 31, 22 index: 64917 block_type: stone z+1
    Adding neighbor
    Neighbors: 5
    Edges: 1
    */
    if constexpr (debug) {
        std::cout << "optimizing..." << std::endl;
        std::cout << "From (xyz): " << begin_x << ", " << begin_y << ", " << begin_z << std::endl;
        std::cout << "To (xyz): " << end_x << ", " << end_y << ", " << end_z << std::endl;
        std::cout << "Size (xyz): " << size_x << ", " << size_y << ", " << size_z << std::endl;
    }

    // #pragma omp parallel for schedule(auto)
    for (size_t i = 0; i < blocks.size(); i++) {
        block& current_cube = blocks[i];

        if (current_cube.block_type == block_type::air) {
            current_cube.is_visible = false;
            continue;
        }

        if constexpr (debug) {
            if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                std::cout << "Block (xyz): " << current_cube.x << ", " << current_cube.y << ", " << current_cube.z
                          << " current" << std::endl;
                current_cube.color = raylib::Color::Blue();
            }
        }

        /*
        int x = i % vecX;z
        int y = (i / vecX) % vecY;
        int z = (i / (vecX * vecY)) % vecZ;
        */

        // Count neighbors
        size_t neighbors = 0;
        // Count edges
        size_t edges = 0;

        // x-1
        size_t i1 = i - 1;
        if (i1 < blocks.size()) {
            if constexpr (debug) {
                if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                    std::cout << "Block (xyz): " << blocks[i1].x << ", " << blocks[i1].y << ", " << blocks[i1].z
                              << " index: " << i1 << " block_type: " << block_type::get_name(blocks[i1].block_type)
                              << " x-1" << std::endl;
                }
            }
            if (current_cube.x > begin_x) {
                if (blocks[i1].block_type != block_type::air) {
                    neighbors++;
                    if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                        std::cout << "Adding neighbor" << std::endl;
                    }
                }
            }
        }
        if (current_cube.x == begin_x) {
            edges++;
            if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                std::cout << "Adding edge" << std::endl;
            }
        }

        // x+1
        size_t i2 = i + 1;
        if (i2 < blocks.size()) {
            if constexpr (debug) {
                if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                    std::cout << "Block (xyz): " << blocks[i2].x << ", " << blocks[i2].y << ", " << blocks[i2].z
                              << " index: " << i2 << " block_type: " << block_type::get_name(blocks[i2].block_type)
                              << " x+1" << std::endl;
                }
            }
            if (current_cube.x < end_x - 1) {
                if (blocks[i2].block_type != block_type::air) {
                    neighbors++;
                    if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                        std::cout << "Adding neighbor" << std::endl;
                    }
                }
            }
        }
        if (current_cube.x == end_x - 1) {
            edges++;
            if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                std::cout << "Adding edge" << std::endl;
            }
        }

        // y-1
        if (current_cube.y == begin_y) {
            edges++;
        }
        size_t i3 = i - size_x * size_y;
        if (i3 < blocks.size()) {
            if constexpr (debug) {
                if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                    std::cout << "Block (xyz): " << blocks[i3].x << ", " << blocks[i3].y << ", " << blocks[i3].z
                              << " block_type: " << block_type::get_name(blocks[i3].block_type) << " y-1" << std::endl;
                }
            }
            if (current_cube.y > begin_y) {
                if (blocks[i3].block_type != block_type::air) {
                    neighbors++;
                    if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                        std::cout << "Adding neighbor" << std::endl;
                    }
                }
            }
        }

        // y+1
        size_t i4 = i + size_x * size_y;
        if (i4 < blocks.size()) {
            if constexpr (debug) {
                if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                    std::cout << "Block (xyz): " << blocks[i4].x << ", " << blocks[i4].y << ", " << blocks[i4].z
                              << " index: " << i4 << " block_type: " << block_type::get_name(blocks[i4].block_type)
                              << " y+1" << std::endl;
                }
            }
            if (current_cube.y < end_y - 1) {
                if (blocks[i4].block_type != block_type::air) {
                    neighbors++;
                    if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                        std::cout << "Adding neighbor" << std::endl;
                    }
                }
            }
        }
        if (current_cube.y == end_y - 1) {
            edges++;
            if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                std::cout << "Adding edge" << std::endl;
            }
        }

        // z-1
        size_t i5 = i - size_x;
        if (i5 < blocks.size()) {
            if constexpr (debug) {
                if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                    std::cout << "Block (xyz): " << blocks[i5].x << ", " << blocks[i5].y << ", " << blocks[i5].z
                              << " index: " << i5 << " block_type: " << block_type::get_name(blocks[i5].block_type)
                              << " z-1" << std::endl;
                }
            }
            if (current_cube.z > begin_z) {
                if (blocks[i5].block_type != block_type::air) {
                    neighbors++;
                    if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                        std::cout << "Adding neighbor" << std::endl;
                    }
                }
            }
        }
        if (current_cube.z == begin_z) {
            edges++;
            if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                std::cout << "Adding edge" << std::endl;
            }
        }

        // z+1
        size_t i6 = i + size_x;
        if (i6 < blocks.size()) {
            if constexpr (debug) {
                if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                    std::cout << "Block (xyz): " << blocks[i6].x << ", " << blocks[i6].y << ", " << blocks[i6].z
                              << " index: " << i6 << " block_type: " << block_type::get_name(blocks[i6].block_type)
                              << " z+1" << std::endl;
                }
            }
            if (current_cube.z < end_z - 1) {
                if (blocks[i6].block_type != block_type::air) {
                    neighbors++;
                    if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                        std::cout << "Adding neighbor" << std::endl;
                    }
                }
            }
        }
        if (current_cube.z == end_z - 1) {
            edges++;
            if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                std::cout << "Adding edge" << std::endl;
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

        if constexpr (debug) {
            if (current_cube.x == debug_x && current_cube.y == debug_y && current_cube.z == debug_z) {
                std::cout << "Neighbors: " << neighbors << std::endl;
                std::cout << "Edges: " << edges << std::endl;
                std::cout << std::endl;
            }
        }
    }
}

template<typename T = int>
void generate(std::vector<block>& blocks,
              const T begin_x,
              const T begin_y,
              const T begin_z,
              const uint32_t size_x,
              const uint32_t size_y,
              const uint32_t size_z,
              siv::PerlinNoise& perlin,
              const float cube_size)
{
    constexpr bool debug = false;

    const T end_x = static_cast<T>(begin_x + size_x);
    const T end_y = static_cast<T>(begin_y + size_y);
    const T end_z = static_cast<T>(begin_z + size_z);

    if constexpr (debug) {
        std::cout << "Generating:" << std::endl;
        std::cout << "From (xyz): " << begin_x << ", " << begin_y << ", " << begin_z << std::endl;
        std::cout << "To (xyz): " << end_x << ", " << end_y << ", " << end_z << std::endl;
        std::cout << "Size (xyz): " << size_x << ", " << size_y << ", " << size_z << std::endl;

        std::cout << "Generating noise..." << std::endl;
    }
    // Generate noise 2D noise map (0-255)
    std::vector<unsigned char> v(size_x * size_y, 0);

    // #pragma omp parallel for collapse(2) schedule(auto)
    for (int x = 0; x < size_x; x++) {
        for (int y = 0; y < size_y; y++) {
            // Calculate real x and y from begin_x and begin_y
            const int real_x = x + begin_x;
            const int real_y = y + begin_y;

            const uint8_t value_int =
                static_cast<uint8_t>(perlin.octave2D_01(real_x / 256.0, real_y / 256.0, 16, 0.2) * 255.0);

            if (debug) {
                std::cout << "x: " << x << ", y: " << y << " index: " << y * size_y + x
                          << ", value: " << static_cast<int>(value_int) << std::endl;
            }
            v[y * size_x + x] = value_int;
        }
    }

    if constexpr (debug) {
        // cout max and min
        auto minmax = std::minmax_element(v.begin(), v.end());
        std::cout << "min: " << static_cast<int>(*minmax.first) << std::endl;
        std::cout << "max: " << static_cast<int>(*minmax.second) << std::endl;
    }

    // Insert blocks if needed to make sure the vector is the correct size
    if (blocks.size() < size_x * size_y * size_z) {
        blocks.insert(blocks.end(), size_x * size_y * size_z - blocks.size(), block());
    }

    if constexpr (debug) {
        std::cout << "Generating blocks..." << std::endl;
    }
// Generate blocks
#pragma omp parallel for collapse(2) schedule(auto)
    for (int x = 0; x < size_x; x++) {
        for (int y = 0; y < size_y; y++) {
            // Calculate real x and y from begin_x and begin_y
            const int real_x = x + begin_x;
            const int real_y = y + begin_y;

            // Noise value is divided by 4 to make it smaller and it is used as the height of the block (z)
            size_t vec_index = y * size_x + x;

            /*
            if constexpr (debug) {
                std::cout << "x: " << x << ", y: " << y << " index: " << vec_index << std::endl;
            }
            */

            unsigned char noise_value = v[vec_index] / 4;

            for (int z = 0; z < size_z; z++) {
                // Calculate real z from begin_z
                const int real_z = z + begin_z;

                // vec_index = y * size_x * size_z + z * size_y + x;
                // vec_index = z * size_x * size_y + y * size_x + x;
                // vec_index = x * size_y * size_z + y * size_z + z;
                vec_index = y * size_x + z * size_x * size_y + x;

                if constexpr (debug) {
                    std::cout << "x: " << x << ", y: " << y << ", z: " << z << " index: " << vec_index
                              << ", noise: " << static_cast<int>(noise_value) << std::endl;
                }

                block& current_block = blocks[vec_index];

                current_block.x = real_x;
                current_block.y = real_z;
                current_block.z = real_y;

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
    optimize(blocks, begin_x, begin_y, begin_z, size_x, size_y, size_z);
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

    siv::PerlinNoise::seed_type seed = 2510586073u;
    std::cout << "seed: " << seed << std::endl;

    siv::PerlinNoise perlin {seed};

    size_t vecX = 64;
    size_t vecY = 32;
    size_t vecZ = 48;

    size_t vec_size = vecX * vecY * vecZ;
    std::vector<block> blocks = std::vector<block>(vec_size);
    std::cout << "blocks size: " << blocks.size() << std::endl;

    generate(blocks, -0, -0, -0, vecX, vecY, vecZ, perlin, cube_size);

    for (size_t i = 0; i < vec_size; i++) {
        blocks[i].texture = &textureGrid;
    }

    raylib::Camera camera(
        raylib::Vector3(static_cast<float>(vecX * cube_size / 4),
                        (vecX / 2) * cube_size + 24.0f,
                        static_cast<float>(vecX * cube_size / 4)),
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
            perlin.reseed(seed);
            generate(blocks, -0, -0, -0, vecX, vecY, vecZ, perlin, cube_size);
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
            std::cout << "Block position: " << closest_block->x << ", " << closest_block->y << ", " <<
            closest_block->z
                      << std::endl;
                      */
            block_info_pos = closest_block->get_position();
            block_info_real_pos = closest_block->get_real_position();
            block_info_index = closest_block->x + closest_block->z * vecX + closest_block->y * vecX * vecZ;
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
        size_t total_skipped = skip_by_display + skip_by_all_neighbors + skip_by_out_of_screen + skip_by_surface_only;
        std::cout << "total skipped: " << total_skipped << std::endl;
        std::cout << "total blocks: " << blocks.size() << std::endl;
        std::cout << "total blocks displayed: " << blocks.size() - total_skipped;
        std::cout << " ("
                  << static_cast<float>(blocks.size() - total_skipped) / static_cast<float>(blocks.size()) * 100.0f
                  << "%)" << std::endl;
        std::cout << std::endl;
        */

        skip_by_display = 0;
        skip_by_all_neighbors = 0;
        skip_by_out_of_screen = 0;
    }

    return 0;
}