/*******************************************************************************************
 *
 *   raylib [core] example - 3d camera first person
 *
 *   Example originally created with raylib 1.3, last time updated with
 *raylib 1.3
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
// #include "rlights.h"
#include <array>
#include <vector>
#include <cstdlib>

#define MAX_COLUMNS 20

constexpr int TILE_SIZE = 10;
constexpr int TILE_WIDTH = 80;
constexpr int TILE_HEIGHT = 80;

enum Direction : int { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };

class Maze {
private:
  std::vector<std::array<bool, TILE_WIDTH>> map;

public:
  bool at(unsigned x, unsigned y) { return map[x][y]; }

  Maze() : map(TILE_WIDTH) {
    for (int x = 0; x < TILE_WIDTH; ++x) {
      map[x][0] = true;
      map[x][TILE_HEIGHT - 1] = true;
    }
    for (int y = 1; y < TILE_HEIGHT - 1; ++y) {
      map[0][y] = true;
      map[TILE_HEIGHT - 1][y] = true;
    }

    int x = 0;
    int y = TILE_HEIGHT / 2;
    map[x][TILE_HEIGHT / 2] = false;
    ++x;

    Direction dir = RIGHT;
    while (true) {
      switch (dir) {
      case LEFT:
      case RIGHT:
        map[x][y] = false;
        map[x][y - 1] = true;
        map[x][y + 1] = true;
        x += dir == LEFT ? -1 : 1;
        break;
      case UP:
      case DOWN:
        map[x][y] = false;
        map[x][y - 1] = true;
        map[x][y + 1] = true;
        y += dir == UP ? -1 : 1;
        break;
      }
      if (GetRandomValue(0, 99) > 70) {
        int delta = GetRandomValue(0, 1) == 1 ? -1 : 1;
        Direction new_dir =
            static_cast<Direction>((static_cast<int>(dir) + delta) % 4);
      }
      if (x == 0 || y == 0 || x == TILE_WIDTH - 1 || y == TILE_HEIGHT - 1) {
        break;
      }
    }
  };

  void draw() {
    for (int i = 0; i < map.size(); i++) {
      for (int j = 0; j < map[0].size(); j++) {
        if (map[i][j]) {
          float cube_height{7};
          float cube_width{4};
          Vector3 cubePosition = {10 + cube_width * i, 0.0f, 10 + cube_width * j};
          DrawCube(cubePosition, cube_width, cube_height, cube_width, GRAY);
        }
      }
    }
  }
};

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

  Model wall = LoadModel("assets/wall.glb");

  Camera camera = {0};
  camera.position = {0.0f, 2.0f, 4.0f}; // Camera position
  camera.target = {0.0f, 2.0f, 0.0f};   // Camera looking at point
  camera.up = {0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
  camera.fovy = 60.0f;            // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

  int cameraMode = CAMERA_FIRST_PERSON;
  float move_speed = 0.3;

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  DisableCursor();  // Limit cursor to relative movement inside the window
  //--------------------------------------------------------------------------------------

  Maze maze{};

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateCameraPro(&camera,
                    {
                        (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) *
                                move_speed - // Move forward-backward
                            (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * move_speed,
                        (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) *
                                move_speed - // Move right-left
                            (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * move_speed,
                        0.0f // Move up-down
                    },
                    {
                        GetMouseDelta().x * 0.2f, // Rotation: yaw
                        GetMouseDelta().y * 0.2f, // Rotation: pitch
                        0.0f                      // Rotation: roll
                    },
                    GetMouseWheelMove() * 2.0f); // Move to target (zoom)

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

      UpdateCameraPro(&camera,
                      {
                          (move_x), // Move forward-backward
                          (move_y), // Move right-left
                          0.0f      // Move up-down
                      },
                      {
                          rot_x, // Rotation: yaw
                          rot_y, // Rotation: pitch
                          0.0f   // Rotation: roll
                      },
                      GetMouseWheelMove() * 2.0f);
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    maze.draw();

    DrawModelEx(wall, {0,0,0}, {0,0,0}, 0, {5,5,5}, WHITE);
    // Draw some cubes around

    EndMode3D();

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
