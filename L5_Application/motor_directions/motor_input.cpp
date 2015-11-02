/*
 * motor_input.cpp
 *
 *  Created on: Oct 25, 2015
 *      Author: Vishnu Reddy Mandalapu
 */

/**
 * Based on sensor and GPS data, instruct the motor
 * for vehicle motion
 */

#include "motor_directions.hpp"
#include <can.h>
#include "string.h"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "stdio.h"



/*TODO: As of it is static, need to update in periodic
        task based on input from GPS team*/

int gps_direction = 1;

extern int sensor1, sensor2, sensor3, sensor4;

class motor_input: public scheduler_task
{
    private:
    public:
        motor_input(uint8_t priority) :
                scheduler_task("Motor input", 1024, priority)
        {
                //NOTHING

        }

        bool init()
        {
            //TODO: As of now nothing...
            return true;
        }

        bool run(void *p)
        {
            const can_t mycan = can1;
            can_msg_t msg;
            memset(&msg,0,sizeof(msg));
            msg.frame = 0;
            //msg.msg_id = ;
            msg.frame_fields.is_29bit = 0;
            msg.frame_fields.data_len = 0;

            char dir = direction_computation(sensor1, sensor2, sensor3, sensor4);

            //should optimise in a better way using gps canID's directly
            if (dir == MOVE_STRAIGHT)
                msg.msg_id = forward; // TODO follow gps co-ordinates

            if (dir == MOVE_RIGHT)
                msg.msg_id = right;

            if (dir == MOVE_LEFT)
                msg.msg_id = left;

            if (dir == MOVE_LEFT)
                msg.msg_id = stop;

            if(CAN_tx(mycan,&msg,100))
            {

                printf("Can direction");
            }
            else
            {

                printf("Motor direction update failed");
                CAN_reset_bus(mycan);
                if(CAN_is_bus_off(mycan))
                {
                    //LOG_INFO("Can is bus off");
                    printf("Can is bus off");
                    CAN_reset_bus(mycan);

                }
            }
            return true;
        }

};
