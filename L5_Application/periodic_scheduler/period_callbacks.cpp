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
#include "LCD.h"
#include "speed.h"

#define distance 18  //This is the wheels circumference.

typedef enum
{
    stop = 0, straight = 1, left = 2, right = 3, reverse = 4, start = 5
} direction_t;

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

//can message id
can_msg_t control;

#define up    7.9
#define up_L  7.6
#define up_R  7.6
#define down  5.5
#define SL    5
#define SR    8

int avg_speed = 0;
float final_time = 0, pwm_mod = 7.9;
int counter = 0;

GPIO rpm(P2_7); // Control P1.20

int vehicle_moved = 0;

const uint32_t HEARTBEAT__MIA_MS = 0;
const DRIVER_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG =
{ 0 };
const uint32_t MOTOR_CMD__MIA_MS = 0;
const DRIVER_TX_MOTOR_CMD_t MOTOR_CMD__MIA_MSG =
{ 0 };

        float sleft=0;                 ///< B19:8  Min: 0 Max: 400   Destination: DRIVER,IO,MOTOR
        float smiddle=0;               ///< B31:20   Destination: DRIVER,IO,MOTOR
        float sright=0;                ///< B43:32   Destination: DRIVER,IO,MOTOR
        float srear=0;                 ///< B55:44   Destination: DRIVER,IO,MOTOR

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

/*Sets the motor speed according to master*/
int lastval = 0;
void period_1Hz(void)
{

    //LD.setNumber((int)pwm_mod*10);
    printf("\nWhite count = %d", white_count);
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
    uint64_t *from;
    msg_hdr_t hdr;

    DC_Motor &dc_motor_instance = DC_Motor::getInstance();
    Steer_Motor &steer = Steer_Motor::getInstance();

//    static float desiredpwm = 0;
//    float newpwm = 0;

    /**/

    /*Initializing DC motor*/
    static int flag = 0;
    if (flag == 0)
    {
        dc_motor_instance.setDriveMotor(7.5); //Initialize the motor
        steer.setSteerMotor(7);
        flag++;

    }

    while (CAN_rx(can1, &control, 0))
    {

        from = (uint64_t *) &control.data;
        hdr.dlc = control.frame_fields.data_len;
        hdr.mid = control.msg_id;
        if(control.msg_id == SENSOR_TX_SONARS_HDR.mid)
        {
            printf("hi\n");
            if (SENSOR_TX_SONARS_decode(&tos,from,&hdr))
            {
                sleft = tos.m0.SENSOR_SONARS_left;
                smiddle = tos.m0.SENSOR_SONARS_middle;
                sright = tos.m0.SENSOR_SONARS_right;
                srear = tos.m0.SENSOR_SONARS_rear;
                printf("l: %2f,m: %2f,r: %2f,b: %2f",sleft,smiddle,sright,srear);
            }
        }
        if (control.msg_id == DRIVER_TX_MOTOR_CMD_HDR.mid)
        {

           /*Start the feedback*/

            if (DRIVER_TX_MOTOR_CMD_decode(&to, from, &hdr))
            {
//                int motor_value = to.MOTOR_CMD_drive;
//
//                switch (motor_value)
//                {
//                    case 1:
//                        newpwm = 7.9; //while taking turns
//                        break;
//                    case 2:
//                        newpwm = 7.9; //while going straight
//                        break;
//                    default:
//                        newpwm = 7.5;
//                        break;
//                }
//            }
//            //No else case for this if |^^
//            if(newpwm-desiredpwm>0.0010)
//            {
//                pwm_mod = newpwm;
//                desiredpwm = newpwm;
//            }
                LD.setNumber(to.MOTOR_CMD_steer);

                if (straight == to.MOTOR_CMD_steer)//go forward - 000
                {
                    LE.toggle(1);
                    //dc_motor_instance.setDriveMotor(pwm_mod);
                    maintain_speed();
                    steer.setSteerMotor(7);
                }
                else if (left == to.MOTOR_CMD_steer) //go left 001, 011
                {
                    LE.toggle(2);
                    steer.setSteerMotor(SL);
                    //dc_motor_instance.setDriveMotor(pwm_mod);
                    maintain_speed();
                }
                else if (right == to.MOTOR_CMD_steer) //go right 010, 100, 110
                {
                    LE.toggle(3);
                    steer.setSteerMotor(SR);
                    //dc_motor_instance.setDriveMotor(pwm_mod);
                    maintain_speed();
                }
                else if (reverse == to.MOTOR_CMD_steer) //go back 111, 101
                {
                    LE.toggle(4);
                    dc_motor_instance.setDriveMotor(7.5);
                    dc_motor_instance.setDriveMotor(down);
                    steer.setSteerMotor(SR);
                }
                else if (stop == to.MOTOR_CMD_steer)
                {
                    LE.toggle(4);
                    dc_motor_instance.setDriveMotor(7.5);
                    steer.setSteerMotor(7);
                }
            }
        }
    }
#if 0
//Should add the functionality to control the motor speed using the speed check

    DC_Motor &dc_motor_instance = DC_Motor::getInstance();
    Steer_Motor &steer = Steer_Motor::getInstance();

    /*Initializing DC motor*/
    static int flag = 0;
    if (flag == 0)
    {
        dc_motor_instance.setDriveMotor(7.5); //Initialize the motor
        steer.setSteerMotor(7);
        flag++;
    }
    while (CAN_rx(can1, &control, 0))
    {

        /* Move forward - */
        if (control.msg_id == straight) //go forward - 000
        {
            if (control.data.bytes[0] == 1)
            {
                pwm_mod1 = 8.1;
                lower_limit = 250, upper_limit = 400;
            }
            else if (control.data.bytes[0] == 0)
            {
                pwm_mod1 = 7.9;
                lower_limit = 100, upper_limit = 200;
            }

            dc_motor_instance.setDriveMotor(pwm_mod1);
            steer.setSteerMotor(7);
            printf("straight\n");
        }
        else if (control.msg_id == left) //go left 001, 011
        {
            dc_motor_instance.setDriveMotor(pwm_mod1);
            steer.setSteerMotor(SL);
            printf("left\n");
            LE.on(2);
            LE.off(3);
        }
        else if (control.msg_id == right) //go right 010, 100, 110
        {
            dc_motor_instance.setDriveMotor(pwm_mod1);
            steer.setSteerMotor(SR);
            printf("right\n");
            LE.on(3);
            LE.off(2);
        }
        else if (control.msg_id == stop)
        {
            steer.setSteerMotor(7);
            dc_motor_instance.setDriveMotor(7.5);
        }
    }

    if (speedchanged == 1)
    {
        dc_motor_instance.setDriveMotor(pwm_mod1);
        steer.setSteerMotor(7);
        speedchanged = 0;
    }
#endif
}

void period_1000Hz(void)
{

}
