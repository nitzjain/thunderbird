#ifndef _MOTOR_DIRECTION_HPP_
#define _MOTOR_DIRECTION_HPP_


#include "tasks.hpp"
#include <can.h>
#include "string.h"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "stdio.h"
#include "storage.hpp"
//#include "can_periodic/canperiodic.hpp"
//#include "can_periodic/canperiodicext.hpp"

//need to update this value based on sensor controller data
#define SENSOR_THRESHOLD 60
#define MOVE_STRAIGHT 0
#define MOVE_RIGHT 1
#define MOVE_LEFT 2
#define MOVE_REVERSE 3
#define STOP 4


/**
 * Can IDs for each direction
 */

typedef enum direction {
    straight = 0X020,
    left = 0x021,
    right = 0X022,
    stop = 0X023,
    reverse = 0X024,
    start = 0x025
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
                scheduler_task("update_sensor_values", 4096, priority)
        {
                //NOTHING

        }

        bool init()
        {
            //TODO: Change setRunDuration based on sensor team frequency
            setRunDuration(80);
            const can_t mycan = can1;

            Storage::append("log_messages","--Log message file Master Controller--", 40, 0);


                if(!CAN_init(mycan,100, 32*8, 32*8, NULL,NULL))
                    return false;

                CAN_reset_bus(mycan);
                CAN_bypass_filter_accept_all_msgs();
                //logger_init(PRIORITY_CRITICAL);
            return true;
        }

        bool run(void *p)
        {
            const can_t mycan = can1;
            can_msg_t msg;

            memset(&msg,0,sizeof(msg));



            if(CAN_rx(can1, &msg, 110))
            {

                if (msg.msg_id == 0x01 ) {  //TODO: Change CANID based on sensor team input
                    sensor_left = msg.data.bytes[0];
                    sensor_stright = msg.data.bytes[1];
                    sensor_right = msg.data.bytes[2];
                    sensor_reverse = msg.data.bytes[3];
                    //printf (" %d %d %d",sensor_left, sensor_stright, sensor_right);
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
