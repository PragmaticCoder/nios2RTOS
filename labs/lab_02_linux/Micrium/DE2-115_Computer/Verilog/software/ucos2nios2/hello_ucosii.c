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

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048

OS_STK task_read_keys_stk[TASK_STACKSIZE];
OS_STK task_state_timer_stk[TASK_STACKSIZE];
OS_STK task_read_ps2_stk[TASK_STACKSIZE];
OS_STK task_verify_access_code[TASK_STACKSIZE];
OS_STK task_flash_success_stk[TASK_STACKSIZE];
OS_STK task_flash_fail_stk[TASK_STACKSIZE];
OS_STK task_add_code_stk[TASK_STACKSIZE];
OS_STK task_delete_code_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */
#define TASK_READ_KEYS_PRIORITY 1
#define TASK_STATE_TIMER_PRIORITY 2
#define TASK_READ_PS2_PRIORITY 3
#define TASK_FLASH_SUCCESS_PRIORITY 4
#define TASK_FLASH_FAIL_PRIORITY 5
#define TASK_ADD_CODE_PRIORITY 6
#define TASK_DELETE_CODE_PRIORITY 7

/* Global Variables */
unsigned KEY_val;
int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

int state_timer;
int timer_code;

int cur_input_idx;
int cur_input_code[MAX_DIGITS] = {-1, -1, -1, -1};
int stored_codes[MAX_CODES][MAX_DIGITS] = {{0, 1, 0, 1}};

int PS2_num;

DoorState state;
DoorState prev_state;

/* function prototypes */
void Task_read_PS2(void *);
void Task_read_KEYs(void *);
void Task_state_timer(void *);
void Task_add_del_code(void *);
void Task_delete_code(void *);

// TODO: Implement Rendouvouz Synchnorization between Task_read_PS2 and
// a new Timer Task.

void reset_PS2_input()
{
  for (int i = 0; i < MAX_DIGITS; i++)
    cur_input_code[i] = -1;
  cur_input_idx = 0;
}

/* Tasks Implementation */
void Task_read_PS2(void *pdata)
{

  debug("Started: Task_read_PS2");

  int PS2_data, RAVAIL;
  *(PS2_ptr) = 0xFF; // reset PS/2

  int flag = 0;

  while (1)
  {
    debug("Executing Read PS2");

    PS2_data = *(PS2_ptr);                  /* read the Data register in the PS/2 port */
    RAVAIL = (PS2_data & 0xFFFF0000) >> 16; /* extract the RAVAIL field */

    if (RAVAIL > 0)
    {
      char PS2_val = PS2_data & 0xFF;
      if (PS2_val == -16)
      {
        flag = 1;
        PS2_num = -1; /* resetting PS2 num press */
      }
      else if (flag == 1)
      {
        flag = 0;

        switch (PS2_val)
        {
        case 69:
        case 112:
          PS2_num = 0;
          debug("0 pressed");
          break;
        case 22:
        case 105:
          PS2_num = 1;
          debug("1 pressed");
          break;
        case 30:
        case 114:
          PS2_num = 2;
          debug("2 pressed");
          break;
        case 38:
        case 122:
          PS2_num = 3;
          debug("3 pressed");
          break;
        case 37:
        case 107:
          PS2_num = 4;
          debug("4 pressed");
          break;
        case 46:
        case 115:
          PS2_num = 5;
          debug("5 pressed");
          break;
        case 54:
        case 116:
          PS2_num = 6;
          debug("6 pressed");
          break;
        case 61:
        case 108:
          PS2_num = 7;
          debug("7 pressed");
          break;
        case 62:
        case 117:
          PS2_num = 8;
          debug("8 pressed");
          break;
        case 70:
        case 125:
          PS2_num = 9;
          debug("9 pressed");
          break;
        default:
          debug("Nothing Pressed");
        }
      }
    }

    if ((state == CODE || state == PROG) && PS2_num != -1)
    {
      if (cur_input_idx % 2 == 1)
        cur_input_code[cur_input_idx++] = timer_code;

      cur_input_code[cur_input_idx++] = PS2_num;
      PS2_num = -1; /* resetting PS2_num */
    }

    if (cur_input_idx >= MAX_DIGITS)
      cur_input_idx = 0;

    debug("Current Input Array: %d %d %d %d",
          cur_input_code[0], cur_input_code[1], cur_input_code[2], cur_input_code[3]);

    if (state == PROG && KEY1_flag && cur_input_code[MIN_DIGITS - 1] != -1)
      OSSemPost(SEM_read_PS2_done);

    OSTimeDlyHMSM(0, 0, 0, 300);
  }
}

