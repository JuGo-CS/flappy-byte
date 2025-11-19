#ifndef FONT_8X8_H
#define FONT_8X8_H

#include <tonc.h>

// Each tile is 8x8 pixels, 8BPP (1 byte per pixel)
const unsigned char font_8x8Tiles[2*64] = {
    // Tile 0: '0' (8x8)
    0,1,1,1,1,1,1,0,
    1,2,2,2,2,2,2,1,
    1,2,0,0,0,0,2,1,
    1,2,0,0,0,0,2,1,
    1,2,0,0,0,0,2,1,
    1,2,0,0,0,0,2,1,
    1,2,2,2,2,2,2,1,
    0,1,1,1,1,1,1,0,

    // Tile 1: '1' (8x8)
    0,0,0,1,1,0,0,0,
    0,0,1,2,1,0,0,0,
    0,1,0,2,1,0,0,0,
    0,0,0,2,1,0,0,0,
    0,0,0,2,1,0,0,0,
    0,0,0,2,1,0,0,0,
    0,1,1,2,1,1,0,0,
    0,0,0,1,1,0,0,0
};

const unsigned int font_8x8TilesLen = sizeof(font_8x8Tiles);

#endif
