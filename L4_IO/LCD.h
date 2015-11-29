/**
 * Author: Dheeraj
 * LCD
 */

#include "uart2.hpp"

/* Declare the variables to print on the screen below*/
extern int white_count;

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

    char buf[100];
    urt.putline("$CLR_SCR");

    /* LINE 1 - TITLE*/
    urt.putline("     THUNDERBIRD    ");

    /* LINE 2 - SPEED and */
    urt.put("REVS = ");
    sprintf(buf, "%d", white_count);
    urt.put(buf);
    urt.put("");
    urt.putline("V");    //this makes the line change

    /* LINE 3 - Coordinates*/
    urt.put("START=    ");
    urt.put("DEST=     ");

    /* LINE 4 - SENSORS*/
    urt.put("L=");
    //2 spaces
    urt.put("F=");
    //2 spaces
    urt.put("R=");
    //2 spaces
    urt.put("B=");

    /*Get revolutions and calculate speed*/
    //printf("Revolutions = %d\n", white_count);
}
