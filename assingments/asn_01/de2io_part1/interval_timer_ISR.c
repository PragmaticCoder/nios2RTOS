#include "address_map_nios2.h"
#include "globals.h" // defines global values

/*******************************************************************************
 * Interval timer interrupt service routine
 *
 * Shifts a PATTERN being displayed on the LED lights. The shift direction
 * is determined by the external variable key_dir.
 ******************************************************************************/
void interval_timer_ISR()
{
    volatile int *interval_timer_ptr = (int *)TIMER_BASE;

    volatile int *hex3_hex0_ptr = (int *)HEX3_HEX0_BASE;
    volatile int *hex5_hex4_ptr = (int *)HEX5_HEX4_BASE;
    volatile int *hex7_hex4_ptr = (int *)HEX7_HEX4_BASE;

    *(interval_timer_ptr) = 0; // clear the interrupt

    if (count == 9)
        count = 0;
    else
        count++;

    *hex3_hex0_ptr = digits[count];

    return;
}
