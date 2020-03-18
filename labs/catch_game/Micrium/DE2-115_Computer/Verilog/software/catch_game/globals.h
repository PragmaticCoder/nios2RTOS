#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#undef NDEBUG

#include "address_map_nios2.h"
#include "debug.h"
#include "includes.h"
#include <stdio.h>

#define TRUE (1 == 1)
#define FALSE (!TRUE)

#define KEY0 0x1
#define KEY1 0x2
#define KEY2 0x4
#define KEY3 0x8

#define KEY_ptr (((volatile unsigned long*)KEY_BASE))

#define STANDARD_X 320
#define STANDARD_Y 240

#define INTEL_BLUE 0x0071C5
#define INTEL_RED 0xF6291B

#define GAMESCREEN_WIDTH 70

/* Global Variables */
INT8U err;

unsigned KEY_val;
int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

volatile int* video_resolution;
volatile int* rgb_status;

int screen_x;
int screen_y;

int res_offset;
int col_offset;

int pos_x;
int pos_y;

char text_disp[2];
char clear_text[2];
char clear_row_text[70];

int score;
int game_hh, game_mm, game_ss;

/* Semaphores */
OS_EVENT* SEM_read_KEYs;
OS_EVENT* SEM_game_timer;

#endif