#pragma once
#include <vector>
#include "SDL2/SDL.h"

class Game
{
public:

    Game();
    void Run();

private:

    bool paused = false;
    bool running = false;
    bool alive = false;
    bool growing = false;

    int fps = 0;
    int x_food;
    int y_food;
    int x_warp1;
    int y_warp1;
    int x_warp2;
    int y_warp2;
    int score = 0;

    static const int FRAME_RATE     = 1000 /60;
    static const int SCREEN_WIDTH   = 640;
    static const int SCREEN_HEIGHT  = 640;
    static const int GRID_WIDTH     = 32;
    static const int GRID_HEIGHT    = 32;

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    enum class Block { head, body, food, warp, empty };
    enum class Move { up, down, left, right };
    

    Move last_dir = Move::up;
    Move dir = Move::up;

    struct { float x = GRID_WIDTH / 2, y = GRID_HEIGHT / 2; } pos;

    SDL_Point head = { (int)(pos.x), (int)(pos.y) };
    SDL_Point food = { (int)(pos.x), (int)(pos.y) };
    SDL_Point warp1 = { (int)(pos.x), (int)(pos.y) };
    SDL_Point warp2 = { (int)(pos.x), (int)(pos.y) };
    std::vector<SDL_Point> body;

    Block grid[GRID_WIDTH][GRID_HEIGHT];

    float speed = 0.25f;

    void GameLoop();
    void Render();
    void Food();
    void Warp();
    void Update();
    void PollEvents();
    void Close();
    void Reload();
};