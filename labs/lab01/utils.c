#include "globals.h"
#include "address_map_nios2.h"

/* shared global variables */
int hex_count;
int random_number;
int elapsed_time;

int questions;
int score;

int hex_3_0_val;
int hex_7_4_val;

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

int hex_3_val(int value)
{
	return (value / 1000) % 10;
}

/* Task Handlers */

void Task_idle_state()
{

	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX7_HEX4_ptr = (int *)HEX7_HEX4_BASE;

	hex_count = 0;
	random_number = 0;
	questions = 0;

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
	hex_7_4_val = digits[hex_5] << 8;
	hex_7_4_val |= digits[hex_4];

	*(HEX7_HEX4_ptr) = hex_7_4_val;
}

void Task_play_state()
{
	int hex_0 = 0;
	int hex_1 = 0;
	int hex_2 = 0;
	int hex_4 = 0;
	int hex_5 = 0;
	int hex_6 = 0;
	int hex_7 = 0;

	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX7_HEX4_ptr = (int *)HEX7_HEX4_BASE;		

	/* HEX 2:0 Random Generator */
	if (hex_count == 0)
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
	{
		hex_count = MAX_TIMER_COUNT;
		questions++;
	}
	else
		hex_count--;

	/* Handling HEX 7:6 Score */
	/* TODO:
	 * Increment the score if the answers match! 
	 */
	score = questions;

	hex_4 = hex_0_val(hex_count);
	hex_5 = hex_1_val(hex_count);

	hex_6 = hex_0_val(score);
	hex_7 = hex_1_val(score);

	hex_7_4_val = digits[hex_7] << 24;
	hex_7_4_val |= digits[hex_6] << 16; 
	hex_7_4_val |= digits[hex_5] << 8;
	hex_7_4_val |= digits[hex_4];

	/* Loading all values to HEX pointers for Display */
	*(HEX3_HEX0_ptr) = random_number;
	*(HEX7_HEX4_ptr) = hex_7_4_val;

}

void Task_paused_state()
{
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	*(HEX3_HEX0_ptr) &= ~0xFFFFFFFF;
}

void Task_power_off()
{
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX7_HEX4_ptr = (int *)HEX7_HEX4_BASE;

	hex_count = 0;
	random_number = 0;
	questions = 0;
	score = 0;

	*(HEX7_HEX4_ptr) &= ~0xFFFFFFFF;
	*(HEX3_HEX0_ptr) &= ~0XFFFFFFFF;
}

void Task_gameover_state()
{
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX7_HEX4_ptr = (int *)HEX7_HEX4_BASE;	

	int hex_0 = 0;
	int hex_1 = 0;
	int hex_2 = 0;
	int hex_3 = 0;

	int hex_4 = 0;
	int hex_5 = 0;

	// Display elapsed time in hours & minute in hex 2 to 0
	// Just for display - for now
	hex_4 = hex_0_val(score);
	hex_5 = hex_1_val(score);

	hex_7_4_val = digits[hex_5] << 8;
	hex_7_4_val |= digits[hex_4];

	*(HEX7_HEX4_ptr) = hex_7_4_val;

	/* displaying elapsed time */
	int m, s;
	int elapsed_hex;

	m = elapsed_time / 60;
	s = elapsed_time % 60;

	hex_0 = hex_0_val(s);
	hex_1 = hex_1_val(s);

	hex_2 = hex_0_val(m);
	hex_3 = hex_1_val(m);

	elapsed_hex = digits[hex_3] << 24;
	elapsed_hex |= digits[hex_2] << 16;
	elapsed_hex |= digits[hex_1] << 8;
	elapsed_hex |= digits[hex_0];

	*(HEX3_HEX0_ptr) = elapsed_hex;
}