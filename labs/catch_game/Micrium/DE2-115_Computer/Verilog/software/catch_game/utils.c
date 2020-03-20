/*
 * utils.c
 *
 *  Created on: March 12, 2020
 *      Author: Alvi
 */

#include "globals.h"

/* Global Variables*/
extern unsigned KEY_val;
extern int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;
extern int left_key_pressed;
extern int right_key_pressed;
extern int esc_key_pressed;
extern int enter_key_pressed;

/* ************************************************************************** */
/*                                State Tracker                               */
/* ************************************************************************** */
const char*
get_State_name(Game_State_t game_state)
{
  switch (game_state) {
    case INIT:
      return "INIT";
    case PLAY:
      return "PLAY";
    case PAUSE:
      return "PAUSE";
    default:
      return "INVALID";
  }
}

/* ************************************************************************** */
/*                                Key Detection                               */
/* ************************************************************************** */
void
Check_KEYs(int* KEY0_ptr, int* KEY1_ptr, int* KEY2_ptr, int* KEY3_ptr)
{

  KEY_val = *(KEY_ptr);

  if (KEY_val == KEY0) {
    debug("KEY0 Pressed!"); // check KEY0
    KEY0_flag = 1;
  } else if (KEY_val == KEY1) // check KEY1
  {
    debug("KEY1 Pressed!");
    KEY1_flag = 1;
  } else if (KEY_val == KEY2) // check KEY2
  {
    debug("KEY2 Pressed!");
    KEY2_flag = 1;
  } else if (KEY_val == KEY3) // check KEY3
  {
    debug("KEY3 Pressed!");
    KEY3_flag = 1;
  }

  if (KEY_val)
    debug("KEY_value: %d", KEY_val);
}

/*******************************************************************************
 * Subroutine to send a string of text to the video monitor
 ******************************************************************************/
void
VGA_text(int x, int y, char* text_ptr)
{
  int offset;
  volatile char* char_buf = (char*)FPGA_CHAR_BASE; // video character buffer

  /* assume that the text string fits on one line */
  offset = (y << 7) + x;
  while (*(text_ptr)) {
    *(char_buf + offset) = *(text_ptr); // write to the character buffer
    ++text_ptr;
    ++offset;
  }
}

/* ************************************************************************** */
/*        Draw a filled rectangle on the video monitor                        */
/*        Takes in points assuming 320x240 resolution and adjusts based on    */
/*        differences in resolution and color bits.                           */
/* ************************************************************************** */
void
VGA_box(int x1, int y1, int x2, int y2, short pixel_color)
{
  int pixel_buf_ptr = *(int*)PIXEL_BUF_CTRL_BASE;
  int pixel_ptr, row, col;

  int x_factor = 0x1 << (res_offset + col_offset);
  int y_factor = 0x1 << (res_offset);

  x1 = x1 / x_factor;
  x2 = x2 / x_factor;
  y1 = y1 / y_factor;
  y2 = y2 / y_factor;

  /* assume that the box coordinates are valid */
  for (row = y1; row <= y2; row++)
    for (col = x1; col <= x2; ++col) {
      pixel_ptr =
        pixel_buf_ptr + (row << (10 - res_offset - col_offset)) + (col << 1);
      *(short*)pixel_ptr = pixel_color; // set pixel color
    }
}

/* ************************************************************************** */
/*              Draw a rectangle box that fills up the character.             */
/*              Character to Pixel mapping: 1 character = 4 pixels            */
/* ************************************************************************** */

void
VGA_char_box(int x, int y, short color)
{
  int x1 = x * 4;
  int y1 = y * 4;
  int x2 = ((x + 1) * 4) - 1;
  int y2 = ((y + 1) * 4) - 1;

  VGA_box(x1, y1, x2, y2, color);
}

/* ************************************************************************** */
/*         Synced Animated Character embedded on top of character box         */
/* ************************************************************************** */
void
VGA_animated_char(int x, int y, char* text_ptr, short char_bg_color)
{
  VGA_char_box(x, y, char_bg_color);
  VGA_text(x, y, text_ptr);
}

/* ************************************************************************** */
/*            Display Character and Pixel Configuration for Sidebar           */
/* ************************************************************************** */
void
VGA_info_right(int x, int y, char* text_ptr, short char_bg_color)
{
  int i = 0;
  for (i = x; i < x + 10; i++)
    VGA_char_box(i, y, char_bg_color);

  VGA_text(x, y, text_ptr);
}

