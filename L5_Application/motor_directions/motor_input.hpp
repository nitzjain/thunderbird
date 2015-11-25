/*
 * motor_input.hpp
 *
 *  Created on: Nov 2, 2015
 *      Author: vishnu
 */

#ifndef _MOTOR_INPUT_HPP_
#define _MOTOR_INPUT_HPP_

#include "motor_directions.hpp"
#include <can.h>
#include "string.h"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "stdio.h"
#include "io.hpp"

#if 0

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
            //TODO: need to change setRunDuration value based on others.
            setRunDuration(50);
            return true;
        }

        bool run(void *p)
        {
            static int8_t mode = 0;
            const can_t mycan = can1;
            can_msg_t msg;
            memset(&msg,0,sizeof(msg));
            msg.frame = 0;
            //msg.msg_id = ; ?? Fill it later
            msg.frame_fields.is_29bit = 0;
            msg.frame_fields.data_len = 0;

            char dir = direction_computation();



            if (mode)
            {
                //should optimise in a better way using gps canID's directly
                if (dir == MOVE_STRAIGHT)
                    msg.msg_id = straight; // TODO follow gps co-ordinates

                else if (dir == MOVE_RIGHT)
                    msg.msg_id = right;

                else if (dir == MOVE_LEFT)
                    msg.msg_id = left;

                else if (dir == MOVE_REVERSE)
                    msg.msg_id = reverse; //TODO reverse and take left -- should be done by motor

                else if (dir == STOP)
                    msg.msg_id = stop;

                else
                    msg.msg_id = 0xFE;
                if(CAN_tx(mycan,&msg,100))
                {

                    printf("Message sent %d\n", msg.msg_id);
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
            }

            if (SW.getSwitch(1))
            {
                msg.msg_id = straight;
                if(CAN_tx(mycan,&msg,100)) {
                    printf("Straight\n");
                    mode = 1;
                }

            }
            else if(SW.getSwitch(2))
            {
                msg.msg_id = stop;
                if(CAN_tx(mycan,&msg,100)) {
                    printf("Stop\n");
                }
                mode = 0;
            }

            return true;
        }
};

#endif

#endif /* L5_APPLICATION_MOTOR_DIRECTIONS_MOTOR_INPUT_HPP_ */
