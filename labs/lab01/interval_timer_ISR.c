#include "address_map_nios2.h"
#include "globals.h"
// defines global values

extern volatile int pattern, shift_dir, shift_enable;

int digits[10];
int count;

int generate_random_value(int lower, int upper)
{
	return (rand() % (upper - lower + 1)) + lower;
}

int hex_0_val(int value)
{
	return value % 10;
}

int hex_1_val(int value)
{
	return (value / 10) % 10;
}

int hex_2_val(int value)
{
	return (value / 100) % 10;
}

/*******************************************************************************
* Interval timer interrupt service routine#include <math.h>

* Shifts a PATTERN being displayed on the HEX displays. The shift direction
* is determined by the external variable key_pressed.
******************************************************************************/

void interval_timer_ISR()
{
	volatile int *interval_timer_ptr = (int *)TIMER_BASE;
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;

	*(interval_timer_ptr) = 0; // clear the interrupt

	int value = 0;

	int hex_0 = 0;
	int hex_1 = 0;
	int hex_2 = 0;

	if (shift_dir == LEFT)
		value = generate_random_value(1, 255);

	hex_0 = hex_0_val(value);
	hex_1 = hex_1_val(value);
	hex_2 = hex_2_val(value);

	value = digits[hex_2] << 16;
	value = value ^ digits[hex_1] << 8;
	value = value ^ digits[hex_0];

	*(HEX3_HEX0_ptr) = value;

	return;
}
