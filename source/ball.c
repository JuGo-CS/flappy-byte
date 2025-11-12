#include <tonc.h>
#include <string.h>
#include <stdbool.h>
#include "ball.h"
#include "../graphics/game_ball.h"

#define BALL_X 25
#define GRAVITY 0.2
#define JUMP_VELOCITY -2

extern OBJ_ATTR obj_buffer[128]; // reference the global buffer
OBJ_ATTR *ball;

int ball_Y = 80;
float ball_velocity = 0;

void ball_init() {
    ball = &obj_buffer[0];   // first slot
    memcpy(pal_obj_mem, game_ballPal, game_ballPalLen);
    memcpy(&tile_mem_obj[0][0], game_ballTiles, game_ballTilesLen);

    obj_set_attr(ball,
        ATTR0_SQUARE | ATTR0_8BPP | (ball_Y & 0xFF),
        ATTR1_SIZE_8 | (BALL_X & 0x1FF),
        0);
}

void ball_update(bool press_jump) {
    if (press_jump)
        ball_velocity = JUMP_VELOCITY;

    // Boundary checks
    if (ball_Y + ball_velocity <= 23) {
        ball_Y = 23;
        ball_velocity = 0;
    }
    if (ball_Y + ball_velocity >= 150) { // 160-8 sprite
        ball_Y = 151;
        ball_velocity = 0;
    }

    ball_velocity += GRAVITY;
    ball_Y += ball_velocity;

    // Update sprite position
    ball->attr0 = (ball->attr0 & ~0xFF) | (ball_Y & 0xFF);
}



// #include <tonc.h>
// #include "ball.h"
// #include "../graphics/ball.h"


// #define BALL_X 50
// #define GRAVITY 1
// #define JUMP_VELOCITY -7

// OBJ_ATTR *ball;
// int ball_Y = 80;
// float ball_velocity = 0;

// int main() {
//     void ball_init() {
//         ball = &obj_buffer[0];
//         memcpy(pal_obj_mem, ballPal, ballPalLen);
//         memcpy(&tile_mem_obj[0][0], ballTiles, ballTilesLen);

//         obj_set_attr(ball,
//             ATTR0_SQUARE | ATTR0_8BPP | (ball_Y & 0xFF),
//             ATTR1_SIZE_8 | (BALL_X & 0x1FF),
//             0);
//     }

//     void ball_update(bool press_jump) {
//         if(press_jump){
//             ball_velocity = JUMP_VELOCITY;
//         }

//         if(ball_Y + ball_velocity <= 25){
//             ball_Y = 25;
//             ball_velocity = 0;
//         }

//         if(ball_Y + ball_velocity >= 151){
//             ball_Y = 232;
//             ball_velocity = 0;
//         }

//         ball_velocity += GRAVITY;
//         ball_Y += ball_velocity;
//         bird->attr0 = (bird->attr0 & ~0xFF) | (bird_Y & 0xFF);
//     }
// }