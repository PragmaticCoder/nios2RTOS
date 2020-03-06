#undef NDEBUG

#include "globals.h"
#include "utils.h"

#define STANDARD_X 320
#define STANDARD_Y 240
#define INTEL_BLUE 0x0071C5
/* global variables */

int screen_x;
extern volatile int* video_resolution;
extern volatile int* rgb_status;

extern int screen_x;
extern int screen_y;
extern int res_offset;
extern int col_offset;

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

  for (;;) {
    debug("%u: \tHello from Task_read_KEYs", OSTime);
    OSTimeDly(1);
  }
}

/* Display Character using VGA Output */
void
Task_VGA_char(void* pdata)
{
  debug("Started: Task_VGA_char");

  for (;;) {
    debug("%u: \tHello from Task_VGA_char", OSTime);
    OSTimeDly(1);
  }
}

/* The main function creates two task and starts multi-tasking */
int
main(void)
{

  /* ************************ Semaphores Initialization ***********************
   */

  SEM_read_KEYs = OSSemCreate(1);

  /* **************************** VGA Display Setup ***************************
   */

  video_resolution = (int*)(PIXEL_BUF_CTRL_BASE + 0x8);
  rgb_status = (int*)(RGB_RESAMPLER_BASE);

  screen_x = *video_resolution & 0xFFFF;
  screen_y = (*video_resolution >> 16) & 0xFFFF;

  int db = get_data_bits(*rgb_status & 0x3F);

  /* check if resolution is smaller than the standard 320 x 240 */
  res_offset = (screen_x == 160) ? 1 : 0;

  /* check if number of data bits is less than the standard 16-bits */
  col_offset = (db == 8) ? 1 : 0;

  /* create a message to be displayed on the video and LCD displays */
  char text_disp[2] = "C\0";
  char clear_text_row[40] = "                                      \0";

  /* update color */
  short background_color = resample_rgb(db, INTEL_BLUE);

  video_text(40, 30, clear_text_row);
  video_text(40, 30, text_disp);

  video_box(0, 0, STANDARD_X, STANDARD_Y, 0); // clear the screen
  // video_box(31 * 4, 28 * 4, 49 * 4 - 1, 32 * 4 - 1, background_color);

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
