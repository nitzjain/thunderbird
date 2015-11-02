/*
 * sensor.c
 *
 *  Created on: Oct 26, 2015
 *      Author: Rajashree
 */

/*
 * sensor_LV.c
 *
 *  Created on: Oct 26, 2015
 *      Author: Rajashree
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
#include "examples/examples.hpp"
#include "can.h"
#include "gpio.hpp"
#include "stdio.h"
#include "io.hpp"
#include "string.h"
#include "adc0.h"
#include "utilities.h"
#include "uart2.hpp"
#include "eint.h"

void trigger_MidSensor();

int SysTimeL=0; int start_time =0; int PW_Left=0; int LeftSensorDistance = 0;
int end_time;
bool Int_type=1;


void trigger_LeftSensor()
{
    LPC_PINCON->PINSEL4 &= ~(15<<0);

    LPC_GPIO2->FIODIR |= (1 << 0);
    LPC_GPIO2->FIOCLR = (1 << 0);
    LPC_GPIO2->FIOSET = (1 << 0);
    //delay_us(25);
    LPC_GPIO2->FIODIR &= ~(1 << 1); //set as input
   // LPC_GPIO2->FIOCLR = (1 << 0);
   // LPC_GPIO2->FIODIR &= ~(1 << 1);
   // printf("left sensor triggered\n");
   // SysTimeL = (int)sys_get_uptime_us();
    //printf("SysTimeLeft is %i\n",SysTimeL );
}

//void initLeft()
//{
 //   trigger_LeftSensor();
 //   LE.init();
//}

int Left_run()
{
   // trigger_LeftSensor();
   // LPC_GPIO2->FIODIR |= (1 << 1);  //set as output
  //  LPC_GPIO2->FIOCLR = (1 << 1);   //clear
   // LPC_GPIO2->FIOSET = (1 << 1);   //set
   // delay_us(20);
    LPC_GPIO2->FIOCLR = (1 << 1);   //clear
    //delay_us(700);
 //   LPC_GPIO2->FIODIR &= ~(1 << 1); //set as input

    initPWleft(eint_rising_edge);
   // delay_ms(250);
    return PW_Left;
}

void initPWleft(eint_intr_t eintType)
{
    eint3_enable_port2(1, eintType, eintCallbackleft);
}

void eintCallbackleft()
{
    if(Int_type)
    {
        //LE.on(1);
        Int_type=0;
        //printf("Rise edge interrupt enabled with start time %i\n",(int) sys_get_uptime_us());
        start_time = (int) sys_get_uptime_us();
        //printf("Rise edge interrupt enabled with start time %i\n",start_time);
        initPWleft( eint_falling_edge);
   }
    else
    {
        //end_time = (int) sys_get_uptime_us();
            PW_Left = ((int) sys_get_uptime_us() - start_time)/147;
          //  printf("Fall edge interrupt enabled at end time %i\n",end_time);
           // LeftSensorDistance = (end_time - start_time)/147;
           // printf("Fall edge interrupt enabled at end time %i\n",end_time);

            //printf("distance left is: %i\n",PW_Left);
            //  delay_ms(5);
            Int_type=1;
          //  delay_ms(5);
    }
}


int GetLeftSensorReading()
{
    int i=0;  int leftSenVal[10];
    delay_ms(250);

    trigger_LeftSensor();
        while(i<10)
        {

            leftSenVal[i]=Left_run();
            delay_ms(50);
            i++;
        }
        LPC_GPIO2->FIOCLR = (1 << 0);

    return leftSenVal[9];
}

void trigger_RightSensor()
{

    LPC_GPIO2->FIODIR |= (1 << 4);
    LPC_GPIO2->FIOCLR = (1 << 4);
    LPC_GPIO2->FIOSET = (1 << 4);
    LPC_GPIO2->FIODIR &= ~(1 << 5); //set as input
}


int Right_run()
{

    LPC_GPIO2->FIOCLR = (1 << 5);   //clear
   // LPC_GPIO2->FIODIR &= ~(1 << 5); //set as input
    initPWright(eint_rising_edge);
    return PW_Left;
}


void initPWright(eint_intr_t eintType)
{
    eint3_enable_port2(5, eintType, eintCallbackright);
}

void eintCallbackright()
{
    if(Int_type)
    {
        Int_type=0;
        start_time = (int) sys_get_uptime_us();
        initPWright( eint_falling_edge);
   }
    else
    {
            PW_Left = ((int) sys_get_uptime_us() - start_time)/147;
            printf("Right is: %i\n",PW_Left);

         //   delay_ms(5);
            Int_type=1;
    }
}

int GetRightSensorReading()
{
    int i=0;  int rightSenVal[10];
    delay_ms(250);
    trigger_RightSensor();

        while(i<10)
        {
            //trigger_RightSensor();
            rightSenVal[i] = Right_run();
            delay_ms(50);
            i++;
            //LPC_GPIO2->FIOCLR = (1 << 4);
        }
        LPC_GPIO2->FIOCLR = (1 << 4);
       // printf("element is %i\n",leftSenVal[9]);

        return rightSenVal[9];


}

void trigger_MidSensor()
{
    LPC_GPIO2->FIODIR |= (1 << 2);
    LPC_GPIO2->FIOCLR = (1 << 2);
    LPC_GPIO2->FIOSET = (1 << 2);
}


int Mid_run()
{

    LPC_GPIO2->FIOCLR = (1 << 3);   //clear
    LPC_GPIO2->FIODIR &= ~(1 << 3); //set as input
    initPWmid(eint_rising_edge);
    return PW_Left;
}


void initPWmid(eint_intr_t eintType)
{
    eint3_enable_port2(3, eintType, eintCallbackmid);
}

void eintCallbackmid()
{
    if(Int_type)
    {
        Int_type=0;
        start_time = (int) sys_get_uptime_us();
        initPWmid( eint_falling_edge);
   }
    else
    {
            PW_Left = ((int) sys_get_uptime_us() - start_time)/147;
            printf("distance is: %i\n",PW_Left);
            Int_type=1;
    }
}

int GetMidSensorReading()
{
    int i=0;  int leftSenVal[10];
    delay_ms(250);
    trigger_MidSensor();

        while(i<10)
        {
            leftSenVal[i]=Mid_run();
            delay_ms(50);
            i++;
        }
       // printf("element is %i\n",leftSenVal[9]);
        LPC_GPIO2->FIOCLR = (1 << 2);
        return leftSenVal[9];


}



