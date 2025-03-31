#ifndef STATE_H
#define STATE_H
#include "map.h"
#include <string>

// The struct containing most of the games actually... 
// important stuffs
struct {
    // Bare essentials to draw to SDL2
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;

    // a buffer to contain the pixels to draw on screen
    u32 pixels[WINDOW_WIDTH * WINDOW_HEIGHT];
    //1D Zbuffer
    double ZBuffer[WINDOW_WIDTH];

    //arrays used to sort the sprites
    int spriteOrder[SPRITE_AMOUNT];
    double spriteDistance[SPRITE_AMOUNT];

    // a bool to keep track of whether to run the mainloop or not
    bool quit;
    bool active = false;
    // the distance to the object in front.
    f32 perpWallDist;
    hit_point current_target;

    int old_mouse_x, mouse_x;
    int pitch = 0;
    
    // three vectors to keep track of the position, direction
    // and something I am yet to understand.
    v2 pos, dir, plane;
    // + the currently looked upon point
    v2i lookingat;
} state;

#endif