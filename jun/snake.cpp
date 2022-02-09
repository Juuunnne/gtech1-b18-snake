#include <iostream>
#include <string>
#include <ctime>
#include "SDL2/SDL.h"
#include "snake.hpp"

using namespace std;
//Initialize playground size
Game::Game()
{   //init grid
    for (int i = 0; i < GRID_WIDTH; ++i)
        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            grid[i][j] = Block::empty;
        }
}
//Initialize window/renderer
void Game::Run()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    // Create Window
    if (window == NULL) window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    // Create renderer
    if(renderer == NULL) renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    alive = true;
    running = true;
    score = 0;
    GameLoop();
}
//Main Game loop
void Game::GameLoop()
{   
    Uint32 before, second = SDL_GetTicks(), after;
    int frame_time, frames = 0;
    Food();
    while (running)
    
    {   //get time since SDL2 start
        before = SDL_GetTicks();

        PollEvents();
        if (paused == false)    Update();
        Render();
        frames++;
        after = SDL_GetTicks();
        frame_time = after - before;

        //if frame lasted less than frame rate, wait until frame rate = frame time
        if (FRAME_RATE > frame_time)
        {
            SDL_Delay(FRAME_RATE - frame_time);
        }
    }

}
//Get snake's direction
void Game::PollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN) //if key pressed
        {   //get the pressed key
            switch (e.key.keysym.sym)
            {
                case SDLK_UP:
                    if (last_dir != Move::down)    dir = Move::up;
                    break;

                case SDLK_DOWN:
                    if (last_dir != Move::up)    dir = Move::down;
                    break;

                case SDLK_LEFT:
                    if (last_dir != Move::right)    dir = Move::left;
                    break;

                case SDLK_RIGHT:
                    if (last_dir != Move::left) dir = Move::right;
                    break;
                
                case SDLK_p:
                    if (!paused)    paused = true;
                    else if (paused)paused = false;
                    break;

                case SDLK_SPACE:
                    if(!alive)    Reload();
                    break;
            }
        }
    }
}
//Update snake's direction  
void Game::Update()
{
    if (!alive)
        return;
    
    switch (dir)
    {
        case Move::up:
            pos.y -= speed;
            pos.x = floorf(pos.x); //round value
            break;

        case Move::down:
            pos.y += speed;
            pos.x = floorf(pos.x);
            break;

        case Move::left:
            pos.x -= speed;
            pos.y = floorf(pos.y);
            break;

        case Move::right:
            pos.x += speed;
            pos.y = floorf(pos.y);
            break;
    }

    if (grid[head.x][head.y] == grid[warp1.x][warp1.y])
    {
        switch (dir)
        {
        case Move::up:
            pos.x = warp2.x;
            pos.y = warp2.y - 1;
            break;

        case Move::down:
            pos.x = warp2.x;
            pos.y = warp2.y + 1;
            break;

        case Move::left:
            pos.x = warp2.x - 1;
            pos.y = warp2.y;
            break;

        case Move::right:
            pos.x = warp2.x + 1;
            pos.y = warp2.y;
            break;
        }
    }

    if (grid[head.x][head.y] == grid[warp2.x][warp2.y])
    {
        switch (dir)
        {
        case Move::up:
            pos.x = warp1.x;
            pos.y = warp1.y - 1;
            break;

        case Move::down:
            pos.x = warp1.x;
            pos.y = warp1.y + 1;
            break;

        case Move::left:
            pos.x = warp1.x - 1;
            pos.y = warp1.y;
            break;

        case Move::right:
            pos.x = warp1.x + 1;
            pos.y = warp1.y;
            break;
        }
    }

    // No wall collision 
    if (pos.x < 0) alive = false;
    else if (pos.x > GRID_WIDTH - 1) alive = false;

    if (pos.y < 0) alive = false;
    else if (pos.y > GRID_HEIGHT - 1) alive = false;

    int new_x = static_cast<int>(pos.x);
    int new_y = static_cast<int>(pos.y);

    // Check if head position has changed
    if (new_x != head.x || new_y != head.y)
    {
        last_dir = dir;
        // If we are growing, just make a new segment
        if (growing == true)
        {
            body.push_back(head);
            growing = false;
            grid[head.x][head.y] = Block::body;
        }
        else
        {
            // We need to shift the body
            SDL_Point free = head;
            vector<SDL_Point>::reverse_iterator rit = body.rbegin();
            for ( ; rit != body.rend(); ++rit)
            {
                grid[free.x][free.y] = Block::body;
                swap(*rit, free);
            }

            grid[free.x][free.y] = Block::empty;
        }

    
    }
    
    head.x = new_x;
    head.y = new_y;

    // Check if there's food over here
    if (grid[head.x][head.y] == Block::food)
    {
        Food();
        growing = true;
        score ++;
        if (score %4 == 0)
        {
            Warp();
        }
        
    }

    grid[head.x][head.y] = Block::head;
}
//render the snake, the playground and the fruit
void Game::Render()
{
    //createa rect with a tile size
    SDL_Rect block;
    block.w = SCREEN_WIDTH / GRID_WIDTH;
    block.h = SCREEN_WIDTH / GRID_HEIGHT;

    // Clear screen
    if (paused) SDL_SetRenderDrawColor(renderer, 85, 160, 80, 255);
    else        SDL_SetRenderDrawColor(renderer, 85, 160, 50, 255);
    SDL_RenderClear(renderer);

    if (score >= 4)
    {
        // Render warp 1
        block.x = warp1.x * block.w;
        block.y = warp1.y * block.h;
        SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255); 
        SDL_RenderFillRect(renderer, &block);

        // Render warp 2
        block.x = warp2.x * block.w;
        block.y = warp2.y * block.h;
        SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255); 
        SDL_RenderFillRect(renderer, &block);
    }
    
    // Render snake's head
    block.x = head.x * block.w;
    block.y = head.y * block.h;
    if (paused) SDL_SetRenderDrawColor(renderer, 230, 165, 45, 255);
    else if (alive) SDL_SetRenderDrawColor(renderer, 230, 165, 15, 255);
    else       SDL_SetRenderDrawColor(renderer, 195, 200, 145, 125);
    SDL_RenderFillRect(renderer, &block);

    //Render snake's body 
    if (paused) SDL_SetRenderDrawColor(renderer, 195, 185, 50, 255);
    else if(alive) SDL_SetRenderDrawColor(renderer, 195, 185, 20, 255);
    else      SDL_SetRenderDrawColor(renderer, 195, 200, 145, 125);
    for (SDL_Point & point : body)
    {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(renderer, &block);
    }

    // Render food
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    if (paused) SDL_SetRenderDrawColor(renderer, 230, 55, 30, 255);
    else SDL_SetRenderDrawColor(renderer, 255, 55, 0, 255);    
    SDL_RenderFillRect(renderer, &block);

    // Update Screen
    SDL_RenderPresent(renderer);
}
//spawn the food 
void Game::Food()
{
    while (true)
    {   //get a random position for the food
        srand(time(NULL));
        x_food = rand() % GRID_WIDTH;
        y_food = rand() % GRID_HEIGHT;

        //if the position is free : set the food here
        if (grid[x_food][y_food] == Block::empty)
        {
            grid[x_food][y_food] = Block::food;
            food.x = x_food;
            food.y = y_food;
            break;
        }
    }
}
//spawn warp
void Game::Warp()
{
    while (true)
    {   
        //get a random position for the food
        x_warp1 = rand() % GRID_WIDTH;
        y_warp1 = rand() % GRID_HEIGHT;

        //if the position is free : set the food here
        if (grid[x_warp1][y_warp1] == Block::empty)
        {
            grid[x_warp1][y_warp1] = Block::warp;
            warp1.x = x_warp1;
            warp1.y = y_warp1;
        }
        //get a random position for warp2
        x_warp2 = rand() % GRID_WIDTH;
        y_warp2 = rand() % GRID_HEIGHT;

        //if the position is free : set the food here
        if (grid[x_warp2][y_warp2] == Block::empty && x_warp2 != x_warp1 && y_warp2 != y_warp1 )
        {
            grid[x_warp2][y_warp2] = Block::warp;
            warp2.x = x_warp2;
            warp2.y = y_warp2;
            break;
        }
    }
}
//Close the sdl window
void Game::Close()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
//method for replay after death
void Game::Reload()
{
    alive = true;
    for (int i = 0; i < GRID_WIDTH; ++i)
    {
        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            grid[i][j] = Block::empty;
        } 
    }
    body.clear();
    head.x = 16;
    head.y = 16;
    pos.x = 16;
    pos.y = 16;
    grid[head.x][head.y] = Block::head;
    dir = Move::up;
    last_dir = Move::up;
    Run();
}