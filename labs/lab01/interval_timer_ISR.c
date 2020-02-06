#include "address_map_nios2.h"
#include "globals.h"

#include "utils.h"
// defines global values
GameState state;

int questions;
int elapsed_time;
/*******************************************************************************
 * Handles all task associated with Timer Interrupt
 * Task handlers are defined in utils.c
******************************************************************************/
void interval_timer_ISR()
{
	volatile int *interval_timer_ptr = (int *)TIMER_BASE;
	volatile int *slider_switch_ptr = (int *)SW_BASE;

	*(interval_timer_ptr) = 0; // clear the interrupt

	/* keep track of time elapsed */
	if(questions <= 10)
		elapsed_time++;

	/* Power on handler*/
	if (!(*(slider_switch_ptr)&0x20000))
	{
		state = OFF;
		Task_power_off();
	}

	if(questions > 10)
	{
		Task_gameover_state();
		return;
	}

	/* Idle state handler */
	if (state == IDLE || ((state == OFF) && (*(slider_switch_ptr)&0x20000)))
	{
		Task_idle_state();
		return;
	}

	/* Paused state handler */
	if (state == PAUSE)
	{
		Task_paused_state();
		return;
	}

	/* Play state handler */
	if (state == PLAY)
	{
		Task_play_state();
		return;
	}
}
