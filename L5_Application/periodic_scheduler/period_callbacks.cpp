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
#include "can_periodic/canperiodicext.hpp"
#include "Can.h"
#include "_can_dbc/can_dbc.h"
#include "uart2.hpp"
#include "utilities.h"
#include "stdio.h"
#include "string.h"
#include "LED.hpp"
#include "stdlib.h"

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);
float latval = 0.0;
float longval = 0.0;
void uart2putstr(char* c_string);
void uart2putch(char out);

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

can_msg_t msg;
msg_hdr_t hdr;
uint64_t *from;
SENSOR_TX_SONARS_t val;
SENSOR_TX_sensorback_t back_sensor;
/*****GPS data*****/
GPS_TX_COMPASS_t compass;
GPS_TX_GPS_longitude_t longitude;
GPS_TX_GPS_latitude_t latitude;
GPS_TX_GPS_heading_t heading;
GPS_TX_GPS_dest_reached_t dest_reached;
int isgpsstarted = 0;
static float mylatval = 0;
    static float mylongval = 0;
void period_1Hz(void)
{
    //LE.toggle(1);
   // canheartbeat();
    Uart2& uart2 = Uart2::getInstance();
    static int counter = 0;
    if(counter%15==0)
    {
        uart2.printf("\r\nlsensorval\r\n");
        char bufi[200];
        sprintf(bufi,"\r\n%f\r\n",val.m0.SENSOR_SONARS_left);
        uart2.printf("%s",bufi);
    }else if(counter%16==0)
    {
        uart2.printf("\r\nmsensorval\r\n");
        char bufi[200];
        sprintf(bufi,"\r\n%f\r\n",val.m0.SENSOR_SONARS_middle);
        uart2.printf("%s",bufi);
    }
    else if(counter%17==0)
    {
        uart2.printf("\r\nrsensorval\r\n");
        char bufi[200];
        sprintf(bufi,"\r\n%f\r\n",val.m0.SENSOR_SONARS_right);
        uart2.printf("%s",bufi);
    }
    else if(counter%18==0)
    {
        uart2.printf("\r\nbsensorval\r\n");
        char bufi[200];
        sprintf(bufi,"\r\n%f\r\n",back_sensor.SENSOR_BACK_cmd);
        uart2.printf("%s",bufi);
    }
    else if(counter%19==0)
    {
        uart2.printf("\r\ncompassval\r\n");
        char bufi[200];
        sprintf(bufi,"\r\n%f\r\n",compass.COMPASS_angle);
        uart2.printf("%s",bufi);
    }
    counter++;
   /* printf("cur lat is %04f long is %04f",latval,longval);
                                uart2.printf("\r\nlat\r\n");
                                char latbuf[100];
                                sprintf(latbuf,"\r\n%f\r\n",latval);
                                uart2.printf(latbuf);
                                uart2.printf("\r\nlong\r\n");
                                char longbuf[100];
                                sprintf(longbuf,"\r\n%f\r\n",longval);
                                uart2.printf(longbuf);*/
}
int is200received = 0;
int isA1received  = 0;
int isB0received = 0;
int isB1received = 0;
int isB2received = 0;
int isB3received = 0;
int isB4received = 0;

