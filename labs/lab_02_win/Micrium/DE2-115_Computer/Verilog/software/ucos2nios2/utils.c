/*
 * utils.c
 *
 *  Created on: Feb 26, 2020
 *      Author: Alvi
 */

#include "globals.h"
#include "debug.h"

const char *Get_state_name(DoorState door_state)
{
    switch (door_state)
    {
    case INIT:
        return "INIT";
    case OPEN:
        return "OPEN";
    case CLOSE:
        return "CLOSE";
    case LOCK:
        return "LOCK";
    case CODE:
        return "CODE";
    case PROG:
        return "PROG";
    case VERIFIED:
        return "VERIFIED";
    case ADD_CODE:
        return "ADD_CODE";
    case DELETE_CODE:
        return "DELETE_CODE";
    default:
        return "INVALID";
    }
}

void Check_KEYs(int *KEY0_ptr, int *KEY1_ptr, int *KEY2_ptr, int *KEY3_ptr)
{

    KEY_val = *(KEY_ptr);

    if (KEY_val == KEY0)
    {
        debug("KEY0 Pressed!"); // check KEY0
        *KEY0_ptr = 1;
    }
    else if (KEY_val == KEY1) // check KEY1
    {
        debug("KEY1 Pressed!");
        *KEY1_ptr = 1;
    }
    else if (KEY_val == KEY2) // check KEY2
    {
        debug("KEY2 Pressed!");
        *KEY2_ptr = 1;
    }
    else if (KEY_val == KEY3) // check KEY3
    {
        debug("KEY3 Pressed!");
        *KEY3_ptr = 1;
    }

    if (KEY_val)
        debug("KEY_value: %d", KEY_val);
}