/*
 *  canperiodic.cpp
 *------------------------------------------------------------------------------
 * THUNDERBIRD
 *------------------------------------------------------------------------------
 *
 *  DESCRIPTION:
 *    The purpose of this file is to send a periodic heart beat message to all
 *    the controllers and check whether each of the controller is alive.
 *
 *    7-segment - Displays the number of active controllers
 *
 *    LED       - 4 Led's for 4 controllers (except master) and lights
 *                when the corresponding controller doesn't work.
 *
 *------------------------------------------------------------------------------
 */


#include "canperiodic.hpp"
#include "canperiodicext.hpp"

/* Local Function Declarations */
static void canbusoffcalback(uint32_t ICR);
static bool canperiodicinit(void);
static void sendcanmesg(const uint8_t msgid);
static void updatecounter(uint8_t mesgid);
static void checkifprocessorsareactive(void);
static void canreceivebasicheartbeat(void);

/*
================================================================#=
NAME:
canbusoffcalback

DESCRIPTION:
The purpose of this call back function is reseting the device
when CAN goes to bus off state.

PARAMETERS:
ICR - Status of error

RETURN VALUES:
None
================================================================#=
*/

void canbusoffcalback(uint32_t ICR)
{
    //We have to force a processor reset here
    CAN_reset_bus(mycan);
}

/*
================================================================#=
NAME:
canperiodicinit

DESCRIPTION:
The purpose of this function is to initialize the CAN bus.

PARAMETERS:
None.

RETURN VALUES:
True if CAN Initialization is successful, else returns FALSE.
================================================================#=
*/

bool canperiodicinit(void)
{
    if(!CAN_init(mycan,CANBUSBAUDRATE,
                    CANBUSQUEUESIZE,CANBUSQUEUESIZE,canbusoffcalback,NULL))
        return false;

    CAN_reset_bus(mycan);
    CAN_bypass_filter_accept_all_msgs();
    logger_init(PRIORITY_CRITICAL);
    return true;
}

/*
================================================================#=
NAME:
sendcanmesg

DESCRIPTION:
The purpose of this function is to send the message ID using CAN Bus.

PARAMETERS:
msgid - The ID of the controller which is sending the message

RETURN VALUES:
None

================================================================#=
*/

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
                printf("Can sent heartbeat");
            }
            else
            {
                printf("Can Tx error");
                CAN_reset_bus(mycan);
                if(CAN_is_bus_off(mycan))
                {
                    printf("Can is Bus off");
                    CAN_reset_bus(mycan);

                }
            }


}

/*
================================================================#=
NAME:
updatecounter

DESCRIPTION:
The purpose of this function is to update the counter of the
respective controller.

PARAMETERS:
mesgid - The ID of the controller whose counter is increased.

RETURN VALUES:
None
================================================================#=
*/
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

/*
================================================================#=
NAME:
checkifprocessorsareactive

DESCRIPTION:
THe processors are checked for aliveness every 10 seconds by comparing
the current and old counter values.
 * If both old and current counter values matches, then corresponding
   controller is considered to be dead and LED is light up.
 * If they doesn't match then the status has been received and controller
   is alive.
 * Using the 7-segment display, the number of active processor's is
   displayed.

PARAMETERS:
None

RETURN VALUES:
None
================================================================#=
*/
void checkifprocessorsareactive(void)
{
    static int counter = 0;
    static int num_of_active_processors = 4;
    static int flag_gps = 0, flag_sensor = 0, flag_bridge = 0, flag_motor = 0;

    if(counter == TENSECONDS)
    {
        if(processorcounter[gpsarrayid]==oldcounter[gpsarrayid])
        {
            //There is no change in gps counter . So gps is dead
            flag_gps = 1;
            gpiogps.setLow();
        }
        else
        {

            gpiogps.setHigh();
            flag_gps = 0;

        }

        if(processorcounter[motorarrayid]==oldcounter[motorarrayid])
        {
            //There is no change in motor counter . So motor is dead
            flag_motor = 1;
            gpiomotor.setLow();

        }
        else
        {
               gpiomotor.setHigh();
               flag_motor = 0;

        }

        if(processorcounter[sensorarrayid] == oldcounter[sensorarrayid])
        {
            //There is no change in sensor counter . So sensor is dead
            gpiosensor.setLow();
            flag_sensor = 1;
        }
        else
        {
                 gpiosensor.setHigh();
                 flag_sensor = 0;
        }

        if(processorcounter[bridgearrayid] == oldcounter[bridgearrayid])
        {
           //There is no change in motor counter . So motor is dead
           gpiobridge.setLow();
           flag_bridge = 1;
        }
        else
        {
                gpiobridge.setHigh();
                flag_bridge = 0;
        }

        oldcounter[motorarrayid]  = processorcounter[motorarrayid];
        oldcounter[sensorarrayid] = processorcounter[sensorarrayid];
        oldcounter[gpsarrayid]    = processorcounter[gpsarrayid];
        oldcounter[bridgearrayid] = processorcounter[bridgearrayid];
        num_of_active_processors = 4 -( flag_gps + flag_sensor + flag_motor + flag_bridge);
        LD.setNumber(num_of_active_processors);

        counter=0;
    }
    counter++;
}

/*
================================================================#=
NAME:
canreceivebasicheartbeat

DESCRIPTION:
The purpose of this function is to receive the heart beat message.
    * If the controller is the master, then it indicates that the message
      is from other controllers and hence update counters.
    * If the controller other than master, the LED is toggled in their
      respective controllers.

PARAMETERS:
None

RETURN VALUES:
None

================================================================#=
*/
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

/*
================================================================#=
NAME:
canheartbeat

DESCRIPTION:
The purpose of this function is to periodically determine
 * If all the processors are alive by sending and receiving the heartbeat
   messages.
 * The master keeps track of all processors in the system.

PARAMETERS:
None

RETURN VALUES:
None

================================================================#=
*/
void canheartbeat(void)
{
    static int isbeginning = 0;

    /* Initialize the CAN Bus once */
    if(isbeginning==0)
    {
        if(canperiodicinit())
        {
            isbeginning = 1;
        }
        else
        {
           LOG_ERROR("CAN Initialization Error\n");
        }
    }

    sendcanmesg(myprocessor);

    canreceivebasicheartbeat();

    if(myprocessor==master)
    {
        checkifprocessorsareactive();
    }
}
