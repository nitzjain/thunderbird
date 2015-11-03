#ifndef _MOTOR_DIRECTION_HPP_
#define _MOTOR_DIRECTION_HPP_


#include "tasks.hpp"
#include <can.h>
#include "string.h"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "stdio.h"
#include "storage.hpp"


//need to update this value based on sensor controller data
#define  SENSOR_THRESHOLD 40
#define MOVE_STRAIGHT 0
#define MOVE_RIGHT 1
#define MOVE_LEFT 2
#define MOVE_REVERSE 3
#define STOP -1


/**
 * Can IDs for each direction
 */

typedef enum direction {
    straight = 0X020,
    left = 0x021,
    right = 0X022,
    stop = 0X023,
    reverse = 0X024,

}direction_t;


//To compute sensor values and decide the direction

char direction_computation();



extern int sensor_left, sensor_stright, sensor_right, sensor_reverse;
extern int gps_direction;

class update_sensor_values: public scheduler_task
{
    private:
    public:
        update_sensor_values(uint8_t priority) :
                scheduler_task("update_sensor_values", 1024, priority)
        {
                //NOTHING
        }

        bool init()
        {
            //TODO: Change setRunDuration based on sensor team frequency
            setRunDuration(50);
            return true;
        }

        bool run(void *p)
        {
            const can_t mycan = can1;
            can_msg_t msg;
            memset(&msg,0,sizeof(msg));

            if(CAN_rx(can1, &msg, portMAX_DELAY))
            {
                if (msg.msg_id == 0x001 ) {  //TODO: Change CANID based on sensor team input
                    sensor_left = msg.data.bytes[0];
                    sensor_stright = msg.data.bytes[1];
                    sensor_right = msg.data.bytes[2];
                    sensor_reverse = msg.data.bytes[3];
                }
            }
            else
            {
                Storage::append("log_messages", "LOG: Update Sensor Failed", 40, 0);
                CAN_reset_bus(mycan);
                if(CAN_is_bus_off(mycan))
                {
                    Storage::append("log_messages", "LOG: Can Bus OFF", 40, 0);
                    CAN_reset_bus(mycan);
                }
            }
            return true;
        }
};


#endif // _MOTOR_DIRECTION_HPP_
