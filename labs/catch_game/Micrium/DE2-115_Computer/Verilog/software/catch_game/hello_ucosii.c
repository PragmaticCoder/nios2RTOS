#include "globals.h"
#include "utils.h"

/* global variables */

extern volatile int* video_resolution;
extern volatile int* rgb_status;

extern int screen_x;
extern int screen_y;

extern int res_offset;
extern int col_offset;

extern int basket_pos_x;
extern int basket_pos_y;

extern int pos1_x;
extern int pos1_y;
extern int pos1_val;

extern int pos2_x;
extern int pos2_y;

extern int pos3_x;
extern int pos3_y;

extern int score;
extern int game_hh, game_mm, game_ss;

extern Game_State_t;

extern unsigned KEY_val;
extern int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

extern char text_disp[1];
extern char clear_text[2] = " \0";
extern char clear_row_text[70] =
  "                                                                      \0";
extern int left_key_pressed;
extern int right_key_pressed;
extern int esc_key_pressed;
extern int enter_key_pressed;

short background_color;
short basket_color;
short sidebar_color;

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048
OS_STK task_key_press_stk[TASK_STACKSIZE];
OS_STK task_disp_vga_char_stk[TASK_STACKSIZE];
OS_STK task_game_timer_stk[TASK_STACKSIZE];
OS_STK task_falling_blocks[TASK_STACKSIZE];
OS_STK task_ps2_keyboard_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */
#define TASK_KEY_PRESS_PRIORITY 1
#define TASK_FALLING_BLOCKS 2
#define TASK_VGA_CHAR_PRIORITY 3
#define TASK_GAME_TIMER_PRIORITY 5
#define TASK_PS2_KEYBOARD_PRIORITY 6

/* Function Prototypes */
void
Task_game_timer(void*);

void
Task_move_basket(void*);

static void
Task_VGA_init(void);

void
Task_VGA_char(void*);

void
Task_read_PS2_Keyboard(void*);

void
Task_falling_blocks(void*);

