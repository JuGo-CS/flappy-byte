// main.c - Main game logic for Flappy Byte GBA game.
// Handles game states, input, updates, collisions, and scoring.

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
#include "../graphics/homescreen_state1.h"
#include "../graphics/homescreen_state2.h"
#include "../graphics/game_background_ver2_restart.h"

// Global sprite buffer and game state flags
OBJ_ATTR obj_buffer[128];
bool byte_updated = false;          // Tracks if byte logic has been applied
bool pipe_passed = false;           // Tracks pipe passage
int game_frame_counter = 0;         // Frame timer
extern unsigned char random_byte;   // Random byte for logic (from bytes.c)
extern const char* current_gate;    // Current gate (from bytes.c)
bool homescreen_state = false;      // Homescreen animation state
bool restart_state = false;         // Restart animation state
int animation_counter = 0;          // Animation timer
bool first_time = true;             // First game launch flag
int game_score = 0;                 // Player score

// Load homescreen background state 1
void load_homescreen_state1() {
    memcpy(pal_bg_mem, homescreen_state1Pal, homescreen_state1PalLen);
    memcpy(&tile_mem[0][0], homescreen_state1Tiles, homescreen_state1TilesLen);
    for(int y=0; y<20; y++) {
        for(int x=0; x<30; x++) {
            se_mem[30][y*32 + x] = homescreen_state1Map[y*30 + x];
        }
    }
}

// Load homescreen background state 2
void load_homescreen_state2() {
    memcpy(pal_bg_mem, homescreen_state2Pal, homescreen_state2PalLen);
    memcpy(&tile_mem[0][0], homescreen_state2Tiles, homescreen_state2TilesLen);
    for(int y=0; y<20; y++) {
        for(int x=0; x<30; x++) {
            se_mem[30][y*32 + x] = homescreen_state2Map[y*30 + x];
        }
    }
}

// Load main game background
void load_game_background() {
    memcpy(pal_bg_mem, game_background_ver2Pal, game_background_ver2PalLen);
    memcpy(&tile_mem[0][0], game_background_ver2Tiles, game_background_ver2TilesLen);
    for(int y=0; y<20; y++) {
        for(int x=0; x<30; x++) {
            se_mem[30][y*32 + x] = game_background_ver2Map[y*30 + x];
        }
    }
}

// Load restart background
void load_restart_background() {
    memcpy(pal_bg_mem, game_background_ver2_restartPal, game_background_ver2_restartPalLen);
    memcpy(&tile_mem[0][0], game_background_ver2_restartTiles, game_background_ver2_restartTilesLen);
    for(int y=0; y<20; y++) {
        for(int x=0; x<30; x++) {
            se_mem[30][y*32 + x] = game_background_ver2_restartMap[y*30 + x];
        }
    }
}

// Main game entry point
int main(void) {
    // Setup display and random seed
    srand(REG_TM0D);
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
    load_homescreen_state1();
    REG_BG0CNT = BG_CBB(0) | BG_SBB(30) | BG_8BPP | BG_REG_32x32 | BG_PRIO(1);

    // Init sprites
    oam_init(obj_buffer, 128);
    ball_init();
    pipes_init();
    
    // Local game vars
    bool key_press = false;
    bool game_started = false;
    int game_second_counter = 0;
    // int flickering_byte = 0;  // Future flickering effect

    // Main game loop
    while(1) {
        vid_vsync();
        key_poll();
        game_frame_counter++;

        if (!game_started) {  // Homescreen or restart screen
            if (first_time) {  // First launch animation
                animation_counter++;
                if (animation_counter >= 30) {  // Toggle every 30 frames
                    animation_counter = 0;
                    homescreen_state = !homescreen_state;
                    if (homescreen_state) {
                        load_homescreen_state2();
                    } else {
                        load_homescreen_state1();
                    }
                }
            } else {  // Restart animation
                animation_counter++;
                if (animation_counter >= 60) {  // Slower toggle
                    animation_counter = 0;
                    restart_state = !restart_state;
                    if (restart_state) {
                        load_game_background();
                    } else {
                        load_restart_background();
                    }
                }
                oam_copy(oam_mem, obj_buffer, 128);  // Hide sprites
            }

            if (key_hit(KEY_A)) {  // Start game on A press
                load_game_background();
                pal_bg_mem[1] = game_ballPal[1];  // Set ball color

                game_started = true;
                first_time = false;
                key_press = true;

                game_byte = generate_next_byte();  // Random start byte
                byte_logic();  // Init random byte and gate
                random_pipes();  // Generate pipes
                draw_byte_bits(game_byte, 30, 12, -1);  // Draw left byte
                oam_copy(oam_mem, obj_buffer, 128);
            }
            continue;
        }

        // Game active
        if (key_hit(KEY_A)){  // Jump on A press
            key_press = true;
        }

        if (game_frame_counter >= 60){  // Every second
            game_frame_counter = 0;
            game_second_counter++;
            if (pipe_speed < 3 && game_second_counter % 60 == 0) {  // Speed up every minute
                pipe_speed++;
            }
        }

        ball_update(key_press);  // Update ball physics
        pipes_update();  // Move pipes

        int collision_result = pipes_check_collision(BALL_X, ball_Y);  // Check collisions
        if (collision_result == 1 || collision_result == 3 || ball_Y <= 23 || ball_Y >= 151) {  // Game over on hit or out of bounds
            game_started = false;
            game_frame_counter = 0;
            game_second_counter = 0;
            // flickering_byte = 0;
            byte_updated = false;
            pipe_passed = false;
            game_score = 0;  // Reset score
            game_byte = generate_next_byte();
            
            clear_byte_bits(30, 12, -1);  // Clear bytes
            clear_byte_bits(150, 12, -1);
            clear_gate();
            reset_ball();  // Reset positions
            reset_pipes();
            oam_copy(oam_mem, obj_buffer, 128);
            continue;
        }

        if (pipes_posX <= 21 && !byte_updated) {  // Apply logic when pipes pass threshold
            apply_byte_logic();  // Update game byte
            byte_logic();  // New random byte/gate
            byte_updated = true;
            game_score++;  // Increment score
            // flickering_byte = 0;
        }
        
        pipe_passed = true;

        // if (flickering_byte <= 30) {  // Future flickering
        //     byte_logic();
        // }

        draw_score(game_score, 5, 147);  // Draw score
        // flickering_byte++;
        oam_copy(oam_mem, obj_buffer, 128);  // Update sprites

        key_press = false;
    }
}
