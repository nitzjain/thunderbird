#include "canperiodic.hpp"
#include "canperiodicext.hpp"

/* Local Function Declarations */
static void canbusoffcalback(uint32_t ICR);
static void canperiodicinit(void);
static void sendcanmesg(const uint8_t msgid);

void canbusoffcalback(uint32_t ICR)
{
    //We have to force a processor reset here
    CAN_reset_bus(mycan);
}

void canperiodicinit(void)
{
    CAN_init(mycan,CANBUSBAUDRATE,
                    CANBUSQUEUESIZE,CANBUSQUEUESIZE,canbusoffcalback,NULL);

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

void canheartbeat(void)
{
    static int isbeginning = 0;

    if(isbeginning==0)
    {
       canperiodicinit();
       isbeginning = 1;
    }

    sendcanmesg(myprocessor);
}
