#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED
#pragma once
#include <vector>
#include "SDL2/SDL.h"
    struct Segment {
        int x;
        int y;
        Segment(int posx, int posy) {
            x = posx;
            y = posy;
        }
    };
class Game
{

public:

    Game();
    void Run();
    int GetScore();
    int GetSize();

private:
    bool running = false;
    bool gameOver = false;
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
    
    std::vector<Segment *> body;

    Block grid[GRID_WIDTH][GRID_HEIGHT];

    float speed = 0.5f;

    void GameLoop();
    void Render();
    void Food();
    void Grow();
    void Update();
    void PollEvents();
    void addSegment(int body_x, int body_y);
    void Close();

};



#endif