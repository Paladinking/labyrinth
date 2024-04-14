#define RLIGHTS_IMPLEMENTATION
#include "maze.h"
#include "raylib.h"
#include "raymath.h"
#include "rlights.h"
#include "rcamera.h"
#include <cstdlib>
#include <iostream>

#if defined(PLATFORM_WEB)  // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#else
    #define GLSL_VERSION            330
#endif

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

    // FOR SHADERS =======================
    Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float amb_light[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(shader, ambientLoc, amb_light, SHADER_UNIFORM_VEC4);
    wall.materials[1].shader = shader;

    Light lights[MAX_LIGHTS] = { 0 };
    float offset = 7;
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){TILE_SIZE / 2 + 0 * offset, TILE_SIZE * 2, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2}, Vector3Zero(), WHITE, shader);
    lights[1] = CreateLight(LIGHT_POINT, (Vector3){TILE_SIZE / 2 + 1 * offset, TILE_SIZE * 2, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2}, Vector3Zero(), WHITE, shader);
    lights[2] = CreateLight(LIGHT_POINT, (Vector3){TILE_SIZE / 2 + 2 * offset, TILE_SIZE * 2, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2}, Vector3Zero(), WHITE, shader);
    lights[3] = CreateLight(LIGHT_POINT, (Vector3){TILE_SIZE / 2 + 3 * offset, TILE_SIZE * 2, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2}, Vector3Zero(), WHITE, shader);
    lights[4] = CreateLight(LIGHT_POINT, (Vector3){TILE_SIZE / 2 + 4 * offset, TILE_SIZE * 2, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2}, Vector3Zero(), WHITE, shader);
    // ^^^^^^^ FOR SHADERS =======================
    
    Maze maze{&wall};

    Camera camera = {0};
    camera.position = {TILE_SIZE / 2, 2.0f, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2}; // Camera position
    camera.target = {TILE_SIZE + TILE_SIZE / 2, 2.0f, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2};   // Camera looking at point
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

        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);

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

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
          if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
          else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
        }
        maze.draw();
        DrawModelEx(wall, {0,0,0}, {0,0,0}, 0, {5,5,5}, WHITE);


        EndMode3D();

        EndDrawing();
    }
    CloseWindow(); // Close window and OpenGL context
    return 0;
}
