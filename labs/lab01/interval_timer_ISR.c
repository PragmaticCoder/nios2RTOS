#include "address_map_nios2.h"
#include "globals.h"
// defines global values

extern volatile int pattern, shift_dir, shift_enable;

int digits[10];
int count;

int generate_random_value()
{
	return 0;
}

int digit_zero(int value)
{
	return value % 10;
}

int digit_one(int value)
{
	return value % 100;
}

int digit_two(int value)
{
	return value % 1000;
}

/*******************************************************************************
* Interval timer interrupt service routine
* Shifts a PATTERN being displayed on the HEX displays. The shift direction
* is determined by the external variable key_pressed.
******************************************************************************/

void interval_timer_ISR()
{
	volatile int *interval_timer_ptr = (int *)TIMER_BASE;
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;

	*(interval_timer_ptr) = 0; // clear the interrupt

	int zero_hex;
	int one_hex;
	int two_hex;

	two_hex = rand() % 2;

	if (shift_dir == LEFT)
	{
		
	}
	else
	{
		// TODO: Reset count to zero
	}
	
	int x;
	x = digits[two_hex] << 16;
	x = x ^ digits[one_hex] << 8;
	x = x ^ digits[zero_hex];
	*(HEX3_HEX0_ptr) = x;

	//*(HEX3_HEX0_ptr) = digits[count]<<8;

	return;
}
