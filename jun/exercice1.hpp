#include <SDL2/SDL.h>
#include <stdio.h>
class MainSDLWindow{
public:
     MainSDLWindow();
    ~MainSDLWindow();
    void  keyboard();
    int Init(const char* title, int Window_height, int Window_weight);
    SDL_Renderer * GetRenderer(void);

private:
    SDL_Window *     window;
    SDL_Renderer * renderer;

};

class Movement{
public:
    Movement();
    
private:

};