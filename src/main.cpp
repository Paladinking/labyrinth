

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
#include "rcamera.h"
#include <iostream>
#include <unordered_set>
#include <vector>

#define MAX_COLUMNS 20

constexpr int TILE_SIZE = 50;
constexpr int TILE_WIDTH = 16;
constexpr int TILE_HEIGHT = 16;

void draw_maze(std::vector<bool> &maze) {
  for (int i = 0; i < TILE_WIDTH; ++i) {
    for (int j = 0; j < TILE_HEIGHT; ++j) {
      Color color = maze[i + TILE_WIDTH * j] ? BLACK : WHITE;
      DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
    }
  }
}

void generate_maze(std::vector<bool> &maze) {
    std::vector<bool> visited{};
    visited.resize(TILE_WIDTH * TILE_HEIGHT);

  std::vector<int> potential_roots{};
  std::unordered_set<int> invalid_roots{};
  auto at = [](auto &maze, int x, int y) -> auto {
    return maze.at(x + TILE_WIDTH * y);
  };

  for (int x = 0; x < TILE_WIDTH; ++x) {
    at(maze, x, 0) = true;
    at(maze, x, TILE_HEIGHT - 1) = true;
    invalid_roots.insert(x);
    invalid_roots.insert(x + TILE_WIDTH * (TILE_HEIGHT - 1));
  }
  for (int y = 1; y < TILE_HEIGHT - 1; ++y) {
    at(maze, 0, y) = true;
    at(maze, TILE_WIDTH - 1, y) = true;
    invalid_roots.insert(y * TILE_WIDTH);
    invalid_roots.insert(TILE_WIDTH - 1 + y * TILE_WIDTH);
  }

  int x = 0;
  int y = TILE_HEIGHT / 2;
  at(maze, x, y) = false;
  ++x;
  enum Direction : int { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };
  auto dx = [](Direction d) -> int {
    return d == LEFT ? -1 : (d == RIGHT ? 1 : 0);
  };
  auto dy = [](Direction d) -> int {
    return d == UP ? -1 : (d == DOWN ? 1 : 0);
  };

  unsigned marked = 0;
  auto mark = [&visited, &at, &marked, &invalid_roots,
               &potential_roots](auto &maze, int x, int y) -> auto{
    if (!at(visited, x, y)) {
      ++marked;
      if (invalid_roots.count(x + TILE_WIDTH * y) == 0) {
        potential_roots.push_back(x + TILE_WIDTH * y);
      }
    }
    at(visited, x, y) = true;
    return at(maze, x, y);
  };

  Direction dir = RIGHT;
  Direction prev_side = UP;
  at(maze, x, y + 1) = true;
  at(maze, x, y - 1) = true;
  int last_press = 0;
  while (GetKeyPressed() == 0) {
      PollInputEvents();
    WaitTime(0.001);
  }
  do {
    while (true) {
      BeginDrawing();
      for (int i = 0; i < TILE_WIDTH; ++i) {
        for (int j = 0; j < TILE_HEIGHT; ++j) {
          Color color;
          if (at(maze, i, j)) {
            if (at(visited, i, j)) {
              if (invalid_roots.count(i + TILE_WIDTH * j) == 0) {
                color = BLACK;
              } else {
                color = DARKGRAY;
              }
            } else {
              if (invalid_roots.count(i + TILE_WIDTH * j) == 0) {
                color = GRAY;
              } else {
                color = GREEN;
              }
            }
          } else {
            if (at(visited, i, j)) {
              if (invalid_roots.count(i + TILE_WIDTH * j) == 0) {
                color = YELLOW;
              } else {
                color = CLITERAL(Color){ 203, 209, 0, 255 };
              }
            } else {
              if (invalid_roots.count(i + TILE_WIDTH * j) == 0) {
                color = WHITE;
              } else {
                color = RED;
              }
            }
          }
          DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                        color);
        }
      }
      DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
      EndDrawing();
      if (GetRandomValue(0, 99) > 70) {
        int delta = GetRandomValue(0, 1) == 1 ? 3 : 1;
        Direction new_dir =
            static_cast<Direction>((static_cast<int>(dir) + delta) % 4);
        mark(maze, x + dx(dir), y + dy(dir)) = true;
        mark(maze, x + dx(dir) + dx(new_dir), y + dy(dir) + dy(new_dir)) = true;
        mark(maze, x + dx(dir) - dx(new_dir), y + dy(dir) - dy(new_dir)) = true;
        dir = new_dir;
      }
      Direction side = static_cast<Direction>((static_cast<int>(dir) + 1) % 4);
      x += dx(dir);
      y += dy(dir);
      bool end = at(visited, x, y);
      mark(maze, x - dx(dir), y - dy(dir));
      mark(maze, x - dx(dir) + dx(prev_side), y - dy(dir) + dy(prev_side));
      mark(maze, x - dx(dir) - dx(prev_side), y - dy(dir) - dy(prev_side));
      if (end) {
        if (!at(visited, x + dx(side), y + dy(side))) {
          mark(maze, x + dx(side), y + dy(side)) = true;
        }
        if (!at(visited, x + dx(side), y + dy(side))) {
          mark(maze, x + dx(side), y + dy(side)) = true;
        }
        break;
      }
      at(maze, x, y) = false;
      at(maze, x + dx(side), y + dy(side)) = true;
      at(maze, x - dx(side), y - dy(side)) = true;
      prev_side = side;
      if (x == 0 || y == 0 || x == TILE_WIDTH - 1 || y == TILE_HEIGHT - 1) {
        at(maze, x, y) = true;
        break;
      }
    }
    while (true) {
      //std::cout << potential_roots.size() << std::endl;
      if (potential_roots.size() == 0) {
        return;
      }
      int ix = GetRandomValue(0, potential_roots.size() - 1);
      int root = potential_roots[ix];
      x = potential_roots[ix] % TILE_WIDTH;
      y = potential_roots[ix] / TILE_WIDTH;
      potential_roots[ix] = potential_roots[potential_roots.size() - 1];
      potential_roots.pop_back();
      int v = GetRandomValue(0, 3);
      bool good = false;
      for (int i = 0; i < 4; ++i) {
        int val = (v + i) % 4;
        dir = static_cast<Direction>(val);
        Direction side = static_cast<Direction>((val + 1) % 4);
        if (!at(maze, x + dx(side), y + dy(side)) ||
            !at(maze, x - dx(side), y - dy(side))) {
          continue;
        }
        if (x + dx(dir) == 0 || x + dx(dir) == TILE_WIDTH - 1 ||
            y + dy(dir) == 0 || y + dy(dir) == TILE_HEIGHT - 1) {
          continue;
        }
        if (at(visited, x + dx(dir), y + dy(dir))) {
          continue;
        }
        if (at(maze, x - dx(dir), y - dy(dir))) {
          continue;
        }
        at(maze, x, y) = false;
        good = true;
        break;
      }
      if (!good) {
        invalid_roots.insert(root);
      } else {
        break;
      }
    }
  } while (marked < TILE_WIDTH * TILE_HEIGHT);
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 800;

  std::vector<bool> maze{};
  maze.resize(TILE_WIDTH * TILE_HEIGHT);

  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight,
             "raylib [core] example - 3d camera first person");
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  generate_maze(maze);

  std::cout << "Generate done" << std::endl;
  //--------------------------------------------------------------------------------------
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    draw_maze(maze);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
