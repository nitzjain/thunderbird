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
#include "_can_dbc\auto_gen.inc"
#include "math.h"

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
/*
float Gps_Task :: ToRadian(float degree){
    float radian = degree * PI/180;
    return radian;
}
float Gps_Task :: ToDegree(float radian){
    float degree =radian *180/PI;
    return degree;
}
float Gps_Task :: GetBearing(float lat1,float lat2, float lng1, float lng2){
    float dLng = ToRadian(lng2-lng1);

        float bearing= atan2(sin(dLng)*cos(ToRadian(lat2)),cos(ToRadian(lat1))*sin(ToRadian(lat2))-sin(ToRadian(lat1))*cos(ToRadian(lat2))*cos(dLng));

        bearing = ToDegree(bearing);
        bearing = fmodf((bearing +360),360);

        //if(bearing>180){
            bearing = 360-bearing;
        //}
        return bearing;
}
*/

class Gps_Task : public scheduler_task
{
    private:
        Uart2 &gps_uart;
        static const int rx_q = 100;
        static const int tx_q = 100;
        char * token[20];
        char *temp;
        int i = 0;
        gps_data_t gps_values,gps_decimal;
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
        bool init(void){
            gps_data_q = xQueueCreate(2,sizeof(gps_data_t));
            addSharedObject("gps_queue",gps_data_q);
            return (NULL!=gps_data_q);
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
            sscanf(token[1],"%lf",&gps_values.UTC_time);
            sscanf(token[2],"%f",&gps_values.Latitude);
            gps_values.NS_indicator = token[3];
            sscanf(token[4],"%f",&gps_values.Longitude);
            gps_values.EW_indicator = token[4];
            sscanf(token[6],"%d",&gps_values.GPS_qualty_indicator);
            sscanf(token[7],"%d",&gps_values.Satelite_used);
            sscanf(token[8],"%f",&gps_values.HDOP);
            sscanf(token[9],"%lf",&gps_values.Altitude);
            sscanf(token[13],"%f",&gps_values.HDOP);        //TODO: separate the values,2 values saperated by '*'

            gps_values.Latitude = convert_to_decimal(gps_values.Latitude);
            gps_values.Longitude = convert_to_decimal(gps_values.Longitude);

            if(!xQueueSend(gps_data_q,&gps_values,0)){
                LOG_ERROR("Not sending in the queue");
            }
            if(SW.getSwitch(1)==1)
            {
             LOG_INFO("GPS Longitude :%f",gps_values.Longitude);
             LOG_INFO("Gps Lattitude:%f", gps_values.Latitude);
            }

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
        compass_data_t compass_values;
        QueueHandle_t compass_data_q;
    public:
        Compass_Task(uint8_t priority) :
            scheduler_task("compass", 512*8, priority),
            compass_uart(Uart3::getInstance())
        {
            compass_uart.init(compass_baudrate,rx_q,tx_q);
        }

        bool init(void){
            compass_data_q = xQueueCreate(2,sizeof(compass_data_t));
            addSharedObject("compass_queue",compass_data_q);
            return (NULL!=compass_data_q);
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

            sscanf(token[3],"%f",&compass_values.roll);
            sscanf(token[2],"%f",&compass_values.pitch);
            sscanf(token[1],"%f",&compass_values.yaw);



            if(!xQueueSend(compass_data_q,&compass_values,0)){
               // LOG_ERROR("Not sending in the queue");
            }
            return true;
        }
};

int main(void)
{
#if 0
    LOG_FLUSH();
#endif
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
    scheduler_add_task(new Gps_Task(PRIORITY_MEDIUM));
    //scheduler_add_task(new Compass_Task(PRIORITY_MEDIUM));

    /**
     * A few basic tasks for this bare-bone system :
     *      1.  Terminal task provides gateway to interact with the board through UART terminal.
     *      2.  Remote task allows you to use remote control to interact with the board.
     *      3.  Wireless task responsible to receive, retry, and handle mesh network.
     *
     * Disable remote task if you are not using it.  Also, it needs SYS_CFG_ENABLE_TLM
     * such that it can save remote control codes to non-volatile memory.  IR remote
     * control codes can be learned by typing the "learn" terminal command.
     */




    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
   scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

    /* Change "#if 0" to "#if 1" to run period tasks; @see period_callbacks.cpp */
    #if 1
    scheduler_add_task(new periodicSchedulerTask());
    #endif

    /* The task for the IR receiver */
    // scheduler_add_task(new remoteTask  (PRIORITY_LOW));

    /* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you want the terminal
     * task to always be responsive so you can poke around in case something goes wrong.
     */

    /**
     * This is a the board demonstration task that can be used to test the board.
     * This also shows you how to send a wireless packets to other boards.
     */
    #if 0
        scheduler_add_task(new example_io_demo());
    #endif

    /**
     * Change "#if 0" to "#if 1" to enable examples.
     * Try these examples one at a time.
     */
    #if 0
        scheduler_add_task(new example_task());
        scheduler_add_task(new example_alarm());
        scheduler_add_task(new example_logger_qset());
        scheduler_add_task(new example_nv_vars());
    #endif

    /**
	 * Try the rx / tx tasks together to see how they queue data to each other.
	 */
    #if 0
        scheduler_add_task(new queue_tx());
        scheduler_add_task(new queue_rx());
    #endif

    /**
     * Another example of shared handles and producer/consumer using a queue.
     * In this example, producer will produce as fast as the consumer can consume.
     */
    #if 0
        scheduler_add_task(new producer());
        scheduler_add_task(new consumer());
    #endif

    /**
     * If you have RN-XV on your board, you can connect to Wifi using this task.
     * This does two things for us:
     *   1.  The task allows us to perform HTTP web requests (@see wifiTask)
     *   2.  Terminal task can accept commands from TCP/IP through Wifly module.
     *
     * To add terminal command channel, add this at terminal.cpp :: taskEntry() function:
     * @code
     *     // Assuming Wifly is on Uart3
     *     addCommandChannel(Uart3::getInstance(), false);
     * @endcode
     */
    #if 0
        Uart3 &u3 = Uart3::getInstance();
        u3.init(WIFI_BAUD_RATE, WIFI_RXQ_SIZE, WIFI_TXQ_SIZE);
        scheduler_add_task(new wifiTask(Uart3::getInstance(), PRIORITY_LOW));
    #endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}
