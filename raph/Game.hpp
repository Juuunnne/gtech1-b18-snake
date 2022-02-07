#ifndef GAME_HPP
#define GAME_CPP
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
    bool food_ate = false;
    bool alive = false;
    bool growing = false;

    int fps = 0;
    int size = 0;

    static const int FRAME_RATE     = 1000 / 60;
    static const int SCREEN_WIDTH   = 640;
    static const int SCREEN_HEIGHT  = 640;
    static const int GRID_WIDTH     = 32;
    static const int GRID_HEIGHT    = 32;

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    enum class Block { head, body, food, empty };
    enum class Move { up, down, left, right };
    

    Move last_dir = Move::up;
    Move dir = Move::up;

    struct { float x = GRID_WIDTH / 2, y = GRID_HEIGHT / 2; } pos;

    SDL_Point head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    SDL_Point food = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    std::vector<SDL_Point> body;
    
    Block grid[GRID_WIDTH][GRID_HEIGHT];

    float speed = 0.5f;

    void GameLoop();
    void Render();
    void Food();
    void Grow();
    void Update();
    void PollEvents();
    void Close();

};


#endif