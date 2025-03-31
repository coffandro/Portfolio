#ifndef MAP_H
#define MAP_H
#include "metal.h"

#define MAP_SIZE 19
static u8 MAPDATA[MAP_SIZE * MAP_SIZE] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 7, 1, 9,  1, 5, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  0, 0, 0, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 8, 0, 0, 0, 0,  0, 0, 0, 4, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0,  0, 0, 0, 1, 0, 0, 0,
    1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,  0, 0, 0, 3, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 10, 1, 6, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
};

struct Sprite {
    double x;
    double y;
    int texture;
};

#define SPRITE_AMOUNT 0

Sprite sprite[SPRITE_AMOUNT] = {};

//function used to sort the sprites
void sortSprites(int* order, double* dist, int amount);

#define TEXTURE_AMOUNT 11
#define TEXTURE_WIDTH 320
#define TEXTURE_HEIGHT 320
static std::string texture_paths[TEXTURE_AMOUNT] = {
    "cache_assets/Brick.png", // 1
    "cache_assets/Guide.png", // 2
    "cache_assets/FalloutLondonLogo.png", // 3
    "cache_assets/FightForForrestLogo.png", // 4
    "cache_assets/GameOn.png", // 5
    "cache_assets/RoundSnakeLogo.png", // 6
    "cache_assets/UrLogo.png", // 7
    "cache_assets/WhackACapitalistLogo.png", // 8
    "cache_assets/FishiesLogo.png", // 9
    "cache_assets/SPCLogo.png", // 10
};
static SDL_Surface* textures[TEXTURE_AMOUNT];

static std::string floor_path = "cache_assets/Floor.png";
static std::string ceiling_path = "cache_assets/Ceiling.png";

static SDL_Surface* floor_texture;
static SDL_Surface* ceiling_texture;

#define LINK_AMOUNT 4

static const char* links[TEXTURE_AMOUNT] = {
    NULL, // 0
    NULL, // 1
    NULL, // 2
    "https://fallout4london.com/", // 3
    "https://coffandro.itch.io/fightforforest", // 4
    "https://on-undervisning.dk/materialer/din-data", // 5
    "https://coffandro.itch.io/round-snake", // 6
    "https://coffandro.itch.io/ur", // 7
    "https://coffandro.itch.io/wack-a-capitalist", // 8
    "https://coffandro.itch.io/fishies", // 9
    "https://coffandro.itch.io/spc", // 10
};

#define CURSOR_SIZE 64
#define CURSOR_AMOUNT 3

static std::string cursor_paths[CURSOR_AMOUNT] = {
    "cache_assets/CursorDefault.png",
    "cache_assets/CursorHover.png",
    "cache_assets/CursorClick.png",
};

static SDL_Texture* cursor_textures[CURSOR_AMOUNT];

#endif