#include "globals.h"
#include "address_map_nios2.h"

int digits[10];

int hex_3_0_val;
int hex_5_4_val;

int hex_count;
int questions_count;
int random_number;
int reset;

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

void reset_system()
{
	
	hex_count = 0;
	random_number = 0;
	questions_count = 0;

	volatile int *HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
	volatile int *HEX5_HEX4_ptr = (int *)HEX5_HEX4_BASE;

	/* TODO: This is display time elapsed later on */
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