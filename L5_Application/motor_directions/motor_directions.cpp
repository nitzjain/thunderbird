/*
 * motor_directions.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: sravani
 */
#include "motor_directions.hpp"
#include "_can_dbc/can_dbc.h"

extern GPS_TX_COMPASS_t compass;
extern GPS_TX_GPS_dest_reached_t dest_reached;
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

direction_t gps_direction()
{
    if (compass.COMPASS_direction == 1)
         return right;
    else
         return left;
}


direction_t direction_computation(int sensor_left, int sensor_straight, int sensor_right, int sensor_reverse)
{

    if (dest_reached.GPS_dest_reached)
            return stop;
    direction_t gps_dir = gps_direction();

    if (sensor_left >= SENSOR_THRESHOLD && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right >= SENSOR_THRESHOLD)
    {
            return gps_dir;
    }
    else if (sensor_left >= SENSOR_THRESHOLD
            && sensor_straight >= SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD)
    {
                if (gps_dir == right)
                       return straight;
                else
                        return gps_dir;
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
        if (gps_dir == left)
               return straight;
        else
                return gps_dir;
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
            && sensor_right < SENSOR_THRESHOLD
            && sensor_reverse > SENSOR_THRESHOLD)
    {
        return reverse;
    }
    else if (sensor_left < SENSOR_THRESHOLD
            && sensor_straight < SENSOR_THRESHOLD
            && sensor_right < SENSOR_THRESHOLD
            && sensor_reverse > SENSOR_THRESHOLD)
    {
        return stop;
    }

}

