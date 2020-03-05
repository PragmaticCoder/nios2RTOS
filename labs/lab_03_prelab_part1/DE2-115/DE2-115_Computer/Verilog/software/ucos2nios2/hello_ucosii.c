#include "globals.h"
#include "utils.h"
/* function prototypes */
void video_text(int, int, char *);
void video_box(int, int, int, int, short);
int  resample_rgb(int, int);
int  get_data_bits(int);

#define STANDARD_X 320
#define STANDARD_Y 240
#define INTEL_BLUE 0x0071C5
/* global variables */

int screen_x;
extern volatile int * video_resolution;
extern volatile int * rgb_status;

extern int screen_x;
extern int screen_y;
extern int res_offset;
extern int col_offset;

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task_disp_vga_char_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK_VGA_CHAR_PRIORITY 2

/* function prototypes */

/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{
  while (1)
  { 
    printf("Hello from task1\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}
/* Display Character using VGA Output */
void Task_VGA_char(void* pdata)
{
  while (1)
  { 
    printf("Hello from Task_VGA_char\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{

/* **************************** VGA Display Setup *************************** */

  video_resolution = (int *)(PIXEL_BUF_CTRL_BASE + 0x8);
  rgb_status = (int *)(RGB_RESAMPLER_BASE);

  screen_x                        = *video_resolution & 0xFFFF;
  screen_y                        = (*video_resolution >> 16) & 0xFFFF;

  int db = get_data_bits(*rgb_status & 0x3F);

  /* check if resolution is smaller than the standard 320 x 240 */
  res_offset = (screen_x == 160) ? 1 : 0;

  /* check if number of data bits is less than the standard 16-bits */
  col_offset = (db == 8) ? 1 : 0;

  /* create a message to be displayed on the video and LCD displays */
  char text_top_row[40]       = "Intel FPGA\0";
  char clear_text_top_row[40] = "          \0";

  char text_bottom_row[40]        = "Computer Systems\0";
  char clear_text_bottom_row[40]  = "                \0";

  /* update color */
  short background_color = resample_rgb(db, INTEL_BLUE);

  video_text(35, 29, clear_text_top_row);
  video_text(35, 29, text_top_row);

  video_text(32, 30, clear_text_bottom_row);
  video_text(32, 30, text_bottom_row);
    
  video_box(0, 0, STANDARD_X, STANDARD_Y, 0); // clear the screen
  video_box(31 * 4, 28 * 4, 49 * 4 - 1, 32 * 4 - 1, background_color);

/* ************************************************************************** */

  printf("MicroC/OS-II Licensing Terms\n");
  printf("============================\n");
  printf("Micrium\'s uC/OS-II is a real-time operating system (RTOS) available in source code.\n");
  printf("This is not open-source software.\n");
  printf("This RTOS can be used free of charge only for non-commercial purposes and academic projects,\n");
  printf("any other use of the code is subject to the terms of an end-user license agreement\n");
  printf("for more information please see the license files included in the BSP project or contact Micrium.\n");
  printf("Anyone planning to use a Micrium RTOS in a commercial product must purchase a commercial license\n");
  printf("from the owner of the software, Silicon Laboratories Inc.\n");
  printf("Licensing information is available at:\n");
  printf("Phone: +1 954-217-2036\n");
  printf("Email: sales@micrium.com\n");
  printf("URL: www.micrium.com\n\n\n");  
  
  

  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(Task_VGA_char,
                  NULL,
                  (void *)&task_disp_vga_char_stk[TASK_STACKSIZE-1],
                  TASK_VGA_CHAR_PRIORITY,
                  TASK_VGA_CHAR_PRIORITY,
                  task_disp_vga_char_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSStart();
  return 0;
}
