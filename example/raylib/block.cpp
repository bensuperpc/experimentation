#include "raylib.h"
#include <cstdio>

static Mesh GenMeshCube(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - mesh generation");

    // We generate a checked image for texturing
    Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    Model model = LoadModelFromMesh(GenMeshCube());
    ExportMesh(model.meshes[0], "custom.obj");

    // Set checked texture as default diffuse component for all models material
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // Define the camera to look into our 3d world
    // Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    Camera camera = { { 2.0f, 0.5f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 60.0f, 0 };
    // Model drawing position
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    int currentModel = 0;

    SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a first person camera mode
    //SetCameraMode(camera, CAMERA_ORBITAL); // Set an orbital camera mode
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

               DrawModel(model, position, 1.0f, WHITE);
               DrawGrid(10, 1.0);
               // Draw point on 1,0,1
                // DrawSphere({1,0,1}, 0.1, RED);

            EndMode3D();

            DrawRectangle(30, 400, 310, 30, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(30, 400, 310, 30, Fade(DARKBLUE, 0.5f));
            DrawText("MOUSE LEFT BUTTON to CYCLE PROCEDURAL MODELS", 40, 410, 10, BLUE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
    UnloadModel(model);

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Generate a simple triangle mesh from code
static Mesh GenMeshCube(void)
{
    float start_x = 0.0f;
    float start_y = 1.0f;
    float start_z = 0.0f;
    Mesh mesh = { 0 };
    mesh.triangleCount = 4; // 2 Per face
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)
    
    // Draw bottom-left triangle
    // Vertex at (0, 0, 0)
    mesh.vertices[0] = 0 + start_x;
    mesh.vertices[1] = 0 + start_y;
    mesh.vertices[2] = 0 + start_z;
    mesh.normals[0] = 0;
    mesh.normals[1] = -1;
    mesh.normals[2] = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    // Vertex at (0, 0, 1)
    mesh.vertices[3] = 1 + start_x;
    mesh.vertices[4] = 0 + start_y;
    mesh.vertices[5] = 0 + start_z;
    mesh.normals[3] = 0;
    mesh.normals[4] = 1;
    mesh.normals[5] = 0;
    mesh.texcoords[2] = 0.0f;
    mesh.texcoords[3] = 1.0f;

    // Vertex at (1, 0, 0)
    mesh.vertices[6] = 1 + start_x;
    mesh.vertices[7] = 0 + start_y;
    mesh.vertices[8] = 1 + start_z;
    mesh.normals[6] = 0;
    mesh.normals[7] = -1;
    mesh.normals[8] = 0;
    mesh.texcoords[4] = 1;
    mesh.texcoords[5] = 0;

    // Draw bottom-right triangle
    // Vertex at (0, 0, 1)
    mesh.vertices[9] = 0 + start_x;
    mesh.vertices[10] = 0 + start_y;
    mesh.vertices[11] = 0 + start_z;
    mesh.normals[9] = 0;
    mesh.normals[10] = -1;
    mesh.normals[11] = 0;
    mesh.texcoords[6] = 0;
    mesh.texcoords[7] = 1;
    
    // Vertex at (1, 0, 1)
    mesh.vertices[12] = 1 + start_x;
    mesh.vertices[13] = 0 + start_y;
    mesh.vertices[14] = 1 + start_z;
    mesh.normals[12] = 0;
    mesh.normals[13] = -1;
    mesh.normals[14] = 0;
    mesh.texcoords[8] = 1;
    mesh.texcoords[9] = 1;

    // Vertex at (1, 0, 0)
    mesh.vertices[15] = 0 + start_x;
    mesh.vertices[16] = 0 + start_y;
    mesh.vertices[17] = 1 + start_z;
    mesh.normals[15] = 0;
    mesh.normals[16] = -1;
    mesh.normals[17] = 0;
    mesh.texcoords[10] = 1;
    mesh.texcoords[11] = 0;
    /*
    // Draw top-left triangle
    // Vertex at (0, 1, 0)
    mesh.vertices[18] = 0 + start_x;
    mesh.vertices[19] = 1 + start_y;
    mesh.vertices[20] = 0 + start_z;
    mesh.normals[18] = 0;
    mesh.normals[19] = 1;
    mesh.normals[20] = 0;
    mesh.texcoords[12] = 0;
    mesh.texcoords[13] = 0;

    // Vertex at (0, 1, 1)
    mesh.vertices[21] = 0 + start_x;
    mesh.vertices[22] = 1 + start_y;
    mesh.vertices[23] = 1 + start_z;
    mesh.normals[21] = 0;
    mesh.normals[22] = 1;
    mesh.normals[23] = 0;
    mesh.texcoords[14] = 0;
    mesh.texcoords[15] = 1;
    
    // Vertex at (1, 1, 0)
    mesh.vertices[24] = 1 + start_x;
    mesh.vertices[25] = 1 + start_y;
    mesh.vertices[26] = 0 + start_z;
    mesh.normals[24] = 0;
    mesh.normals[25] = 1;
    mesh.normals[26] = 0;
    mesh.texcoords[16] = 1;
    mesh.texcoords[17] = 0;

    // Draw top-right triangle
    // Vertex at (0, 1, 1)// Draw left-left triangle
    // Vertex at (0, 0, 0)
    mesh.vertices[36] = 0 + start_x;
    mesh.vertices[37] = 0 + start_y;
    mesh.vertices[38] = 0 + start_z;
    mesh.normals[36] = -1;
    mesh.normals[37] = 0;
    mesh.normals[38] = 0;
    mesh.texcoords[24] = 0;
    mesh.texcoords[25] = 0;

    // Vertex at (0, 0, 1)
    mesh.vertices[39] = 0 + start_x;
    mesh.vertices[40] = 0 + start_y;
    mesh.vertices[41] = 1 + start_z;
    mesh.normals[39] = -1;
    mesh.normals[40] = 0;
    mesh.normals[41] = 0;
    mesh.texcoords[26] = 0;
    mesh.texcoords[27] = 1;

    // Vertex at (0, 1, 0)
    mesh.vertices[42] = 0 + start_x;
    mesh.vertices[43] = 1 + start_y;
    mesh.vertices[44] = 0 + start_z;
    mesh.normals[42] = -1;
    mesh.normals[43] = 0;
    mesh.normals[44] = 0;
    mesh.texcoords[28] = 1;
    mesh.texcoords[29] = 0;
    
    // Draw left-right triangle
    // Vertex at (0, 0, 1)
    mesh.vertices[45] = 0 + start_x;
    mesh.vertices[46] = 0 + start_y;
    mesh.vertices[47] = 1 + start_z;
    mesh.normals[45] = -1;
    mesh.normals[46] = 0;
    mesh.normals[47] = 0;
    mesh.texcoords[30] = 0;
    mesh.texcoords[31] = 1;

    // Vertex at (0, 1, 1)
    mesh.vertices[48] = 0 + start_x;
    mesh.vertices[49] = 1 + start_y;
    mesh.vertices[50] = 1 + start_z;
    mesh.normals[48] = -1;
    mesh.normals[49] = 0;
    mesh.normals[50] = 0;
    mesh.texcoords[32] = 1;
    mesh.texcoords[33] = 1;

    // Vertex at (0, 1, 0)
    mesh.vertices[51] = 0 + start_x;
    mesh.vertices[52] = 1 + start_y;
    mesh.vertices[53] = 0 + start_z;
    mesh.normals[51] = -1;
    mesh.normals[52] = 0;
    mesh.normals[53] = 0;
    mesh.texcoords[34] = 1;
    mesh.texcoords[35] = 0;

    // Draw right-left triangle
    // Vertex at (1, 0, 0)
    mesh.vertices[54] = 1 + start_x;
    mesh.vertices[55] = 0 + start_y;
    mesh.vertices[56] = 0 + start_z;
    mesh.normals[54] = 1;
    mesh.normals[55] = 0;
    mesh.normals[56] = 0;
    mesh.texcoords[36] = 0;
    mesh.texcoords[37] = 0;

    // Vertex at (1, 0, 1)
    mesh.vertices[57] = 1 + start_x;
    mesh.vertices[58] = 0 + start_y;
    mesh.vertices[59] = 1 + start_z;
    mesh.normals[57] = 1;
    mesh.normals[58] = 0;
    mesh.normals[59] = 0;
    mesh.texcoords[38] = 0;
    mesh.texcoords[39] = 1;

    mesh.vertices[27] = 0 + start_x;
    mesh.vertices[28] = 1 + start_y;
    mesh.vertices[29] = 1 + start_z;
    mesh.normals[27] = 0;
    mesh.normals[28] = 1;
    mesh.normals[29] = 0;
    mesh.texcoords[18] = 0;
    mesh.texcoords[19] = 1;

    // Vertex at (1, 1, 1)
    mesh.vertices[30] = 1 + start_x;
    mesh.vertices[31] = 1 + start_y;
    mesh.vertices[32] = 1 + start_z;
    mesh.normals[30] = 0;
    mesh.normals[31] = 1;
    mesh.normals[32] = 0;
    mesh.texcoords[20] = 1;
    mesh.texcoords[21] = 1;
    
    // Vertex at (1, 1, 0)
    mesh.vertices[33] = 1 + start_x;
    mesh.vertices[34] = 1 + start_y;
    mesh.vertices[35] = 0 + start_z;
    mesh.normals[33] = 0;
    mesh.normals[34] = 1;
    mesh.normals[35] = 0;
    mesh.texcoords[22] = 1;
    mesh.texcoords[23] = 0;

    
    // Draw left-left triangle
    // Vertex at (0, 0, 0)
    mesh.vertices[36] = 0 + start_x;
    mesh.vertices[37] = 0 + start_y;
    mesh.vertices[38] = 0 + start_z;
    mesh.normals[36] = -1;
    mesh.normals[37] = 0;
    mesh.normals[38] = 0;
    mesh.texcoords[24] = 0;
    mesh.texcoords[25] = 0;

    // Vertex at (0, 0, 1)
    mesh.vertices[39] = 0 + start_x;
    mesh.vertices[40] = 0 + start_y;
    mesh.vertices[41] = 1 + start_z;
    mesh.normals[39] = -1;
    mesh.normals[40] = 0;
    mesh.normals[41] = 0;
    mesh.texcoords[26] = 0;
    mesh.texcoords[27] = 1;

    // Vertex at (0, 1, 0)
    mesh.vertices[42] = 0 + start_x;
    mesh.vertices[43] = 1 + start_y;
    mesh.vertices[44] = 0 + start_z;
    mesh.normals[42] = -1;
    mesh.normals[43] = 0;
    mesh.normals[44] = 0;
    mesh.texcoords[28] = 1;
    mesh.texcoords[29] = 0;

    // Draw left-right triangle
    // Vertex at (0, 0, 1)
    mesh.vertices[45] = 0 + start_x;
    mesh.vertices[46] = 0 + start_y;
    mesh.vertices[47] = 1 + start_z;
    mesh.normals[45] = -1;
    mesh.normals[46] = 0;
    mesh.normals[47] = 0;
    mesh.texcoords[30] = 0;
    mesh.texcoords[31] = 1;

    // Vertex at (0, 1, 1)
    mesh.vertices[48] = 0 + start_x;
    mesh.vertices[49] = 1 + start_y;
    mesh.vertices[50] = 1 + start_z;
    mesh.normals[48] = -1;
    mesh.normals[49] = 0;
    mesh.normals[50] = 0;
    mesh.texcoords[32] = 1;
    mesh.texcoords[33] = 1;

    // Vertex at (0, 1, 0)
    mesh.vertices[51] = 0 + start_x;
    mesh.vertices[52] = 1 + start_y;
    mesh.vertices[53] = 0 + start_z;
    mesh.normals[51] = -1;
    mesh.normals[52] = 0;
    mesh.normals[53] = 0;
    mesh.texcoords[34] = 1;
    mesh.texcoords[35] = 0;

    // Draw right-left triangle
    // Vertex at (1, 0, 0)
    mesh.vertices[54] = 1 + start_x;
    mesh.vertices[55] = 0 + start_y;
    mesh.vertices[56] = 0 + start_z;
    mesh.normals[54] = 1;
    mesh.normals[55] = 0;
    mesh.normals[56] = 0;
    mesh.texcoords[36] = 0;
    mesh.texcoords[37] = 0;

    // Vertex at (1, 0, 1)
    mesh.vertices[57] = 1 + start_x;
    mesh.vertices[58] = 0 + start_y;
    mesh.vertices[59] = 1 + start_z;
    mesh.normals[57] = 1;
    mesh.normals[58] = 0;
    mesh.normals[59] = 0;
    mesh.texcoords[38] = 0;
    mesh.texcoords[39] = 1;

    // Vertex at (1, 1, 0)
    mesh.vertices[60] = 1 + start_x;
    mesh.vertices[61] = 1 + start_y;
    mesh.vertices[62] = 0 + start_z;
    mesh.normals[60] = 1;
    mesh.normals[61] = 0;
    mesh.normals[62] = 0;
    mesh.texcoords[40] = 1;
    mesh.texcoords[41] = 0;

    // Draw right-right triangle
    // Vertex at (1, 0, 1)
    mesh.vertices[63] = 1 + start_x;
    mesh.vertices[64] = 0 + start_y;
    mesh.vertices[65] = 1 + start_z;
    mesh.normals[63] = 1;
    mesh.normals[64] = 0;
    mesh.normals[65] = 0;
    mesh.texcoords[42] = 0;
    mesh.texcoords[43] = 1;

    // Vertex at (1, 1, 1)
    mesh.vertices[66] = 1 + start_x;
    mesh.vertices[67] = 1 + start_y;
    mesh.vertices[68] = 1 + start_z;
    mesh.normals[66] = 1;
    mesh.normals[67] = 0;
    mesh.normals[68] = 0;
    mesh.texcoords[44] = 1;
    mesh.texcoords[45] = 1;

    // Vertex at (1, 1, 0)
    mesh.vertices[69] = 1 + start_x;
    mesh.vertices[70] = 1 + start_y;
    mesh.vertices[71] = 0 + start_z;
    mesh.normals[69] = 1;
    mesh.normals[70] = 0;
    mesh.normals[71] = 0;
    mesh.texcoords[46] = 1;
    mesh.texcoords[47] = 0;
    */



    printf("Triangle count: %d\r\n", mesh.triangleCount);
    printf("Vertex count: %d\r\n", mesh.vertexCount);


    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}
