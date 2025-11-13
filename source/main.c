#include <tonc.h>
#include <stdlib.h>    // for srand()
#include <stdbool.h>
#include <string.h>
#include "ball.h"
#include "byte_flag.h"
#include "../graphics/game_background.h"

OBJ_ATTR obj_buffer[128];

int main(void)
{
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;

    srand(REG_VCOUNT); // seed RNG

    // Load background (BG0)
    memcpy(pal_bg_mem, game_backgroundPal, game_backgroundPalLen);
    memcpy(&tile_mem[0][0], game_backgroundTiles, game_backgroundTilesLen);
    for(int y=0; y<20; y++)
        for(int x=0; x<30; x++)
            se_mem[30][y*32 + x] = game_backgroundMap[y*30 + x];
    REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;

    // Text layer for bytes (BG1)
    memcpy16(&tile_mem[1][0], toncfontTiles, toncfontTilesLen/2); // tiles only
    tte_init_se_default(0, BG_CBB(1) | BG_SBB(31));

    oam_init(obj_buffer, 128);
    ball_init();

    bool key_press = false;
    bool game_started = false;

    int game_byte_counter = 0;
    const int game_byte_delay = 300; // 5 seconds at 60fps

    // Initialize first byte
    game_byte = generate_next_byte();

    while(1)
    {
        vid_vsync();
        key_poll();

        // Wait for game start
        if(!game_started)
        {
            if(key_hit(KEY_A))
                game_started = true;

            oam_copy(oam_mem, obj_buffer, 128);
            continue;
        }

        // Ball input
        if(key_hit(KEY_A))
            key_press = true;

        ball_update(key_press);

        // Update game_byte every 5 seconds
        game_byte_counter++;
        if(game_byte_counter >= game_byte_delay)
        {
            byte_logic();            // applies logic gate + redraw
            game_byte_counter = 0;
        }

        oam_copy(oam_mem, obj_buffer, 128);
        key_press = false;
    }
}
