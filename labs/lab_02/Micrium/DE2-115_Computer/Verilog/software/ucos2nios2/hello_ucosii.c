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
#undef NDEBUG

#include <stdio.h>
#include "includes.h"
#include "address_map_nios2.h"
#include "globals.h"
#include "debug.h"

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048

OS_STK task_read_keys_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
OS_STK task_read_ps2_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK_READ_KEYS_PRIORITY 1
#define TASK2_PRIORITY 2
#define TASK_READ_PS2_PRIORITY 4

/* Global Variables */
DoorState state;
DoorState prev_state;

/* function prototypes */
void Check_KEYs(int *, int *, int *, int *);
void Task_read_PS2(void *);

/* Helper functions */

const char *Get_state_name(DoorState door_state)
{
  switch (door_state)
  {
  case INIT:
    return "INIT";
  case OPEN:
    return "OPEN";
  case CLOSE:
    return "CLOSE";
  case LOCK:
    return "LOCK";
  case CODE:
    return "CODE";
  case PROG:
    return "PROG";
  case VERIFIED:
    return "VERIFIED";
  case ADD_CODE:
    return "ADD_CODE";
  case DELETE_CODE:
    return "DELETE_CODE";
  default:
    return "INVALID";
  }
}

void Check_KEYs(int *KEY0_ptr, int *KEY1_ptr, int *KEY2_ptr, int *KEY3_ptr)
{

  KEY_val = *(KEY_ptr);

  if (KEY_val == KEY0)
  {
    debug("KEY0 Pressed!"); // check KEY0
    *KEY0_ptr = 1;
  }
  else if (KEY_val == KEY1) // check KEY1
  {
    debug("KEY1 Pressed!");
    *KEY1_ptr = 1;
  }
  else if (KEY_val == KEY2) // check KEY2
  {
    debug("KEY2 Pressed!");
    *KEY2_ptr = 1;
  }
  else if (KEY_val == KEY3) // check KEY3
  {
    debug("KEY3 Pressed!");
    *KEY3_ptr = 1;
  }

  if (KEY_val)
    debug("KEY_value: %d", KEY_val);
}

/* Tasks Implementation */
void Task_read_PS2(void *pdata)
{

  debug("Started: Task_read_PS2");

  char byte5;
  int PS2_data, RAVAIL;

  *(PS2_ptr) = 0xFF; // reset PS/2
  int flag = 0;

  while (1)
  {

    OSSemPend(SEM_read_PS2, 0, &err);

    PS2_data = *(PS2_ptr);                  /* read the Data register in the PS/2 port */
    RAVAIL = (PS2_data & 0xFFFF0000) >> 16; /* extract the RAVAIL field */

    if (RAVAIL > 0)
    {
      byte5 = PS2_data & 0xFF;

      /* TODO: Refactor this code */
      if (byte5 == -16)
        flag = 1;

      else if (flag == 1)
      {
        flag = 0;

        switch (byte5)
        {
        case 69:
        case 112:
          debug("0 pressed");
          break;
        case 22:
        case 105:
          debug("1 pressed");
          break;
        case 30:
        case 114:
          debug("2 pressed");
          break;
        case 38:
        case 122:
          debug("3 pressed");
          break;
        case 37:
        case 107:
          debug("4 pressed");
          break;
        case 46:
        case 115:
          debug("5 pressed");
          break;
        case 54:
        case 116:
          debug("6 pressed");
          break;
        case 61:
        case 108:
          debug("7 pressed");
          break;
        case 62:
        case 117:
          debug("8 pressed");
          break;
        case 70:
        case 125:
          debug("9 pressed");
          break;
        default:
          debug("Nothing Pressed");
        }
      }
    }

    OSSemPost(SEM_read_PS2);
    OSTimeDlyHMSM(0, 0, 100, 0);
  }
}

/* Prints "Hello World" and sleeps for three seconds */
void Task_read_KEYs(void *pdata)
{
  debug("Started: Task_read_KEYs");

  while (1)
  {
    OSSemPend(SEM_read_KEYS, 0, &err);

    if (KEY1_flag)
      KEY1_flag = 0;

    Check_KEYs(0, &KEY1_flag, 0, 0);

    if (KEY1_flag)
      debug("KEY1_flag: %d", KEY1_flag);

    /* Logic for getting to Open State */
    if ((state == INIT && SW0_VALUE == 1) || (state == ADD_CODE && KEY1_flag))
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = OPEN;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    /* Logic for moving to Close State */
    if (state == INIT && SW0_VALUE == 0)
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = CLOSE;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    OSSemPost(SEM_read_KEYS);
    OSTimeDlyHMSM(0, 0, 0, 100);
  }
}
/* Prints "Hello World" and sleeps for three seconds */
void Task_state_timer(void *pdata)
{
  debug("Started: Task_state_timer");

  while (1)
  {
    log_info("%u: State: %s\t State Time: %ds", OSTime, Get_state_name(state), state_timer);

    if (prev_state != state)
    {
      OSSemPend(SEM_state_change, 0, &err);
      prev_state = state;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    state_timer++;

    OSTimeDlyHMSM(0, 0, 1, 0);
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

  KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag = 0, 0, 0, 0;

  /* Initialization Code */
  // state = INIT;

  // TODO: Uncomment this code in Production
  /* For Debugging Purpose */
  state = ADD_CODE;
  /************************/
  state_timer = 0;

  SEM_read_PS2 = OSSemCreate(1);
  SEM_read_KEYS = OSSemCreate(1);
  SEM_state_change = OSSemCreate(1);

  /* Task creation */
  OSTaskCreateExt(Task_read_KEYs,
                  NULL,
                  (void *)&task_read_keys_stk[TASK_STACKSIZE - 1],
                  TASK_READ_KEYS_PRIORITY,
                  TASK_READ_KEYS_PRIORITY,
                  task_read_keys_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_state_timer,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE - 1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_read_PS2,
                  NULL,
                  (void *)&task_read_ps2_stk[TASK_STACKSIZE - 1],
                  TASK_READ_PS2_PRIORITY,
                  TASK_READ_PS2_PRIORITY,
                  task_read_ps2_stk,
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
