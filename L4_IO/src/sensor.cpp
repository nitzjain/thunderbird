/*
 * sensor.c
 *
 *  Created on: Nov 26, 2015
 *      Author: Nitesh
 */

#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "utilities.h"
#include "LPC17xx.h"
#include "tasks.hpp"
//#include "examples/examples.hpp"
#include "can.h"
#include "gpio.hpp"
#include "stdio.h"
#include "io.hpp"
#include "string.h"
#include "adc0.h"
#include "utilities.h"
//#include "uart2.hpp"
#include "eint.h"
#include "file_logger.h"
#include "_can_dbc/can_dbc.h"

extern can_msg_t msg1;
int SysTimeL=0; int start_left =0; int PW_Left=0; int PW_Right; int PW_Mid; int LeftSensorDistance = 0;
int end_time;
int start_mid=0,start_right=0;
//int hasfirstrightinterruptoccured = 0;

extern int leftfall, midfall, rightfall;

//int maxf(int* a,int size)
//{
//    int i,j;
//int popular = a[0];
//int temp=0, tempCount, count=1;
//
//        for (i=0;i<size;i++)
//    {
//        tempCount = 0;
//        temp=a[i];
//        tempCount++;
//            for(j=i+1;j<10;j++)
//        {
//            if(a[j] == temp)
//            {
//                tempCount++;
//                if(tempCount > count)
//                {
//                    popular = temp;
//                count = tempCount;
//                }
//            }
//        }
//    }
//return popular;
//}
void sendsensorvalues(uint32_t l,uint32_t m,uint32_t r)
{
    msg_hdr_t hdr;
    uint64_t *to;
    SENSOR_TX_SONARS_t from;

    from.m0.SENSOR_SONARS_left = l;
    from.m0.SENSOR_SONARS_right = r;
    from.m0.SENSOR_SONARS_middle = m;
    to = (uint64_t *)&msg1.data;
    hdr = SENSOR_TX_SONARS_encode(to, &from);
    msg1.msg_id = hdr.mid;
    msg1.frame_fields.data_len = hdr.dlc;

    CAN_tx(can1, &msg1, 100);
    if(l<20)
    {
        LE.on(1);
    }
    else
    {
        LE.off(1);
    }
    if(m<0)
    {
            LE.on(2);
    }
    else
    {
            LE.off(2);
    }
    if(r<30)
    {
            LE.on(3);
    }
    else
    {
            LE.off(3);
    }
//    LOG_INFO("Left val %i",l);
   // LOG_INFO("Right val %i",r);
   // LOG_INFO("Middle val %i",m);
}
void InitInterruptLeft()
{
    LPC_GPIO2->FIODIR |= (1 << 0);
        LPC_GPIO2->FIOCLR = (1 << 0);
        LPC_GPIO2->FIODIR &= ~(1 << 1); //set as input
    eint3_enable_port2(1, eint_rising_edge, eintCallbackleft_Rise);
    eint3_enable_port2(1, eint_falling_edge, eintCallbackleft_Fall);

}

void eintCallbackleft_Rise()
{
        start_left = (int) sys_get_uptime_us();
}

void eintCallbackleft_Fall()
{
    PW_Left = ((int) sys_get_uptime_us() - start_left)/147;
    leftfall = 1;
    start_left=0;
}


//int GetLeftSensorReading()
//{
//    int i=0;  int leftSenVal[10];
//
//    LPC_GPIO2->FIOSET = (1 << 0);
//        while(i<10)
//        {
//            leftSenVal[i]=PW_Left;
//           // delay_ms(10);
//           // printf("%i left value is: %i\n ",i,leftSenVal[i]);
//            i++;
//        }
//        LPC_GPIO2->FIOCLR = (1 << 0);
//
//    return maxf(leftSenVal,10);
//}




void InitInterruptRight()
{
    LPC_GPIO2->FIODIR |= (1 << 4);
    LPC_GPIO2->FIOCLR = (1 << 4);
    LPC_GPIO2->FIODIR &= ~(1 << 5); //set as input
    eint3_enable_port2(5, eint_rising_edge, eintCallbackright_Rise);
    eint3_enable_port2(5, eint_falling_edge, eintCallbackright_Fall);
}




void eintCallbackright_Rise()
{
        start_right = (int) sys_get_uptime_us();
}

void eintCallbackright_Fall()
{
    PW_Right= ((int) sys_get_uptime_us() - start_right)/147;
    //delay_ms(50);
    rightfall = 1;
    start_right=0;
}

//int GetRightSensorReading()
//{
//    int i=0,j=0;  int rightSenVal;
//  //  delay_ms(250);
//    LPC_GPIO2->FIOSET = (1 << 4);
//    hasfirstrightinterruptoccured = 0;
////        while(i<10&&j<1000)
////        {
////            rightSenVal[i] = 0;
//            if(hasfirstrightinterruptoccured==1)
//            {
//                rightSenVal = PW_Right;
//                hasfirstrightinterruptoccured = 0;
//
//           // delay_ms(10);
//           // printf("%i Right value is: %i\n ",i,rightSenVal[i]);
//            }
//            // i++;
////        }
//        LPC_GPIO2->FIOCLR = (1 << 4);
//       // printf("element is %i\n",leftSenVal[9]);
//
//        return rightSenVal;
//
//
//}

void InitInterruptMid()
{
    LPC_GPIO2->FIODIR |= (1 << 2);
        LPC_GPIO2->FIOCLR = (1 << 2);
         LPC_GPIO2->FIODIR &= ~(1 << 3); //set as input
    eint3_enable_port2(3, eint_rising_edge, eintCallbackmid_Rise);
    eint3_enable_port2(3, eint_falling_edge, eintCallbackmid_Fall);
}

void eintCallbackmid_Rise()
{
        start_mid = (int) sys_get_uptime_us();
}

void eintCallbackmid_Fall()
{
    PW_Mid = ((int) sys_get_uptime_us() - start_mid)/147;
  //  delay_ms(50);
    midfall = 1;
    start_mid=0;

}

//int GetMidSensorReading()
//{
//    int i=0;  int MidSenVal[10];
//    LPC_GPIO2->FIOSET = (1 << 2);
//
//        while(i<10)
//        {
//            MidSenVal[i]=PW_Mid;
////            delay_ms(10);
////            printf("%i Mid value is: %i\n ",i,MidSenVal[i]);
//            i++;
//        }
//       // printf("element is %i\n",leftSenVal[9]);
//        LPC_GPIO2->FIOCLR = (1 << 2);
//        return maxf(MidSenVal,10);
//
//
//}