/* ************************************************************************** */
/*                     Track Elapsed Time: Total Game Time                    */
/* ************************************************************************** */
void
Task_game_timer(void* pdata)
{
  debug("Started Game Timer");

  for (;;) {
    OSSemPend(SEM_game_timer, 0, &err);
    game_ss++;

    if (game_ss >= 60) {
      game_mm++;
      game_ss = 0;
    }

    if (game_mm >= 60) {
      game_hh++;
      game_mm = 0;
    }

    if (game_hh >= 24)
      game_hh, game_mm, game_ss = 0, 0, 0;

    VGA_gametime_display(game_hh, game_mm, game_ss);

    OSSemPost(SEM_game_timer);
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
}

/* ************************************************************************** */
/*                   Detecting KEY1, KEY2, KEY3, KEY4 Press                   */
/* ************************************************************************** */
void
Task_move_basket(void* pdata)
{
  debug("Started: Task_move_basket");
  KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag = 0, 0, 0, 0;

  for (;;) {
    OSSemPend(SEM_read_KEYs, 0, &err);
    Check_KEYs(&KEY0_flag, &KEY1_flag, &KEY2_flag, &KEY3_flag);

    if ((KEY0_flag || right_key_pressed) && basket_pos_x < 69) {
      debug("MOVE RIGHT");

      VGA_clear_game_row(59);
      ++basket_pos_x;

      KEY0_flag = 0;
    }

    if ((KEY3_flag || left_key_pressed) && basket_pos_x > 0) {
      debug("MOVE LEFT");

      VGA_clear_game_row(59);
      --basket_pos_x;

      KEY3_flag = 0;
    }

    OSSemPost(SEM_read_KEYs);
    OSTimeDly(1);
  }
}

/* Initial Display Setup */
static void
Task_VGA_init(void)
{
  debug("Initializing VGA Display");

  VGA_clear_screen();
  VGA_display_sidebar(background_color);
}

/* Display Character using VGA Output */
void
Task_VGA_char(void* pdata)
{
  debug("Started: Task_VGA_char");

  for (;;) {
    VGA_animated_char(basket_pos_x, 59, " ", basket_color);

    OSTimeDly(1);
  }
}

/* ************************************************************************** */
/*                    Reads PS2 Keyboard input periodically                   */
/* ************************************************************************** */

void
Task_read_PS2_Keyboard(void* pdata)
{
  debug("Started: Read PS2 Keyboard Task");

  for (;;) {
    read_PS2_KeyboardInput();
    OSTimeDly(1);
  }
}

void
Task_falling_blocks(void* pdata)
{
  debug("Started: Falling Block");

  for (;;) {
    if (pos1_y >= 60) {

      int lower = 0;
      int upper = GAMESCREEN_WIDTH - 1;

      /* Assuming the basket's velocity is same as falling block */
      if (basket_pos_x - lower > GAMESCREEN_HEIGHT)
        lower = basket_pos_x - GAMESCREEN_HEIGHT;

      if (upper - basket_pos_x > GAMESCREEN_HEIGHT)
        upper = basket_pos_x + GAMESCREEN_HEIGHT;

      pos1_x = (rand() % (upper - lower + 1)) + lower;
      pos1_y = 0;

      upper = 9;
      lower = 1;

      pos1_val = (rand() % (upper - lower + 1)) + lower;
      snprintf(text_disp, 2, "%d\0", pos1_val);
    }

    VGA_clear_game_row(pos1_y);
    pos1_y++;

    VGA_animated_char(pos1_x, pos1_y, text_disp, background_color);

    if (pos1_y == 59 && basket_pos_x == pos1_x) {
      score += pos1_val;
      VGA_display_score(score);
    }

    OSTimeDly(1);
  }
}

/* The main function creates two task and starts multi-tasking */
int
main(void)
{

  /* ***************************** Initialization *****************************
   */

  score = 0;
  game_hh, game_mm, game_ss = 0, 0, 0;

  KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag = 0, 0, 0, 0;

  left_key_pressed = 0;
  right_key_pressed = 0;
  esc_key_pressed = 0;
  enter_key_pressed = 0;

  /* ************************ Semaphores Initialization ***********************
   */

  SEM_read_KEYs = OSSemCreate(1);
  SEM_game_timer = OSSemCreate(1);
  SEM_KEY_press = OSSemCreate(1);

  /* **************************** VGA Display Setup ***************************
   */

  video_resolution = (int*)(PIXEL_BUF_CTRL_BASE + 0x8);
  rgb_status = (int*)(RGB_RESAMPLER_BASE);

  screen_x = *video_resolution & 0xFFFF;
  screen_y = (*video_resolution >> 16) & 0xFFFF;

  /* initially basket positioned at the center of screen */
  basket_pos_x = 40;

  /* letter initially positioning falling box randomly */

  int lower = 0;
  int upper = 69;

  pos1_x = (rand() % (upper - lower + 1)) + lower;
  pos1_y = 0;

  upper = 9;
  lower = 1;

  pos1_val = (rand() % (upper - lower + 1)) + lower;
  snprintf(text_disp, 2, "%d\0", pos1_val);

  int db = get_data_bits(*rgb_status & 0x3F);

  /* check if resolution is smaller than the standard 320 x 240 */
  res_offset = (screen_x == 160) ? 1 : 0;

  /* check if number of data bits is less than the standard 16-bits */
  col_offset = (db == 8) ? 1 : 0;

  /* update color */
  background_color = resample_rgb(db, INTEL_RED);
  basket_color = resample_rgb(db, INTEL_LIGHT_YELLOW);

  VGA_animated_char(pos1_x, pos1_y, text_disp, background_color);

  /* **************************************************************************
   */

  Task_VGA_init(); /* Initial Display Layout Setup */

  OSTaskCreateExt(Task_game_timer,
                  NULL,
                  (void*)&task_game_timer_stk[TASK_STACKSIZE - 1],
                  TASK_GAME_TIMER_PRIORITY,
                  TASK_GAME_TIMER_PRIORITY,
                  task_game_timer_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_read_PS2_Keyboard,
                  NULL,
                  (void*)&task_ps2_keyboard_stk[TASK_STACKSIZE - 1],
                  TASK_PS2_KEYBOARD_PRIORITY,
                  TASK_PS2_KEYBOARD_PRIORITY,
                  task_ps2_keyboard_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_move_basket,
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

  OSTaskCreateExt(Task_falling_blocks,
                  NULL,
                  (void*)&task_falling_blocks[TASK_STACKSIZE - 1],
                  TASK_FALLING_BLOCKS,
                  TASK_FALLING_BLOCKS,
                  task_falling_blocks,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSStart();
  return 0;
}
