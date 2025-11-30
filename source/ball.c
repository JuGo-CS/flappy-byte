// ball.c - Handles ball sprite setup, physics, and reset.

#include <tonc.h>
#include <string.h>
#include <stdbool.h>
#include "ball.h"
#include "bytes.h"
#include "../graphics/game_ball.h"

// Ball constants
#define BALL_X 25              // Fixed horizontal position
#define GRAVITY 0.2            // Downward acceleration
#define JUMP_VELOCITY -2       // Upward jump speed

extern OBJ_ATTR obj_buffer[128];  // Global sprite buffer
OBJ_ATTR *ball;                   // Pointer to ball sprite

int ball_Y = 80;                 // Current vertical position
float ball_velocity = 0;         // Current velocity

// Initialize ball sprite
void ball_init() {
    ball = &obj_buffer[0];  // Assign first OAM slot

    memcpy(&pal_obj_mem[0], game_ballPal, 32);  // Load palette
    memcpy(&tile_mem_obj[0][0], game_ballTiles, game_ballTilesLen);  // Load tiles

    obj_set_attr(ball,
        ATTR0_SQUARE | ATTR0_4BPP | (ball_Y & 0xFF),  // 8x8 sprite at Y
        ATTR1_SIZE_8 | (BALL_X & 0x1FF),             // Size and X position
        ATTR2_BUILD(0, 0, 0)                        // Tile and palette
    );
}

// Update ball position and physics
void ball_update(bool press_jump) {
    if (press_jump)  // Jump on input
        ball_velocity = JUMP_VELOCITY;

    // Prevent going above top boundary
    if (ball_Y + ball_velocity <= 23) {
        ball_Y = 23;
        ball_velocity = 0;
    }
    // Prevent going below bottom boundary
    if (ball_Y + ball_velocity >= 150) {
        ball_Y = 151;
        ball_velocity = 0;
    }

    ball_velocity += GRAVITY;  // Apply gravity
    ball_Y += ball_velocity;   // Update position

    ball->attr0 = (ball->attr0 & ~0xFF) | (ball_Y & 0xFF);  // Update sprite Y
}

// Reset ball to start position
void reset_ball(){
    ball_Y = 80;
    ball_velocity = 0;
    ball->attr0 = (ball->attr0 & ~0xFF) | (ball_Y & 0xFF);  // Reset sprite Y
}

// void updating_score(){  // Placeholder for future score updates
// }
