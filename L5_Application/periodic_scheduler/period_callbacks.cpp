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
#include "_can_dbc\auto_gen.inc"
#include <stdint.h>
#include "stdio.h"
#include "io.hpp"
#include "periodic_callback.h"
#include "gps_datatype.h"
#include "tasks.hpp"
#include <stdint.h>
#include "compass.hpp"
#include "can.h"

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);
int calculate_sector();


void period_1Hz(void)
{

    LE.toggle(1);
}

void period_10Hz(void)
{
    //printf("10");
    //static QueueHandle_t gps_data_q = scheduler_task::getSharedObject("gps_queue");
    //gps_data_t data_gps;

    /*if (NULL == gps_data_q) {
        printf("data-load error");
    }
    else if (xQueueReceive(gps_data_q, &data_gps, 0))
    {
        printf("longitude: %f",data_gps.Longitude);
    }
*/
    static QueueHandle_t compass_data_q = scheduler_task::getSharedObject("compass_queue");
    compass_data_t data_compass;

    if (NULL == compass_data_q) {
        printf("data-load error");
    }
    else if (xQueueReceive(compass_data_q, &data_compass, 0))
    {
        printf("yaw: %f",data_compass.yaw);
    }
    //calculate_desired_angle();
    //int sector = calculate_sector();
    /*
    can_msg_t msg;
    GPS_TX_COORDINATES_t gps_c;
        gps_c.GPS_LAT1 = 8;
        gps_c.GPS_LAT2 = -2;
        msg_hdr_t h = GPS_TX_COORDINATES_encode((uint64_t*)&msg.data, &gps_c);
        msg.msg_id = h.mid;
        msg.frame_fields.data_len = h.dlc;
        CAN_tx(can1, &msg, 0);
    */
}
void period_100Hz(void)
{
    LE.toggle(3);
}

void period_1000Hz(void)
{
    LE.toggle(4);
}
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


