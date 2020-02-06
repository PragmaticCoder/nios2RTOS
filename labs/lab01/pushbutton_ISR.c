#include "address_map_nios2.h"
#include "globals.h" // defines global values
#include "utils.h"

 /* Global Variables */
GameState state;

/* ISR Handler */
 void pushbutton_ISR(void)
{
    volatile int *KEY_ptr = (int *)KEY_BASE;
    volatile int *slider_switch_ptr = (int *)SW_BASE;
    int press;

    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt

    if (press & 0x1) // KEY0
    {
        reset |= 1;
        state = IDLE;
    }

    /* KEY 1 is pressed and current state is PLAY */
    if ((press & 0x2) && state == PLAY)
    {
        pause |= 1;
        state = PAUSE;
    }

    /* KEY 1 is pressed and current state is PLAY */
    if ((press & 0x2) && state == PLAY)
    {
        pause |= 0;
        state = PAUSE;
    }

    if (pause)
        hex_5_4_val &= ~0xFFFFFFFF;

    return;
}
