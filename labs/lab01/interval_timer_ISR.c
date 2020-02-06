#include "address_map_nios2.h"
#include "globals.h"

#include "utils.h"
// defines global values
GameState state;

/*******************************************************************************
 * Handles all task associated with Timer Interrupt
 * Task handlers are defined in utils.c
******************************************************************************/
void interval_timer_ISR()
{
	volatile int *interval_timer_ptr = (int *)TIMER_BASE;

	*(interval_timer_ptr) = 0; // clear the interrupt

	if (state == IDLE)
	{
		Task_idle_state();
		return;
	}

	if (state == PAUSE)
	{
		Task_paused_state();
		return;
	}

	if (state == PLAY)
	{
		Task_play_state();
		return;
	}
}
