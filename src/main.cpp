#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <algorithm>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "metal.h"
#include "map.h"
#include "state.h"
#include "setup.cpp"
#include "drawing.cpp"
#include "bridge.cpp"
#include "moving.cpp"

void destroy(){
    for (int i = 0; i < TEXTURE_AMOUNT; i++) {
        SDL_FreeSurface(textures[i]);
    };
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

void main_loop() {
    #ifdef __EMSCRIPTEN__
    if (state.quit) {
        destroy();
        emscripten_cancel_main_loop();
        return;
    }
    #endif

    process_input();

    // clear framebuf? i think??
    memset(state.pixels, 0, sizeof(state.pixels));

    SDL_RenderClear(state.renderer);

    draw();

    SDL_UpdateTexture(state.texture, NULL, state.pixels, WINDOW_WIDTH * 4);
    SDL_RenderCopyEx(
        state.renderer,
        state.texture,
        NULL,
        NULL,
        0.0,
        NULL,
        SDL_FLIP_VERTICAL);
    SDL_RenderPresent(state.renderer);
}

void test_close_function() {
    state.active = true;
}

int main() {
    setupSDL();
    init();
    close_function = test_close_function;
    close_function();

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 60, true);
    #endif
}