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
    const u32 clicked = SDL_GetMouseState(&state.mouse_x, &state.mouse_y);

    if (clicked == 1) {
        const char* link = links[state.current_target.val - 1];
        if (link != state.last_link && link != NULL) {
            state.last_link = link;
            open_embed(link);
        }
    }

    if (state.old_mouse_x != state.mouse_x) {
        int diff = std::clamp((state.mouse_x-state.old_mouse_x)/2, -1, 1);
        rotate(diff * rotspeed);
        state.mouse_x = WINDOW_WIDTH/2;
        state.old_mouse_x = state.mouse_x;
        SDL_WarpMouseInWindow(state.window, state.mouse_x, state.mouse_y);
    }

    if (state.old_mouse_y != state.mouse_y) {
        int diff = std::clamp((state.mouse_y-state.old_mouse_y)/2, -1, 1);
        state.pitch += diff * 5;
        state.mouse_y = WINDOW_WIDTH/2;
        state.old_mouse_y = state.mouse_y;
        SDL_WarpMouseInWindow(state.window, state.mouse_x, state.mouse_y);
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