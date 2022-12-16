#include "raylib-cpp.hpp"

#include <cmath> // NOLINT

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "raylib [shapes] example - collision area");
    SetTargetFPS(60); 

    while (!window.ShouldClose()) { 
        auto mousePosition = raylib::Vector2(GetMousePosition());

        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        DrawFPS(10, 10);

        EndDrawing();
    }
    return 0;
}