void Task_read_KEYs(void *pdata)
{
  debug("Started: Task_read_KEYs");

  while (1)
  {
    OSSemPend(SEM_read_KEYS, 0, &err);
    // debug("%u: \tState: %s", OSTime, Get_state_name(state));
    /***************************************************/
    /* Signaling Semaphores used for Activity Control */
    /***************************************************/
    if (state == LOCK || state == CODE)
      OSSemPost(SEM_read_PS2);

    if (state == CODE || state == PROG || state == CLOSE || state || VERIFIED)
      OSSemPost(SEM_timer_start);

    /* OPEN & CLOSE STATE LED */
    if (state == CLOSE)
    {
      *(LEDG_ptr) |= 0x01;
      *(LEDR_ptr) &= ~0x01;
    }
    else if (state == OPEN)
    {
      *(LEDG_ptr) &= ~0x01;
      *(LEDR_ptr) |= 0x01;
    }

    /**************************************************/
    /**************************************************/

    if (KEY1_flag)
      KEY1_flag = 0;

    Check_KEYs(0, &KEY1_flag, 0, 0);

    if (KEY1_flag)
      debug("KEY1_flag: %d", KEY1_flag);

    /* Logic for getting to Open State */
    if ((state == INIT && SW0_VALUE == 1) ||
        (state == ADD_CODE && KEY1_flag) ||
        ((state == VERIFIED) && (state_timer < 10) && (SW0_VALUE == 1))
        // TODO: Need to add transition logic from PROG State
    )
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = OPEN;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    /* Logic for moving to CLOSE State */
    if (state == INIT && SW0_VALUE == 0)
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = CLOSE;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    /* Logics for Transitioning to PROG STATE */
    if (state == OPEN && KEY1_flag)
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = PROG;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }
    /* Signalling tasks associated with PROG State */
    if (state == PROG && KEY1_flag)
      OSSemPost(SEM_add_code);

    /* Logics for Transitioning to PROG State */
    if (state == OPEN && SW0_VALUE == 1)
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = PROG;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    /* Logics for Transitioning to LOCK State */
    if (state == CLOSE && state_timer >= 5)
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = LOCK;
      state_timer = 0;

      OSSemPend(SEM_timer_start, 0, &err); /* pausing state timer task here */
      OSSemPost(SEM_state_change);
    }

    /* Logics for Transitioning to CODE State */
    if (state == LOCK && PS2_num != -1)
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = CODE;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    /* Logics for transitioning to VERIFIED State */
    int matched = 0;

    if (state == CODE && KEY1_flag)
    {

      for (int i = 0; i < MAX_CODES; i++)
      {
        for (int j = 0; j < MAX_DIGITS; j++)
        {
          if (stored_codes[i][j] != cur_input_code[j] && i >= MAX_CODES && j >= MAX_DIGITS - 1)
            goto fail;

          if (stored_codes[i][j] != cur_input_code[j])
            continue;

          /* if we have reached the last element of the array */
          /* and and all digits matched */
          if (j >= MAX_DIGITS - 1)
          {
            debug("ACCESS CODE MATCHED!!!");
            OSSemPend(SEM_state_change, 0, &err);
            state = VERIFIED;
            OSSemPost(SEM_state_change);
            matched = 1;
            break;
          }
        }

        if (matched == 1)
        {
          OSSemPost(SEM_flash_success); /* Signal flash fail if not matched! */
          /* resetting input digits */
          reset_PS2_input();
          break;
        }
      }

    fail:
      if (matched == 0)
        OSSemPost(SEM_flash_fail); /* Signal flash fail if not matched! */

      matched = 1; /* Initializing rest of the array elements to -1 */
      reset_PS2_input();
    }

    OSSemPost(SEM_read_KEYS);
    OSTimeDlyHMSM(0, 0, 0, 300); /* Delay */
  }
}

