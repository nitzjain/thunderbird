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
#include "io.hpp"
#include "periodic_callback.h"
#include "can.h"
#include "lpc_pwm.hpp"

//CAN id's
#define forward 0x120
#define reverse 0x121
#define left 0x122
#define right 0x123

#include "can_periodic/canperiodicext.hpp"


/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

//can message id
can_msg_t control;

//Initialize PWM
/*PWM1 - Controls the DC motor
 *PWM2 - Controls the Servo motor
 */
PWM pwm1(PWM::pwm1, 50);
PWM pwm2(PWM::pwm2, 50);



void period_1Hz(void)
{

}

void period_10Hz(void)
{

}

void period_100Hz(void)
{

    //CAN RX Task
    CAN_rx(can1, &control, portMAX_DELAY); //receive message to turn on the led
    if(control.msg_id == forward)
    {
        pwm1.set(8);
    }
    else if(control.msg_id == reverse)
    {
        pwm1.set(7);
    }
    else if(control.msg_id == left)
    {
        pwm2.set(7);
    }
    else if(control.msg_id == right)
    {
        pwm2.set(8);
    }
}

void period_1000Hz(void)
{

}