/* ************************************************************************** */
/*               Resamples 24-bit color to 16-bit or 8-bit color              */
/* ************************************************************************** */
int
resample_rgb(int num_bits, int color)
{
  if (num_bits == 8) {

    color = (((color >> 16) & 0x000000E0) | ((color >> 11) & 0x0000001C) |
             ((color >> 6) & 0x00000003));

    color = (color << 8) | color;

  } else if (num_bits == 16) {

    color = (((color >> 8) & 0x0000F800) | ((color >> 5) & 0x000007E0) |
             ((color >> 3) & 0x0000001F));
  }

  return color;
}

/* ************************************************************************** */
/*                             Clear game screen                              */
/* ************************************************************************** */

void
VGA_clear_screen(void)
{
  debug("Clearing the screen");
  VGA_box(0, 0, STANDARD_X, STANDARD_Y, 0); /* clear the screen */

  /* clearing all characters from the screen */
  for (int i = 0; i < 60; i++)
    VGA_text(0, i, clear_row_text);
}

/* ************************************************************************** */
/*                            Clear Game Screen Row                           */
/* ************************************************************************** */
void
VGA_clear_game_row(int pos_y)
{

  for (int i = 0; i < 70; i++)
    VGA_char_box(i, pos_y, 0);

  VGA_text(0, pos_y, clear_row_text);
}

/* ************************************************************************** */
/*                 Finds the number of data bits from the mode                */
/* ************************************************************************** */

int
get_data_bits(int mode)
{
  switch (mode) {
    case 0x0:
      return 1;
    case 0x7:
      return 8;
    case 0x11:
      return 8;
    case 0x12:
      return 9;
    case 0x14:
      return 16;
    case 0x17:
      return 24;
    case 0x19:
      return 30;
    case 0x31:
      return 8;
    case 0x32:
      return 12;
    case 0x33:
      return 16;
    case 0x37:
      return 32;
    case 0x39:
      return 40;
  }
}

/* ************************************************************************** */
/*                            Display Right Sidebar                           */
/* ************************************************************************** */

void
VGA_display_sidebar(short sidebar_color)
{
  VGA_info_right(70, 0, "          ", sidebar_color);
  VGA_info_right(70, 1, " Catch the", sidebar_color);
  VGA_info_right(70, 2, " Numbers! ", sidebar_color);
  VGA_info_right(70, 3, "          ", sidebar_color);
  VGA_info_right(70, 4, "          ", sidebar_color);
  VGA_info_right(70, 5, " Score:   ", sidebar_color);
  VGA_info_right(70, 6, "      000 ", sidebar_color);
  VGA_info_right(70, 7, "          ", sidebar_color);
  VGA_info_right(70, 8, " Time:    ", sidebar_color);
  VGA_info_right(70, 9, " 00:00:00 ", sidebar_color);
  VGA_info_right(70, 10, "          ", sidebar_color);
  VGA_info_right(70, 11, "          ", sidebar_color);
  VGA_info_right(70, 12, "          ", sidebar_color);
  VGA_info_right(70, 13, "          ", sidebar_color);
  VGA_info_right(70, 14, "          ", sidebar_color);
  VGA_info_right(70, 15, "          ", sidebar_color);
  VGA_info_right(70, 16, "          ", sidebar_color);
  VGA_info_right(70, 17, "          ", sidebar_color);
  VGA_info_right(70, 18, "          ", sidebar_color);
  VGA_info_right(70, 19, "          ", sidebar_color);
  VGA_info_right(70, 20, "          ", sidebar_color);
  VGA_info_right(70, 21, "          ", sidebar_color);
  VGA_info_right(70, 22, "          ", sidebar_color);
  VGA_info_right(70, 23, "          ", sidebar_color);
  VGA_info_right(70, 24, "          ", sidebar_color);
  VGA_info_right(70, 25, "          ", sidebar_color);
  VGA_info_right(70, 26, "          ", sidebar_color);
  VGA_info_right(70, 27, "          ", sidebar_color);
  VGA_info_right(70, 28, "          ", sidebar_color);
  VGA_info_right(70, 29, "          ", sidebar_color);
  VGA_info_right(70, 30, "          ", sidebar_color);
  VGA_info_right(70, 31, "          ", sidebar_color);
  VGA_info_right(70, 32, "          ", sidebar_color);
  VGA_info_right(70, 33, "          ", sidebar_color);
  VGA_info_right(70, 34, "          ", sidebar_color);
  VGA_info_right(70, 35, "          ", sidebar_color);
  VGA_info_right(70, 36, "          ", sidebar_color);
  VGA_info_right(70, 37, "          ", sidebar_color);
  VGA_info_right(70, 38, "          ", sidebar_color);
  VGA_info_right(70, 39, "          ", sidebar_color);
  VGA_info_right(70, 40, "          ", sidebar_color);
  VGA_info_right(70, 41, "          ", sidebar_color);
  VGA_info_right(70, 42, "          ", sidebar_color);
  VGA_info_right(70, 43, "          ", sidebar_color);
  VGA_info_right(70, 44, "          ", sidebar_color);
  VGA_info_right(70, 45, "          ", sidebar_color);
  VGA_info_right(70, 46, "          ", sidebar_color);
  VGA_info_right(70, 47, "          ", sidebar_color);
  VGA_info_right(70, 48, "          ", sidebar_color);
  VGA_info_right(70, 49, "          ", sidebar_color);
  VGA_info_right(70, 50, "          ", sidebar_color);
  VGA_info_right(70, 51, "          ", sidebar_color);
  VGA_info_right(70, 52, "          ", sidebar_color);
  VGA_info_right(70, 53, "          ", sidebar_color);
  VGA_info_right(70, 54, "          ", sidebar_color);
  VGA_info_right(70, 55, "          ", sidebar_color);
  VGA_info_right(70, 56, " Press    ", sidebar_color);
  VGA_info_right(70, 57, " 'ESC'    ", sidebar_color);
  VGA_info_right(70, 58, " to Pause ", sidebar_color);
  VGA_info_right(70, 59, "          ", sidebar_color);
}

