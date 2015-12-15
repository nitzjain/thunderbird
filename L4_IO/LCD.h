/**
 * Author: Dheeraj
 * Description: Print values for respective modules to the LCD
 */

#include "uart2.hpp"

/* Declare the variables to print on the screen below*/
extern int white_count;                                 //Optical sensor reading
extern float sleft, smiddle, sright, srear, pwm_mod, gps_start, gps_end;             //Values from the sensors
extern int degrees;
char pwm[100], buf1[100], buf2[100],buf3[100],buf4[100],buf5[100],compass[100],start_gps[100],end_gps[100];//buffers to write information to print on LCD

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

    /* Clear Screen */
    urt.putline("$CLR_SCR");

    /* LINE 1 - TITLE*/
    urt.putline("   THUNDERBIRD");

    /* LINE 2 - REVS and PWM value */
    sprintf(buf1, "%d", white_count);
    sprintf(pwm, "%d", (int)(10*pwm_mod));

    urt.put("REVS = ");
    urt.put(buf1);

    urt.put(" PWM = ");
    urt.putline(pwm);

    /* LINE 3 - Coordinates*/
    sprintf(compass, "%3d", degrees);
    sprintf(start_gps, "%3d", (int)gps_start);
    sprintf(end_gps, "%3d", (int)gps_end);

    urt.put("S=");
    urt.put(start_gps);

    urt.put(" D=");
    urt.put(end_gps);

    urt.put(" C=");
    urt.putline(compass);

    /* LINE 4 - SENSORS*/
    sprintf(buf2, "%3d", (int)sleft);
    sprintf(buf3,"%3d",(int)smiddle);
    sprintf(buf4,"%3d",(int)sright);
    sprintf(buf5,"%3d",(int)srear);

    urt.put("L");
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
