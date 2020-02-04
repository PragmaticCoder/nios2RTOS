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

	*(interval_timer_ptr) = 0; // clear the interrupt

	int hex_0 = 0;
	int hex_1 = 0;
	int hex_2 = 0;

	if (shift_dir == LEFT)
	{
		random_number = generate_random_value(1, 255);
	}

	hex_0 = hex_0_val(random_number);
	hex_1 = hex_1_val(random_number);
	hex_2 = hex_2_val(random_number);

	random_number = digits[hex_2] << 16;
	random_number = random_number ^ digits[hex_1] << 8;
	random_number = random_number ^ digits[hex_0];

	*(HEX3_HEX0_ptr) = random_number;

	return;
}
