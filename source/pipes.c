// pipes.c — fixed & refactored
// Manages the crazy pipe obstacles in Flappy Byte! Handles spawning, moving, and checking those tricky collisions.
// Pipes have openings with byte values — pick the right one or boom!

#include <tonc.h>
#include <string.h>
#include <stdlib.h>
#include "pipes.h"
#include "bytes.h"
#include "../graphics/game_pipes_topright.h"
#include "../graphics/game_pipes_topleft.h"
#include "../graphics/game_pipes_bottomright.h"
#include "../graphics/game_pipes_bottomleft.h"

extern OBJ_ATTR obj_buffer[128];  
extern bool byte_updated;         
extern bool pipe_passed;          
extern int game_frame_counter;  
extern unsigned char game_byte;   
extern unsigned char random_byte; 
extern const char* current_gate;  

int pipes_posX = 264;            
static const int RESET_X = 264;  
static const int OFFSCREEN_LEFT = -40;  
int pipe_speed = 1;             

// Sprite slots for showing byte values on pipes
#define PIPE_DISPLAY_BASE 36     
#define NUM_PIPES 4              

// The brain behind the byte values on pipes
static unsigned char pipe_correct_value;  
static unsigned char pipe_incorrect_value; 
static int display_y[2];        
static bool is_correct[2];      

// Pointers to each pipe sprite part (TL, TR, BL, BR)
static OBJ_ATTR *pipe_tl[NUM_PIPES];
static OBJ_ATTR *pipe_tr[NUM_PIPES];
static OBJ_ATTR *pipe_bl[NUM_PIPES];
static OBJ_ATTR *pipe_br[NUM_PIPES];

static int pipe_posY[NUM_PIPES] = {23, 23, 23, 23};


static void draw_pipe_values(int posX) { // Draw the correct/decoy bytes on the pipes at given X
    // Slot 0: Correct or decoy based on flag
    unsigned char value0 = is_correct[0] ? pipe_correct_value : pipe_incorrect_value;
    draw_byte_bits(value0, posX, display_y[0], PIPE_DISPLAY_BASE);

    // Slot 1: The other one
    unsigned char value1 = is_correct[1] ? pipe_correct_value : pipe_incorrect_value;
    draw_byte_bits(value1, posX, display_y[1], PIPE_DISPLAY_BASE + 8);
}

// Wipe the byte displays clean
static void clear_pipe_values() {
    clear_byte_bits(0, 0, PIPE_DISPLAY_BASE);      // Bye-bye first byte
    clear_byte_bits(0, 0, PIPE_DISPLAY_BASE + 8);  // And the second
}

// Set up a single pipe sprite piece
static void set_single_piece(OBJ_ATTR *o, int x, int y, u16 palbank, u16 tile_index) {
    obj_set_attr(o,
        ATTR0_SQUARE | ATTR0_4BPP | (y & 0xFF),
        ATTR1_SIZE_8 | (x & 0x1FF),
        ATTR2_BUILD(tile_index, palbank, 0) | ATTR2_PRIO(0)
    );
}

// Position all four pieces of one pipe
static void set_pipe_sprites_for_pipe(int i, int posX, int posY) {
    set_single_piece(pipe_tl[i], posX, posY, 1, 1);       // Top-left corner
    set_single_piece(pipe_tr[i], posX+8, posY, 2, 2);     // Top-right
    set_single_piece(pipe_bl[i], posX, posY+8, 3, 3);     // Bottom-left
    set_single_piece(pipe_br[i], posX+8, posY+8, 4, 4);   // Bottom-right
}

// loads graphics and set pointers
void pipes_init() {
    // Load up those colorful palettes for each pipe part
    memcpy(&pal_obj_mem[16], game_pipes_topleftPal, 32);
    memcpy(&pal_obj_mem[32], game_pipes_toprightPal, 32);
    memcpy(&pal_obj_mem[48], game_pipes_bottomleftPal, 32);
    memcpy(&pal_obj_mem[64], game_pipes_bottomrightPal, 32);

    // Grab the tile data for drawing pipes
    memcpy(&tile_mem_obj[0][1], game_pipes_topleftTiles, game_pipes_topleftTilesLen);
    memcpy(&tile_mem_obj[0][2], game_pipes_toprightTiles, game_pipes_toprightTilesLen);
    memcpy(&tile_mem_obj[0][3], game_pipes_bottomleftTiles, game_pipes_bottomleftTilesLen);
    memcpy(&tile_mem_obj[0][4], game_pipes_bottomrightTiles, game_pipes_bottomrightTilesLen);

    // Point to the sprite slots for each pipe
    for(int i = 0; i < NUM_PIPES; i++) {
        int base = 1 + i * 4;  // Slots 1-16 for pipes
        pipe_tl[i] = &obj_buffer[base + 0];
        pipe_tr[i] = &obj_buffer[base + 1];
        pipe_bl[i] = &obj_buffer[base + 2];
        pipe_br[i] = &obj_buffer[base + 3];
    }

    pipes_posX = RESET_X;
    for(int i = 0; i < NUM_PIPES; i++) {
        set_pipe_sprites_for_pipe(i, pipes_posX, pipe_posY[i]);
    }
}

