// #include <tonc.h>
// #include <string.h>
// #include <stdlib.h>
// #include "pipes.h"
// #include "../graphics/game_pipes.h"

// extern OBJ_ATTR obj_buffer[128];
// OBJ_ATTR *pipe_1;
// OBJ_ATTR *pipe_2;
// OBJ_ATTR *pipe_3;
// OBJ_ATTR *pipe_4;

// int pipes_posX = 240;

// int pipe1_posY = 160;
// int pipe2_posY = 160;
// int pipe3_posY = 160;
// int pipe4_posY = 160;

// int pipe_speed = 1;
// int max_speed = 6;

// // Helper macro: tells GBA to use OBJ tile block N.
// #define TILEBLOCK(n) ((n)<<12)

// void pipes_init()
// {
//     // Load pipe palette into OBJ palette bank 1
//     memcpy(&pal_obj_mem[16], game_pipesPal, sizeof(game_pipesPal));

//     // Load pipe tiles into OBJ tile block 1
//     memcpy(&tile_mem_obj[1][0], game_pipesTiles, sizeof(game_pipesTiles));

//     // Assign sprite slots
//     pipe_1 = &obj_buffer[1];
//     pipe_2 = &obj_buffer[2];
//     pipe_3 = &obj_buffer[3];
//     pipe_4 = &obj_buffer[4];

//     // Common attributes for all pipes
//     obj_set_attr(pipe_1,
//         ATTR0_SQUARE | ATTR0_4BPP | (pipe1_posY & 0xFF),
//         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
//         ATTR2_ID(0) | ATTR2_PALBANK(1) | TILEBLOCK(1)
//     );

//     obj_set_attr(pipe_2,
//         ATTR0_SQUARE | ATTR0_4BPP | (pipe2_posY & 0xFF),
//         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
//         ATTR2_ID(0) | ATTR2_PALBANK(1) | TILEBLOCK(1)
//     );

//     obj_set_attr(pipe_3,
//         ATTR0_SQUARE | ATTR0_4BPP | (pipe3_posY & 0xFF),
//         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
//         ATTR2_ID(0) | ATTR2_PALBANK(1) | TILEBLOCK(1)
//     );

//     obj_set_attr(pipe_4,
//         ATTR0_SQUARE | ATTR0_4BPP | (pipe4_posY & 0xFF),
//         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
//         ATTR2_ID(0) | ATTR2_PALBANK(1) | TILEBLOCK(1)
//     );

//     pipes_reset();
// }


// // void pipes_init()
// // {
// //     // ------------------------------
// //     // Load pipe palette into OBJ palette bank 1
// //     // (ball uses bank 0)
// //     // ------------------------------
// //     memcpy(&pal_obj_mem[16], game_pipesPal, sizeof(game_pipesPal));

// //     // ------------------------------
// //     // Load pipe tiles into OBJ tile block 1
// //     // ------------------------------
// //     memcpy(&tile_mem_obj[1][0], game_pipesTiles, sizeof(game_pipesTiles));

// //     // Assign sprite slots
// //     pipe_1 = &obj_buffer[1];
// //     pipe_2 = &obj_buffer[2];
// //     pipe_3 = &obj_buffer[3];
// //     pipe_4 = &obj_buffer[4];

// //     // Common attributes for all pipes
// //     // u16 tile_id = ATTR2_ID(0) | ATTR2_PALBANK(1) | TILEBLOCK(1);

// //     // Ball:
// //     // palette slot = 0
// //     // tile index   = 0  ATTR2_BUILD(tile index, palette slot, 0)

// //     // Pipe:
// //     // palette slot = 1
// //     // tile index   = 32

// //     obj_set_attr(pipe_1,
// //         ATTR0_SQUARE | ATTR0_4BPP | (pipe1_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         ATTR2_BUILD(0, 1, 0)
// //     );

// //     obj_set_attr(pipe_2,
// //         ATTR0_SQUARE | ATTR0_4BPP | (pipe2_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         ATTR2_BUILD(0, 1, 0)
// //     );

// //     obj_set_attr(pipe_3,
// //         ATTR0_SQUARE | ATTR0_4BPP | (pipe3_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         ATTR2_BUILD(0, 1, 0)
// //     );

// //     obj_set_attr(pipe_4,
// //         ATTR0_SQUARE | ATTR0_4BPP | (pipe4_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         ATTR2_BUILD(0, 1, 0)
// //     );

// //     pipes_reset();
// // }

// void pipes_reset()
// {
//     pipes_posX = 240;
// }

// void pipe_randomizer(int type)
// {
//     pipes_posX = 240;

//     switch(type)
//     {
//         case 0:
//             pipe1_posY = 23;
//             pipe2_posY = 59;
//             pipe3_posY = 91;
//             pipe4_posY = 107;
//             break;

//         case 1:
//             pipe1_posY = 59;
//             pipe2_posY = 111;
//             pipe3_posY = 127;
//             pipe4_posY = 143;
//             break;

//         case 2:
//             pipe1_posY = 23;
//             pipe2_posY = 39;
//             pipe3_posY = 56;
//             pipe4_posY = 107;
//             break;
//     }

//     pipe_1->attr0 = (pipe_1->attr0 & ~0xFF) | (pipe1_posY & 0xFF);
//     pipe_2->attr0 = (pipe_2->attr0 & ~0xFF) | (pipe2_posY & 0xFF);
//     pipe_3->attr0 = (pipe_3->attr0 & ~0xFF) | (pipe3_posY & 0xFF);
//     pipe_4->attr0 = (pipe_4->attr0 & ~0xFF) | (pipe4_posY & 0xFF);
// }

