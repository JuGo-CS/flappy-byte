#ifndef BYTES_H
#define BYTES_H

#include <tonc.h>
#include <stdbool.h>

// void bytes_init();
// void bytes_draw_u16(u16 value);
extern unsigned char game_byte;
extern unsigned char random_byte;
extern const char* current_gate;
extern const char* gate;



void byte_logic(void);
unsigned char generate_next_byte(void);
void draw_byte_bits(unsigned char value, int pixel_x, int pixel_y);
void clear_byte_bits(int pixel_x, int pixel_y);
void draw_gate(const char* gate, int y);
void clear_gate(void);
const char* generate_next_flag(void);
void apply_byte_logic(void);



#endif
