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
#define INTEL_LIGHT_YELLOW 0xFF8E3A

#define GAMESCREEN_WIDTH 70
#define GAMESCREEN_HEIGHT 60

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

int basket_pos_x;
int basket_pos_y;

int pos1_x;
int pos1_y;

int pos2_x;
int pos2_y;

int pos3_x;
int pos3_y;

int pos1_val;
int pos2_val;
int pos3_val;

char text_disp[1];
char clear_text[2];
char clear_row_text[70];

int score;
int game_hh, game_mm, game_ss;

/* Keyboard Press */
int left_key_pressed;
int right_key_pressed;
int esc_key_pressed;
int enter_key_pressed;
int any_key_pressed;

/* Semaphores */
OS_EVENT* SEM_read_KEYs;
OS_EVENT* SEM_game_timer;
OS_EVENT* SEM_KEY_press;

/* Game State */
typedef enum
{
  INIT,
  PLAY,
  PAUSE
} Game_State_t;

#endif