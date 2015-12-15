/*
 * motor_directions.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: sravani
 */
#include "motor_directions.hpp"


int gps_direction = 1;


/**
 * Gives the direction based on sensor values.
 */
bool isnearobstacle(int sensor_left,int sensor_straight,int sensor_right)
{
    if(sensor_left<SENSOR_THRESHOLD||sensor_straight<SENSOR_THRESHOLD||sensor_right<SENSOR_THRESHOLD)
     return true;
}
direction_t fardirection_computation(int sensor_left,int sensor_straight,int sensor_right,int sensor_reverse,direction_t neardirection)
{
    if(sensor_left<FAR_SENSOR_THRESHOLD&&sensor_right<FAR_SENSOR_THRESHOLD)
        return straight;
    else if(sensor_left<FAR_SENSOR_THRESHOLD&&sensor_straight<FAR_SENSOR_THRESHOLD&&sensor_right>FAR_SENSOR_THRESHOLD)
        return slight_right;
    else if(sensor_right<FAR_SENSOR_THRESHOLD&&sensor_straight<FAR_SENSOR_THRESHOLD&&sensor_left>FAR_SENSOR_THRESHOLD)
        return slight_left;

    return neardirection;

}
direction_t direction_computation(int sensor_left, int sensor_straight, int sensor_right, int sensor_reverse)
{
    if (sensor_left >= SENSOR_THRESHOLD && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        return straight; // it should follow GPS co ordinates
    }
    else if (sensor_left >= SENSOR_THRESHOLD
            && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        return straight;
    }
    else if (sensor_left >= SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        return right;
    }
    else if (sensor_left >= SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        return left;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        return straight;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        return straight;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
        return right;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
        return stop;
    }
}

