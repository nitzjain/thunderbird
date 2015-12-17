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

#include "Motor_Speed_Precise_Directions/precise_steer.hpp"
#include <stdint.h>
#include "stdio.h"
#include "io.hpp"
#include "gpio.hpp"
#include "periodic_callback.h"
#include "can.h"
#include "lpc_pwm.hpp"
#include "singleton_template.hpp"
#include "can_periodic/canperiodicext.hpp"
#include "MotorControl.hpp"
#include "lpc_sys.h"
#include "_can_dbc/can_dbc.h"
#include "uart2.hpp"
#include "utilities.h"
#include "../LCD/LCD_20.hpp"
#include "Motor_Speed_Precise_Directions/motor_speed.hpp"

typedef enum direction
{
    stop = 0,
    straight = 1,
    left = 2,
    right = 3,
    reverse = 4,
    start = 5,
    slight_left = 6,
    slight_right = 7,
} direction_t;

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

//can message id
can_msg_t control;

#define up    7.9
#define up_L  7.6
#define up_R  7.6
#define down  6.0
#define SL    5
#define S_SL  6.25
#define SR    8
#define S_SR  7.5

float pwm_mod = 7.92;

GPIO rpm(P2_7); // Control P1.20
GPIO rear_light_1(P2_1);
GPIO rear_light_2(P2_2);

int vehicle_moved = 0;

const uint32_t HEARTBEAT__MIA_MS = 0;
//const DRIVER_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG ={ 0 };
const uint32_t MOTOR_CMD__MIA_MS = 0;
const DRIVER_TX_MOTOR_CMD_t MOTOR_CMD__MIA_MSG =
{ 0 };

float sleft = 0; ///< B19:8  Min: 0 Max: 400   Destination: DRIVER,IO,MOTOR
float smiddle = 0; ///< B31:20   Destination: DRIVER,IO,MOTOR
float sright = 0; ///< B43:32   Destination: DRIVER,IO,MOTOR
float srear = 0; ///< B55:44   Destination: DRIVER,IO,MOTOR
uint8_t degrees = 0;
float gps_start, gps_end;
extern int X;
/*Motor Initialization*/
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
//    maintain_speed();
//    printf("\nX inclination = %d", X);
//    printf(" PWM = %f\n", pwm_mod);
    white_count = 0;
}

void period_10Hz(void)
{
    //moved the LCD and maintain speed to 100 Hz function
    LCD_Display();
}

void period_100Hz(void)
{
    DRIVER_TX_MOTOR_CMD_t to;
    SENSOR_TX_SONARS_t tos;
    SENSOR_TX_sensorback_t tob;
    uint64_t *from;
    msg_hdr_t hdr;

    DC_Motor &dc_motor_instance = DC_Motor::getInstance();
    Steer_Motor &steer = Steer_Motor::getInstance();

    /*Initializing DC motor*/
    static int flag = 0;
    if (flag == 0)
    {
        dc_motor_instance.setDriveMotor(7.5); //Initialize the motor
        steer.setSteerMotor(6.8);
        rear_light_1.setAsOutput();
        rear_light_2.setAsOutput();
        flag++;
    }

    while (CAN_rx(can1, &control, 0))
    {
        from = (uint64_t *) &control.data;
        hdr.dlc = control.frame_fields.data_len;
        hdr.mid = control.msg_id;

        /*Displaying Front Sensors - Left, Middle and Right*/
        if (control.msg_id == SENSOR_TX_SONARS_HDR.mid)
        {
            if (SENSOR_TX_SONARS_decode(&tos, from, &hdr))
            {
                sleft = tos.m0.SENSOR_SONARS_left;
                smiddle = tos.m0.SENSOR_SONARS_middle;
                sright = tos.m0.SENSOR_SONARS_right;
            }
        }

        /*Back Sensor*/
        if (control.msg_id == SENSOR_TX_sensorback_HDR.mid)
        {
            if (SENSOR_TX_sensorback_decode(&tob, from, &hdr))
            {
                srear = tob.SENSOR_BACK_cmd;
            }
        }

        /*GPS Information*/
        if (control.msg_id == 0x564)
        {
            gps_start = 42;
            gps_end = 62;
        }

        if (control.msg_id == DRIVER_TX_MOTOR_CMD_HDR.mid)
        {
            /*Start the feedback*/
            if (DRIVER_TX_MOTOR_CMD_decode(&to, from, &hdr))
            {
                LD.setNumber((char)to.MOTOR_CMD_steer);
                degrees = to.MOTOR_CMD_angle;
                if (straight == to.MOTOR_CMD_steer) //go forward - 000
                {
                    LE.toggle(1);
                     maintain_speed();
                    //dc_motor_instance.setDriveMotor(7.91);
                    steer.setSteerMotor(6.8);
                }
                else if (left == to.MOTOR_CMD_steer) //go left 001, 011
                {
                    LE.toggle(2);
                    precise_steer_left(to.MOTOR_CMD_angle );//give command value instead of 10
                    maintain_speed();
                    //dc_motor_instance.setDriveMotor(7.91);
                }
                else if(slight_left == to.MOTOR_CMD_steer)
                {
                    steer.setSteerMotor(4);
                    maintain_speed();
                    //dc_motor_instance.setDriveMotor(7.91);
                }
                else if (right == to.MOTOR_CMD_steer) //go right 010, 100, 110
                {
                    LE.toggle(3);
                    precise_steer_right(to.MOTOR_CMD_angle );//give command value instead of 10
                    maintain_speed();
                    //dc_motor_instance.setDriveMotor(7.91);

                }
                else if(slight_right == to.MOTOR_CMD_steer)
                {
                    steer.setSteerMotor(8.8);
                    maintain_speed();
                    //dc_motor_instance.setDriveMotor(7.91);
                }
                else if (reverse == to.MOTOR_CMD_steer) //go back 111, 101
                {
                    LE.toggle(4);
                    dc_motor_instance.setDriveMotor(7.5);
                    steer.setSteerMotor(8.8);
                    dc_motor_instance.setDriveMotor(down);
                    rear_light_1.setHigh();
                    rear_light_2.setHigh();
                }
                else if (stop == to.MOTOR_CMD_steer)
                {
                    dc_motor_instance.setDriveMotor(7.5);
                    steer.setSteerMotor(6.8);
                }
                rear_light_1.setLow();
                rear_light_2.setLow();
            }
        }
    }
}

void period_1000Hz(void)
{

}
