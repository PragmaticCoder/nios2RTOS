#ifndef UTILS_H
#define UTILS_H

int generate_random_value(int lower, int upper);

int hex_0_val(int value);
int hex_1_val(int value);
int hex_2_val(int value);

void Task_idle_state();
void Task_paused_state();
void Task_play_state();
void Task_power_off();

#endif