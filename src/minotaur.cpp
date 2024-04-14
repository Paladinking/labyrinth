#include "minotaur.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>

Minotaur::Minotaur(float x, float y) : x(x), y(y), last_update(0), anims_count{0}, anim_index{2}, anim_current_frame{0} {}

void Minotaur::calculate_path(Maze &maze, float px, float py) {
    int tx = px / TILE_SIZE;
    int ty = py / TILE_SIZE;

    int sx = x / TILE_SIZE;
    int sy = y / TILE_SIZE;
    struct Node {
        int x, y, cost;
        Node *last;
    };
    std::vector<std::unique_ptr<Node>> nodes;

    auto comp = [](Node *n1, Node *n2) -> bool { return n1->cost < n2->cost; };
    std::unordered_set<int> visited{};
    std::priority_queue<Node *, std::vector<Node *>, decltype(comp)> queue{
        comp};
    nodes.emplace_back(new Node{sx, sy, 0, nullptr});
    queue.push(nodes.back().get());
    while (queue.size() > 0) {
        Node *n = queue.top();
        queue.pop();
        if (n->x == tx && n->y == ty) {
            path.clear();
            while (true) {
                path.push_back({n->x * TILE_SIZE + TILE_SIZE / 2,
                                n->y * TILE_SIZE + TILE_SIZE / 2});
                if (n->last != nullptr) {
                    n = n->last;
                } else {
                    break;
                }
            }
            std::cout << "Found it!!!" << std::endl;
            return;
        }
        auto add = [&n, &nodes, &queue, &visited](int x, int y) {
            if (visited.count(x + MAZE_WIDTH * y) == 0) {
                nodes.emplace_back(new Node{x, y, n->cost + 1, n});
                queue.push(nodes.back().get());
                visited.insert(x + MAZE_WIDTH * y);
            }
        };
        if (n->x > 1 && !maze.at(n->x - 1, n->y)) {
            add(n->x - 1, n->y);
        }
        if (n->x < MAZE_WIDTH - 2 && !maze.at(n->x + 1, n->y)) {
            add(n->x + 1, n->y);
        }
        if (n->y > 1 && !maze.at(n->x, n->y - 1)) {
            add(n->x, n->y - 1);
        }
        if (n->y < MAZE_HEIGHT - 2 && !maze.at(n->x, n->y + 1)) {
            add(n->x, n->y + 1);
        }
    }
    std::cout << "Did not find it" << std::endl;
}

bool Minotaur::can_rush_player(Maze& maze, float px, float py, float dx, float dy, Vector2& end) {
    int tx = x / TILE_SIZE;
    int ty = y / TILE_SIZE;
    int ptx = px / TILE_SIZE;
    int pty = py / TILE_SIZE;
    end = {ptx * TILE_SIZE + TILE_SIZE / 2, pty * TILE_SIZE + TILE_SIZE / 2};
    if (dx > 0.7f) {
        if (ptx < tx || ty != pty) {
            return false;
        }
        for (int x_cor = tx; x_cor <= ptx; ++x_cor) {
            if (maze.at(x_cor, ty)) {
                return false;
            }
        }
        int endx = ptx;
        while (!maze.at(endx + 1, ty) && endx < MAZE_WIDTH - 2) {
            ++endx;
        }
        end.x = TILE_SIZE * endx + TILE_SIZE / 2;
        return true;
    } else if (dx < -0.7f) {
        if (ptx > tx || ty != pty) {
            return false;
        }
        for (int x_cor = ptx; x_cor >= px; --x_cor) {
            if (maze.at(x_cor, ty)) {
                return false;
            }
        }
        int endx = ptx;
        while (!maze.at(endx - 1, ty) && endx > 1) {
            --endx;
        }
        end.x = TILE_SIZE * endx + TILE_SIZE / 2;
        return true;
    }
    if (dy > 0.7f) {
        if (pty < ty || tx != ptx) {
            return false;
        }
        for (int y_cor = ty; y_cor <= pty; ++y_cor) {
            if (maze.at(tx, y_cor)) {
                return false;
            }
        }
        int endy = pty;
        while (!maze.at(tx, endy + 1) && endy < MAZE_HEIGHT - 2) {
            ++endy;
        }
        end.y = TILE_SIZE * endy + TILE_SIZE / 2;
        return true;
    } else if (dy < -0.7f) {
        if (pty > ty || tx != ptx) {
            return false;
        }
        for (int y_cor = pty; y_cor >= py; --y_cor) {
            if (maze.at(tx, y_cor)) {
                return false;
            }
        }
        int endy = pty;
        while (!maze.at(tx, endy - 1) && endy > 1) {
            --endy;
        }
        end.y = TILE_SIZE * endy + TILE_SIZE / 2;
        return true;
    }
    return false;
}

bool Minotaur::tick(Maze &maze, float px, float py, float p_raduis) {
    double now = GetTime();
    if (now - last_update > 5.0f && !rushing) {
        calculate_path(maze, px, py);
        last_update = now;
    }
    if (path.empty()) {
        return false;
    }
    Vector2 pos = path.back();
    Vector2 delta = {pos.x - x, pos.y - y};
    if (Vector2LengthSqr(delta) < 0.2f) {
        path.pop_back();
        if (path.empty()) {
            if (rushing) {
                rushing = false;
                last_update = now;
            }
            return false;
        }
        pos = path.back();
        delta = {pos.x - x, pos.y - y};
    }
    float move_speed = 0.5f;
    delta = Vector2Normalize(delta);

    if (rushing) {
        std::cout << x << ',' << y << std::endl;
        move_speed = 1.0f;
    } else {


        Vector2 rush;
        if (can_rush_player(maze, px, py, delta.x, delta.y, rush)) {
            rushing = false;
            path.clear();
            path.push_back(rush);
            std::cout << "Rush from " << x << ',' << y << " to " << rush.x << ',' << rush.y << std::endl;
            move_speed = 1.0f;
            rushing = true;
        }
    }
    float angle = Vector2Angle({delta.x, delta.y}, {0.0f, 1.0f});
    model.transform = MatrixRotateY(-angle);

    x += delta.x * move_speed;
    y += delta.y * move_speed;
    if (static_cast<int>(x / TILE_SIZE) == static_cast<int>(px / TILE_SIZE) && 
        static_cast<int>(y / TILE_SIZE) == static_cast<int>(py / TILE_SIZE)) {
        return true;
    }
    return false;
}

void Minotaur::load_animation_stuff() {
    model = LoadModel("assets/minotaur.m3d");
    model_animations = LoadModelAnimations("assets/minotaur.m3d", &anims_count);
}

void Minotaur::unload_animation_stuff() {
    UnloadModel(model);
    UnloadModelAnimations(model_animations, anims_count);
}

void Minotaur::draw() {
    // DrawCube({x, 0.0f, y}, 1.0f, 1.0f, 1.0f, RED);

    ModelAnimation anim = model_animations[anim_index];
    anim_current_frame = (anim_current_frame + 1) % anim.frameCount;
    UpdateModelAnimation(model, anim, anim_current_frame);
    DrawModel(model, {x, 0.0f, y}, 5.0f, WHITE);
}
