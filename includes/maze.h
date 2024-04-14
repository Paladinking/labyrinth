#pragma once

constexpr int MAZE_HEIGHT = 30;
constexpr int MAZE_WIDTH = 30;
constexpr float TILE_SIZE = 4.0f;

#include <array>
#include <raylib.h>
#include <vector>

enum Direction : int { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };

class Maze {
private:
    std::vector<std::array<bool, MAZE_HEIGHT>> map;
    std::vector<Matrix> transforms;
    Model* wall;

public:
    std::pair<int,int> victory_con;

    bool &at(unsigned x, unsigned y) { return map[x][y]; }

    Vector2 get_start() const;

    Maze(Model* wall);
    void adjust_movement(float x, float y, float& dx, float& dy, float radius);

    void draw(Camera* camera);

    void set(int x, int y, bool val);
};
