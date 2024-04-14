#pragma once
#include <raylib.h>
#include <vector>
#include "maze.h"

class Minotaur {
public:
    Minotaur(float x, float y);

    void draw();

    void tick(Maze& maze, float px, float py, float prad);

private:
    void calculate_path(Maze& maze, float px, float py);

    bool can_rush_player(Maze& maze, float px, float py, float dx, float dy, Vector2& end);

    float x;
    float y;

    bool rushing = false;

    std::vector<Vector2> path;
    double last_update;
};
