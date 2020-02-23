/**
 * Name: Alvi Akbar
 * Student No: 11118887
 * NSID: ala273
 */

#include "address_map_nios2.h"
#include "globals.h" // defines global values

/* shared global variable */
LockState state;

/* ISR Handler */
void pushbutton_ISR(void)
{
    volatile int *KEY_ptr = (int *)KEY_BASE;

    int press;

    press = *(KEY_ptr + 3); // read the pushbutton interrupt register
    
    printf("Got here to pushbutton_ISR");
    *(KEY_ptr + 3) = press; // Clear the interrupt

    if (press & 0x2){
        printf("Pushbutton pressed");
        return;
    }

    return;
}
