#include "globals.h"
#include "utils.h"

/* global variables */

extern volatile int* video_resolution;
extern volatile int* rgb_status;

extern int screen_x;
extern int screen_y;
extern int res_offset;
extern int col_offset;
extern int pos_x;
extern int pos_y;

extern unsigned KEY_val;
extern int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

extern char text_disp[2] = "C\0";
extern char clear_text[2] = " \0";
extern char clear_row_text[81] =
  "                                                  "
  "                              \0";

short background_color;

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048
OS_STK task_key_press_stk[TASK_STACKSIZE];
OS_STK task_disp_vga_char_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */
#define TASK_KEY_PRESS_PRIORITY 1
#define TASK_VGA_CHAR_PRIORITY 2

/* Prints "Hello World" and sleeps for three seconds */
void
Task_read_KEYs(void* pdata)
{
  debug("Started: Task_read_KEYs");
  KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag = 0, 0, 0, 0;

  for (;;) {
    OSSemPend(SEM_read_KEYs, 0, &err);

    debug("%u: \tHello from Task_read_KEYs", OSTime);
    Check_KEYs(&KEY0_flag, &KEY1_flag, &KEY2_flag, &KEY3_flag);

    if (KEY0_flag) {
      debug("MOVE RIGHT");
      ++pos_x;
      KEY0_flag = 0;
    }

    if (KEY1_flag) {
      debug("MOVE DOWN");
      ++pos_y;
      KEY1_flag = 0;
    }

    if (KEY2_flag) {
      debug("MOVE UP");
      --pos_y;
      KEY2_flag = 0;
    }

    if (KEY3_flag) {
      debug("MOVE LEFT");
      --pos_x;
      KEY3_flag = 0;
    }

    OSSemPost(SEM_read_KEYs);
    OSTimeDly(1);
  }
}

/* Display Character using VGA Output */
void
Task_VGA_char(void* pdata)
{
  debug("Started: Task_VGA_char");

  for (;;) {
    debug(
      "%u: \tTask_VGA_char: (pos_x, pos_y) = (%d, %d)", OSTime, pos_x, pos_y);
    VGA_animated_char(pos_x, pos_y, text_disp, background_color);

    OSTimeDly(1);
  }
}

/* The main function creates two task and starts multi-tasking */
int
main(void)
{

  /* ***************************** Initialization *****************************
   */

  KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag = 0, 0, 0, 0;

  /* ************************ Semaphores Initialization ***********************
   */

  SEM_read_KEYs = OSSemCreate(1);

  /* **************************** VGA Display Setup ***************************
   */

  video_resolution = (int*)(PIXEL_BUF_CTRL_BASE + 0x8);
  rgb_status = (int*)(RGB_RESAMPLER_BASE);

  screen_x = *video_resolution & 0xFFFF;
  screen_y = (*video_resolution >> 16) & 0xFFFF;

  /* letter initially positioned at the centre of screen */
  pos_x = 40;
  pos_y = 30;

  int db = get_data_bits(*rgb_status & 0x3F);

  /* check if resolution is smaller than the standard 320 x 240 */
  res_offset = (screen_x == 160) ? 1 : 0;

  /* check if number of data bits is less than the standard 16-bits */
  col_offset = (db == 8) ? 1 : 0;

  /* update color */
  background_color = resample_rgb(db, INTEL_BLUE);
  VGA_animated_char(pos_x, pos_y, text_disp, background_color);

  /* **************************************************************************
   */

  printf("MicroC/OS-II Licensing Terms\n");
  printf("============================\n");

  printf("Micrium\'s uC/OS-II is a real-time operating system (RTOS) available "
         "in source code.\n");

  printf("This is not open-source software.\n");

  printf("This RTOS can be used free of charge only for non-commercial "
         "purposes and academic projects,\n");

  printf("any other use of the code is subject to the terms of an end-user "
         "license agreement\n");

  printf("for more information please see the license files included in the "
         "BSP project or contact Micrium.\n");

  printf("Anyone planning to use a Micrium RTOS in a commercial product must "
         "purchase a commercial license\n");

  printf("from the owner of the software, Silicon Laboratories Inc.\n");

  printf("Licensing information is available at:\n");

  printf("Phone: +1 954-217-2036\n");

  printf("Email: sales@micrium.com\n");

  printf("URL: www.micrium.com\n\n\n");

  OSTaskCreateExt(Task_read_KEYs,
                  NULL,
                  (void*)&task_key_press_stk[TASK_STACKSIZE - 1],
                  TASK_KEY_PRESS_PRIORITY,
                  TASK_KEY_PRESS_PRIORITY,
                  task_key_press_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_VGA_char,
                  NULL,
                  (void*)&task_disp_vga_char_stk[TASK_STACKSIZE - 1],
                  TASK_VGA_CHAR_PRIORITY,
                  TASK_VGA_CHAR_PRIORITY,
                  task_disp_vga_char_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSStart();
  return 0;
}