// void random_pipes()
// {
//     pipe_randomizer(rand() % 3);
// }

// void pipes_update()
// {
//     pipes_posX -= pipe_speed;

//     pipe_1->attr1 = (pipe_1->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);
//     pipe_2->attr1 = (pipe_2->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);
//     pipe_3->attr1 = (pipe_3->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);
//     pipe_4->attr1 = (pipe_4->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);

//     if(pipes_posX <= -16)
//         pipes_reset();
// }



// // #include <tonc.h>
// // #include <string.h>
// // #include <stdlib.h>
// // #include "pipes.h"
// // #include "../graphics/game_pipes.h"


// // extern OBJ_ATTR obj_buffer[128];
// // OBJ_ATTR pipe_1*;
// // OBJ_ATTR pipe_2*;
// // OBJ_ATTR pipe_3*;
// // OBJ_ATTR pipe_4*;


// // const int pipes_posX = 240;

// // const int pipe1_posY = 160;
// // const int pipe2_posY = 160;
// // const int pipe3_posY = 160;
// // const int pipe4_posY = 160;

// // int pipe_speed = 1;
// // int max_speed = 6;


// // void pipes_init(){
// //     pipe_1 = &obj_buffer[1];
// //     memcpy(pal_obj_mem, pipesPal, pipesPalLen);
// //     memcpy(&tile_mem_obj[1][0], pipesTiles, pipesTilesLen);

// //     obj_set_attr(pipe_1,
// //         ATTR0_SQUARE | ATTR0_8BPP | (pipe1_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         0);


// //     pipe_2 = &obj_buffer[2];
// //     memcpy(pal_obj_mem, pipesPal, pipesPalLen);
// //     memcpy(&tile_mem_obj[2][0], pipesTiles, pipesTilesLen);

// //     obj_set_attr(pipe_2,
// //         ATTR0_SQUARE | ATTR0_8BPP | (pipe2_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         0);


// //     pipe_3 = &obj_buffer[3];
// //     memcpy(pal_obj_mem, pipesPal, pipesPalLen);
// //     memcpy(&tile_mem_obj[3][0], pipesTiles, pipesTilesLen);

// //     obj_set_attr(pipe_3,
// //         ATTR0_SQUARE | ATTR0_8BPP | (pipe3_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         0);


// //     pipe_4 = &obj_buffer[4];
// //     memcpy(pal_obj_mem, pipesPal, pipesPalLen);
// //     memcpy(&tile_mem_obj[4][0], pipesTiles, pipesTilesLen);

// //     obj_set_attr(pipe_4,
// //         ATTR0_SQUARE | ATTR0_8BPP | (pipe4_posY & 0xFF),
// //         ATTR1_SIZE_16 | (pipes_posX & 0x1FF),
// //         0);
// // }




// // void pipe_randomizer(int type){
// //     pipes_posX = 240;

// //     switch(type) {

// //         case 0:
// //             pipe1_posY = 23;
// //             pipe1->attr0 = (pipe1->attr0 & ~0xFF) | (pipe1_posY & 0xFF);

// //             pipe2_posY = 59;
// //             pipe2->attr0 = (pipe2->attr0 & ~0xFF) | (pipe2_posY & 0xFF);

// //             pipe3_posY = 91;
// //             pipe3->attr0 = (pipe3->attr0 & ~0xFF) | (pipe3_posY & 0xFF);

// //             pipe4_posY = 107;
// //             pipe4->attr0 = (pipe4->attr0 & ~0xFF) | (pipe4_posY & 0xFF);

// //             break;

// //         case 1:
// //             pipe1_posY = 59;
// //             pipe1->attr0 = (pipe1->attr0 & ~0xFF) | (pipe1_posY & 0xFF);

// //             pipe2_posY = 111;
// //             pipe2->attr0 = (pipe2->attr0 & ~0xFF) | (pipe2_posY & 0xFF);

// //             pipe3_posY = 127;
// //             pipe3->attr0 = (pipe3->attr0 & ~0xFF) | (pipe3_posY & 0xFF);

// //             pipe4_posY = 143;
// //             pipe4->attr0 = (pipe4->attr0 & ~0xFF) | (pipe4_posY & 0xFF);
            

// //             break;

// //         case 2:
// //             pipe1_posY = 23;
// //             pipe1->attr0 = (pipe1->attr0 & ~0xFF) | (pipe1_posY & 0xFF);

// //             pipe2_posY = 39;
// //             pipe2->attr0 = (pipe2->attr0 & ~0xFF) | (pipe2_posY & 0xFF);

// //             pipe3_posY = 56;
// //             pipe3->attr0 = (pipe3->attr0 & ~0xFF) | (pipe3_posY & 0xFF);

// //             pipe4_posY = 107;
// //             pipe4->attr0 = (pipe4->attr0 & ~0xFF) | (pipe4_posY & 0xFF);

// //             break;
// //     }
// // }

// // void random_pipes(){
// //     pipe_randomizer(rand() % 3);
// // }

// // void pipes_update(){
// //     pipes_posX -= 1;
// //     pipe1->attr1 = (pipe1->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);
// //     pipe2->attr1 = (pipe2->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);
// //     pipe3->attr1 = (pipe3->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);
// //     pipe4->attr1 = (pipe4->attr1 & ~0x1FF) | (pipes_posX & 0x1FF);


// // }