void Task_state_timer(void *pdata)
{
  debug("Started: Task_state_timer");
  while (1)
  {
    log_info("%u: \tState: %s\t State Time: %ds", OSTime, Get_state_name(state), state_timer);
    if (prev_state != state)
    {
      OSSemPend(SEM_state_change, 0, &err);
      prev_state = state;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    state_timer++;

    if (state == CODE && state_timer > 5)
    {
      log_info("Time Out!");
      reset_PS2_input();
      OSSemPost(SEM_flash_fail);
      state == PROG;
      state_timer = 0;
      timer_code = 0;
    }

    if ((state == CODE || state == PROG) && timer_code < 5)
    {
      timer_code++;
    }
    else
      timer_code = 0;

    if (state == PROG && ((state_timer >= 30) || ((cur_input_code[0] == -1) && KEY1_flag)))
    {
      OSSemPend(SEM_state_change, 0, &err);
      state = OPEN;
      state_timer = 0;
      OSSemPost(SEM_state_change);
    }

    OSTimeDlyHMSM(0, 0, 1, 0);
  }
}

void Task_flash_success(void *pdata)
{
  debug("Started: Task_flash_success");

  while (1)
  {
    OSSemPend(SEM_flash_success, 0, &err);
    int pattern = 0xE;
    debug("Flashing SUCCESS");

    *(LEDG_ptr) |= pattern;
    OSTimeDlyHMSM(0, 0, 1, 0);
    *(LEDG_ptr) &= ~pattern;

    OSTimeDlyHMSM(0, 0, 1, 0);
  }
}

void Task_flash_fail(void *pdata)
{
  debug("Started: Task_flash_fail");
  while (1)
  {
    int pattern = 0xE;
    OSSemPend(SEM_flash_fail, 0, &err);
    debug("Flashing FAIL");

    *(LEDR_ptr) |= pattern;
    OSTimeDlyHMSM(0, 0, 1, 0);
    *(LEDR_ptr) &= ~pattern;
  }
}

void Task_add_del_code(void *pdata)
{
  debug("Started: Task_add_del_code");

  while (1)
  {

    OSSemPend(SEM_add_code, 0, &err);
    debug("Running Task_add_del_code");

    /* signaling the read ps2 input into cur_input_code array */
    OSSemPost(SEM_timer_start);

    /* signaling the read ps2 input into cur_input_code array */
    debug("Signalling read_PS2");
    OSSemPost(SEM_read_PS2);

    /* wait for Task read PS input to populate the global array */
    debug("Waiting for PS2 Key to read");
    OSSemPend(SEM_read_PS2_done, 0, &err);

    debug("Add Delete Operation Invoked");
    int all_matched = 0;
    int add_new_code = 0;

    for (int i = 0; i < MAX_CODES; i++)
    {
      /* When storage is full */
      if (stored_codes[MAX_CODES - 1][0] != -1)
      {
        OSSemPost(SEM_flash_fail);
        break;
      }

      int matched = 0;
      for (int j = 0; j < MAX_DIGITS; j++)
      {
        // Add the code and post Success
        if (stored_codes[i][0] == -1)
        {
          add_new_code = 1;
          break;
        }

        if (stored_codes[i][j] == cur_input_code[j])
          matched++;

        if (matched == MAX_DIGITS)
        {
          all_matched = 1;
          break;
        }
      }

      if (all_matched)
      {
        // delete code
        for (int k = 0; k < MAX_DIGITS; k++)
          stored_codes[i][k] = 0;

        OSSemPost(SEM_flash_success);
        all_matched = 0;
        break;
      }

      if (add_new_code)
      {
        for (int j = 0; j < MAX_DIGITS; j++)
        {
          stored_codes[1][j] = cur_input_code[j];
        }
        OSSemPost(SEM_flash_success);
        add_new_code = 0;
        break;
      }
    }

    reset_PS2_input();
    OSTimeDlyHMSM(0, 0, 0, 300);
  }
}

void Task_delete_code(void *pdata)
{
  debug("Started: Task_delete_code");

  while (1)
  {
    OSSemPend(SEM_delete_code, 0, &err);
    debug("Within Task Delete Code");
    OSTimeDlyHMSM(0, 0, 0, 300);
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
  PS2_num = -1;
  timer_code = 0;
  cur_input_idx = 0;
  state_timer = 0;

  /* Initialization Code */
  // state = INIT;

  // TODO: Uncomment this code in Production
  /* For Debugging Purpose */
  state = INIT;
  /************************/

  /* Semaphore for activity/sequence control */
  SEM_read_PS2 = OSSemCreate(0);      /* Blocking initially */
  SEM_read_PS2_done = OSSemCreate(0); /* Blocking initially */
  SEM_timer_start = OSSemCreate(0);   /* Blocking initially */
  SEM_flash_success = OSSemCreate(0); /* Blocking initially */
  SEM_flash_fail = OSSemCreate(0);    /* Blocking initially */
  SEM_add_code = OSSemCreate(0);      /* Blocking initially */

  SEM_read_KEYS = OSSemCreate(1);
  SEM_state_change = OSSemCreate(1);

  /* Initializing rest of the array elements to -1 */
  for (int i = 1; i < MAX_CODES; i++)
    for (int j = 0; j < MAX_DIGITS; j++)
      stored_codes[i][j] = -1;

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
                  (void *)&task_state_timer_stk[TASK_STACKSIZE - 1],
                  TASK_STATE_TIMER_PRIORITY,
                  TASK_STATE_TIMER_PRIORITY,
                  task_state_timer_stk,
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

  OSTaskCreateExt(Task_flash_success,
                  NULL,
                  (void *)&task_flash_success_stk[TASK_STACKSIZE - 1],
                  TASK_FLASH_SUCCESS_PRIORITY,
                  TASK_FLASH_SUCCESS_PRIORITY,
                  task_flash_success_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_flash_fail,
                  NULL,
                  (void *)&task_flash_fail_stk[TASK_STACKSIZE - 1],
                  TASK_FLASH_FAIL_PRIORITY,
                  TASK_FLASH_FAIL_PRIORITY,
                  task_flash_fail_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_add_del_code,
                  NULL,
                  (void *)&task_add_code_stk[TASK_STACKSIZE - 1],
                  TASK_ADD_CODE_PRIORITY,
                  TASK_ADD_CODE_PRIORITY,
                  task_add_code_stk,
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
