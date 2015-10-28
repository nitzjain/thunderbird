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

/* Current Processor Name */
proccanmesgid myprocessor = sensor;

#endif /* CANPERIODIC_HPP */
