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

class Gps_Task : public scheduler_task
{
    private:
        Uart2 &gps_uart;
        static const int rx_q = 100;
        static const int tx_q = 100;
        char * token[20];
        char *temp;
        int i = 0;
        gps_data_t gps_values;
        QueueHandle_t gps_data_q;
    public:
        Gps_Task(uint8_t priority) :
            scheduler_task("Gps", 512*8, priority),
            gps_uart(Uart2::getInstance())
        {
            gps_uart.init(gps_baudrate,rx_q,tx_q);

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
            sscanf(token[2],"%lf",&gps_values.Latitude);
            gps_values.NS_indicator = token[3];
            sscanf(token[4],"%lf",&gps_values.Longitude);
            gps_values.EW_indicator = token[4];
            sscanf(token[6],"%d",&gps_values.GPS_qualty_indicator);
            sscanf(token[7],"%d",&gps_values.Satelite_used);
            sscanf(token[8],"%f",&gps_values.HDOP);
            sscanf(token[9],"%lf",&gps_values.Altitude);
            sscanf(token[13],"%f",&gps_values.HDOP);        //TODO: separate the values,2 values saperated by '*'
            //printf("NS: %s",gps_values.NS_indicator);
            //printf("LAT: %lf", gps_values.Latitude);
            //printf("LON: %lf",gps_values.Longitude);

            if(!xQueueSend(gps_data_q,&gps_values,0)){
                LOG_ERROR("Not sending in the queuE");
            }
            return true;
        }
};

int main(void)
{

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
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
    scheduler_add_task(new Gps_Task(PRIORITY_MEDIUM));
  //  scheduler_add_task(new compass(PRIORITY_MEDIUM));
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
