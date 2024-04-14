#include "maze.h"
#include <raylib.h>
#include <raymath.h>
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
        float dx, dy, rot_x, rot_y;
        if (IsGamepadAvailable(0)) {
            dx = -GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
            dy = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            rot_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
            rot_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
            if (std::abs(dx) < 0.4)
                dx = 0;
            if (std::abs(dy) < 0.4)
                dy = 0;
            if (std::abs(rot_x) < 0.4)
                rot_x = 0;
            if (std::abs(rot_y) < 0.4)
                rot_y = 0;
            dx *= 0.1;
            dy *= 0.1;
            rot_x *= 1.7;
            rot_y *= 1.7;
            //UpdateCameraPro(&camera, {move_x, move_y, 0.0f},
            //                {rot_x, rot_y, 0.0f}, GetMouseWheelMove() * 2.0f);
        } else {
            dx = (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * move_speed - 
                 (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * move_speed;
            dy = (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * move_speed - 
                 (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * move_speed;
            rot_x = GetMouseDelta().x * 0.2f;
            rot_y = GetMouseDelta().y * 0.2f;
        }

        Vector3 forward = GetCameraForward(&camera);
        forward.y = 0;
        forward = Vector3Scale(Vector3Normalize(forward), dx);
        Vector3 right = GetCameraRight(&camera);
        right.y = 0;
        right = Vector3Scale(Vector3Normalize(right), dy);
        Vector3 movement = Vector3Add(forward, right);
        CameraPitch(&camera, -rot_y*DEG2RAD, true, false, false);
        CameraYaw(&camera, -rot_x*DEG2RAD, false);

        maze.adjust_movement(camera.position.x, camera.position.z, movement.x, movement.z, 1.0f);
        
        camera.position = Vector3Add(movement, camera.position);
        camera.target = Vector3Add(movement, camera.target);
        //UpdateCameraPro(&camera, {dx, dy, 0.0f}, {rot_x, rot_y, 0.0f}, 0.0f);

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
