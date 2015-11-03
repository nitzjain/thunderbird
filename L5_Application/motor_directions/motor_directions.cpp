/*
 * motor_directions.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: sravani
 */
#include "motor_directions.hpp"

/**
 * Gives the direction based on sensor values.
 */

char direction_computation(int s1, int s2, int s3, int s4)
{
    if(s1 >= SENSOR_THRESHOLD && s2>=SENSOR_THRESHOLD && s3>=SENSOR_THRESHOLD)
          return MOVE_STRAIGHT; // it should follow GPS co ordinates
    else if(s1>=SENSOR_THRESHOLD && s2>=SENSOR_THRESHOLD && s3<SENSOR_THRESHOLD)
           return MOVE_LEFT;
    else if(s1 >= SENSOR_THRESHOLD && s2 < SENSOR_THRESHOLD && s3  >= SENSOR_THRESHOLD)
              return MOVE_RIGHT;
    else if(s1 >= SENSOR_THRESHOLD &&  s2 < SENSOR_THRESHOLD && s3 < SENSOR_THRESHOLD)
              return MOVE_LEFT;
    else if(s1 < SENSOR_THRESHOLD && s2>=SENSOR_THRESHOLD && s3>=SENSOR_THRESHOLD)
              return MOVE_RIGHT;
    else if(s1 < SENSOR_THRESHOLD && s2 >= SENSOR_THRESHOLD && s3 < SENSOR_THRESHOLD)
              return MOVE_REVERSE;
    else if(s1 < SENSOR_THRESHOLD && s2 < SENSOR_THRESHOLD && s3 >= SENSOR_THRESHOLD)
              return MOVE_RIGHT;
    else if(s1 < SENSOR_THRESHOLD && s2 < SENSOR_THRESHOLD && s3 < SENSOR_THRESHOLD) {
         if (s4 >=  SENSOR_THRESHOLD)
                 return MOVE_REVERSE;
         else
                 return STOP;
    }

}


