/**
 * Name: Alvi Akbar
 * Student No: 11118887
 * NSID: ala273
 */

#ifndef _globals_h_
#define _globals_h_

#define KEY0					0x01
#define KEY1					0x02
#define KEY2					0x04
#define KEY3					0x08

/* States */
typedef enum
{
  LOCKED,
  UNLOCKED
} LockState;

extern LockState lock_state;

/* Global Variables */

#endif