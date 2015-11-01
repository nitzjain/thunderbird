#include "Can.h"
#include <stdio.h>
#include "lpc_pwm.hpp"
#define forward 0x120
#define reverse 0x121
#define left 0x122
#define right 0x123
void motorprocessmessage(can_msg_t control)
{
    PWM pwm1(PWM::pwm1, 50);
    PWM pwm2(PWM::pwm2, 50);
    pwm1.set(7.5);
    pwm2.set(7);
    static int flag = 0;
    if(flag == 0)
    {
        printf("Hi\n");
        flag = 1;
    }
    if (control.msg_id == forward)
        {
           printf("1\n");
            pwm1.set(9);
        }
        else if(control.msg_id == reverse)
        {
            printf("2\n");
            pwm1.set(7);
        }
        else if(control.msg_id == left)
        {
            printf("3\n");
            pwm2.set(9);
        }
        else if(control.msg_id == right)
        {
            printf("4\n");
            pwm2.set(5);
        }
}
