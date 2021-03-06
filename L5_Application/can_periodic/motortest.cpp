#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "lpc_sys.h"
#include "can.h"
#include "gpio.hpp"
#include "file_logger.h"
#include "string.h"
#include "io.hpp"


typedef enum direction {
    stop = 0,
    straight = 1,
    left = 2,
    right = 3,
    reverse = 4,
};
const int switchdebounce = 1; // 50 ns debounce delay must be present. adjust value based on periodic task.
const can_t mycan = can1;
void testmotorwithswitch()
{
            static int debounce = 1;
            if(debounce == switchdebounce)
            {
                can_msg_t msg;
                memset(&msg,0,sizeof(msg));
                msg.frame = 0;
                msg.frame_fields.is_29bit = 0;
                msg.frame_fields.data_len = 0;

                //char dir = direction_computation(sensor1, sensor2, sensor3, sensor4);

                if (SW.getSwitch(1))
                {
                    msg.msg_id = straight;
                    if(CAN_tx(mycan,&msg,100)) {
                     printf("Straight\n");
                    }

                }
                else if(SW.getSwitch(2))
                {
                    msg.msg_id = stop;
                    if(CAN_tx(mycan,&msg,100)) {
                     printf("Right\n");
                   }

                }
                else if(SW.getSwitch(3))
                {
                    msg.msg_id = left;
                    if(CAN_tx(mycan,&msg,100)) {
                     printf("Left\n");
                   }

                }
                else if(SW.getSwitch(4))
                {
                    msg.msg_id = reverse;
                    if(CAN_tx(mycan,&msg,100)) {
                     printf("Reverse\n");
                   }

                }
            }
            //debounce = ( debounce + 1 ) % (switchdebounce +1);
}
