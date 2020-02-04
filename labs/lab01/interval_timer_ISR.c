#include "address_map_nios2.h"
#include "globals.h"

#include "utils.h"
// defines global values

extern volatile int pattern, shift_dir, shift_enable;

int digits[10];
int random_number;
int hex_count;

/*******************************************************************************
* Interval timer interrupt service routine#include <math.h>

* Shifts a PATTERN being displayed on the HEX displays. The shift direction
* is determined by the external variable key_pressed.
******************************************************************************/

void interval_timer_ISR()
{
	volatile int *interval_timer_ptr = (int *)TIMER_BASE;
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

	*(interval_timer_ptr) = 0; // clear the interrupt

	int hex_0 = 0;
	int hex_1 = 0;
	int hex_2 = 0;

	int hex_4 = 0;
	int hex_5 = 0;

	/* HEX 2:0 Random Generator */
	if (shift_dir == LEFT)
	{
		random_number = generate_random_value(1, 255);
	}

	hex_0 = hex_0_val(random_number);
	hex_1 = hex_1_val(random_number);
	hex_2 = hex_2_val(random_number);

	random_number = digits[hex_2] << 16;
	random_number |= digits[hex_1] << 8;
	random_number |= digits[hex_0];

	/* Handling HEX 5:4 counter */
	if (hex_count == 0)
		hex_count = 30;
	else
		hex_count--;

	hex_4 = hex_0_val(hex_count);
	hex_5 = hex_1_val(hex_count);

	int hex_count_value;

	hex_count_value = digits[hex_5] << 8;
	hex_count_value |= digits[hex_4];

	/* Loading all values to HEX pointers for Display */

	*(HEX3_HEX0_ptr) = random_number;
	*(HEX5_HEX4_ptr) = hex_count_value;

	return;
}
