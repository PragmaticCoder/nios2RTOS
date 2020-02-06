#ifndef _globals_h_
#define _globals_h_

#define LEFT 0
#define RIGHT 1

#define DISABLE 0
#define ENABLE 1

#define MAX_QUESTIONS 10
#define MAX_TIMER_COUNT 5

/* States */
typedef enum
{
  IDLE,
  PLAY,
  PAUSE,
  OFF
} GameState;

extern GameState state;

/* Global Variables */
extern int hex_count; /* timer digit count */
extern int random_number;
extern int questions_count;

/* Global value storage registers */
extern int hex_3_0_val;
extern int hex_5_4_val;
extern int switch_val;

extern int digits[10];

#endif