#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"
#include "globals.h" // defines global values
#include "utils.h"
/*******************************************************************************
 * This program demonstrates use of interrupts. It
 * first starts the interval timer with 50 msec timeouts, and then enables
 * Nios II interrupts from the interval timer and pushbutton KEYs
 *
 * The interrupt service routine for the interval timer displays a pattern on
 * the LED lights, and shifts this pattern either left or right. The shifting
 * direction is reversed when KEY[1] is pressed
********************************************************************************/
int digits[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

int main(void)
{
    /* Declare volatile pointers to I/O registers (volatile means that IO load
     * and store instructions will be used to access these pointer locations,
     * instead of regular memory loads and stores)
     */
    volatile int *interval_timer_ptr = (int *)TIMER_BASE;  /* interal timer base address */
	volatile int *slider_switch_ptr = (int *)SW_BASE;
    volatile int *KEY_ptr = (int *)KEY_BASE; /* pushbutton KEY address */

    /* Initialization */
    hex_count = MAX_TIMER_COUNT;

    /* Initial State */
    if (*(slider_switch_ptr) & 0x20000)
    {
        state = IDLE;
        Task_idle_state();
    }


    /* set the interval timer period for scrolling the LED lights */
    int counter = 25000000; // 1/(50 MHz) x (50000000) = 1sec

    *(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
    *(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

    /* start interval timer, enable its interrupts */
    *(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1

    *(KEY_ptr + 2) = 0xF; // enable interrupts for all pushbuttons

    /* set interrupt mask bits for levels 0 (interval timer) and level 1
     * (pushbuttons) */
    NIOS2_WRITE_IENABLE(0x3);

    NIOS2_WRITE_STATUS(1); // enable Nios II interrupts

    while (1)
        ; // main program simply idles
}
