#pragma once

constexpr int MAZE_HEIGHT = 80;
constexpr int MAZE_WIDTH = 80;
constexpr float TILE_SIZE = 4.0f;

#include <array>
#include <raylib.h>
#include <vector>

enum Direction : int { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };

class Maze {
private:
    std::vector<std::array<bool, MAZE_HEIGHT>> map;
    Model* wall;

public:
    bool &at(unsigned x, unsigned y) { return map[x][y]; }

    bool free_at(float x, float y, float radius);

    Maze(Model* wall);
    void adjust_movement(float x, float y, float& dx, float& dy, float radius);

    void draw();
};
