


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
#include <array>
#include <cstddef>
#include <sound.h>
#include <iostream>

#define MAX_COLUMNS 20

constexpr int TILE_SIZE = 10;
constexpr int TILE_WIDTH = 80;
constexpr int TILE_HEIGHT = 80;
bool walking = false;
enum Direction: int {
    LEFT=0, UP=1, RIGHT=2, DOWN=3
};

class Maze {
private:    
    std::array<std::array<bool, TILE_WIDTH>, TILE_HEIGHT> map;
public:
    bool at(unsigned x, unsigned y){
        return map[x][y];
    }
    
    Maze() : map{} {
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
                Direction new_dir = static_cast<Direction>((static_cast<int>(dir) + delta) % 4);
            }
            if (x == 0 || y == 0 || x == TILE_WIDTH - 1 || y == TILE_HEIGHT - 1) {
                break;
            }
        }

    };

    void draw(){
        ClearBackground(RAYWHITE);

        for (int i = 0; i < TILE_WIDTH; ++i) {
            for (int j = 0; j < TILE_HEIGHT; ++j) {
                Color color = map[i][j] ? BLACK : WHITE;
                DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
            }
        }
    }
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitAudioDevice(); 
    //Load sounds
    MySound stepsWalk = MySound( { LoadSound("resources/Step1.mp3"), LoadSound("resources/Step2.mp3") , LoadSound("resources/Step3.mp3")});
    MySound stepsRun = MySound( { LoadSound("resources/Run(1).mp3"), LoadSound("resources/Run(2).mp3") , LoadSound("resources/Run(2).mp3")});
    MySound monsterSteps = MySound( { LoadSound("resources/M3Step(1).mp3"), LoadSound("resources/M3Step(2).mp3") });
    MySound chainRattle = MySound( { LoadSound("resources/Chain(1).mp3"), LoadSound("resources/Chain(2).mp3"), LoadSound("resources/Chain(3).mp3"), LoadSound("resources/Chain(4).mp3") });
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Maze maze{};
    // Main game loop
    stepsWalk.SetVolume(100);
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        if(IsKeyDown(KEY_RIGHT)) {
             monsterSteps.PlayMySound();
             chainRattle.PlayMySound();
        }
         if(IsKeyDown(KEY_LEFT)) {
            stepsWalk.PlayMySound();
        }
         if(IsKeyDown(KEY_UP)) {
            stepsRun.PlayMySound();
        }
        


       
        BeginDrawing();
            maze.draw();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


