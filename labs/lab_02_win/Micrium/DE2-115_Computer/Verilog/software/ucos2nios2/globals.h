/**
 * Name: Alvi Akbar
 * Student No: 11118887
 * NSID: ala273
 */

#ifndef _globals_h_
#define _globals_h_

#include <stdio.h>
#include "includes.h"
#include "debug.h"
#include "address_map_nios2.h"

#define TRUE (1 == 1)
#define FALSE (!TRUE)

#define KEY0 0x1
#define KEY1 0x2
#define KEY2 0x4
#define KEY3 0x8

#define MAX_CODES 16
#define MAX_DIGITS 4
#define MIN_DIGITS 4

#define KEY_ptr (((volatile unsigned long *)KEY_BASE))
#define SW_ptr (((volatile unsigned long *)SW_BASE))
#define PS2_ptr (((volatile unsigned long *)PS2_BASE))
#define LEDG_ptr (((volatile unsigned long *)LEDG_BASE))
#define LEDR_ptr (((volatile unsigned long *)LEDR_BASE))

#define SW0_VALUE (*(SW_ptr)&0x1)

/* Definition of Semaphores */
OS_EVENT *SEM_read_PS2;
OS_EVENT *SEM_read_PS2_done;
OS_EVENT *SEM_read_KEYS;
OS_EVENT *SEM_state_change;
OS_EVENT *SEM_timer_start;
OS_EVENT *SEM_flash_success;
OS_EVENT *SEM_flash_fail;
OS_EVENT *SEM_add_code;
OS_EVENT *SEM_delete_code;
OS_EVENT *SEM_timer_code;

/* States */
typedef enum
{
  INIT,
  OPEN,
  CLOSE,
  LOCK,
  CODE,
  PROG,
  VERIFIED,
  ADD_CODE,
  DELETE_CODE
} DoorState;

/* Global Variables */
INT8U err;
extern unsigned KEY_val;
extern int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

extern int PS2_num;/* Used to store Num key press */

extern int state_timer;

extern DoorState state;
extern DoorState prev_state;

#endif