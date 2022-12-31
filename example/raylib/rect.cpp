#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "raylib-cpp.hpp"
#include "raylib.h"

auto main() -> int
{
    const int screen_width = 1920;
    const int screen_height = 1080;
    const uint32_t targetFPS = 120;
    const uint32_t gridUpdatePerSecond = 10;

    const float gridOffsetX = 0.0f;
    const float gridOffsetY = 0.0f;

    const int grid_width = 200;
    const int grid_height = 100;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);

    raylib::Window window(screen_width, screen_height, "raylib-cpp game of life");

    SetTargetFPS(targetFPS);

    std::random_device rd;
    const std::uint64_t seed = (static_cast<std::uint64_t>(rd()) << 32) | rd();
    std::mt19937_64 rng(seed);

    // uniform_int_distribution is a template class
    std::uniform_int_distribution<int> uniform_dist(0, 255);

    // Fill vector with random numbers
    std::vector<int> v(grid_width * grid_height, 0);

    std::generate(v.begin(), v.end(), [&]() { return uniform_dist(rng); });

    float cellXSize = 10.0f;
    float cellYSize = 10.0f;

    raylib::Camera2D camera = {};
    camera.target = (Vector2) {screen_width / 2.0f + gridOffsetX, screen_height / 2.0f + gridOffsetY};
    camera.offset = (Vector2) {screen_width / 2.0f, screen_height / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    uint64_t framesCounter = 0;

    bool paused = false;
    bool displayGrid = false;
    bool displayHelp = true;

    while (!WindowShouldClose()) {
        framesCounter++;

        Vector2 mousePosition = GetMousePosition();

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 vect = camera.GetScreenToWorld(mousePosition);

            // Apply grid offset
            vect.x -= gridOffsetX;
            vect.y -= gridOffsetY;

            auto x = static_cast<int64_t>(vect.x / cellXSize);
            auto y = static_cast<int64_t>(vect.y / cellYSize);

            if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
                v.at(y * grid_width + x) = 255;
            }
        }

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            Vector2 vect = camera.GetScreenToWorld(mousePosition);

            // Apply grid offset
            vect.x -= gridOffsetX;
            vect.y -= gridOffsetY;

            auto x = static_cast<int64_t>(vect.x / cellXSize);
            auto y = static_cast<int64_t>(vect.y / cellYSize);
            if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
                v.at(y * grid_width + x) = 0;
            }
        }

        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) {
            if (paused) {
                paused = false;
            } else {
                paused = true;
            }
        }

        if (IsKeyPressed(KEY_G)) {
            displayGrid = !displayGrid;
        }

        if (IsKeyPressed(KEY_H)) {
            displayHelp = !displayHelp;
        }

        if (framesCounter % (targetFPS / gridUpdatePerSecond) == 0 && !paused) {
            // std::cout << "Update grid" << std::endl;
        }

        if (IsKeyDown(KEY_LEFT)) {
            camera.target.x -= 5.0f;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            camera.target.x += 5.0f;
        }
        if (IsKeyDown(KEY_UP)) {
            camera.target.y -= 5.0f;
        }
        if (IsKeyDown(KEY_DOWN)) {
            camera.target.y += 5.0f;
        }

        if (IsKeyPressed(KEY_C)) {
            std::cout << "Clear grid" << std::endl;
        }

        if (IsKeyPressed(KEY_R)) {
            std::generate(v.begin(), v.end(), [&]() { return uniform_dist(rng); });
        }

        if (IsKeyPressed(KEY_B)) {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
            camera.target = (Vector2) {screen_width / 2.0f + gridOffsetX, screen_height / 2.0f + gridOffsetY};
        }

        if (IsKeyDown(KEY_A)) {
            camera.rotation--;
        }

        if (IsKeyDown(KEY_S)) {
            camera.rotation++;
        }

        if (camera.rotation > 40) {
            camera.rotation = 40;
        }

        if (camera.rotation < -40) {
            camera.rotation = -40;
        }

        camera.zoom += ((float)GetMouseWheelMove() * 0.10f);
        if (camera.zoom > 35.0f)
            camera.zoom = 5.0f;
        else if (camera.zoom < 0.01f)
            camera.zoom = 0.01f;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        // Drawing world

        // Draw only on screen
        Vector2 vectH = camera.GetScreenToWorld((Vector2) {0.0f, 0.0f});
        Vector2 vectB =
            camera.GetScreenToWorld((Vector2) {static_cast<float>(screen_width), static_cast<float>(screen_height)});

        // Apply grid offset
        vectH.x -= gridOffsetX;
        vectH.y -= gridOffsetY;
        vectB.x -= gridOffsetX;
        vectB.y -= gridOffsetY;

        int64_t xStart = static_cast<int64_t>(vectH.x / cellXSize) - 1;
        int64_t yStart = static_cast<int64_t>(vectH.y / cellYSize) - 1;

        int64_t xEnd = static_cast<int64_t>(vectB.x / cellXSize) + 1;
        int64_t yEnd = static_cast<int64_t>(vectB.y / cellYSize) + 1;

        if (xStart > xEnd) {
            std::swap(xStart, xEnd);
        }
        if (yStart > yEnd) {
            std::swap(yStart, yEnd);
        }

        // Avoid out of bounds of the grid
        if (xStart < 0) {
            xStart = 0;
        }
        if (yStart < 0) {
            yStart = 0;
        }

        if (xEnd > grid_width) {
            xEnd = grid_width;
        }
        if (yEnd > grid_height) {
            yEnd = grid_height;
        }

        // Draw grid
        for (uint64_t x = xStart; x < xEnd; x++) {
            for (uint64_t y = yStart; y < yEnd; y++) {
                // If cell is alive
                DrawRectangle(
                    static_cast<int>(x * cellXSize + gridOffsetX),
                    static_cast<int>(y * cellYSize + gridOffsetY),
                    static_cast<int>(cellXSize),
                    static_cast<int>(cellYSize),
                    raylib::Color(v.at(y * grid_width + x), v.at(y * grid_width + x), v.at(y * grid_width + x), 255));

                if (displayGrid) {
                    DrawRectangleLinesEx((Rectangle) {static_cast<float>(x * cellXSize + gridOffsetX),
                                                      static_cast<float>(y * cellYSize + gridOffsetY),
                                                      static_cast<float>(cellXSize),
                                                      static_cast<float>(cellYSize)},
                                         0.7f,
                                         BLACK);
                }
            }
        }

        EndMode2D();
        // Drawing interface

        if (displayHelp) {
            DrawRectangle(10, 10, 250, 113, Fade(SKYBLUE, 0.95f));
            DrawRectangleLines(10, 10, 250, 113, BLUE);

            DrawText("Free 2d camera controls:", 20, 20, 10, BLACK);
            DrawText("- D-Pad to move Offset", 40, 40, 10, DARKGRAY);
            DrawText("- Mouse Wheel to Zoom in-out", 40, 60, 10, DARKGRAY);
            DrawText("- A / S to Rotate", 40, 80, 10, DARKGRAY);
            DrawText("- B to reset Zoom and Rotation", 40, 100, 10, DARKGRAY);

            DrawRectangle(10, 123, 250, 153, Fade(SKYBLUE, 0.95f));
            DrawRectangleLines(10, 123, 250, 153, BLUE);

            DrawText("Game of life controls:", 20, 133, 10, BLACK);
            DrawText("- R to random values", 40, 153, 10, DARKGRAY);
            DrawText("- C to clear game", 40, 173, 10, DARKGRAY);
            DrawText("- Space to pause", 40, 193, 10, DARKGRAY);
            DrawText("- G to display grid (Less perf)", 40, 213, 10, DARKGRAY);
            DrawText("- H to display help", 40, 233, 10, DARKGRAY);
            DrawText("- U to Serialize, L to Deserialize", 40, 253, 10, DARKGRAY);
        }

        // display FPS
        DrawRectangle(screen_width - 90, 10, 80, 20, Fade(SKYBLUE, 0.95f));
        DrawText(TextFormat("FPS: %02d", GetFPS()), screen_width - 80, 15, 15, DARKGRAY);

        EndDrawing();
    }

    // CloseWindow();  // Close window and OpenGL context

    return 0;
}
