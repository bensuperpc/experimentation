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

// Raylib
#include "raylib-cpp.hpp"
#include "raylib.h"
#include "raymath.h"

int main()
{
    std::ios_base::sync_with_stdio(false);

    const int screen_width = 1920;
    const int screen_height = 1080;
    const int target_fps = 60;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    raylib::Window window(screen_width, screen_height, "Minecube");
    SetTargetFPS(target_fps);

    raylib::Camera camera(raylib::Vector3(4.0f, 4.0f, 4.0f),
                          raylib::Vector3(0.0f, 0.0f, 0.0f),
                          raylib::Vector3(0.0f, 1.0f, 0.0f),
                          60.0f,
                          CAMERA_PERSPECTIVE);

    camera.SetMode(CAMERA_ORBITAL);

    // create a texture from an image
    raylib::Image img = GenImageChecked(256, 256, 32, 32, GREEN, RED);
    raylib::Texture2D textureGrid = LoadTextureFromImage(img);
    SetTextureFilter(textureGrid, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(textureGrid, TEXTURE_WRAP_CLAMP);

    std::vector<raylib::Model> models;
    std::vector<raylib::Mesh> meshes;
    for (int i =0; i < 100; i++)
    {
        Mesh mesh = raylib::Mesh::Cubicmap(img, Vector3{ 1.0f, 1.0f, 1.0f });
        raylib::Model model(mesh);

    }
    // Create a model from mesh
    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureGrid;

    raylib::Vector3 cubePosition(0.0f, 0.0f, 0.0f);
    raylib::Vector3 cubeSize(2.0f, 2.0f, 2.0f);

    raylib::Vector2 PlanePosition(0.0f, 0.0f);

    while (!window.ShouldClose()) {
        // !IsWindowFocused()
        if (IsWindowMinimized()) {
            continue;
        }

        camera.Update();

        if (IsKeyPressed(KEY_F5)) {
            // Take screenshot
            TakeScreenshot("screenshot.png");
        }

        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            camera.BeginMode();
            {

                DrawGrid(10, 1.0f);
            }
            camera.EndMode();

            // Draw FPS
            DrawFPS(10, 10);
            DrawText("Press F5 to take a screenshot", 10, 30, 20, GRAY);
            DrawText("Press F6 to change benchmark mode", 10, 50, 20, GRAY);
        }
        EndDrawing();
    }
    UnloadModel(model);

    return 0;
}