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


#include "rlgl.h"

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // Vertex data transformation can be defined with the commented lines,
    // but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
    //rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        //rlTranslatef(2.0f, 0.0f, 0.0f);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(2.0f, 2.0f, 2.0f);

        rlBegin(RL_QUADS);
            rlColor4ub(color.r, color.g, color.b, color.a);
            // Front Face
            rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            // Back Face
            rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            // Top Face
            rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            // Bottom Face
            rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            // Right face
            rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
            // Left Face
            rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
            rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
            rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
            rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
            rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
        rlEnd();
    //rlPopMatrix();

    rlSetTexture(0);
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    const int screen_width = 1920;
    const int screen_height = 1080;
    const int target_fps = 60;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    raylib::Window window(screen_width, screen_height, "Minecube");
    SetTargetFPS(target_fps);

    raylib::Camera camera({ 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f);


    // create a texture from an image
    raylib::Image img = GenImageChecked(256, 256, 32, 32, GREEN, RED);
    raylib::Texture2D textureGrid = LoadTextureFromImage(img);
    SetTextureFilter(textureGrid, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureWrap(textureGrid, TEXTURE_WRAP_CLAMP);

    // Create a model from mesh
    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);
    Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureGrid;

    size_t bench_mode = 0;
    size_t bench_mode_count = 4;

    size_t x = 5000;
    raylib::Vector3 cubePosition(0.0f, 0.0f, 0.0f);
    raylib::Vector3 cubeSize(2.0f, 2.0f, 2.0f);

    raylib::Vector2 PlanePosition(0.0f, 0.0f);

    while (!window.ShouldClose()) {
        // !IsWindowFocused()
        if (IsWindowMinimized()) {
            continue;
        }

        camera.Update(CAMERA_ORBITAL);

        if (IsKeyPressed(KEY_F5)) {
            // Take screenshot
            TakeScreenshot("screenshot.png");
        }

        if (IsKeyPressed(KEY_F6)) {
            if (bench_mode < bench_mode_count) {
                bench_mode++;
            } else {
                bench_mode = 0;
            }
        }

        BeginDrawing();
        {
            window.ClearBackground(RAYWHITE);

            camera.BeginMode();
            {
                if (bench_mode == 0) {
                    // Benchmark DrawCubeV
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < x; i++) {
                        DrawCubeV(cubePosition, cubeSize, RED);
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = end - start;
                    std::cout << "DrawCubeV elapsed: " << elapsed.count() << " ms"
                              << " " << x / elapsed.count() << " cubes/ms" << std::endl;
                } else if (bench_mode == 1) {
                    // Benchmark DrawCubeTexture
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < x; i++) {
                        DrawCubeTexture(textureGrid, cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = end - start;
                    std::cout << "DrawCubeTexture elapsed: " << elapsed.count() << " ms"
                              << " " << x / elapsed.count() << " cubes/ms" << std::endl;
                } else if (bench_mode == 2) {
                    // Benchmark DrawCubeWires
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < x; i++) {
                        DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = end - start;
                    std::cout << "DrawCubeWires elapsed: " << elapsed.count() << " ms"
                              << " " << x / elapsed.count() << " cubes/ms" << std::endl;
                } else if (bench_mode == 3) {
                    // Benchmark DrawTextureEx
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < x; i++) {
                        DrawTextureEx(textureGrid, PlanePosition, 0.0f, 1.0f, WHITE);
                        // DrawTexturePro
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = end - start;
                    std::cout << "DrawTextureEx elapsed: " << elapsed.count() << " ms"
                              << " " << x / elapsed.count() << " texture/ms" << std::endl;
                } else if (bench_mode == 4) {
                    // Benchmark Draw Model
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < x; i++) {
                        DrawModel(model, cubePosition, 1.0f, RED);
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::milli> elapsed = end - start;
                    std::cout << "DrawModel elapsed: " << elapsed.count() << " ms"
                              << " " << x / elapsed.count() << " models/ms" << std::endl;
                }

                std::cout << std::endl << std::endl;
                DrawGrid(10, 1.0f);
            }
            camera.EndMode();

            // Draw FPS
            DrawFPS(10, 10);
            DrawText("Press F5 to take a screenshot", 10, 30, 20, GRAY);
            DrawText("Press F6 to change benchmark mode", 10, 50, 20, GRAY);

            // Current benchmark mode
            if (bench_mode == 0) {
                DrawText("Current benchmark mode: DrawCubeV", 10, 90, 20, GRAY);
            } else if (bench_mode == 1) {
                DrawText("Current benchmark mode: DrawCubeTexture", 10, 90, 20, GRAY);
            } else if (bench_mode == 2) {
                DrawText("Current benchmark mode: DrawCubeWires", 10, 90, 20, GRAY);
            } else if (bench_mode == 3) {
                DrawText("Current benchmark mode: DrawTextureEx", 10, 90, 20, GRAY);
            } else if (bench_mode == 4) {
                DrawText("Current benchmark mode: DrawModel", 10, 90, 20, GRAY);
            }
        }
        EndDrawing();
    }
    UnloadModel(model);

    return 0;
}