void period_10Hz(void)
{
    //LE.toggle(2);

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
                        if(is200received==0)
                        {
                            printf("200 received\n");
                            is200received = 1;
                        }
                        break;
                    case 0xA1:
                        hdr.mid = msg.msg_id;
                        hdr.dlc = msg.frame_fields.data_len;
                        from = (uint64_t *) &msg.data;

                        if (!SENSOR_TX_sensorback_decode(&back_sensor, from, &hdr))
                            printf("\nSensor back Decode failed");

                        if(isA1received==0)
                        {
                            printf("0xa1 received\n");
                            isA1received = 1;
                        }
                        break;
                    case 0xB0:
                        hdr.mid = msg.msg_id;
                        hdr.dlc = msg.frame_fields.data_len;
                        from = (uint64_t *) &msg.data;

                        isgpsstarted = 1;
                        if(isB0received==0)
                        {
                            printf("0xb0 received\n");
                            isB0received = 1;
                        }
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
                        if(isB1received==0)
                        {
                            printf("0xb1 received\n");

                            isB1received = 1;
                        }
                        if(1)
                        {float a;

                        memcpy(&a, &longitude.GPS_longitude_cur, sizeof(float));
                        mylongval=a;

                        //printf("%f", mylongval);
                        }
                        break;

                    case 0xB2:
                        hdr.mid = msg.msg_id;
                        hdr.dlc = msg.frame_fields.data_len;
                        from = (uint64_t *) &msg.data;

                        if (!GPS_TX_GPS_latitude_decode(&latitude, from, &hdr))
                            printf("\nLatitude Decode failed");
                        if(isB2received==0)
                        {
                            printf("0xb2 received\n");
                            isB2received = 1;


                        }
                        if(1){

                        float a;

                        memcpy(&a, &latitude.GPS_latitude_cur, sizeof(float));
                        mylatval=a;
                        //printf("%f", latval);
                        }
                        break;
                    case 0xB3:
                        hdr.mid = msg.msg_id;
                        hdr.dlc = msg.frame_fields.data_len;
                        from = (uint64_t *) &msg.data;

                        if (!GPS_TX_GPS_heading_decode(&heading, from, &hdr))
                            printf("\nHeading Decode failed");
                        if(isB3received==0)
                        {
                           printf("0xb3 received\n");
                           isB3received = 1;
                        }
                        break;
                    case 0xB4:
                        hdr.mid = msg.msg_id;
                        hdr.dlc = msg.frame_fields.data_len;
                        from = (uint64_t *) &msg.data;
                        //printf(" %d %d",hdr.mid,hdr.dlc);
                        if (!GPS_TX_GPS_dest_reached_decode(&dest_reached, from, &hdr));
                            //printf("\nDest reached Decode failed");
                        if(isB4received==0)
                        {
                            printf("0xb4 received\n");
                            isB4received = 1;
                        }
                        break;
                    default:
                        //printf("Unknown message received, msg id %d\n", (int) msg.msg_id);
                        printf("U");
                        break;
                }
            }
}

