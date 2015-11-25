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

int SysTimeL=0; int start_time =0; int PW_Left=0; int PW_Right; int PW_Mid; int LeftSensorDistance = 0;
int end_time;


void trigger_LeftSensor()
{
    LPC_PINCON->PINSEL4 &= ~(15<<0);

    LPC_GPIO2->FIODIR |= (1 << 0);
    LPC_GPIO2->FIOCLR = (1 << 0);
    LPC_GPIO2->FIOSET = (1 << 0);
    LPC_GPIO2->FIODIR &= ~(1 << 1); //set as input
}

void InitInterruptLeft()
{
    eint3_enable_port2(1, eint_rising_edge, eintCallbackleft_Rise);
    eint3_enable_port2(1, eint_falling_edge, eintCallbackleft_Fall);
}

void eintCallbackleft_Rise()
{
        start_time = (int) sys_get_uptime_us();
}

void eintCallbackleft_Fall()
{
    PW_Left = ((int) sys_get_uptime_us() - start_time)/147;
  //  delay_ms(50);

}


int GetLeftSensorReading()
{
    int i=0;  int leftSenVal[10];

    trigger_LeftSensor();
        while(i<10)
        {
            leftSenVal[i]=PW_Left;
           // delay_ms(10);
           // printf("%i left value is: %i\n ",i,leftSenVal[i]);
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



void InitInterruptRight()
{
    eint3_enable_port2(5, eint_rising_edge, eintCallbackright_Rise);
    eint3_enable_port2(5, eint_falling_edge, eintCallbackright_Fall);
}




void eintCallbackright_Rise()
{
        start_time = (int) sys_get_uptime_us();
}

void eintCallbackright_Fall()
{
    PW_Right= ((int) sys_get_uptime_us() - start_time)/147;
    //delay_ms(50);

}

int GetRightSensorReading()
{
    int i=0;  int rightSenVal[10];
  //  delay_ms(250);
     trigger_RightSensor();

        while(i<10)
        {
            rightSenVal[i] = PW_Right;
           // delay_ms(10);
           // printf("%i Right value is: %i\n ",i,rightSenVal[i]);
            i++;
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
     LPC_GPIO2->FIODIR &= ~(1 << 3); //set as input

}
void InitInterruptMid()
{
    eint3_enable_port2(3, eint_rising_edge, eintCallbackmid_Rise);
    eint3_enable_port2(3, eint_falling_edge, eintCallbackmid_Fall);
}

void eintCallbackmid_Rise()
{
        start_time = (int) sys_get_uptime_us();
}

void eintCallbackmid_Fall()
{
    PW_Mid = ((int) sys_get_uptime_us() - start_time)/147;
  //  delay_ms(50);

}

int GetMidSensorReading()
{
    int i=0;  int MidSenVal[10];
    trigger_MidSensor();

        while(i<10)
        {
            MidSenVal[i]=PW_Mid;
//            delay_ms(10);
//            printf("%i Mid value is: %i\n ",i,MidSenVal[i]);
            i++;
        }
       // printf("element is %i\n",leftSenVal[9]);
        LPC_GPIO2->FIOCLR = (1 << 2);
        return MidSenVal[9];
}



