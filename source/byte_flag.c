#include <tonc.h>
#include <stdlib.h>
#include <string.h>
#include "byte_flag.h"

unsigned char game_byte = 0;

// Array of logic gates
const char* logic_gates[] = {"AND","OR","XOR","NOT"};

// ------------------------------------------------------------------
// Generate a random byte (8 bits)
// ------------------------------------------------------------------
unsigned char generate_next_byte(void)
{
    unsigned char result = 0;
    for(int i=0;i<8;i++)
    {
        result <<= 1;
        result |= rand() & 1;
    }
    return result;
}

// ------------------------------------------------------------------
// Pick a random logic gate
// ------------------------------------------------------------------
const char* generate_next_flag(void)
{
    int count = sizeof(logic_gates)/sizeof(logic_gates[0]);
    return logic_gates[rand() % count];
}

// ------------------------------------------------------------------
// Draw a byte as 8-bit string on BG1 at exact pixel location
// pixel_x, pixel_y = top-left of first bit
// ------------------------------------------------------------------
void draw_byte_bits(unsigned char value, int pixel_x, int pixel_y)
{
    char bits[9];
    for(int i=0;i<8;i++)
        bits[i] = (value & (1 << (7-i))) ? '1' : '0';
    bits[8] = '\0';

    char buffer[32];
    sprintf(buffer, "#{P:%d,%d}%s", pixel_x, pixel_y, bits);
    tte_write(buffer);
}

// ------------------------------------------------------------------
// Apply logic gate to game_byte with random_byte and draw both
// ------------------------------------------------------------------
void byte_logic(void)
{
    unsigned char random_byte = generate_next_byte();
    const char* gate = generate_next_flag();

    // Apply the logic
    if(strcmp(gate,"AND")==0)       game_byte &= random_byte;
    else if(strcmp(gate,"OR")==0)   game_byte |= random_byte;
    else if(strcmp(gate,"XOR")==0)  game_byte ^= random_byte;
    else if(strcmp(gate,"NOT")==0)  game_byte = ~game_byte;

    // Draw both bytes as bits on BG1
    // Convert pixel positions to exact top-left coordinates
    draw_byte_bits(game_byte, 30, 12);

    draw_byte_bits(random_byte, 150, 12);
}






// #include <string.h>
// #include <stdlib.h> // Required for rand(), srand()
// #include <time.h>   // Required for time()
// #include <stdio.h>  // Required for printf()

// //starts with 12 ends with 19
// //Y: first 3-117, 124-237
// //
// unsigned char game_byte = 0;

// unsigned char generate_next_byte() {
//     unsigned char return_byte = 0;

//     for (int counter = 0; counter < 8; counter++) {
//         int random_bit = rand() % 2;
//         return_byte <<= 1;
//         return_byte |= random_bit;
//     }
    
//     printf("Generated random byte (bits): ");
//     // Call our new helper function to display the byte in binary format
//     print_byte_as_bits(return_byte);

//     return return_byte; // Return the actual byte value
// }

// const char* logic_gates[] = {"AND", "OR", "XOR", "NOT"};

// const char* generate_next_flag() {
//     int num_gates = sizeof(logic_gates) / sizeof(logic_gates[0]);
//     int logic_gate_randomizer = rand() % num_gates;

//     return logic_gates[logic_gate_randomizer];
// }

// int byte_logic(){
//     const char* flag_generated = generate_next_flag();

// }