void calculate_pipe_values() {
    unsigned char correct = game_byte;  
    if (strcmp(current_gate, "NOT") == 0) {
        correct = ~game_byte;
    } else if (strcmp(current_gate, "AND") == 0) {
        correct &= random_byte;
    } else if (strcmp(current_gate, "OR") == 0) {
        correct |= random_byte;
    } else if (strcmp(current_gate, "XOR") == 0) {
        correct ^= random_byte;
    }
    pipe_correct_value = correct;
    pipe_incorrect_value = ~correct;  // Decoy is the inverse

    // Randomly pick which display gets the correct one
    bool correct_at_first = (rand() % 2) == 0;
    is_correct[0] = correct_at_first;
    is_correct[1] = !correct_at_first;
}

// Shift all pipe sprites left on X
static void update_all_pipes_attr1_X(int posX) {
    for(int i = 0; i < NUM_PIPES; i++) {
        pipe_tl[i]->attr1 = (pipe_tl[i]->attr1 & ~0x1FF) | (posX & 0x1FF);
        pipe_tr[i]->attr1 = (pipe_tr[i]->attr1 & ~0x1FF) | ((posX + 8) & 0x1FF);
        pipe_bl[i]->attr1 = (pipe_bl[i]->attr1 & ~0x1FF) | (posX & 0x1FF);
        pipe_br[i]->attr1 = (pipe_br[i]->attr1 & ~0x1FF) | ((posX + 8) & 0x1FF);
    }
}

// Update Y positions for one pipe
static void update_pipe_attr0_Y(int i) {
    int y = pipe_posY[i] & 0xFF;
    pipe_tl[i]->attr0 = (pipe_tl[i]->attr0 & ~0xFF) | y;
    pipe_tr[i]->attr0 = (pipe_tr[i]->attr0 & ~0xFF) | y;
    pipe_bl[i]->attr0 = (pipe_bl[i]->attr0 & ~0xFF) | ((y + 8) & 0xFF);
    pipe_br[i]->attr0 = (pipe_br[i]->attr0 & ~0xFF) | ((y + 8) & 0xFF);
}

// Randomize pipe layout — three fun configs
void pipe_randomizer(int type) {
    pipes_posX = RESET_X;  // Reset to start

    switch(type) {
        case 0: 
            pipe_posY[0] = 59;
            pipe_posY[1] = 75;
            pipe_posY[2] = 91;
            pipe_posY[3] = 107;
            display_y[0] = 23;      // Byte at first gap top
            display_y[1] = 123;     // Byte at second gap top
            break;

        case 1:  
            pipe_posY[0] = 59;
            pipe_posY[1] = 111;
            pipe_posY[2] = 127;
            pipe_posY[3] = 143;
            display_y[0] = 23;      // Byte at first gap top
            display_y[1] = 75;      // Byte at second gap top
            break;

        case 2:  
        default:
            pipe_posY[0] = 23;
            pipe_posY[1] = 39;
            pipe_posY[2] = 55;
            pipe_posY[3] = 107;
            display_y[0] = 71;      // Byte at first gap top
            display_y[1] = 123;     // Byte at second gap top
            break;
    }

    for(int i = 0; i < NUM_PIPES; i++){
        update_pipe_attr0_Y(i);
    }

    update_all_pipes_attr1_X(pipes_posX);
}


// Spawn new pipes with random layout and fresh values
void random_pipes() {
    pipe_randomizer(rand() % 3);  // Pick a random config
    calculate_pipe_values();      // Compute the byte goodies
    draw_pipe_values(pipes_posX - 24);  // Draw 2 of them on the pipes
}

void pipes_update() {
    pipes_posX -= pipe_speed;
    update_all_pipes_attr1_X(pipes_posX);

    // Move the byte display sprites too
    int new_x = pipes_posX - 24;
    for(int j = 0; j < 16; j++) {
        obj_buffer[PIPE_DISPLAY_BASE + j].attr1 = (obj_buffer[PIPE_DISPLAY_BASE + j].attr1 & ~0x1FF) | ((new_x + (j % 8) * 8) & 0x1FF);
    }

    if(pipes_posX <= OFFSCREEN_LEFT){
        random_pipes();
        byte_updated = false;
    }
}

void reset_pipes(){
    pipes_posX = RESET_X;     
    pipe_speed = 1;           
    random_pipes();           
    update_all_pipes_attr1_X(pipes_posX);  
    clear_pipe_values();      
}

// Check if the ball hits pipes or flies through openings
int pipes_check_collision(int ball_x, int ball_y) {
    int ball_left = ball_x;
    int ball_right = ball_x + 8;
    int ball_top = ball_y;
    int ball_bottom = ball_y + 8;

    // Crash into pipe blocks? Game over!
    for(int i = 0; i < NUM_PIPES; i++) {
        int x0 = pipes_posX;
        int x1 = pipes_posX + 16;
        if ((ball_right > x0 && ball_left < x1 && ball_bottom > pipe_posY[i] && ball_top < pipe_posY[i] + 8) ||
            (ball_right > x0 && ball_left < x1 && ball_bottom > pipe_posY[i] + 8 && ball_top < pipe_posY[i] + 16)) {
            return 1;  // Ouch, hit!
        }
    }

    // Flying through the gaps? Check if correct or decoy
    int x0 = pipes_posX;
    int x1 = pipes_posX + 16;
    if (ball_right > x0 && ball_left < x1) {
        if (ball_bottom > display_y[0] && ball_top < display_y[0] + 36) {
            return is_correct[0] ? 2 : 3;  // Correct or wrong at first gap
        }
        if (ball_bottom > display_y[1] && ball_top < display_y[1] + 36) {
            return is_correct[1] ? 2 : 3;  // Correct or wrong at second gap
        }
    }
    return 0;  // All clear
}
