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


float ToRadian(float degree);
float ToDegree(float radian);
float GetBearing(gps_data_t data_gps, gps_data_t chkp_data);
int calculate_sector(float angle);
void GetDirection(direction_t *move_to_dir,int bearing_sector,int heading_sector);
float get_bearing_new(float lat1,float lat2, float lng1, float lng2);
void period_1Hz(void)
{

    LE.toggle(1);
}

void period_10Hz(void)
{
    gps_data_t data_gps;
    gps_data_t chkp_data;
    direction_t move_to_dir;
#if 1
    static QueueHandle_t gps_data_q = scheduler_task::getSharedObject("gps_queue");
    if (NULL == gps_data_q) {
        printf("data-load error");
    }
    else if (xQueueReceive(gps_data_q, &data_gps, 0))
    {
        printf(" %f, %f :",data_gps.Longitude, data_gps.Latitude);
    }
#endif
    compass_data_t data_compass;
#if 0
    static QueueHandle_t compass_data_q = scheduler_task::getSharedObject("compass_queue");


    if (NULL == compass_data_q) {
        printf("data-load error");
    }
    else if (xQueueReceive(compass_data_q, &data_compass, 0))
    {
        printf("yaw: %f",data_compass.yaw);
    }
    //int sector = calculate_sector(GetBearing(data_gps,chkp_data),data_compass.yaw);

#endif
    chkp_data.Latitude = 37.336194;
    chkp_data.Longitude = 121.883162;
    //data_gps.Latitude = 37.336834;
    //data_gps.Longitude = 121.881977;
    int bearing_sector = calculate_sector(GetBearing(data_gps,chkp_data));
    int heading_sector = calculate_sector(3.2);//data_compass.yaw);
    printf("Data: %d, %d, %f",bearing_sector,heading_sector,GetBearing(data_gps,chkp_data));//get_bearing_new(37.336834,37.336194,121.881977,121.883162));

    if(bearing_sector == -1 || heading_sector == -1)
        printf("Error: Sector is wrong, Bearing sector = %d, Heading_sector = %d",bearing_sector,heading_sector);
    else
    GetDirection(&move_to_dir,bearing_sector,heading_sector);
    printf("direction is %d,%d",move_to_dir.angle,move_to_dir.dir);

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


