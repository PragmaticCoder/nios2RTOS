/**
 * Name: Alvi Akbar
 * Student No: 11118887
 * NSID: ala273
 */

#ifndef _globals_h_
#define _globals_h_

#define TRUE (1 == 1)
#define FALSE (!TRUE)

#define KEY0 0x1
#define KEY1 0x2
#define KEY2 0x4
#define KEY3 0x8

#define KEY_ptr (((volatile unsigned long *)KEY_BASE))
#define PS2_ptr (((volatile unsigned long *)PS2_BASE))

/* Definition of Semaphores */
OS_EVENT *SEM_read_keyboard;

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

unsigned KEY_val;
int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

#endif