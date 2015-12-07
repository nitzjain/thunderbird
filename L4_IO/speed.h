#include "io.hpp"

//float kp=1;
//float ki=1;
//float kd=10;
//float err_c=0;
//float err_prev,err_acc,err_diff=0;
//float p,i,d,output;
extern float pwm_mod;
extern int white_count;
//int wi= white_count;
//int sp=15;
int ismoving = 0;
void maintain_speed()
{
//    err_prev=err_c;
//    err_c=sp-wi;
//    err_acc= err_acc+err_c;
//    err_diff= err_c-err_prev;
//
//    p=kp*err_c;
//    i=ki*err_acc;
//    d=kd*err_diff;
//
//    output=p+i+d;
//    printf("%f\n",output);

    if (white_count > 0)
    {
        ismoving = 1;
    }
    if (ismoving == 1)
    {
        if (white_count < 15)
        {
            pwm_mod = pwm_mod + 0.1;
            if (pwm_mod >= 8.0) //max car speed
            {
                pwm_mod = 8.0;
            }
            else if (pwm_mod <= 7.9) //min car speed
            {
                pwm_mod = 7.9;
            }
        }
        else
        {
            pwm_mod = 7.9;
        }
    }
    //LD.setNumber(white_count);
    extern int lastval;
    printf("\nwhite count = %d", white_count);
    lastval = white_count;
    white_count = 0;
}
