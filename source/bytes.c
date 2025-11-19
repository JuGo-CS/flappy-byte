#include "bytes.h"
#include <tonc.h>
#include <string.h>

extern OBJ_ATTR obj_buffer[128]; // reference the global sprite buffer

#define BYTE_SPRITE_START 3   // start after ball sprite
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

#include "font_8x8.h" // minimal 8x8 tiles for '0' and '1'

// pointer to first text sprite
static OBJ_ATTR *text_sprites;

void bytes_init() {
    text_sprites = &obj_buffer[BYTE_SPRITE_START];

    // Put our gold color in OBJ palette slot 1
    pal_obj_mem[1] = RGB15(31, 25, 0); // #FCCF0D approximated in RGB15 (GBA uses 5-bit RGB)

    // Copy minimal font tiles for '0' and '1' into tile_mem_obj[0]
    memcpy(&tile_mem_obj[3][0], font_8x8Tiles, font_8x8TilesLen);

    // Hide text sprites at start
    for(int i=0;i<16;i++)
        obj_set_attr(&text_sprites[i], ATTR0_HIDE, 0, 0);
}

void bytes_draw_u16(u16 value) {
    char bits[17];

    // convert value to 16-bit binary
    for(int i=0;i<16;i++)
        bits[15-i] = (value & (1 << i)) ? '1' : '0';
    bits[16] = 0;

    int start_x = (240 - 16*CHAR_WIDTH)/2; // center horizontally
    int y = (160 - CHAR_HEIGHT)/2;         // center vertically

    for(int i=0;i<16;i++) {
        OBJ_ATTR *s = &text_sprites[i];
        int tile_index = bits[i]-'0'; // 0 or 1

        obj_set_attr(s,
            ATTR0_SQUARE | ATTR0_8BPP | (y & 0xFF),
            ATTR1_SIZE_8 | ((start_x + i*CHAR_WIDTH) & 0x1FF),
            tile_index);

        s->attr2 = ATTR2_PALBANK(1) | (tile_index & 0x3FF); // use palette 1 for gold
    }
}
