


/*******************************************************************************************
*
*   raylib [core] example - 3d camera first person
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "rcamera.h"
#include <array>
#include <iostream>

#define MAX_COLUMNS 20

constexpr int TILE_SIZE = 10;
constexpr int TILE_WIDTH = 80;
constexpr int TILE_HEIGHT = 80;


void generate_maze(std::array<bool, TILE_WIDTH * TILE_HEIGHT> &maze) {
    auto at = [](auto& maze, int x, int y) -> bool& {
        return maze[x + TILE_WIDTH * y];
    }; 

    for (int x = 0; x < TILE_WIDTH; ++x) {
        at(maze, x, 0) = true;
        at(maze, x, TILE_HEIGHT - 1) = true;
    }
    for (int y = 1; y < TILE_HEIGHT - 1; ++y) {
        at(maze, 0, y) = true;
        at(maze, TILE_WIDTH - 1, y) = true;
    }

    int x = 0;
    int y = TILE_HEIGHT / 2;
    at(maze, x, y) = false;
    ++x;
    enum Direction: int {
        LEFT=0, UP=1, RIGHT=2, DOWN=3
    };

    Direction dir = RIGHT;
    while (true) {
        switch (dir) {
            case LEFT:
            case RIGHT:
                at(maze, x, y) = false;
                at(maze, x, y - 1) = true;
                at(maze, x, y + 1) = true;
                x += dir == LEFT ? -1 : 1;
                break;
            case UP:
            case DOWN:
                at(maze, x, y) = false;
                at(maze, x - 1, y) = true;
                at(maze, x + 1, y) = true;
                y += dir == UP ? -1 : 1;
                break;
        }
        if (GetRandomValue(0, 99) > 70) {
            int delta = GetRandomValue(0, 1) == 1 ? -1 : 1;
            Direction new_dir = static_cast<Direction>((static_cast<int>(dir) + delta) % 4);
        }
        if (x == 0 || y == 0 || x == TILE_WIDTH - 1 || y == TILE_HEIGHT - 1) {
            break;
        }
    }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    std::array<bool, TILE_WIDTH * TILE_HEIGHT> maze{};

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");
    generate_maze(maze);


    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);
    
            for (int i = 0; i < TILE_WIDTH; ++i) {
                for (int j = 0; j < TILE_HEIGHT; ++j) {
                    Color color = maze[i + TILE_WIDTH * j] ? BLACK : WHITE;
                    DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


