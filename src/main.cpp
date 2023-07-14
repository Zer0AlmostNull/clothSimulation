#include <SDL.h>
#include <iostream>

#include "../include/simulation.h"

#include "../include/settings.h"
#include "../include/types.h"
#include "../include/macros.h"
#include "../include/render_context.h"


RenderContext rc;


int main(int argc, char* args[])
{
    // Init SDL
    ASSERT(!SDL_Init(SDL_INIT_EVERYTHING),
        "SDL failed to initialize: %s\n",
        SDL_GetError());

    // create window
    rc.window = SDL_CreateWindow("-", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WND_WIDTH, WND_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    ASSERT(rc.window, "Failed to initialize window: %s\n", SDL_GetError());

    // create renderer  
    rc.renderer = SDL_CreateRenderer(rc.window, -1, SDL_RENDERER_PRESENTVSYNC);
    ASSERT(rc.renderer, "Failed to initialize renderer: %s\n", SDL_GetError());


    // init simulation
    Simulation sim(rc);


    // init clock/timer
    u64 now = SDL_GetPerformanceCounter();
    u64 last = 0;

    f32 deltaTime = .0001f;

    // event loop
    SDL_Event windowEvent;
    while (1)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;

        }

        // ----- handle timing-----
        last = now;
        now = SDL_GetPerformanceCounter();
        deltaTime = (f32)((now - last) * 1000 / (f32)SDL_GetPerformanceFrequency()) * 0.001f;
        //f32 fps = 1 / deltaTime;

        // ----- handle input -----
        //const u8* keystate = SDL_GetKeyboardState(NULL);

        // --- update ---
        sim.update(deltaTime);

        // --- draw ---
        sim.draw();


        SDL_RenderPresent(rc.renderer);

        // reset the screen
        SDL_SetRenderDrawColor(rc.renderer, 0, 0, 0, 255);
        SDL_RenderClear(rc.renderer);
    }


    SDL_DestroyWindow(rc.window);
    SDL_Quit();

    return 0;
}