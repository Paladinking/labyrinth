#pragma once
#include <raylib.h>
#include <vector>
#include "maze.h"

class Minotaur {
public:
    Minotaur(float x, float y);

    void draw();

    bool tick(Maze& maze, float px, float py, float prad);

    void load_animation_stuff();

    void unload_animation_stuff();

    void reset(float x, float y);
private:
    void calculate_path(Maze& maze, float px, float py);

    bool can_rush_player(Maze& maze, float px, float py, float dx, float dy, Vector2& end);
public:
    float x;
    float y;
    Model model;
    ModelAnimation *model_animations;
    int anims_count;
    unsigned anim_index;
    unsigned anim_current_frame;

    bool rushing = false;

    std::vector<Vector2> path;
    double last_update;
};
