#ifndef DRAWING_CPP
#define DRAWING_CPP
#include "metal.h"
#include "state.h"

// draw vertical line on x placement between y0 and y1
static void verline(int x, int y0, int y1, u32 color) {
    // for every int between y0 and y1 on the x cord place a
    // pixel on the screen with the color 
    for (int y = y0; y <= y1; y++) {
        state.pixels[(y * WINDOW_WIDTH) + x] = color;
    }
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    // x = min(max(x,TEXTURE_WIDTH-1),0);
    // y = min(max(y,TEXTURE_WIDTH-1),0);

    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

switch (bpp)
{
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
      }
}


void draw()
{
    for(int y = WINDOW_HEIGHT / 2 + 1; y < WINDOW_HEIGHT; ++y)
    {
      // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      float rayDirX0 = state.dir.x - state.plane.x;
      float rayDirY0 = state.dir.y - state.plane.y;
      float rayDirX1 = state.dir.x + state.plane.x;
      float rayDirY1 = state.dir.y + state.plane.y;

      // Current y position compared to the center of the screen (the horizon)
      int p = y - WINDOW_HEIGHT / 2;

      // Vertical position of the camera.
      float posZ = 0.5 * WINDOW_HEIGHT;

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      float rowDistance = posZ / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WINDOW_WIDTH;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WINDOW_WIDTH;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = state.pos.x + rowDistance * rayDirX0;
      float floorY = state.pos.y + rowDistance * rayDirY0;

      for(int x = 0; x < WINDOW_WIDTH; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // get the texture coordinate from the fractional part
        int tx = (int)(TEXTURE_WIDTH * (floorX - cellX)) & (TEXTURE_WIDTH - 1);
        int ty = (int)(TEXTURE_HEIGHT * (floorY - cellY)) & (TEXTURE_HEIGHT - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        Uint32 color;

        // floor
        color = getpixel(floor_texture, ty, tx);
        color = (color >> 1) & 8355711; // make a bit darker
        state.pixels[(y * WINDOW_WIDTH) + x] = color;

        //ceiling (symmetrical, at screenHeight - y - 1 instead of y)
        color = getpixel(ceiling_texture, ty, tx);
        color = (color >> 1) & 8355711; // make a bit darker
        state.pixels[((WINDOW_HEIGHT-y-1) * WINDOW_WIDTH) + x] = color;
      }
    }

    // WALL CAST
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        // x coordinate in space from [-1, 1]
        const f32 xcam = (2 * (x / (f32) (WINDOW_WIDTH))) - 1;

        // ray direction through this column
        const v2 dir = {
            state.dir.x + state.plane.x * xcam,
            state.dir.y + state.plane.y * xcam
        };

        // set local position and int position to state position
        v2 pos = state.pos;
        v2i ipos = { (int) pos.x, (int) pos.y };

        // distance ray must travel from one x/y side to the next
        const v2 deltadist = {
            static_cast<f32>(fabsf(dir.x) < 1e-20 ? 1e30 : fabsf(1.0f / dir.x)),
            static_cast<f32>(fabsf(dir.y) < 1e-20 ? 1e30 : fabsf(1.0f / dir.y)),
        };

        // distance from start position to first x/y side
        v2 sidedist = {
            deltadist.x * (dir.x < 0 ? (pos.x - ipos.x) : (ipos.x + 1 - pos.x)),
            deltadist.y * (dir.y < 0 ? (pos.y - ipos.y) : (ipos.y + 1 - pos.y)),
        };

        // integer step direction for x/y, calculated from overall diff
        const v2i step = { (int) sign(dir.x), (int) sign(dir.y) };

        // DDA hit
        hit_point hit = { 0, 0, { 0.0f, 0.0f } };

        while (!hit.val) {
            if (sidedist.x < sidedist.y) {
                sidedist.x += deltadist.x;
                ipos.x += step.x;
                hit.side = 0;
            } else {
                sidedist.y += deltadist.y;
                ipos.y += step.y;
                hit.side = 1;
            }

            hit.val = MAPDATA[ipos.y * MAP_SIZE + ipos.x];
        }

        // distance to hit
        state.perpWallDist =
            hit.side == 0 ?
                (sidedist.x - deltadist.x)
                : (sidedist.y - deltadist.y);
        
        if (x == WINDOW_WIDTH/2) {
            if (state.perpWallDist < 10) {
                state.current_target = hit;
            } else {
                state.current_target = { 0, 0, { 0.0f, 0.0f } };
            }
        }

        // perform perspective division, calculate line height relative to
        // screen center
        int
            h = (int) (WINDOW_HEIGHT / state.perpWallDist),
            y0 = max((WINDOW_HEIGHT / 2) - (h / 2) + (state.pitch), 0),
            y1 = min((WINDOW_HEIGHT / 2) + (h / 2) + (state.pitch), WINDOW_HEIGHT - 1);

        //calculate value of wallX
        double wallX; //where exactly the wall was hit
        if (hit.side == 0) wallX = state.pos.y + state.perpWallDist * dir.y;
        else           wallX = state.pos.x + state.perpWallDist * dir.x;
        wallX -= floor((wallX));
        //x coordinate on the texture
        int texX = int(wallX * double(TEXTURE_WIDTH));
        if(hit.side == 0 && dir.x > 0) texX = TEXTURE_WIDTH - texX - 1;
        if(hit.side == 1 && dir.y < 0) texX = TEXTURE_WIDTH - texX - 1;

        // How much to increase the texture coordinate per screen pixel
        double texStep = 1.0 * TEXTURE_HEIGHT / h;
        // Starting texture coordinate
        double texPos = (y0 - state.pitch - WINDOW_HEIGHT / 2 + h / 2) * texStep;
        for (int y = y0; y <= y1; y++) {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos;

            texX = std::clamp(texX, 0, TEXTURE_WIDTH - 1);
            texY = std::clamp(texY, 0, TEXTURE_HEIGHT - 1);

            texPos += texStep;
            
            u32 color = getpixel(textures[hit.val-1], texX, texY);
            
            if (hit.side == 1) {
                const u32
                    br = ((color & 0xFF00FF) * 0xC0) >> 8,
                    g  = ((color & 0x00FF00) * 0xC0) >> 8;
                color = 0xFF000000 | (br & 0xFF00FF) | (g & 0x00FF00);
            }
            
            state.pixels[(y * WINDOW_WIDTH) + x] = color;
        }

        // bottom of screen
        verline(x, 0, y0, 0x808080);
        // top of screen
        verline(x, y1, WINDOW_HEIGHT - 1, 0xA9A9A9);
    }
}

#endif