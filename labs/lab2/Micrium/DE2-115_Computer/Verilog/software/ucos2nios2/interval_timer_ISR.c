/**
 * Name: Alvi Akbar
 * Student No: 11118887
 * NSID: ala273
 */

#include "address_map_nios2.h"
#include "globals.h"

// defines global values
LockState state;

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

}
