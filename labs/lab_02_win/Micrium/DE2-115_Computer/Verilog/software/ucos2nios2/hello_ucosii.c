/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/

#include <stdio.h>
#include "includes.h"
#include "address_map_nios2.h"

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048

OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
OS_STK task_read_keyboard_stk[TASK_STACKSIZE];
OS_STK task_read_KEY_press_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK_READ_KEYPRESS_PRIORITY 4
#define TASK_READ_KEYBOARD_PRIORITY 3
#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2

void task_read_KEY_press(void *pdata)
{
  volatile int *KEY_ptr = (int *)KEY_BASE; /* pushbutton KEY address */
  int press;

  while (1)
  {
    press = *(KEY_ptr + 3);
    *(KEY_ptr + 3) = press;

    if (press & 0x2)
      printf("KEY 2 Pressed!");
  }
}

void task_read_keyboard_input(void *pdata)
{
  volatile int *PS2_ptr = (int *)PS2_BASE;
  char byte1, byte2, byte3, byte4, byte5;
  int PS2_data, RAVAIL;

  byte1 = 0;
  byte2 = 0;
  byte3 = 0;
  byte4 = 0;
  byte5 = 0;

  *(PS2_ptr) = 0xFF; // reset PS/2
  int flag = 0;

  while (1)
  {

    // volatile int *KEY_ptr = (int *)KEY_BASE; /* pushbutton KEY address */
    // int press;

    // press = *(KEY_ptr + 3);
    // *(KEY_ptr + 3) = press;

    // if (press & 0x2)
    //   printf("KEY 2 Pressed!\n");

    PS2_data = *(PS2_ptr);                  // read the Data register in the PS/2 port
    RAVAIL = (PS2_data & 0xFFFF0000) >> 16; // extract the RAVAIL field
    if (RAVAIL > 0)
    {
      byte5 = PS2_data & 0xFF;

      /* Refactor this code */
      if (byte5 == -16)
        flag = 1;
      else if (flag == 1)
      {
        flag = 0;

        if (byte5 == 69 || byte5 == 112)
          printf("0 pressed\n");
        else if (byte5 == 22 || byte5 == 105)
          printf("1 pressed\n");
        else if (byte5 == 30 || byte5 == 114)
          printf("2 pressed\n");
        else if (byte5 == 38 || byte5 == 122)
          printf("3 pressed\n");
        else if (byte5 == 37 || byte5 == 107)
          printf("4 pressed\n");
        else if (byte5 == 46 || byte5 == 115)
          printf("5 pressed\n");
        else if (byte5 == 54 || byte5 == 116)
          printf("6 pressed\n");
        else if (byte5 == 61 || byte5 == 108)
          printf("7 pressed\n");
        else if (byte5 == 62 || byte5 == 117)
          printf("8 pressed\n");
        else if (byte5 == 70 || byte5 == 125)
          printf("9 pressed\n");
      }
      
    }
  }
}

/* Prints "Hello World" and sleeps for three seconds */
void task1(void *pdata)
{
  while (1)
  {
    printf("Hello from task1\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}
/* Prints "Hello World" and sleeps for three seconds */
void task2(void *pdata)
{
  while (1)
  {
    printf("Hello from task2\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{

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

  /* Setting up interrupts */
  volatile int *slider_switch_ptr = (int *)SW_BASE;
  volatile int *KEY_ptr = (int *)KEY_BASE; /* pushbutton KEY address */

  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE - 1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE - 1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(task_read_keyboard_input,
                  NULL,
                  (void *)&task_read_keyboard_stk[TASK_STACKSIZE - 1],
                  TASK_READ_KEYBOARD_PRIORITY,
                  TASK_READ_KEYBOARD_PRIORITY,
                  task_read_keyboard_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(task_read_KEY_press,
                  NULL,
                  (void *)&task_read_KEY_press_stk[TASK_STACKSIZE - 1],
                  TASK_READ_KEYPRESS_PRIORITY,
                  TASK_READ_KEYPRESS_PRIORITY,
                  task_read_KEY_press_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSStart();

  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