/* ************************************************************************** */
/*                              Display Game Time                             */
/* ************************************************************************** */
void
VGA_gametime_display(int hour, int minute, int second)
{
  int cx;
  char buffer[10];

  cx = snprintf(buffer, 10, " %02d:%02d:%02d\0", game_hh, game_mm, game_ss);
  check(cx >= 0, "cx out of range");

  VGA_text(70, 9, buffer);
  return;

error:
  log_err("Error caused while formatting string");
  log_err("buffer: %s", buffer);
}

/* ************************************************************************** */
/*                             Display Game Score                             */
/* ************************************************************************** */
void
VGA_display_score(int score)
{
  char buffer[10];

  int cx;
  cx = snprintf(buffer, 10, "      %03d\0", score);

  debug("buffer: %s", buffer);
  check(cx >= 0, "cx out of range");

  VGA_text(70, 6, buffer);
  return;

error:
  log_err("Error caused while formatting string");
  log_err("buffer: %s", buffer);
}

/* ************************************************************************** */
/*                           Read PS2 Keyboard Input                          */
/* ************************************************************************** */
void
read_PS2_KeyboardInput(void)
{
  volatile int* PS2_ptr = (int*)PS2_BASE;

  int PS2_data, RVALID;
  unsigned char byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0, byte5 = 0;

  *(PS2_ptr) = 0xFF; // reset PS/2

  while (1) {
    PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    RVALID = (PS2_data & 0xFFFF0000) >> 16; // extract the RVALID field

    if (RVALID > 0) {
      /* shift the next data byte into the display */
      byte1 = byte2;
      byte2 = byte3;
      byte3 = byte4;
      byte4 = byte5;
      byte5 = PS2_data & 0xFF;

      debug("byte1: 0x%02x | byte2: 0x%02x | byte3: 0x%02x | byte4: 0x%02x | "
            "byte5: 0x%02x",
            byte1 & 0xFF,
            byte2 & 0xFF,
            byte3 & 0xFF,
            byte4 & 0xFF,
            byte5 & 0xFF);

      if (byte4 == 0xE0 && byte5 == 0x6B)
        left_key_pressed = 1;

      if (byte3 == 0xE0 && byte4 == 0xF0 && byte5 == 0x6B)
        left_key_pressed = 0;

      if (byte4 == 0xE0 && byte5 == 0x74)
        right_key_pressed = 1;

      if (byte3 == 0xE0 && byte4 == 0xF0 && byte5 == 0x74)
        right_key_pressed = 0;

      if (byte5 == 0x76)
        esc_key_pressed = 1;

      if (byte4 == 0xF0 && byte5 == 0x76)
        esc_key_pressed = 0;

      if (byte5 == 0x5A)
        enter_key_pressed = 1;

      if (byte4 == 0xF0 && byte5 == 0x5A)
        enter_key_pressed = 0;

      debug("left_key_pressed: %d | right_key_pressed: %d | esc_key_pressed: "
            "%d | enter_key_pressed: %d",
            left_key_pressed,
            right_key_pressed,
            esc_key_pressed,
            enter_key_pressed);
    } else {
      left_key_pressed, right_key_pressed, esc_key_pressed,
        enter_key_pressed = 0, 0, 0, 0;
    }
  }
}
