/*
 * utils.h
 *
 *  Created on: March 08, 2020
 *      Author: Alvi
 */

#ifndef UTILS_H_
#define UTILS_H_

/* Function Prototypes */
void
Check_KEYs(int*, int*, int*, int*);

void
VGA_text(int, int, char*);

void
VGA_box(int, int, int, int, short);

void
VGA_char_box(int, int, short);

int
resample_rgb(int, int);

int
get_data_bits(int);

void
VGA_animated_char(int x, int y, char* text_ptr, short char_bg_color);

void
VGA_info_right(int x, int y, char* text_ptr, short char_bg_color);

void
VGA_display_sidebar(short sidebar_color);

void
VGA_clear_all_characters(void);

void
VGA_clear_screen(void);

void
VGA_gametime_display(int hour, int minute, int second);

void
read_PS2_KeyboardInput(void);

void
VGA_clear_game_row(int pos_y);


#endif /* UTILS_H_ */
