#include "io.hpp"


extern float pwm_mod;
extern int white_count;

void maintain_speed()
{
    if (white_count < 25)
    {
        pwm_mod = pwm_mod + 0.1;
    }
    else
    {
        pwm_mod = pwm_mod - 0.05;
    }

    if (pwm_mod > 8.5)
    {
        pwm_mod = 8.5;
    }
    if (pwm_mod <= 7.9)
    {
        pwm_mod = 7.9;
    }
    LD.setNumber(white_count);
    white_count = 0;
}
