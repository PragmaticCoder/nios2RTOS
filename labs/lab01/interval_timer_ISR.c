#include "address_map_nios2.h"
#include "globals.h"
// defines global values

extern volatile int pattern, shift_dir, shift_enable;

int digits[10];
int count;

/*******************************************************************************
* Interval timer interrupt service routine
* Shifts a PATTERN being displayed on the HEX displays. The shift direction
* is determined by the external variable key_pressed.
******************************************************************************/
void interval_timer_ISR()
{
	volatile int * interval_timer_ptr = (int *) TIMER_BASE;
	volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
	
	*(interval_timer_ptr) = 0;	// clear the interrupt
	
	int twoHex;
	int oneHex;
	int zeroHex;

	twoHex = rand()%2;
	if(twoHex == 2)
		oneHex = rand()%5;
	else
		oneHex = rand()%9;
	if(twoHex == 2 && oneHex == 5)
		zeroHex = rand()%5;
	else
		zeroHex = rand()%9;

	if(shift_dir == LEFT)
	{
		twoHex = rand()%2;
		if(twoHex == 2)
			oneHex = rand()%5;
		else
			oneHex = rand()%9;
		if(twoHex == 2 && oneHex == 5)
			zeroHex = rand()%5;
		else
			zeroHex = rand()%9;
	}
	else
	{
		twoHex = 0;
		oneHex = 0;
		zeroHex = 0;
	}
	int x;
	x = digits[twoHex]<<16;
	x = x^digits[oneHex]<<8;
	x = x^digits[zeroHex];
	*(HEX3_HEX0_ptr) = x;

	//*(HEX3_HEX0_ptr) = digits[count]<<8; 


	return;
}
