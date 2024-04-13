#include "maze.h"
#include "raylib.h"
#include "rcamera.h"
#include <cstdlib>
#include <iostream>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight,
               "raylib [core] example - 3d camera first person");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    std::cout << "Generate done" << std::endl;
    Model wall = LoadModel("assets/wall.glb");
    Maze maze{};

    Camera camera = {0};
    camera.position = {TILE_SIZE / 2, 2.0f, TILE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2}; // Camera position
    camera.target = {TILE_SIZE + TILE_SIZE / 2, 2.0f, TILE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2};   // Camera looking at point
    camera.up = {0.0f, 1.0f,
                 0.0f};  // Camera up vector (rotation towards target)
    camera.fovy = 60.0f; // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type
    int cameraMode = CAMERA_FIRST_PERSON;
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    DisableCursor();  // Limit cursor to relative movement inside the window

    float move_speed = 0.3;

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        Camera old_pos {camera};
        if (IsGamepadAvailable(0)) {
            float move_x = -GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
            float move_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            float rot_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
            float rot_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
            if (std::abs(move_x) < 0.4)
                move_x = 0;
            if (std::abs(move_y) < 0.4)
                move_y = 0;
            if (std::abs(rot_x) < 0.4)
                rot_x = 0;
            if (std::abs(rot_y) < 0.4)
                rot_y = 0;
            move_x *= 0.1;
            move_y *= 0.1;
            rot_x *= 1.7;
            rot_y *= 1.7;

            UpdateCameraPro(&camera, {move_x, move_y, 0.0f},
                            {rot_x, rot_y, 0.0f}, GetMouseWheelMove() * 2.0f);
        } else {
            UpdateCameraPro(
                &camera,
                {(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * move_speed -
                     (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * move_speed,
                 (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * move_speed -
                     (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * move_speed,
                 0.0f},
                {
                    GetMouseDelta().x * 0.2f, // Rotation: yaw
                    GetMouseDelta().y * 0.2f, // Rotation: pitch
                    0.0f                      // Rotation: roll
                },
                GetMouseWheelMove() * 2.0f); // Move to target (zoom)
        }
        if (!maze.free_at(camera.position.x, camera.position.z, 1.0f)) {
            camera = old_pos;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        maze.draw();

        DrawModelEx(wall, {0, 0, 0}, {0, 0, 0}, 0, {5, 5, 5}, WHITE);
        // Draw some cubes around

        EndMode3D();

        EndDrawing();
    }
    CloseWindow(); // Close window and OpenGL context
    return 0;
}
