#pragma once

#include <SDL.h>

class Application final
{
private:
    SDL_Window *window;
    SDL_GLContext ctx;

    bool shouldQuit = false;

public:
    Application(int argc, char** argv);
    ~Application();

    int run();
    void loopOnce();

};
