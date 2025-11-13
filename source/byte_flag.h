#ifndef BYTE_FLAG_H
#define BYTE_FLAG_H

#include <tonc.h>

extern unsigned char game_byte;

unsigned char generate_next_byte(void);
const char* generate_next_flag(void);
void byte_logic(void);
void draw_byte_bits(unsigned char value, int pixel_x, int pixel_y);

#endif // BYTE_FLAG_H
