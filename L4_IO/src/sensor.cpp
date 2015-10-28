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

//eint3_enable_port2(uint8_t pin_num, eint_intr_t type, void_func_t func);

void trigger_LeftSensor()
{
    LPC_GPIO2->FIODIR |= (1 << 0);
    LPC_GPIO2->FIOCLR = (1 << 0);
    LPC_GPIO2->FIOSET = (1 << 0);
    delay_us(25);
    LPC_GPIO2->FIOCLR = (1 << 0);
    printf("left sensor triggered\n");
    SysTimeL = (int)sys_get_uptime_us();
    printf("SysTimeLeft is %i\n",SysTimeL );
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
        start_time = (int) sys_get_cpu_clock();
        printf("Rise edge interrupt enabled\n");
}

void leftSensorfallEdge()
{

    PW_Left = (int) sys_get_cpu_clock() - start_time;
    printf("Fall edge interrupt enabled\n");
    LeftSensorDistance = PW_Left/147;
    printf("distance is %i: \n",LeftSensorDistance);
}



