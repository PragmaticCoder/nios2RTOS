/*
 * utils.h
 *
 *  Created on: Feb 26, 2020
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
int
resample_rgb(int, int);
int
get_data_bits(int);
void
clear_all_chars();

#endif /* UTILS_H_ */
