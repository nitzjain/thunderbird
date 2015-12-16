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
#include "utilities.h"
#include "motor_directions/motor_directions.hpp"

#include "can_periodic/canperiodicext.hpp"

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);
void reset_can_bus();


static int turnedon = 0;
static int firstturnedon =0;
uint8_t speed_counter = 0;
SENSOR_TX_SONARS_t val;
SENSOR_TX_sensorback_t back_sensor;
/*****GPS data*****/
GPS_TX_COMPASS_t compass;
GPS_TX_GPS_longitude_t longitude;
GPS_TX_GPS_latitude_t latitude;
GPS_TX_GPS_heading_t heading;
GPS_TX_GPS_dest_reached_t dest_reached;

float longitude_act[2];
float latitude_act[2];
float heading_act[2];
float distance;

extern const can_t mycan = can1;
can_msg_t msg;
direction_t dir;
DRIVER_TX_MOTOR_CMD_t motor_data;
msg_hdr_t hdr;
uint64_t *from;

static int isgpsstarted = 0;

/// Called once before the RTOS is started, this is a good place to initialize things once
bool period_init(void)
{


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
    memcpy(&longitude_act[0], &longitude.GPS_longitude_cur, sizeof(float));
    memcpy(&longitude_act[1], &longitude.GPS_longitude_dst, sizeof(float));

    memcpy(&latitude_act[0], &latitude.GPS_latitude_cur, sizeof(float));
    memcpy(&latitude_act[1], &latitude.GPS_latitude_dst, sizeof(float));

    memcpy(&heading_act[0], &heading.GPS_heading_cur, sizeof(float));
    memcpy(&heading_act[1], &heading.GPS_heading_dst, sizeof(float));
    memcpy(&distance, &dest_reached.GPS_dest_reached, sizeof(float));
    printf("Dist %f, Angle %d Dir %d\n ", distance, (int)compass.COMPASS_angle, (int)compass.COMPASS_direction);
    LD.setNumber((char)distance);
    delay_ms(100);
    LD.setNumber((char)compass.COMPASS_angle);

}

void period_100Hz(void)
{

    val.m0.SENSOR_SONARS_rear = back_sensor.SENSOR_BACK_cmd;

    dir = direction_computation(val.m0.SENSOR_SONARS_left,
                               val.m0.SENSOR_SONARS_middle,
                               val.m0.SENSOR_SONARS_right,
                               val.m0.SENSOR_SONARS_rear);
    bool b = isnearobstacle(val.m0.SENSOR_SONARS_left,
                               val.m0.SENSOR_SONARS_middle,
                               val.m0.SENSOR_SONARS_right);
    if(!b)
    {
        dir = fardirection_computation(val.m0.SENSOR_SONARS_left,
                               val.m0.SENSOR_SONARS_middle,
                               val.m0.SENSOR_SONARS_right,
                               val.m0.SENSOR_SONARS_rear,
                               dir);
    }
    LD.setNumber(dir);
    if (speed_counter > 50)
        motor_data.MOTOR_CMD_drive = 2;
    else
        motor_data.MOTOR_CMD_drive = 1;

    motor_data.MOTOR_CMD_steer = dir;
    if (dir == straight)
        motor_data.MOTOR_CMD_angle = compass.COMPASS_angle;
    else
        motor_data.MOTOR_CMD_angle = compass.COMPASS_angle;
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
    if (turnedon == 1 && isgpsstarted == 1)
    {
    if (CAN_tx(mycan, &msg, 0))
    {
        //printf("Message sent  %d\n", msg.data.bytes[0]);
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
            //printf("Message sent  %d\n", msg.data.bytes[0]);
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
                        printf("\nSensor Decode failed");
                    break;
                case 0xA1:
                    hdr.mid = msg.msg_id;
                    hdr.dlc = msg.frame_fields.data_len;
                    from = (uint64_t *) &msg.data;

                    if (!SENSOR_TX_sensorback_decode(&back_sensor, from, &hdr))
                        printf("\nSensor back Decode failed");
                    break;
                case 0xB0:
                    hdr.mid = msg.msg_id;
                    hdr.dlc = msg.frame_fields.data_len;
                    from = (uint64_t *) &msg.data;

                    isgpsstarted = 1;

                    if (!GPS_TX_COMPASS_decode(&compass, from, &hdr))
                        printf("\nCompass Decode failed");
                        //printf("d %d a %d", compass.COMPASS_direction, compass.COMPASS_angle);
                        //LD.setNumber((char)compass.COMPASS_angle);
                    break;
                case 0xB1:
                    hdr.mid = msg.msg_id;
                    hdr.dlc = msg.frame_fields.data_len;
                    from = (uint64_t *) &msg.data;

                    if (!GPS_TX_GPS_longitude_decode(&longitude, from, &hdr));
                        //printf(" %d %d", hdr.dlc, hdr.mid);
                    break;

                case 0xB2:
                    hdr.mid = msg.msg_id;
                    hdr.dlc = msg.frame_fields.data_len;
                    from = (uint64_t *) &msg.data;

                    if (!GPS_TX_GPS_latitude_decode(&latitude, from, &hdr))
                        printf("\nLatitude Decode failed");
                    break;
                case 0xB3:
                    hdr.mid = msg.msg_id;
                    hdr.dlc = msg.frame_fields.data_len;
                    from = (uint64_t *) &msg.data;

                    if (!GPS_TX_GPS_heading_decode(&heading, from, &hdr))
                        printf("\nHeading Decode failed");
                    break;
                case 0xB4:
                    hdr.mid = msg.msg_id;
                    hdr.dlc = msg.frame_fields.data_len;
                    from = (uint64_t *) &msg.data;
                    //printf(" %d %d",hdr.mid,hdr.dlc);
                    if (!GPS_TX_GPS_dest_reached_decode(&dest_reached, from, &hdr));
                        //printf("\nDest reached Decode failed");
                    break;
                default:
                    //printf("Unknown message received, msg id %d\n", (int) msg.msg_id);
                    printf("U");
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
