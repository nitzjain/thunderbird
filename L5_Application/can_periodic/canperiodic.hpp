#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "lpc_sys.h"
#include "can.h"
const can_t mycan = can1;

int P1_0 = ((1 << 5) | 0);
int P1_1 =((1 << 5) | 1);
int P1_4 = ((1 << 5) | 4);
int P1_8 = ((1 << 5) | 8);

typedef enum proccanmesgid
{
    master=0xA5,
    gps = 0xA1,
    motor = 0xA2,
    sensor = 0xA3,
    bridge = 0xA4,
} proccanmesgid;
typedef enum procarrayid
{
    masterarrayid=0,
    gpsarrayid = 1,
    motorarrayid = 2,
    sensorarrayid = 3,
    bridgearrayid = 4,
} procarrayid;
const uint8_t hzmultiplier = 15;
proccanmesgid myprocessor = motor;
int processorcounter[5]={0,0,0,0,0};
int oldcounter[5]={0,0,0,0,0};
