#ifndef GLOBALS_H
#define GLOBALS_H

#define LEFT 0
#define RIGHT 1

#define DISABLE 0
#define ENABLE 1

#define MAX_QUESTIONS 10

/* States */
typedef enum
{
  IDLE,
  PLAY,
  PAUSE,
  OFF
} GameState;

GameState state;

/* Global Variables */
volatile int hex_count; /* timer digit count */
volatile int random_number;
volatile int questions_count;

/* Global value storage registers */
volatile int hex_3_0_val;
volatile int hex_5_4_val;
volatile int switch_val;

#endif