void period_100Hz(void)
{
    //LE.toggle(3);

    Uart2& uart2 = Uart2::getInstance();

    static char c;
    static char buf[200];
    static int bufcount = 0;
    static int state = 0;
    static int gpscoordsstate = 0;
    static char floatbuf[40];
    static int floatcount = 0;
    static int npoints = 0;
    static int pointscount = 0;
    static float latval[50];

    static float longval[50];
    static int currentval = 0;
    switch(gpscoordsstate)
           {
               case 1:
                   msg_hdr_t hdr;
                    uint64_t *to;
                    IO_TX_IOCHECKPOINTCOUNT_t fromb;
                    fromb.IOCOUNT_cmd= npoints;
                    pointscount = npoints;
                    can_msg_t msg1;
                    to = (uint64_t *)&msg1.data;
                    hdr = IO_TX_IOCHECKPOINTCOUNT_encode(to,&fromb);
                    msg1.msg_id = hdr.mid;
                    msg1.frame_fields.data_len = hdr.dlc;
                    if(!CAN_tx(can1, &msg1, 10))
                        printf("Can tx failed");
                    gpscoordsstate = 2;
                    break;
               case 2:


                   if(currentval<npoints)
                   {
                       IO_TX_IOGPSCHECKPOINT_t fromc;

                       char aa[4];
                       char bb[4];
                       memcpy(aa,(&latval[currentval]),4);

                       memcpy(bb,(&longval[currentval]),4);


                      // fromc.IOLATITUDE = latval[currentval];
                       //fromc.IOLONGTITUDE = longval[currentval];
               //        memcpy(&fromc.IOLONGTITUDE,(&longval[currentval]),4);
                       can_msg_t msg1;

                msg1.data.bytes[0] = aa[0];
                msg1.data.bytes[1] = aa[1];
                msg1.data.bytes[2] = aa[2];
                msg1.data.bytes[3] = aa[3];

                msg1.data.bytes[4] = bb[0];
                msg1.data.bytes[5] = bb[1];
                msg1.data.bytes[6] = bb[2];
                msg1.data.bytes[7] = bb[3];

                 //      to = (uint64_t *)&msg1.data;
             //          hdr = IO_TX_IOGPSCHECKPOINT_encode(to,&fromc);
                       msg1.msg_id = 0x04;
                       msg1.frame_fields.data_len = 8;
                       if(!CAN_tx(can1, &msg1, 10))
                       {
                           printf("can fail");
                       }
                       printf("sent %f %f %f\n",currentval, latval[currentval],longval[currentval]);
                       currentval ++;
                   }
                   else
                   {
                       gpscoordsstate = 3;
                       currentval = 0;
                       printf("Sent all points\n");
                       npoints = 0;
                   }
                   break;

            }

    if(uart2.getChar(&c,0))
    {
       printf("%c",c);


       switch(state)
       {
           case 1:
           {
               if(c!=',')
               {
                   floatbuf[floatcount++] = c;
               }
           }
           break;
           case 2:
           {
               if(c!=')')
               {
                   floatbuf[floatcount++] = c;
               }
           }
           break;

       }
       if(c=='(')
       {
           state = 1;
           floatcount = 0;
           memset(floatbuf,0,40);

       }
       if(c==')')
       {
           state = 0;
           floatbuf[floatcount]='\0';
                      float longv = atof(floatbuf);
                      //printf("\nlong: %f :%s\n",longv,floatbuf);
                      floatcount = 0;
                      memset(floatbuf,0,40);
                      longval[npoints++] = (-1)*longv;
       }
       if(c==',')
       {
           state = 2;
           floatbuf[floatcount]='\0';
           float lat = atof(floatbuf);
           //printf("\nlat: %f %s\n",lat,floatbuf);
           floatcount = 0;
           memset(floatbuf,0,40);
           latval[npoints] = lat;
       }
        if(c!='*'&& ((c>='a'&&c<='z')||(c>'A'&&c<'Z')))
       {
            buf[bufcount++] = c;
       }
       else if(c=='*')
       {
            buf[bufcount++] = '\0';
            char *pch = strstr(buf, "Hello");
            if(pch)
            {
                //uart2.printf("\r\nhi1\r\n");
                //printf("hi");
                printf("cur lat is %04f long is %04f",mylatval,mylongval);
                                                uart2.printf("\r\nlat\r\n");
                                                char latbuf[100];
                                                sprintf(latbuf,"\r\n%f\r\n",mylatval);
                                                uart2.printf(latbuf);
                                                uart2.printf("\r\nlong\r\n");
                                                char longbuf[100];
                                                sprintf(longbuf,"\r\n%f\r\n",mylongval);
                                                uart2.printf(longbuf);
                LED::getInstance().toggle(0);
            }
            pch = strstr(buf,"start");
            if(pch)
            {
                uart2.printf("\r\ngetval\r\n");
            }
            pch = strstr(buf,"stop");
            if(pch)
            {
                printf("received %d points",npoints);
                gpscoordsstate = 1;


            }
            pch = strstr(buf,"Good");
            if(pch)
            {
                printf("Lets start\n");
                msg_hdr_t hdr;
                    uint64_t *to;

                    IO_TX_IOSTART_t fromb;
                    fromb.IOSTART_cmd = 1;
                    can_msg_t msg1;
                        to = (uint64_t *)&msg1.data;
                        hdr = IO_TX_IOSTART_encode(to,&fromb);
                        msg1.msg_id = hdr.mid;
                        msg1.frame_fields.data_len = hdr.dlc;
                        CAN_tx(can1, &msg1, 10);


            }
            pch = strstr(buf,"Bad");
            if(pch)
            {
                printf("Lets stop\n");
                msg_hdr_t hdr;
                uint64_t *to;
                IO_TX_IOSTOP_t fromc;
                fromc.IOSTOP_cmd = 1;
                                    can_msg_t msg1;
                                        to = (uint64_t *)&msg1.data;
                                        hdr = IO_TX_IOSTOP_encode(to,&fromc);
                                        msg1.msg_id = hdr.mid;
                                        msg1.frame_fields.data_len = hdr.dlc;
                                        CAN_tx(can1, &msg1, 10);
            }
            bufcount = 0;
            memset(buf,'0',100);

       }
    }
}

void period_1000Hz(void)
{

            if (LS.getPercentValue() < 20){
            LPC_GPIO2->FIOSET = (1 << 1);
            LPC_GPIO2->FIOSET = (1 << 2);
            }
            else{
            LPC_GPIO2->FIOCLR = (1 << 1);
            LPC_GPIO2->FIOCLR = (1 << 2);
            }

}
