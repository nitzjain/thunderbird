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

#define distance 18  //This is the wheels circumference.

typedef enum
{
    stop = 0, straight = 1, left = 2, right = 3, reverse = 4, start = 5
} direction_t;
/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

//can message id
can_msg_t control;

unsigned int valueNeeded;

#define up    7.9
#define up_L  7.6
#define up_R  7.6
#define down  5.5
#define SL    5
#define SR    8

int start_time, elapsed_time, last_seen, time_f = 0;
float final_time = 0, speed = 0, pwm_mod1;
int counter = 0;
static int speedchanged = 0;
GPIO rpm(P2_7); // Control P1.20

unsigned int lower_limit, upper_limit;

int vehicle_moved = 0;

const uint32_t HEARTBEAT__MIA_MS = 0;
const DRIVER_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG =
{ 0 };
const uint32_t MOTOR_CMD__MIA_MS = 0;
const DRIVER_TX_MOTOR_CMD_t MOTOR_CMD__MIA_MSG =
{ 0 };

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

//1800 - 1200
void period_1Hz(void)
{
    /*Speed check*/
    //In progress - depending on the speed the motor will be controlled.
    speed = (distance * 1000 / final_time);
    //float speed2 = (distance * 1000 / elapsed_time);
    //printf("\nTime f is %d ms\n", time_f);
    //  printf("\nFinal Time  %d ms\n", final_time);
    printf("\nSpeed is %0.2f cm/s\n", speed);
    //printf("\nSpeed2 is %0.2f cm/s\n", speed2);
    final_time = 1;

    //100 - 200 = 7.9
    //250-400 = 8.1

#if 1
    if (speed > 30 && speed < 200)
    {
        if ((speed < 30 || speed >= 18000)&& speedchanged == 0)
        {
            speedchanged = 1;
            pwm_mod1 = pwm_mod1 + 0.1;
            if (pwm_mod1 > 8.1)
            {
                pwm_mod1 = 8.1;
            }
        }
        else if (speed > 70 && speedchanged == 0)
        {
            speedchanged = 1;
            pwm_mod1 = pwm_mod1 - 0.2;
            if (pwm_mod1 < 7.9)
            {
                pwm_mod1 = 7.9;
            }
        }

    }
#endif
    //Take avg here

}

void period_10Hz(void)
{
    //get time_f values
    counter++;
    final_time = final_time + time_f;
    // printf("Time_f=%f\n",time_f);

    if (counter == 100)
    {
        final_time = final_time / 100;
        //  printf("final_time=%f\n",final_time);
        time_f = 0;
        counter = 0;
    }
}
void period_100Hz(void)
{
    DRIVER_TX_MOTOR_CMD_t to;
    uint64_t *from;
    msg_hdr_t hdr;

    static int currentval = 0;
    static int oldval =9;

#if 1
    //Should add the functionality to control the motor speed using the speed check

    DC_Motor &dc_motor_instance = DC_Motor::getInstance();
    Steer_Motor &steer = Steer_Motor::getInstance();

    /*Initializing DC motor*/
    static int flag = 0;
    if (flag == 0)
    {
        printf("in if\n");
        dc_motor_instance.setDriveMotor(7.5); //Initialize the motor
        steer.setSteerMotor(7);
        flag++;
    }

    /* Move forward -*/
    while (CAN_rx(can1, &control, 0))
    {

        if (control.msg_id == 0x020)
        {
            from = (uint64_t *) &control.data;
            hdr.dlc = control.frame_fields.data_len;
            hdr.mid = control.msg_id;
            //printf("%d\n", control.data.bytes[0]);
            if (DRIVER_TX_MOTOR_CMD_decode(&to, from, &hdr))
            {
                //  printf("%d\n", to.MOTOR_CMD_steer);
                LD.setNumber(to.MOTOR_CMD_steer);
                currentval = to.MOTOR_CMD_drive;

                if(currentval != oldval)
                {
                    oldval = currentval;
                    switch(currentval)
                    {
                        case 1:
                        pwm_mod1 = 7.9;
                        break;
                        case 2:
                        pwm_mod1 = 8.1;
                        break;
                        default:
                        pwm_mod1 = 7.5;
                        break;
                    }

                }

                if (straight == to.MOTOR_CMD_steer) //go forward - 000
                {
                    //printf("Motor_CMD_Steer = %d",to.MOTOR_CMD_steer);
                    //printf("Straight\n");
                    LE.toggle(1);
                    dc_motor_instance.setDriveMotor(pwm_mod1);
                    //dc_motor_instance.setDriveMotor(7.5);
                    steer.setSteerMotor(7);
                }
                else if (left == to.MOTOR_CMD_steer) //go left 001, 011
                {
                    LE.toggle(2);
                    //printf("Left\n");
                    steer.setSteerMotor(SL);
                    dc_motor_instance.setDriveMotor(pwm_mod1);
                }
                else if (right == to.MOTOR_CMD_steer) //go right 010, 100, 110
                {
                    LE.toggle(3);
                    //printf("Right\n");
                    steer.setSteerMotor(SR);
                    dc_motor_instance.setDriveMotor(pwm_mod1);
                }
                else if (reverse == to.MOTOR_CMD_steer) //go back 111, 101
                {
                    LE.toggle(4);
                    // printf("Reverse\n");
                    dc_motor_instance.setDriveMotor(7.5);
                    dc_motor_instance.setDriveMotor(down);
                    steer.setSteerMotor(SR);
                }
                else if (stop == to.MOTOR_CMD_steer)
                {
                    //printf("Stop\n");
                    dc_motor_instance.setDriveMotor(7.5);
                    steer.setSteerMotor(7);
                }
            }
            else
            {
                LD.setNumber(5);
            }
        }
    }
    if (speedchanged == 1)
    {
        dc_motor_instance.setDriveMotor(pwm_mod1);
        //steer.setSteerMotor(7);
        speedchanged = 0;
    }

#endif
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
    /*The interrupts rising and falling edge will act as SW1 and SW2*/
    //In progress - this will be replaced with the input from IR sensors
    rpm.setAsInput(); // Use the pin as output pin

    bool value = rpm.read();

    if (value)
    {
        //LE.on(4);
        if (last_seen == 1)
        {
            time_f = elapsed_time;
        }
        start_time = (int) sys_get_uptime_ms();
        last_seen = 0;
    }
    else
    {
        elapsed_time = (int) sys_get_uptime_ms() - start_time;
        last_seen = 1;
    }
}
