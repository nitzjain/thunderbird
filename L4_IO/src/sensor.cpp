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

int SysTimeL=0; int start_time =0; int PW_Left=0; int LeftSensorDistance = 0;
int end_time;
bool Int_type=1;

void trigger_LeftSensor()
{
    LPC_GPIO2->FIODIR |= (1 << 0);
    LPC_GPIO2->FIOCLR = (1 << 0);
    LPC_GPIO2->FIOSET = (1 << 0);
  //  delay_us(25);
   // LPC_GPIO2->FIOCLR = (1 << 0);
   // LPC_GPIO2->FIODIR &= ~(1 << 1);
   // printf("left sensor triggered\n");
   // SysTimeL = (int)sys_get_uptime_us();
    //printf("SysTimeLeft is %i\n",SysTimeL );
}

void initLeft()
{
    trigger_LeftSensor();
    LE.init();
}

void Left_run()
{
    trigger_LeftSensor();
   // LPC_GPIO2->FIODIR |= (1 << 1);  //set as output
  //  LPC_GPIO2->FIOCLR = (1 << 1);   //clear
   // LPC_GPIO2->FIOSET = (1 << 1);   //set
   // delay_us(20);
    LPC_GPIO2->FIOCLR = (1 << 1);   //clear
    //delay_us(700);
    LPC_GPIO2->FIODIR &= ~(1 << 1); //set as input

    initPWint(eint_rising_edge);
    //delay_ms(250);


}

void trigger_CentralSensor()
{
    LPC_GPIO2->FIODIR |= (1 << 2);
    LPC_GPIO2->FIOCLR = (1 << 2);
    LPC_GPIO2->FIOSET = (1 << 2);
    delay_us(25);
    LPC_GPIO2->FIOSET = (1 << 2);
}

void trigger_RightSensor()
{
    LPC_GPIO2->FIODIR |= (1 << 4);
    LPC_GPIO2->FIOCLR = (1 << 4);
    LPC_GPIO2->FIOSET = (1 << 4);
    delay_us(25);
    LPC_GPIO2->FIOSET = (1 << 4);
}

void trigger_all_sensors()
{
   trigger_LeftSensor();
   trigger_CentralSensor();
   trigger_RightSensor();
}


void leftSensorRiseEdge()
{
        start_time = (int) sys_get_uptime_us();
        printf("Rise edge interrupt enabled with start time %i\n",start_time);
}

void leftSensorfallEdge()
{
    end_time = (int) sys_get_uptime_us();
    PW_Left = end_time - start_time;
    printf("Fall edge interrupt enabled ant end time %i\n",end_time);
    LeftSensorDistance = PW_Left/147;
    printf("distance is %i: \n",LeftSensorDistance);
}

void routine()
{
    eint3_enable_port2(1, eint_rising_edge, leftSensorRiseEdge);
        eint3_enable_port2(1, eint_falling_edge, leftSensorfallEdge);
        delay_ms(1);
}

void initPWint(eint_intr_t eintType)
{
    eint3_enable_port2(1, eintType, eintCallback);
}

void eintCallback()
{
    if(Int_type)
    {
        //LE.on(1);
        Int_type=0;
        //printf("Rise edge interrupt enabled with start time %i\n",(int) sys_get_uptime_us());
        start_time = (int) sys_get_uptime_us();
        //printf("Rise edge interrupt enabled with start time %i\n",start_time);
        initPWint( eint_falling_edge);
   }
    else
    {
        //end_time = (int) sys_get_uptime_us();
           // PW_Left = end_time - start_time;
          //  printf("Fall edge interrupt enabled at end time %i\n",end_time);
           // LeftSensorDistance = (end_time - start_time)/147;
           // printf("Fall edge interrupt enabled at end time %i\n",end_time);
            printf("distance is: %i\n",((int) sys_get_uptime_us() - start_time)/147);
            Int_type=1;
          //  delay_ms(5);


    }
}


