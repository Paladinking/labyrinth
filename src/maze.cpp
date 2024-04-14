#include "maze.h"
#include <unordered_set>
#include <algorithm>
#include <raymath.h>

void generate_maze(Maze &maze) {
    std::vector<std::array<bool, MAZE_HEIGHT>> visited{};
    visited.resize(MAZE_WIDTH);

    std::vector<int> potential_roots{};
    std::unordered_set<int> invalid_roots{};

    for (int x = 0; x < MAZE_WIDTH; ++x) {
        maze.at(x, 0) = true;
        maze.at(x, MAZE_HEIGHT - 1) = true;
        invalid_roots.insert(x);
        invalid_roots.insert(x + MAZE_WIDTH * (MAZE_HEIGHT - 1));
    }
    for (int y = 1; y < MAZE_HEIGHT - 1; ++y) {
        maze.at(0, y) = true;
        maze.at(MAZE_WIDTH - 1, y) = true;
        invalid_roots.insert(y * MAZE_WIDTH);
        invalid_roots.insert(MAZE_WIDTH - 1 + y * MAZE_WIDTH);
    }

    int x = 0;
    int y = MAZE_HEIGHT / 2;
    maze.at(x, y) = false;
    ++x;
    enum Direction : int { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };
    auto dx = [](Direction d) -> int {
        return d == LEFT ? -1 : (d == RIGHT ? 1 : 0);
    };
    auto dy = [](Direction d) -> int {
        return d == UP ? -1 : (d == DOWN ? 1 : 0);
    };

    unsigned marked = 0;
    auto mark = [&visited, &marked, &invalid_roots,
                 &potential_roots](auto &maze, int x, int y) -> bool & {
        if (!visited[x][y]) {
            ++marked;
            if (invalid_roots.count(x + MAZE_WIDTH * y) == 0) {
                potential_roots.push_back(x + MAZE_WIDTH * y);
            }
        }
        visited[x][y] = true;
        return maze.at(x, y);
    };

    Direction dir = RIGHT;
    Direction prev_side = UP;
    maze.at(x, y + 1) = true;
    maze.at(x, y - 1) = true;
    do {
        while (true) {
            if (GetRandomValue(0, 99) > 70) {
                int delta = GetRandomValue(0, 1) == 1 ? 3 : 1;
                Direction new_dir =
                    static_cast<Direction>((static_cast<int>(dir) + delta) % 4);
                mark(maze, x + dx(dir), y + dy(dir)) = true;
                mark(maze, x + dx(dir) + dx(new_dir),
                     y + dy(dir) + dy(new_dir)) = true;
                mark(maze, x + dx(dir) - dx(new_dir),
                     y + dy(dir) - dy(new_dir)) = true;
                dir = new_dir;
            }
            Direction side =
                static_cast<Direction>((static_cast<int>(dir) + 1) % 4);
            x += dx(dir);
            y += dy(dir);
            bool end = visited[x][y];
            mark(maze, x - dx(dir), y - dy(dir));
            mark(maze, x - dx(dir) + dx(prev_side),
                 y - dy(dir) + dy(prev_side));
            mark(maze, x - dx(dir) - dx(prev_side),
                 y - dy(dir) - dy(prev_side));
            if (end) {
                if (!visited[x + dx(side)][y + dy(side)]) {
                    mark(maze, x + dx(side), y + dy(side)) = true;
                }
                if (!visited[x + dx(side)][y + dy(side)]) {
                    mark(maze, x + dx(side), y + dy(side)) = true;
                }
                break;
            }
            maze.at(x, y) = false;
            maze.at(x + dx(side), y + dy(side)) = true;
            maze.at(x - dx(side), y - dy(side)) = true;
            prev_side = side;
            if (x == 0 || y == 0 || x == MAZE_WIDTH - 1 ||
                y == MAZE_HEIGHT - 1) {
                maze.at(x, y) = true;
                break;
            }
        }
        while (true) {
            if (potential_roots.size() == 0) {
                return;
            }
            int ix = GetRandomValue(0, potential_roots.size() - 1);
            int root = potential_roots[ix];
            x = potential_roots[ix] % MAZE_WIDTH;
            y = potential_roots[ix] / MAZE_WIDTH;
            potential_roots[ix] = potential_roots[potential_roots.size() - 1];
            potential_roots.pop_back();
            int v = GetRandomValue(0, 3);
            bool good = false;
            for (int i = 0; i < 4; ++i) {
                int val = (v + i) % 4;
                dir = static_cast<Direction>(val);
                Direction side = static_cast<Direction>((val + 1) % 4);
                if (!maze.at(x + dx(side), y + dy(side)) ||
                    !maze.at(x - dx(side), y - dy(side))) {
                    continue;
                }
                if (x + dx(dir) == 0 || x + dx(dir) == MAZE_WIDTH - 1 ||
                    y + dy(dir) == 0 || y + dy(dir) == MAZE_HEIGHT - 1) {
                    continue;
                }
                if (visited[x + dx(dir)][y + dy(dir)]) {
                    continue;
                }
                if (maze.at(x - dx(dir), y - dy(dir))) {
                    continue;
                }
                maze.at(x, y) = false;
                good = true;
                break;
            }
            if (!good) {
                invalid_roots.insert(root);
            } else {
                break;
            }
        }
    } while (marked < MAZE_WIDTH * MAZE_HEIGHT);

}

