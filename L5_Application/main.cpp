/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */
/*
This is the gpssensor branch
*/
/**
 * @file
 * @brief This is the application entry point.
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */

#include <_can_dbc/auto_gen.h>
#include "uart2.hpp"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include "stdio.h"
#include "io.hpp"
#include "file_logger.h"
#include "gps_datatype.h"
#include "string.h"
#include "compass.hpp"
#include "switches.hpp"
#include "math.h"
#include "can.h"
/**
 * The main() creates tasks or "threads".  See the documentation of scheduler_task class at scheduler_task.hpp
 * for details.  There is a very simple example towards the beginning of this class's declaration.
 *
 * @warning SPI #1 bus usage notes (interfaced to SD & Flash):
 *      - You can read/write files from multiple tasks because it automatically goes through SPI semaphore.
 *      - If you are going to use the SPI Bus in a FreeRTOS task, you need to use the API at L4_IO/fat/spi_sem.h
 *
 * @warning SPI #0 usage notes (Nordic wireless)
 *      - This bus is more tricky to use because if FreeRTOS is not running, the RIT interrupt may use the bus.
 *      - If FreeRTOS is running, then wireless task may use it.
 *        In either case, you should avoid using this bus or interfacing to external components because
 *        there is no semaphore configured for this bus and it should be used exclusively by nordic wireless.
 */
#define gps_baudrate 38400
#define compass_baudrate 57600
#define PI 3.1415926535

gps_data_t gps_values;
compass_data_t compass_values;

class Gps_Task : public scheduler_task
{
    private:
        Uart2 &gps_uart;
        static const int rx_q = 100;
        static const int tx_q = 100;
        char * token[20];
        char *temp;
        int i = 0;
        gps_data_t gps_decimal;
        QueueHandle_t gps_data_q;

    public:

        Gps_Task(uint8_t priority) :
           scheduler_task("Gps", 512*8, priority),
            gps_uart(Uart2::getInstance())
        {
            gps_uart.init(gps_baudrate,rx_q,tx_q);
        }

        float convert_to_decimal(float cordinate){
            int deg,minute,second;
            float second_float;
            deg = (int) cordinate/100;                       //Get degree
            minute = (int)cordinate - deg*100;        //Get minute
            second_float = cordinate - (int)cordinate;    //Get Seconds
            second = (int) (second_float * 60);
            cordinate = deg+(float)minute/60+(float)second/3600;
            return cordinate;
        }

        bool run(void *p)
        {
            i = 0;
            const char s[2] = ",";
            char rx_buff[100];
            char rx_str[100];
            gps_uart.gets(rx_buff, sizeof(rx_buff), portMAX_DELAY);         //get data from GPS module
            strcpy(rx_str,rx_buff);                                         //copy the GPS data to a temporary string
            temp = strtok(rx_str,s);                                        //Separate the data by ','
            while(temp!=NULL && i<20){
                token[i++] = temp;
                temp = strtok(NULL,s);
                //printf("token%d is %s \n",i-1,token[i-1]);                //print the separated variables
            }
            sscanf(token[2],"%f",&gps_values.Latitude);
            sscanf(token[4],"%f",&gps_values.Longitude);

            gps_values.Latitude = convert_to_decimal(gps_values.Latitude);
            gps_values.Longitude = convert_to_decimal(gps_values.Longitude);

            printf("Lat %f, %f\n",gps_values.Latitude,gps_values.Longitude);

            return true;
        }
};
class Compass_Task : public scheduler_task
{
    private:
        Uart3 &compass_uart;
        static const int rx_q = 100;
        static const int tx_q = 100;
        char * token[4];
        char *temp;
        int i = 0;
        QueueHandle_t compass_data_q;
    public:
        Compass_Task(uint8_t priority) :
            scheduler_task("compass", 512*8, priority),
            compass_uart(Uart3::getInstance())
        {
            compass_uart.init(compass_baudrate,rx_q,tx_q);
        }

        bool run(void *p)
        {
            i = 0;
            const char s[2] = ",";
            const char t[2] = "=";
            char rx_buff[100];
            char rx_str[100];
            compass_uart.gets(rx_buff, sizeof(rx_buff), portMAX_DELAY);         //get data from GPS module
            strcpy(rx_str,rx_buff);                                         //copy the GPS data to a temporary string
            temp = strtok(rx_str,t);                                        //Separate the data by ','
                while(temp!=NULL && i<4){
                    token[i++] = temp;
                    temp = strtok(NULL,s);
                    //printf("token%d is %s \n",i-1,token[i-1]);                //print the separated variables
                }

            sscanf(token[1],"%f",&compass_values.yaw);

            return true;
        }
};

int main(void)
{
    CAN_init(can1, 250, 200, 200, NULL, NULL); //initialize can bus 1
    CAN_reset_bus(can1); //resets the CAN bus*/
    CAN_bypass_filter_accept_all_msgs(); //accept all messages

    scheduler_add_task(new Gps_Task(PRIORITY_MEDIUM));
    scheduler_add_task(new Compass_Task(PRIORITY_MEDIUM));


    #if 1
    scheduler_add_task(new periodicSchedulerTask());
    #endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}
