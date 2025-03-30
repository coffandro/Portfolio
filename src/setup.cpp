#ifndef SETUP_CPP
#define SETUP_CPP

#include "metal.h"
#include "map.h"
#include "state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void setupSDL()
{
    // check if SDL can init video
    ASSERT(
        !SDL_Init(SDL_INIT_VIDEO),
        "SDL failed to initialize: %s\n",
        SDL_GetError());

    // create SDL window and check if working
    state.window =
        SDL_CreateWindow(
            "Portfolio",
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            WINDOW_WIDTH * WINDOW_SCALE,
            WINDOW_HEIGHT * WINDOW_SCALE,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    ASSERT(
        state.window,
        "failed to create SDL window: %s\n", SDL_GetError());

    // create SDL renderer and check if working
    state.renderer =
        SDL_CreateRenderer(state.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    ASSERT(
        state.renderer,
        "failed to create SDL renderer: %s\n", SDL_GetError());

    // create SDL texture and check if working
    state.texture =
        SDL_CreateTexture(
            state.renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STREAMING,
            WINDOW_WIDTH,
            WINDOW_HEIGHT);
    ASSERT(
        state.texture,
        "failed to create SDL texture: %s\n", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
}

void init()
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // set start position of player, may consider removing
    // for 0,0 start
    state.pos = (v2) { 1.5, 9.5 };
    // set the current direction of the player
    state.dir = normalize(((v2) { 1.0f, 0.1f }));
    // still unsure
    state.plane = (v2) { 0.0f, 0.66f };

    for(unsigned int i = 0; i < TEXTURE_AMOUNT; i++) {
        if (texture_paths[i] != ""){ 
            textures[i] = IMG_Load(texture_paths[i].c_str());
            ASSERT(
                textures[i], 
                "failed to create SDL surface: %s\n", IMG_GetError());
            SDL_LockSurface(textures[i]);
        }
    }
}
#endif