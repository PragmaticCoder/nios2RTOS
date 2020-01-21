#include "address_map_nios2.h"
#include "globals.h" // defines global values

extern volatile int pattern, shift_dir, shift_enable;
/*******************************************************************************
 * Interval timer interrupt service routine
 *
 * Shifts a PATTERN being displayed on the LED lights. The shift direction
 * is determined by the external variable key_dir.
 ******************************************************************************/
void interval_timer_ISR() {
    volatile int * interval_timer_ptr = (int *)TIMER_BASE;
    volatile int * LEDG_ptr           = (int *)LED_BASE; // LED address

    *(interval_timer_ptr) = 0; // clear the interrupt

    *(LEDG_ptr) = pattern; // display pattern on LED

    if (shift_enable == DISABLE) // check if shifting is disabled
        return;

    /* rotate the pattern shown on the LEDG lights */
    if (shift_dir == LEFT) // rotate left
        if (pattern & 0x80000000)
            pattern = (pattern << 1) | 1;
        else
            pattern = pattern << 1;
    else // rotate right
        if (pattern & 0x00000001)
        pattern = (pattern >> 1) | 0x80000000;
    else
        pattern = (pattern >> 1) & 0x7FFFFFFF;

    return;
}

