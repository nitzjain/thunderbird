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
#include <_can_dbc/auto_gen.h>
#include <stdint.h>
#include "stdio.h"
#include "io.hpp"
#include "periodic_callback.h"
#include "gps_datatype.h"
#include "tasks.hpp"
#include <stdint.h>
#include "compass.hpp"
#include "can.h"
#include "string.h"

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

float Check_latitude[50];
float Check_longitude[50];

extern gps_data_t gps_values;
extern compass_data_t compass_values;


float ToRadian(float degree);
float ToDegree(float radian);
float GetBearing(gps_data_t data_gps, gps_data_t chkp_data);
int calculate_sector(float angle);
void GetDirection(direction_t *move_to_dir,int bearing_sector,int heading_sector);
float get_bearing_new(float lat1,float lat2, float lng1, float lng2);
float dist_to_destination(gps_data_t data_gps, gps_data chkp_data);

/*Get the n check points*/
void period_1Hz(void)
{


}

void period_10Hz(void)
{
    gps_data_t chkp_data;
    direction_t move_to_dir;
    float distance = 0;

    chkp_data.Latitude = 37.335399; ///37.3352778;
    chkp_data.Longitude =121.881348;//121.881388;
    //data_gps.Latitude = 37.336834;
    //data_gps.Longitude = 121.881977;
    int bearing_sector = calculate_sector(GetBearing(gps_values,chkp_data));
    int heading_sector = calculate_sector(compass_values.yaw);
    //printf("Data: %d, %d, %f",bearing_sector,heading_sector,GetBearing(gps_values,chkp_data));//get_bearing_new(37.336834,37.336194,121.881977,121.883162));

    if(bearing_sector == -1 || heading_sector == -1){

    }
        //printf("Error: Sector is wrong, Bearing sector = %d, Heading_sector = %d",bearing_sector,heading_sector);
    else{
    GetDirection(&move_to_dir,bearing_sector,heading_sector);
    distance = dist_to_destination(gps_values,chkp_data);
    }

    can_msg_t msg1,msg2,msg3,msg4,msg5;
        GPS_TX_COMPASS_t Compass;
        GPS_TX_GPS_longitude_t Longitude;
        GPS_TX_GPS_latitude_t Latitude;
        GPS_TX_GPS_heading_t Heading;

        GPS_TX_GPS_dest_reached_t Dest;
        Compass.COMPASS_angle = move_to_dir.angle;
        Compass.COMPASS_direction = move_to_dir.dir;

        memcpy(&Longitude.GPS_longitude_cur, &gps_values.Longitude, sizeof(float));
        memcpy(&Longitude.GPS_longitude_dst, &chkp_data.Longitude, sizeof(float));
        memcpy(&Latitude.GPS_latitude_cur, &gps_values.Latitude, sizeof(float));
        memcpy(&Latitude.GPS_latitude_dst, &chkp_data.Latitude, sizeof(float));
        memcpy(&Heading.GPS_heading_cur, &compass_values.yaw, sizeof(float));
        memcpy(&Heading.GPS_heading_dst, &move_to_dir.angle, sizeof(float));
        memcpy(&Dest.GPS_dest_reached, &distance, sizeof(float));

        msg_hdr_t h1 = GPS_TX_COMPASS_encode((uint64_t*)&msg1.data, &Compass);
        msg1.msg_id = h1.mid;
        msg1.frame_fields.data_len = h1.dlc;

        if(gps_values.Longitude != 0.0)
        //if(1)
        {
            if(CAN_tx(can1, &msg1, 0)){
            }
            else{
            }


            msg_hdr_t h2 = GPS_TX_GPS_longitude_encode((uint64_t*)&msg2.data,&Longitude);
            msg2.msg_id = h2.mid;
            msg2.frame_fields.data_len = h2.dlc;
            if(CAN_tx(can1,&msg2,0)){
            }


            msg_hdr_t h3 = GPS_TX_GPS_latitude_encode((uint64_t*)&msg3.data,&Latitude);
            msg3.msg_id = h3.mid;
            msg3.frame_fields.data_len = h3.dlc;
            if(CAN_tx(can1,&msg3,0)){
            }

            msg_hdr_t h4 = GPS_TX_GPS_heading_encode((uint64_t*)&msg4.data,&Heading);
            msg4.msg_id = h4.mid;
            msg4.frame_fields.data_len = h4.dlc;
            if(CAN_tx(can1,&msg4,0)){
            }
            msg_hdr_t h5 = GPS_TX_GPS_dest_reached_encode((uint64_t*)&msg5.data, &Dest);
            msg5.msg_id = h5.mid;
            msg5.frame_fields.data_len = h5.dlc;

            if(CAN_tx(can1, &msg5, 0)){
            }
            else{
            }
        }
}
void period_100Hz(void)
{}

void period_1000Hz(void)
{}
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


