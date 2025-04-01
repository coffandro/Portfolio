#ifndef MOVING_CPP
#define MOVING_CPP

#include <SDL2/SDL.h>

#include "metal.h"
#include "map.h"
#include "state.h"
#include "bridge.cpp"

static void rotate(f32 rot) {
    const v2 d = state.dir, p = state.plane;
    state.dir.x = d.x * cos(rot) - d.y * sin(rot);
    state.dir.y = d.x * sin(rot) + d.y * cos(rot);
    state.plane.x = p.x * cos(rot) - p.y * sin(rot);
    state.plane.y = p.x * sin(rot) + p.y * cos(rot);
}

bool can_move_to(f32 new_x, f32 new_y) {
    for(int _x = 0; _x < MAP_SIZE; _x++) {
        for(int _y = 0; _y < MAP_SIZE; _y++) {
            if (MAPDATA[(_y*MAP_SIZE)+_x] != 0) {
                if (MAPDATA[(_y*MAP_SIZE)+_x] == 
                    MAPDATA[(int(new_y)*MAP_SIZE)+int(new_x)]
                ) {
                    return false;
                }
            }
        }
    }
    return true;
}

void move(float x, float y) {
    v2 cpos = state.pos;

    if (can_move_to(cpos.x + x, cpos.y)) {
        cpos.x += x;
    }

    if (can_move_to(cpos.x, cpos.y + y)) {
        cpos.y += y;
    }

    if (can_move_to(cpos.x, cpos.y)) {
        state.pos = cpos;
    }
}

void process_input() {
    if (!state.active) return;

    const f32
        rotspeed = 3.0f * 0.016f,
        movespeed = 3.0f * 0.016f;

    const u8 *keystate = SDL_GetKeyboardState(NULL);
    const u32 clicked = SDL_GetMouseState(&state.mouse_x, NULL);

    if (state.old_mouse_x != state.mouse_x) {
        int diff = (state.mouse_x-state.old_mouse_x);
        if (diff > 0) {
            diff = diff/diff;
        } else {
            diff = -diff/diff;
        }
        rotate(diff * rotspeed);
        state.mouse_x = WINDOW_WIDTH/2;
        state.old_mouse_x = state.mouse_x;
        SDL_WarpMouseInWindow(state.window, state.mouse_x, NULL);
    }

    if (state.perpWallDist < 2.5 && links[state.current_target.val] != NULL) {
        state.cursor_index = 1;
    } else if (clicked != 0) {
        state.cursor_index = 2;
    } else {
        state.cursor_index = 0;
    }

    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if(e.button.button == SDL_BUTTON_LEFT && state.perpWallDist < 2.5) {
                const char* link = links[state.current_target.val];
                if (link != NULL) {
                    state.active = false;
                    open_embed(link);
                }
            }
        }
    }


    if (keystate[SDL_SCANCODE_Q]) {
        rotate(-rotspeed);
    }
    if (keystate[SDL_SCANCODE_E]) {
        rotate(rotspeed);
    }
    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        move(state.dir.x * movespeed,
            state.dir.y * movespeed);
    }
    if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        move(state.dir.x * -movespeed,
            state.dir.y * -movespeed);
    }
    if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
        move((state.dir.x * cos(-1.5f) - state.dir.y * sin(-1.5f)) * movespeed,
            (state.dir.x * sin(-1.5f) + state.dir.y * cos(-1.5f)) * movespeed);
    }
    if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
        move((state.dir.x * cos(1.5f) - state.dir.y * sin(1.5f)) * movespeed,
            (state.dir.x * sin(1.5f) + state.dir.y * cos(1.5f)) * movespeed);
    }
}

#endif