Maze::Maze(Model* wall) : map{MAZE_WIDTH}, wall{wall} { 
    generate_maze(*this);
}

Vector2 Maze::get_start() const {
    return {TILE_SIZE / 2, MAZE_HEIGHT / 2 * TILE_SIZE + TILE_SIZE / 2 };

}

void Maze::draw(Camera* camera) {
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j]) {
                Color color = (i % 2 == 0) ? ((j % 2 == 0) ? LIGHTGRAY : DARKGRAY) : ((j % 2 == 0) ? GRAY : BLACK);
                float cube_height{7};
                Vector3 cubePosition = {TILE_SIZE * i + TILE_SIZE / 2, 0.0f,
                                        TILE_SIZE * j + TILE_SIZE / 2};
                DrawModelEx(*wall, cubePosition, {0, 0, 0}, 0, {TILE_SIZE/2 , TILE_SIZE/2, TILE_SIZE/2}, WHITE);
            }
        }
    }
}

void Maze::adjust_movement(float x, float y, float &dx, float &dy, float radius) {
    float x_adj[] = {1, 1, -1, -1};
    float y_adj[] = {1, -1, 1, -1};
    for (int i = 0; i < TILE_SIZE; ++i) {
        float new_x = x + dx + radius * x_adj[i];
        float new_y = y + dy + radius * y_adj[i];
        int tx = new_x / TILE_SIZE;
        int ty = new_y / TILE_SIZE;
        if (tx < 0 || ty < 0 || tx >= MAZE_WIDTH || ty >= MAZE_HEIGHT) {
            continue;
        }
        if (!at(tx, ty)) {
            continue;
        }
        Rectangle r = {tx * TILE_SIZE, ty * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        if (!CheckCollisionCircleRec({x + dx, y + dy}, radius, r)) {
            continue;
        }
        // Get closest point to circle on rectangle
        float nx = std::clamp(x + dx, r.x, r.x + r.width);
        float ny = std::clamp(y + dy, r.y, r.y + r.height);
            
        // Get vector from position to nearest point.
        Vector2 ray = {nx - (x + dx), ny - (y + dy)};
            
        // Get vector from position to where circle meets rectangle.
        float adjust = radius - Vector2Length(ray);
        ray = Vector2Scale(Vector2Normalize(ray), -adjust); 
        dx += ray.x;
        dy += ray.y;
    }
}

