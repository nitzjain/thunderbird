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

#include <sensor.h>
#include <stdint.h>
#include "io.hpp"
#include "periodic_callback.h"
#include "stdio.h"
#include "file_logger.h"
#include "eint.h"
#include "utilities.h"
#include "can.h"
#include "_can_dbc/can_dbc.h"

extern can_msg_t msg1;

#define STARTLEFT() (LPC_GPIO2->FIOSET = (1 << 0))
#define STOPLEFT() (LPC_GPIO2->FIOCLR = (1 << 0))
#define STARTMID() (LPC_GPIO2->FIOSET = (1 << 2))
#define STOPMID() (LPC_GPIO2->FIOSET = (1 << 2))
#define STARTRIGHT() (LPC_GPIO2->FIOSET = (1 << 4))
#define STOPRIGHT() (LPC_GPIO2->FIOSET = (1 << 4))
#define STARTBACK() (LPC_GPIO2->FIOSET = (1 << 6))
#define STOPBACK() (LPC_GPIO2->FIOSET = (1 << 6))

typedef enum{
    start = 0,
    left,
    mid,
    right,
    back,
}sensorstate;


static sensorstate currentstate = start;

int leftfall = 0, midfall = 0, rightfall = 0,backfall=0;

extern int PW_Right,PW_Left,PW_Mid,PW_Back;

int leftvalue = -1, midvalue = -1, rightvalue = -1,backvalue=-1;

int issensorvaluepresent = -1;
//extern uint8_t Sen_val[3];
extern can_msg_t msg1;
#define THRESHOLD 60
/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

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

}

void period_10Hz(void)
{

}

void period_100Hz(void)
{

    if(issensorvaluepresent==1)
           {
               sendsensorvalues(leftvalue,midvalue,rightvalue,backvalue);
               issensorvaluepresent = -1;
           }

}

void period_1000Hz(void)
{
    switch(currentstate){
        case start:
            STARTLEFT();
            currentstate = left;
            leftfall = -1;
            break;
        case left:
            if(leftfall == 1){
                leftvalue = PW_Left;
                STOPLEFT();
                STARTMID();
                midfall = -1;
                currentstate = mid;
            }
            break;
        case mid:
                if(midfall == 1){
                    midvalue = PW_Mid;
                    STOPMID();
                    STARTRIGHT();
                    rightfall = -1;
                    currentstate = right;
                }
                break;
        case right:
                if(rightfall == 1){
                    rightvalue = PW_Right;
                    STOPRIGHT();
                  // STARTBACK();
                    currentstate = start;
                    issensorvaluepresent = 1;
                }
                break;
//        case back:
//                 if(backfall == 1){
//                     backvalue = PW_Back;
//                     STOPBACK();
//                     currentstate = start;
//                     issensorvaluepresent = 1;
//                 }
//                 break;

   }
}
