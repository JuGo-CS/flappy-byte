#include <tonc.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "ball.h"
#include "pipes.h"
#include "bytes.h"
#include "../graphics/game_ball.h"
#include "../graphics/game_background_ver2.h"

OBJ_ATTR obj_buffer[128];
bool byte_updated = false;
bool pipe_passed = false;
int game_frame_counter = 0;
extern unsigned char random_byte;
extern const char* current_gate;

int main(void) {
    // ------------------------------
    // Display & background setup
    // ------------------------------
    // srand(time(NULL));
    srand(REG_TM0D);
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;

    memcpy(pal_bg_mem, game_background_ver2Pal, game_background_ver2PalLen);
    memcpy(&tile_mem[0][0], game_background_ver2Tiles, game_background_ver2TilesLen);
    
    pal_bg_mem[1] = game_ballPal[1];

    for(int y=0; y<20; y++)
        for(int x=0; x<30; x++)
            se_mem[30][y*32 + x] = game_background_ver2Map[y*30 + x];

    // REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32;
    REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32 | BG_PRIO(1);

    // ------------------------------
    // OAM & sprites setup
    // ------------------------------
    oam_init(obj_buffer, 128);
    ball_init();
    pipes_init();      // <--- initialize pipes
    

    bool key_press = false;
    bool game_started = false;   
    // int game_frame_counter = 0;
    int game_second_counter = 0;
    int flickering_byte = 0;

    while(1) {
        vid_vsync();
        key_poll();
        
        game_frame_counter++;

        if (!game_started) {
            if (key_hit(KEY_A)) {
                game_started = true;
                key_press = true;
                game_byte = generate_next_byte();
                // random_byte = generate_next_byte();
                draw_byte_bits(game_byte, 30, 12);
                byte_logic();
                oam_copy(oam_mem, obj_buffer, 128);
            }
            continue;   
        }

        // Game started — normal update
        if (key_hit(KEY_A))
            key_press = true;

        if (game_frame_counter >= 60){
            game_frame_counter = 0;
            game_second_counter++;
            // byte_updated = false;

            if (pipe_speed < 3 && game_second_counter % 60 == 0){
                pipe_speed++;
            }
        }

        // ------------------------------
        // Update game objects
        // ------------------------------
        // draw_gate(current_gate, 12);
        ball_update(key_press);
        pipes_update();            // <--- move pipes

        if (pipes_posX <= 21 && !byte_updated) {
            // apply_byte_logic();
            // byte_logic();
            apply_byte_logic();
            byte_updated = true;
            flickering_byte = 0;
        }

        //Collision with pipes
        if(pipes_check_collision(BALL_X, ball_Y) || ball_Y <= 23 || ball_Y >= 151)
        {
            game_started = false;
            game_frame_counter = 0;
            game_second_counter = 0;
            flickering_byte = 0;
            byte_updated = false;
            pipe_passed = false;
            clear_byte_bits(30, 12);
            clear_byte_bits(150, 12);
            clear_gate();
            reset_ball();
            reset_pipes();
            oam_copy(oam_mem, obj_buffer, 128);
            continue;
        }
        
        pipe_passed = true;


        if (flickering_byte <= 30) {
            byte_logic();
        }

        flickering_byte++;
        oam_copy(oam_mem, obj_buffer, 128);

        key_press = false;
    }
}

