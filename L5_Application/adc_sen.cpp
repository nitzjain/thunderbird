#include "adc_sen.hpp"


int sensval()
{
int r_left = 0,distance=0;
LPC_PINCON->PINSEL1 |= (1 << 20);

                    while(1) {
                        r_left = adc0_get_reading(3);
                        distance=r_left/9.76; //Formula Used:Measured Voltages/(Input Voltage/512) i.e. reading/(5/512)
                      }
                    return distance;
}
