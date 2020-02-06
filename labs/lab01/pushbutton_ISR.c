#include "address_map_nios2.h"
#include "globals.h" // defines global values
#include "utils.h"

/* shared global variable */
GameState state;

/* ISR Handler */
void pushbutton_ISR(void)
{
    volatile int *KEY_ptr = (int *)KEY_BASE;
    volatile int *slider_switch_ptr = (int *)SW_BASE;

    int press;

    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    *(KEY_ptr + 3) = press; // Clear the interrupt

    if ((state == PAUSE) && press & 0x1) // KEY0
    {
        state = IDLE;
        Task_idle_state();
        return;
    }

    /* In PLAY state and KEY1 Pressed */
    if ((state == PLAY) && (press & 0x2))
    {
        state = PAUSE;
        Task_paused_state();
        return;
    }

    /* In PAUSED or IDLE state, and KEY 1 pressed */
    if (((state == OFF) || (state == PAUSE) || (state == IDLE)) && (press & 0x2))
    {  
        state = PLAY;
        Task_play_state();
        return;
    }
}
