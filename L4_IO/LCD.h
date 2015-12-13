/**
 * Author: Dheeraj
 * Description: Print values for respective modules to the LCD
 */

#include "uart2.hpp"

/* Declare the variables to print on the screen below*/
extern int white_count;                                 //Optical sensor reading
extern float sleft, smiddle, sright, srear, pwm_mod;             //Values from the sensors
char pwm[100], buf1[100], buf2[100],buf3[100],buf4[100],buf5[100];//buffers to write information to print on LCD

void LCD_Display()
{
    Uart2 &urt = Uart2::getInstance();
    static int uart_flag = 1;
    if (uart_flag == 1)
    {
        urt.init(38400, 50, 50);
        urt.putChar(0xF0);
        uart_flag = 0;
    }


    urt.putline("$CLR_SCR");

    /* LINE 1 - TITLE*/
    urt.putline("   THUNDERBIRD");

    /* LINE 2 - SPEED and */
    urt.put("REVS = ");
    sprintf(buf1, "%d", white_count);
    urt.put(buf1);

    urt.put(" PWM = ");
    sprintf(pwm, "%d", (int)(10*pwm_mod));
    urt.putline(pwm);

    /* LINE 3 - Coordinates*/
    urt.put("START=");
    urt.put(buf1);
    urt.put(" DEST=");
    urt.putline(buf1);

    /* LINE 4 - SENSORS*/
    urt.put("L");
    sprintf(buf2, "%3d", (int)sleft);
    sprintf(buf3,"%3d",(int)smiddle);
    sprintf(buf4,"%3d",(int)sright);
    sprintf(buf5,"%3d",(int)srear);
    urt.put(buf2);
    //2 spaces
    urt.put(" M");
    urt.put(buf3);
    //2 spaces
    urt.put(" R");
    urt.put(buf4);
    //2 spaces
    urt.put(" B");
    urt.put(buf5);
}
