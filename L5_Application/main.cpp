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

/**
 * @file
 * @brief This is the application entry point.
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
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
#include "sensor.h"
#include "_can_dbc/can_dbc.h"

const uint32_t HEARTBEAT__MIA_MS = 0;
const uint32_t SONARS__MIA_MS = 0;
const SENSOR_TX_SONARS_t SONARS__MIA_MSG = { 0 };
const uint32_t MOTOR_CMD__MIA_MS = 0;




//uint8_t Sen_val[3];

can_msg_t msg1, msg2;
#if 0
class CanBus : public scheduler_task
{

    private:

    public:
    CanBus(uint8_t priority) :
    scheduler_task("CanBus", 3000, priority)
    {
        /* Nothing to init */
    }

    bool init(void)
    {
        CAN_init(can1, 100, 100, 100, 0, 0);
        CAN_reset_bus(can1);
        CAN_bypass_filter_accept_all_msgs();
        memset(&msg1,0,sizeof(msg1));
        // memset(&msg2,0,sizeof(msg1));
        msg1.msg_id = 0x001;
        // msg.frame_fields.is_29bit = 0;
        msg1.frame_fields.data_len = 3;// Send 3 bytes
        msg1.data.bytes[0] = Sen_val[0];
        msg1.data.bytes[1] = Sen_val[1];
        msg1.data.bytes[2] = Sen_val[2];

//            msg2.msg_id = 0x010;
//          // msg.frame_fields.is_29bit = 0;
//            msg2.frame_fields.data_len = 8;       // Send 8 bytes
//            msg2.data.qword = 0x0000000000000001; // Write all 8 bytes of data at once
        return true;
    }

    bool run(void *p)
    {
        int ret;

//            printf("%d",CAN_is_bus_off(can1));

        // ret = CAN_init(can1, 100, 10, 10, 0, 0);
        //printf("ret val INIT is: %i\n",ret);

        CAN_tx(can1, &msg1, portMAX_DELAY);

//            if(SW.getSwitch(1))
//            {
//                //msg.data.qword = 0x0000000000000010;
//                //msg.msg_id = 0x001;
//                ret = CAN_tx(can1, &msg1, portMAX_DELAY);
//                printf("ret val TX is: %i\n",ret);
//            }
//
//           // msg.data.qword = 0x0000000000000001;
//            ret = CAN_tx(can1, &msg2, portMAX_DELAY);
//            printf("ret val TX with swich off is:  %i\n",ret);
//
//            vTaskDelay(100);
        return true;
    }
};
#endif

void CAN_INIT()
{
    CAN_init(can1, 100, 100, 100, 0, 0);
    CAN_reset_bus(can1);
    CAN_bypass_filter_accept_all_msgs();
    memset(&msg1, 0, sizeof(msg1));
    //msg1.msg_id = 0x001;
    //msg1.frame_fields.data_len = 3;
    //msg1.data.bytes[0] = Sen_val[0];
   // msg1.data.bytes[1] = Sen_val[1];
    //msg1.data.bytes[2] = Sen_val[2];
    //printf("CAN initialized");
}

//class Sensor: public scheduler_task
//{
//    public:
//        Sensor(uint8_t priority) :
//                scheduler_task("Sensor_Task", 4096, priority)
//        {
//            CAN_init(can1, 100, 100, 100, 0, 0);
//            CAN_reset_bus(can1);
//            CAN_bypass_filter_accept_all_msgs();
//            memset(&msg1, 0, sizeof(msg1));
//            msg1.msg_id = 0x001;
//            msg1.frame_fields.data_len = 1;
//            msg1.data.bytes[0] = Sen_val[0];
//            // msg1.data.bytes[1] = Sen_val[1];
//            //  msg1.data.bytes[2] = Sen_val[2];
//        }
//
//        bool run(void *p)
//        {
//
//          //  Sen_val[0] = GetLeftSensorReading();
//            //           Sen_val[1]=GetMidSensorReading();
////            Sen_val[2]=GetRightSensorReading();
//
//            //printf("Reading LEFT is: %i\n", Sen_val[0]);
////            printf("Reading MID is: %i\n",Sen_val[1]);
////            printf("Reading RIGHT is: %i\n",Sen_val[2]);
//           // CAN_tx(can1, &msg1, portMAX_DELAY);
//            delay_ms(1000);
//            return true;
//        }
//};

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
int main(void)
{
    delay_ms(250); //sensor RX pin requires 250 ms after start-up to get activated

    InitInterruptLeft();
    InitInterruptMid();
    InitInterruptRight();
    InitInterruptBack();


    CAN_INIT();

    /* while(1){

     Sen_val[0]=GetLeftSensorReading();
     delay_ms(10);
     Sen_val[1]=GetMidSensorReading();
     delay_ms(10);
     Sen_val[2]=GetRightSensorReading();
     delay_ms(10);

     printf("Reading LEFT is: %i\n",Sen_val[0]);
     printf("Reading MID is: %i\n",Sen_val[1]);
     printf("Reading RIGHT is: %i\n",Sen_val[2]);

     delay_ms(1000);

     }*/

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
    // scheduler_add_task(new terminalTask(PRIORITY_HIGH));
    /* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
    //cheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));
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

    // CAN_tx(can1, &msg, portMAX_DELAY);

    // scheduler_add_task(new CanBus(PRIORITY_HIGH)); //for can
    // printf("This is before task call");
    //    scheduler_add_task(new Sensor(PRIORITY_HIGH)); //this one
    // printf("This is after task call");

#if 0
    float reading = 0;

    // Initialization :
    LPC_PINCON->PINSEL3 |= (3 << 28);// ADC-4 is on P1.30, select this as ADC0.4

    while(1)
    {
        reading = adc0_get_reading(4); // Read current value of ADC-4
        printf("\nADC Reading: %f", reading/6.44);
        delay_ms(1000);
    }

    return 0;
#endif

    scheduler_start(); ///< This shouldn't return
    return -1;
}
