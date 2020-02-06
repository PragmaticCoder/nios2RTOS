#include "globals.h"
#include "address_map_nios2.h"

/* shared global variables */
int hex_count;
int random_number;
int questions_count;
int hex_3_0_val;
int hex_5_4_val;

int digits[10];

/* utility functions */

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

/* Task Handlers */

void Task_idle_state()
{

	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

	hex_count = 0;
	random_number = 0;
	questions_count = 0;

	int hex_2 = 0;
	int hex_1 = 0;
	int hex_0 = 0;

	int hex_5 = 3;
	int hex_4 = 0;

	/* Resetting HEX 3:0 */
	hex_3_0_val = digits[hex_2] << 16;
	hex_3_0_val |= digits[hex_1] << 8;
	hex_3_0_val |= digits[hex_0];

	*(HEX3_HEX0_ptr) = hex_3_0_val;

	/* Resetting HEX 5:4 */
	hex_5_4_val = digits[hex_5] << 8;
	hex_5_4_val |= digits[hex_4];

	*(HEX5_HEX4_ptr) = hex_5_4_val;
}

void Task_play_state()
{

	int hex_0 = 0;
	int hex_1 = 0;
	int hex_2 = 0;
	int hex_4 = 0;
	int hex_5 = 0;

	/* HEX 2:0 Random Generator */
	if (hex_count == MAX_TIMER_COUNT)
	{
		random_number = generate_random_value(1, 255);

		hex_0 = hex_0_val(random_number);
		hex_1 = hex_1_val(random_number);
		hex_2 = hex_2_val(random_number);

		random_number = digits[hex_2] << 16;
		random_number |= digits[hex_1] << 8;
		random_number |= digits[hex_0];
	}

	/* Handling HEX 5:4 counter */
	if (hex_count == 0)
		hex_count = MAX_TIMER_COUNT;
	else
		hex_count--;

	hex_4 = hex_0_val(hex_count);
	hex_5 = hex_1_val(hex_count);

	hex_5_4_val = digits[hex_5] << 8;
	hex_5_4_val |= digits[hex_4];

	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

	/* Loading all values to HEX pointers for Display */

	*(HEX3_HEX0_ptr) = random_number;
	*(HEX5_HEX4_ptr) = hex_5_4_val;

	if (hex_count == MAX_TIMER_COUNT)
		return;
}

void Task_paused_state()
{
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	*(HEX3_HEX0_ptr) &= ~0xFFFFFFFF;
}

void Task_power_off()
{
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

	hex_count = 0;
	random_number = 0;

	*(HEX5_HEX4_ptr) &= ~0xFFFFFFFF;
	*(HEX3_HEX0_ptr) &= ~0XFFFFFFFF;
}