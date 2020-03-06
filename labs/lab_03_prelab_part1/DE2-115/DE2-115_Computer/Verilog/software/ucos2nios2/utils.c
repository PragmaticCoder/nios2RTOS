/*
 * utils.c
 *
 *  Created on: Feb 26, 2020
 *      Author: Alvi
 */

#include "globals.h"

/* Global Variables*/
extern unsigned KEY_val;
extern int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

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
    KEY3_flag= 1;
  }

  if (KEY_val)
    debug("KEY_value: %d", KEY_val);
}

/*******************************************************************************
 * Subroutine to send a string of text to the video monitor
 ******************************************************************************/
void
video_text(int x, int y, char* text_ptr)
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

/*******************************************************************************
 * Draw a filled rectangle on the video monitor
 * Takes in points assuming 320x240 resolution and adjusts based on differences
 * in resolution and color bits.
 ******************************************************************************/
void
video_box(int x1, int y1, int x2, int y2, short pixel_color)
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
