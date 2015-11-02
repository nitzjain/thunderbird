/*
 * motor_directions.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: sravani
 */
#include "motor_directions.hpp"


int sensor_left, sensor_stright, sensor_right, sensor_reverse;

int gps_direction = 1;


/**
 * Gives the direction based on sensor values.
 */

char direction_computation()
{
    if(sensor_left <= SENSOR_THRESHOLD && sensor_stright<=SENSOR_THRESHOLD && sensor_right<=SENSOR_THRESHOLD)
          return MOVE_STRAIGHT;
    else if(sensor_left<=SENSOR_THRESHOLD && sensor_stright<=SENSOR_THRESHOLD && sensor_right>SENSOR_THRESHOLD)
           return MOVE_LEFT;
    else if(sensor_left <= SENSOR_THRESHOLD && sensor_stright > SENSOR_THRESHOLD && sensor_right  <= SENSOR_THRESHOLD)
              return MOVE_RIGHT;
    else if(sensor_left <= SENSOR_THRESHOLD &&  sensor_stright > SENSOR_THRESHOLD && sensor_right > SENSOR_THRESHOLD)
              return MOVE_LEFT;
    else if(sensor_left > SENSOR_THRESHOLD && sensor_stright<=SENSOR_THRESHOLD && sensor_right<=SENSOR_THRESHOLD)
              return MOVE_RIGHT;
    else if(sensor_left > SENSOR_THRESHOLD && sensor_stright <= SENSOR_THRESHOLD && sensor_right > SENSOR_THRESHOLD)
              return MOVE_REVERSE;
    else if(sensor_left > SENSOR_THRESHOLD && sensor_stright > SENSOR_THRESHOLD && sensor_right <= SENSOR_THRESHOLD)
              return MOVE_RIGHT;
    else if(sensor_left > SENSOR_THRESHOLD && sensor_stright > SENSOR_THRESHOLD && sensor_right > SENSOR_THRESHOLD) {
         if (sensor_reverse <=  SENSOR_THRESHOLD)
                 return MOVE_REVERSE;
         else
                 return STOP;
         }
    Storage::append("log_messages", "LOG: You Should never see this message", 50, 0);
    return MOVE_STRAIGHT;
}

