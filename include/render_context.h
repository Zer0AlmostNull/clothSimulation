#include <SDL_render.h>

#include "types.h"
#include "settings.h"

#pragma once

struct RenderContext
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
};