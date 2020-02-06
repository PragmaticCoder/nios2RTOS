/**
 * Name: Alvi Akbar
 * Student No: 11118887
 * NSID: ala273
 */

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

/* Global variables for this file */
int switch_value;

/* utility functions */

int generate_random_value(int lower, int upper)
{
	return ((rand() % upper) + lower);
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

	volatile int *red_LED_ptr = (int *)LEDR_BASE;

	/* reading sw 7-0 value */
	volatile int *slider_switch_ptr = (int *)SW_BASE;

	/* HEX 2:0 Random Generator */
	if (hex_count <= 0)
	{
		random_number = generate_random_value(1, 255);
	}

	hex_0 = hex_0_val(random_number);
	hex_1 = hex_1_val(random_number);
	hex_2 = hex_2_val(random_number);

	int hex_rand = digits[hex_2] << 16;
	hex_rand |= digits[hex_1] << 8;
	hex_rand |= digits[hex_0];

	/* Reading switch 7 - 0 value */
	switch_value = ((*(slider_switch_ptr)) & 0xFF);

	/* Handling HEX 5:4 counter */
	if (hex_count == 0)
	{
		hex_count = MAX_TIMER_COUNT;
		questions++;
	}
	else if (hex_count < 0)
		hex_count = MAX_TIMER_COUNT;
	else
		hex_count--;

	/* when switch 16 is on, show answer */
	if ((*(slider_switch_ptr)) & 0x10000)
		*(red_LED_ptr) = random_number;
	else /* clear */
		*(red_LED_ptr) &= ~random_number;

	/* Handling score display */

	hex_4 = hex_0_val(hex_count);
	hex_5 = hex_1_val(hex_count);

	hex_6 = hex_0_val(score);
	hex_7 = hex_1_val(score);

	hex_7_4_val = digits[hex_7] << 24;
	hex_7_4_val |= digits[hex_6] << 16;
	hex_7_4_val |= digits[hex_5] << 8;
	hex_7_4_val |= digits[hex_4];

	/* Loading all values to HEX pointers for Display */
	*(HEX3_HEX0_ptr) = hex_rand;
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

	volatile int *red_LED_ptr = (int *)LEDR_BASE;
	*(red_LED_ptr) &= ~0xFF;

	hex_count = 0;
	random_number = 0;
	questions = 0;
	score = 0;
	elapsed_time = 0;
	switch_value = 0;

	*(HEX7_HEX4_ptr) &= ~0xFFFFFFFF;
	*(HEX3_HEX0_ptr) &= ~0XFFFFFFFF;
}

void Task_gameover_state()
{
	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX7_HEX4_ptr = (int *)HEX7_HEX4_BASE;

	volatile int *red_LED_ptr = (int *)LEDR_BASE;
	*(red_LED_ptr) &= ~0xFF;

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

void Task_score_calculation()
{
	questions++;

	volatile int *slider_switch_ptr = (int *)SW_BASE;
	switch_value = ((*(slider_switch_ptr)) & 0xFF);

	if (switch_value == random_number)
		score++;

	/* minor hack: just to skip the cycle and prevent
	*  questions increment twice
	*/
	hex_count = -1;
}