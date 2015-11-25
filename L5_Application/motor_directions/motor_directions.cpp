/*
 * motor_directions.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: sravani
 */
#include "motor_directions.hpp"


int gps_direction = 1;
extern uint8_t speed_counter;

/**
 * Gives the direction based on sensor values.
 */

direction_t direction_computation(int sensor_left, int sensor_straight, int sensor_right, int sensor_reverse)
{
    if (sensor_left >= SENSOR_THRESHOLD && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        speed_counter++;
        return straight; // it should follow GPS co ordinates
    }
    else if (sensor_left >= SENSOR_THRESHOLD
            && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        speed_counter = 0;
        return straight;
    }
    else if (sensor_left >= SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        speed_counter = 0;
        return right;
    }
    else if (sensor_left >= SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        speed_counter = 0;
        return left;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        speed_counter = 0;
        return straight;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        speed_counter = 0;
        return straight;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        speed_counter = 0;
        return right;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        speed_counter = 0;
        return stop;
    }
#if 0
    {
         //enable after the demo
           if (sensor_reverse >=  SENSOR_THRESHOLD)
                 return reverse;
         else
                 return stop;

    }
#endif
}

