#include "canperiodic.hpp"
#include "gpio.hpp"
#include "can.h"
#include "file_logger.h"
#include "string.h"
#define CANBUSBAUDRATE (100)
#define CANBUSQUEUESIZE (32*8)

GPIO gpioop((LPC1758_GPIO_Type)P1_0);
GPIO gpiogps((LPC1758_GPIO_Type)P1_1);
GPIO gpiomotor((LPC1758_GPIO_Type)P1_4);
GPIO gpiosensor((LPC1758_GPIO_Type)P1_8);
GPIO gpiobridge((LPC1758_GPIO_Type)P1_0);
void canbusoffcalback(uint32_t ICR)
{
    //We have to force a processor reset here
    //sys_reboot_abnormal();
    CAN_reset_bus(mycan);

}

void canperiodicinit(void)
{
    CAN_init(mycan,CANBUSBAUDRATE,CANBUSQUEUESIZE,CANBUSQUEUESIZE,canbusoffcalback,NULL);
    CAN_reset_bus(mycan);
    CAN_bypass_filter_accept_all_msgs();
    logger_init(PRIORITY_CRITICAL);
}
void sendcanmesg(const uint8_t msgid)
{
           can_msg_t msg;
            memset(&msg,0,sizeof(msg));
            msg.frame = 0;
            msg.msg_id = msgid;
            msg.frame_fields.is_29bit = 0;
            msg.frame_fields.data_len = 1;
            msg.data.bytes[0] = 1;
            if(CAN_tx(mycan,&msg,100))
            {
                //LOG_INFO("Switchval is %d sent with msg-id %d \n",switchval,msg.msg_id);
                printf("Can sent heartbeat");
            }
            else
            {
                //LOG_INFO("Can tx error");
                printf("Can tx error");
                CAN_reset_bus(mycan);
                if(CAN_is_bus_off(mycan))
                {
                    //LOG_INFO("Can is bus off");
                    printf("Can is bus off");
                    CAN_reset_bus(mycan);

                }
            }


}

void updatecounter(uint8_t mesgid)
{
    switch(mesgid)
    {
        case gps:
            processorcounter[gpsarrayid]++;
            break;
        case motor:
            processorcounter[motorarrayid]++;
            break;
        case sensor:
            processorcounter[sensorarrayid]++;
            break;
        case bridge:
            processorcounter[bridgearrayid]++;
            break;

    };
}

void checkifprocessorsareactive(void)
{
    static int counter = 0;

    if(counter==hzmultiplier)
    {
        //printf("cntr0,1,2,3,4 %d,%d,%d,%d,%d",processorcounter[0],processorcounter[1],processorcounter[2],processorcounter[3],processorcounter[4]);
        //printf("cntr0,1,2,3,4 %d,%d,%d,%d,%d",oldcounter[0],oldcounter[1],oldcounter[2],oldcounter[3],oldcounter[4]);
        if(processorcounter[gpsarrayid]==oldcounter[gpsarrayid])
        {
            //There is no change in gps counter . So gps is dead
            gpiogps.setLow();

        }
        if(processorcounter[motorarrayid]==oldcounter[motorarrayid])
        {
            //There is no change in motor counter . So motor is dead
            gpiomotor.setLow();

        }
        if(processorcounter[sensorarrayid]==oldcounter[sensorarrayid])
        {
            //There is no change in sensor counter . So sensor is dead
            gpiosensor.setLow();

        }
        if(processorcounter[bridgearrayid]==oldcounter[bridgearrayid])
        {
           //There is no change in motor counter . So motor is dead
           gpiobridge.setLow();

        }
        oldcounter[motorarrayid] = processorcounter[motorarrayid];
        oldcounter[sensorarrayid] = processorcounter[sensorarrayid];
        oldcounter[gpsarrayid] = processorcounter[gpsarrayid];
        oldcounter[bridgearrayid] = processorcounter[bridgearrayid];
        counter=0;
    }
    counter++;
}
void canreceivebasicheartbeat(void)
{
    can_msg_t msg;
    while(CAN_rx(mycan,&msg,1))
    {
        if(msg.msg_id == master)
        {
           /* Light on the LED */
           gpioop.toggle();
        }
        if(myprocessor == master)
        {
            updatecounter(msg.msg_id);
        }


    }
}
/* Function canheartbeat
 * Arguments void
 * The aim of this function is to periodically determine
 * if all the processors are connected to can bus and are
 * able to send and receive can messages successfully
 * Visually, if a processor other than master is connected
 * successfully to the can bus, we blink the led on that processor
 * The master keeps track of all processors in the system and if any
 * processor does not send its heartbeat within a specific duration,
 * we light up a led to indicate the same.
 */
void canheartbeat(void)
{
    static int isbeginning = 0;
    if(isbeginning==0)
    {
        canperiodicinit();
        isbeginning = 1;
    }
    sendcanmesg(myprocessor);
    canreceivebasicheartbeat();
    if(myprocessor==master)
    {
        checkifprocessorsareactive();
    }
}


