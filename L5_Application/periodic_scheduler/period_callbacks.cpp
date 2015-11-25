/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "io.hpp"
#include <can.h>
#include "periodic_callback.h"
#include "_can_dbc/can_dbc.h"
#include "storage.hpp"
#include "motor_directions/motor_directions.hpp"

#include "can_periodic/canperiodicext.hpp"

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);
void reset_can_bus();
direction_t direction_computation(int sensor_left, int sensor_straight,
        int sensor_right, int sensor_reverse);

static int turnedon = 0;
static int firstturnedon =0;
uint8_t speed_counter = 0;
SENSOR_TX_SONARS_t val;
const can_t mycan = can1;
can_msg_t msg;
direction_t dir;
DRIVER_TX_MOTOR_CMD_t motor_data;
msg_hdr_t hdr;
uint64_t *from;

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{
    const can_t mycan = can1;

    Storage::append("log_messages","--Log message file Master Controller--", 40, 0);


    if(!CAN_init(mycan,100, 32*8, 32*8, NULL,NULL))
        return false;

    CAN_reset_bus(mycan);
    CAN_bypass_filter_accept_all_msgs();


    return true; // Must return true upon success
}

/// Register any telemetry variables
bool period_reg_tlm(void)
{
    // Make sure "SYS_CFG_ENABLE_TLM" is enabled at sys_config.h to use Telemetry
    return true; // Must return true upon success
}



void period_1Hz(void)
{

}

void period_100Hz(void)
{



    dir = direction_computation(val.m0.SENSOR_SONARS_left,
                               val.m0.SENSOR_SONARS_middle,
                               val.m0.SENSOR_SONARS_right,
                               val.m0.SENSOR_SONARS_rear);
    LD.setNumber(dir);
    if (speed_counter > 50)
        motor_data.MOTOR_CMD_drive = 2;
    else
        motor_data.MOTOR_CMD_drive = 1;

    motor_data.MOTOR_CMD_steer = dir;

    // HANDLE MIAs:
    if (SENSOR_TX_SONARS_handle_mia(&val, 10))
    {
        // Message has gone missing
        //motor_data.MOTOR_CMD_steer = stop;
        //motor_data.MOTOR_CMD_drive = 1;
    }

    hdr = DRIVER_TX_MOTOR_CMD_encode((uint64_t *) &msg.data, &motor_data);
    msg.msg_id = hdr.mid;
    msg.frame_fields.data_len = hdr.dlc;
    if (turnedon == 1)
    {
    if (CAN_tx(mycan, &msg, 0))
    {
        printf("Message sent  %d\n", msg.data.bytes[0]);
    }
    else
    {
        reset_can_bus();
    }
    }

}

void period_10Hz(void)
{





    if (SW.getSwitch(1))
    {
        turnedon =1;

    }
    else if(SW.getSwitch(2))
    {
        turnedon = 0;
        motor_data.MOTOR_CMD_drive = 1; //TODO: Discuss with motor team and decide the values.
        motor_data.MOTOR_CMD_steer = stop;
        hdr = DRIVER_TX_MOTOR_CMD_encode((uint64_t *) &msg.data, &motor_data);
        msg.msg_id = hdr.mid;
        msg.frame_fields.data_len = hdr.dlc;

        if (CAN_tx(mycan, &msg, 0))
        {
            printf("Message sent  %d\n", msg.data.bytes[0]);
        }

    }
    if (turnedon == 1)
    {

        while (CAN_rx(can1, &msg, 0)) //TODO: what if other message receives before processing prev msg.. it will crash ??
        {
            switch (msg.msg_id)
            {

                case 200: //TODO: Change to enums

                    hdr.mid = msg.msg_id;
                    hdr.dlc = msg.frame_fields.data_len;
                    from = (uint64_t *) &msg.data;

                    if (!SENSOR_TX_SONARS_decode(&val, from, &hdr))
                        printf("\nDecode failed");




                    break;

                default:
                    printf("Unknown message received, msg id %d\n",
                            (int) msg.msg_id);
                    break;
            } // Switch
        } // while*/


    }

}

void period_1000Hz(void)
{
    //LE.toggle(4);

}

void reset_can_bus()
{
    can_t mycan = can1;
    Storage::append("log_messages", "Motor direction update failed", 30, 0);
    CAN_reset_bus(mycan);
    if (CAN_is_bus_off(mycan))
    {
        Storage::append("log_messages", "Can is bus off", 20, 0);
        CAN_reset_bus(mycan);
    }
}
