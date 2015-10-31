#ifndef CANPERIODIC_HPP
#define CANPERIODIC_HPP

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "lpc_sys.h"
#include "can.h"
#include "gpio.hpp"
#include "file_logger.h"
#include "string.h"
#include "io.hpp"

/* CAN receive message buffer queue size */
#define CANBUSBAUDRATE  (100)
#define CANBUSQUEUESIZE (32*8)
#define TENSECONDS      10

/* LED 1 to 4 */
#define P1_0  ((1 << 5) | 0)
#define P1_1  ((1 << 5) | 1)
#define P1_4  ((1 << 5) | 4)
#define P1_8  ((1 << 5) | 8)

/* LED's to indicate the CAN Bus operation */
GPIO gpioop((LPC1758_GPIO_Type)P1_0);
GPIO gpiogps((LPC1758_GPIO_Type)P1_1);
GPIO gpiomotor((LPC1758_GPIO_Type)P1_4);
GPIO gpiosensor((LPC1758_GPIO_Type)P1_8);
GPIO gpiobridge((LPC1758_GPIO_Type)P1_0);

/* Constant Global variable */
const can_t mycan = can1;

/* Controller's list */
typedef enum proccanmesgid
{
    master=0xA0,
    gps = 0xA1,
    motor = 0xA2,
    sensor = 0xA3,
    bridge = 0xA4,
} proccanmesgid;

/* Controller ID */
typedef enum procarrayid
{
    masterarrayid=0,
    gpsarrayid = 1,
    motorarrayid = 2,
    sensorarrayid = 3,
    bridgearrayid = 4,
} procarrayid;

/* Number of seconds to wait for a CAN heart beat message */
//const uint8_t hzmultiplier = 15;

/* Current Processor Name */
proccanmesgid myprocessor = master;

int processorcounter[5]={0,0,0,0,0};

int oldcounter[5]={0,0,0,0,0};

#endif /* CANPERIODIC_HPP */
