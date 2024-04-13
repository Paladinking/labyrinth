#pragma once

constexpr int TILE_SIZE = 10;

constexpr int TILE_HEIGHT = 80;
constexpr int TILE_WIDTH = 80;

#include <array>
#include <raylib.h>
#include <vector>

enum Direction : int { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };

class Maze {
private:
    std::vector<std::array<bool, TILE_HEIGHT>> map;

public:
    bool &at(unsigned x, unsigned y) { return map[x][y]; }

    Maze();

    void draw();
};
