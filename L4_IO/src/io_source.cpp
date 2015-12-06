/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

#include <stdint.h>

#include "io.hpp" // All IO Class definitions
#include "bio.h"
#include "adc0.h"
#include "utilities.h"
#include "stdio.h"
#include "i2c_base.hpp"
#include "compass.hpp"
#include "string.h"

/**
 * The following diagram shows bit number corresponding to the LED
 *
 *      0
 *     ---
 *  5 | 6 | 1
 *     ---
 *  4 |   | 2
 *     ---
 *      3   *7
 */
static const unsigned char LED_DISPLAY_CHARMAP[128] =
{
        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        0,    // 32 : Space
        0x86, // 33 : !
        0,0,0,0, 0,0,0,0, 0,0,0, // 34-44
        0x40, // 45 : -
        0x80, // 46 : .
        0x00, // 47 : /
        0x3F, // 48 : 0
        0x30, // 49 : 1
        0x5B, // 50 : 2
        0x4F, // 51 : 3
        0x66, // 52 : 4
        0x6D, // 53 : 5
        0x7C, // 54 : 6
        0x07, // 55 : 7
        0x7F, // 56 : 8
        0x67, // 57 : 9
        0,0,0,0,0,0,0, // 58-64
        0x77, // 65 : A
        0x7F, // 66 : B
        0x39, // 67 : C
        0x3F, // 68 : D
        0x79, // 69 : E
        0x71, // 70 : F
        0x3D, // 71 : G
        0x76, // 72 : H
        0x30, // 73 : I
        0x1F, // 74 : J
        0x00, // 75 : K
        0x38, // 76 : L
        0x00, // 77 : M
        0x00, // 78 : N
        0x3F, // 79 : O
        0x73, // 80 : P
        0x00, // 81 : Q
        0x00, // 82 : R
        0x6D, // 83 : S
        0x00, // 84 : T
        0x3F, // 85 : U
        0x00, // 86 : V
        0x00, // 87 : W
        0x00, // 88 : X
        0x00, // 89 : Y
        0x00, // 90 : Z

        0, 0, 0, 0, 0, 0,

        0x77, // 97 : a
        0x7F, // 98 : b
        0x39, // 99 : c
        0x3F, // 100 : d
        0x79, // 101 : e
        0x71, // 102 : f
        0x3D, // 103 : g
        0x76, // 104 : h
        0x30, // 105 : i
        0x1F, // 106 : j
        0x00, // 107 : k
        0x38, // 108 : l
        0x00, // 109 : m
        0x00, // 110 : n
        0x3F, // 111 : o
        0x73, // 112 : p
        0x00, // 113 : q
        0x00, // 114 : r
        0x6D, // 115 : s
        0x00, // 116 : t
        0x3F, // 117 : u
        0x00, // 118 : v
        0x00, // 119 : w
        0x00, // 120 : x
        0x00, // 121 : y
        0x00, // 122 : z
};



bool Acceleration_Sensor::init()
{
    const unsigned char activeModeWith100Hz = (1 << 0) | (3 << 3); // Active Mode @ 100Hz

    writeReg(Ctrl_Reg1, activeModeWith100Hz);
    const char whoAmIReg = readReg(WhoAmI);

    return (mWhoAmIExpectedValue == whoAmIReg);
}
int16_t Acceleration_Sensor::getX()
{
    return (int16_t)get16BitRegister(X_MSB) / 16;
}
int16_t Acceleration_Sensor::getY()
{
    return (int16_t)get16BitRegister(Y_MSB) / 16;
}
int16_t Acceleration_Sensor::getZ()
{
    return (int16_t)get16BitRegister(Z_MSB) / 16;
}



/**
 * The design of the IR Sensor is as follows:
 *  Timer1 captures falling edges of CAP1.0 and timestamps are saved when this happens.
 *  Every time capture happens, a 20ms match interrupt is setup/reset.  When no more
 *  signals arrive, the 20ms match interrupt will decode the timestamps and convert
 *  timestamps into binary signal.
 *
 *  Note that this doesn't decode into real data sent through the IR, but nevertheless,
 *  the decoded signal will be unique per button pressed on an IR remote.
 */


#define MAX_FALLING_EDGES_PER_IR_FRAME      32
static uint32_t g_ir_timings[MAX_FALLING_EDGES_PER_IR_FRAME] = { 0 };   ///< IR signal falling edges
static uint16_t g_signal_count = 0;         ///< The number of falling edges
static uint32_t g_last_decoded_signal = 0;  ///< Value of the last decoded signals

void IR_Sensor::storeIrCode(uint32_t value)
{
    // Just store the timestamp of this signal
    if(g_signal_count < MAX_FALLING_EDGES_PER_IR_FRAME) {
        g_ir_timings[g_signal_count++] = value;
    }
}

void IR_Sensor::decodeIrCode(void)
{
    if(g_signal_count > 1)
    {
        /* Calculate differences of falling edges */
        for(int i = 0; i < g_signal_count-1; i++) {
            g_ir_timings[i] = g_ir_timings[i+1] - g_ir_timings[i];
        }

        /**
         * First falling edge value should indicate binary 0.
         * So anything higher than 50% of this value is considered binary 1.
         */
        const uint32_t binary1Threshold = g_ir_timings[1] + (g_ir_timings[1]/2);
        uint32_t decodedSignal = 0;
        for(uint16_t i=0; i < g_signal_count-1; i++) {
            if(g_ir_timings[i] > binary1Threshold) {
                decodedSignal |= (1 << i);
            }
        }
        g_last_decoded_signal = decodedSignal;
    }

    g_signal_count = 0;
}

/**
 * IR Sensor is attached to P1.18 - CAP1.0, so it needs TIMER1 to capture the times on P1.18
 */
bool IR_Sensor::init()
{
    /* Power up the timer 1 in case it is off */
    lpc_pconp(pconp_timer1, true);

    /* Timer 1 should be initialized by high_level_init.cpp using lpc_sys.c API
     * We will just add on the capture functionality here.
     */
    LPC_TIM1->CCR &= ~(7 << 0);            // Clear Bits 2:1:0
    LPC_TIM1->CCR |=  (1 << 2) | (1 << 1); // Enable Falling Edge capture0 with interrupt

    // Select P1.18 as CAP1.0 by setting bits 5:4 to 0b11
    LPC_PINCON->PINSEL3 |= (3 << 4);

    return true;
}

bool IR_Sensor::isIRCodeReceived()
{
    return (0 != g_last_decoded_signal);
}
uint32_t IR_Sensor::getLastIRCode()
{
    const uint32_t signal = g_last_decoded_signal;
    g_last_decoded_signal = 0;
    return signal;
}



bool LED_Display::init()
{
    bool devicePresent = checkDeviceResponse();
    if(devicePresent)
    {
        const unsigned char cfgAsOutput = 0x00;
        writeReg(cfgPort0, cfgAsOutput);
        writeReg(cfgPort1, cfgAsOutput);

        setLeftDigit('.');
        setRightDigit('.');
    }

    return devicePresent;
}
void LED_Display::clear()
{
    setLeftDigit(' ');
    setRightDigit(' ');
}
void LED_Display::setNumber(char num)
{
    num %= 100;
    setRightDigit(num%10 + '0');

    if(num < 10)
        setLeftDigit(' ');
    else
        setLeftDigit (num/10 + '0');
}
void LED_Display::setLeftDigit(char alpha)
{
    mNumAtDisplay[0] = alpha;
    writeReg(outputPort1, LED_DISPLAY_CHARMAP[(unsigned) (alpha & 0x7F) ]);
}
void LED_Display::setRightDigit(char alpha)
{
    mNumAtDisplay[1] = alpha;
    writeReg(outputPort0, LED_DISPLAY_CHARMAP[(unsigned) (alpha & 0x7F) ]);
}




bool LED::init()
{
    /* Pins initialized by bio.h */
    return true;
}
void LED::on(uint8_t ledNum)
{
    portENTER_CRITICAL();
    mLedValue or_eq (1 << (ledNum-1));
    setAll(mLedValue);
    portEXIT_CRITICAL();
}
void LED::off(uint8_t ledNum)
{
    portENTER_CRITICAL();
    mLedValue and_eq ~(1 << (ledNum-1));
    setAll(mLedValue);
    portEXIT_CRITICAL();
}
void LED::toggle(uint8_t ledNum)
{
    portENTER_CRITICAL();
    mLedValue xor_eq (1 << (ledNum-1));
    setAll(mLedValue);
    portEXIT_CRITICAL();
}
void LED::set(uint8_t ledNum, bool ON)
{
    ON ? on(ledNum) : off(ledNum);
}
void LED::setAll(uint8_t value)
{
    portENTER_CRITICAL();
    {
        /* LEDs are active low */
        #define led_set(num, realbit)               \
            if (mLedValue & (1 << num))             \
                LPC_GPIO1->FIOCLR = (1 << realbit); \
            else                                    \
                LPC_GPIO1->FIOSET = (1 << realbit)

        mLedValue = value & 0x0F;
        led_set(0, 0);
        led_set(1, 1);
        led_set(2, 4);
        led_set(3, 8);
    }
    portEXIT_CRITICAL();
}
uint8_t LED::getValues(void) const
{
    return mLedValue;
}


bool Light_Sensor::init()
{
    return true;
}
uint16_t Light_Sensor::getRawValue()
{
    return adc0_get_reading(BIO_LIGHT_ADC_CH_NUM);
}
uint8_t Light_Sensor::getPercentValue()
{
    const unsigned int maxAdcValue = 4096;
    return (getRawValue() * 100) / maxAdcValue;
}



bool Switches::init()
{
    /* Pins initialized by bio.h */
    return true;
}
uint8_t Switches::getSwitchValues()
{
    /* Return contiguous bits 0-3 from scattered switch pins */
    const int swValues = LPC_GPIO1->FIOPIN;
    const int p9_p10  = (swValues >> 9)  & 0x3;
    const int p14_p15 = (swValues >> 14) & 0x3;
    return (p9_p10 | (p14_p15 << 2));
}
bool Switches::getSwitch(int num)
{
    if (num >= 1 && num <= 4) {
        return getSwitchValues() & (1 << (num-1));
    }
    else {
        return false;
    }
}



bool I2C_Temp::init()
{
    const unsigned char temperatureCfgRegPtr = 0x01;
    const unsigned char oneShotShutdownMode = 0x81;
    const unsigned char expectedBitsThatAreNotZero = 0x60; // R1:R0 bits are 1s at startup

    // Enter one-shot shutdown mode.
    writeReg(temperatureCfgRegPtr, oneShotShutdownMode);

    const unsigned char cfgRegByte0 = readReg(temperatureCfgRegPtr);
    return (0 != (cfgRegByte0 & expectedBitsThatAreNotZero));
}
float I2C_Temp::getCelsius()
{
    // Get signed 16-bit data of temperature register pointer
    const unsigned char temperatureRegsiterPtr = 0x00;
    signed short temperature = get16BitRegister(temperatureRegsiterPtr);

    // Trigger next conversion:
    const unsigned char temperatureCfgRegPtr = 0x01;
    const unsigned char oneShotShutdownMode = 0x81;
    writeReg(temperatureCfgRegPtr, oneShotShutdownMode);

    // Temperature data is in bits 15:3 which contains signed 16-bit data
    temperature /= 16;

    // Each bit is of 0.0625 degree per bit resolution
    return (0.0625F * temperature) + mOffsetCelcius;
}

float I2C_Temp::getFarenheit()
{
    return (getCelsius() * 9.0F / 5) + 32;
}


#if 0
bool I2C_comp::init_compass()
{
    const unsigned char com_cfg_reg_a_ptr = 0x00;
    const unsigned char com_cfg_samples = 0x70;
    const unsigned char com_cfg_reg_b_ptr = 0x01;
    const unsigned char com_cfg_gain = 0xA0;
    const unsigned char com_mode_reg_ptr = 0x02;
    const unsigned char com_mode_reg_cont = 0x00;

    writeReg(com_cfg_reg_a_ptr,com_cfg_samples);        //set Config A reg for 8 samples and 15hx default mode
    unsigned char buff0 = readReg(com_cfg_reg_a_ptr);   //get Config A reg value to verify the value set
    writeReg(com_cfg_reg_b_ptr,com_cfg_gain);           //set Config B reg for gain
    unsigned char buff1 = readReg(com_cfg_reg_b_ptr);   //get Config B reg value to verify the value set
    writeReg(com_mode_reg_ptr,com_mode_reg_cont);       //set Mode reg for cont. measurement
    unsigned char buff2 = readReg(com_mode_reg_ptr);     //get Mode reg value to verify the value set
    delay_ms(6);                                        //required delay to update configuration

    //if((com_cfg_samples == buff0) && (com_cfg_gain == buff1) && (com_mode_reg_cont == buff2)){ //verifying of the registers are set
    //    printf("Success");
        printf("CFG A %x, CFG B %x, Mode %x",readReg(com_cfg_reg_a_ptr),readReg(com_cfg_reg_b_ptr),readReg(com_mode_reg_ptr) );
    //    while(readReg(0x03)!=0);
        return true;
    //}
     //   else{
      //      printf("Init registers are not set\n");
     //       return false;
      //  }
}

void I2C_comp::get_compass_data(){
    //compass_data_t* c_data;
    uint8_t* temp;
    uint8_t buff[6];
    const unsigned char data_out_x_msb = 0x03;
    const unsigned char data_out_y_msb = 0x07;
    const unsigned char data_out_z_msb = 0x05;
    const unsigned char status_reg = 0x09;
    const unsigned char com_mode_reg_ptr = 0x02;
    const unsigned char com_mode_reg_cont = 0x00;
    //uint16_t data_x,data_y=0,data_z=0;
//if(readReg(status_reg)== 1){
    writeReg(com_mode_reg_ptr,com_mode_reg_cont);       //set Mode reg for cont. measurement
    char buff2 = readReg(com_mode_reg_ptr);     //get Mode reg value to verify the value set
    delay_ms(10);
    temp = getAllRegisters(0x03,buff);
    //data_x = get16BitRegister(data_out_x_msb);
    //data_y = get16BitRegister(data_out_y_msb);
    //data_z = get16BitRegister(data_out_z_msb);
    //char* data_y = &((char)get16BitRegister(data_out_y_msb));
    //char* data_z = &((char)get16BitRegister(data_out_z_msb));
    //sprintf(c_data->x,"%d",get16BitRegister(data_out_x_msb));
    //c_data->x = &data_x;
    //c_data->y = data_y;
    //c_data->z = data_z;
    printf("Mode Reg: %c\n", buff2);
    printf("%u,%u,%u,%u,%u,%u",temp[0],temp[1],temp[2],temp[3],temp[4],temp[5]);
    //printf("%d, %d, %d",~data_x+1,~data_y+1,~data_z+1);
    //printf("%d, %d, %d",((!data_x)+1),((!data_y)+1),((!data_z)+1));
    //printf("%s",c_data->x);
    //delay_ms(67);
//}
//else
//    printf("status reg is not reseted, %x",readReg(status_reg));

    delay_ms(100);
}
#end if

bool I2C_comp::init_compass()
{
    const unsigned char config_regA = 0x00;
    const unsigned char config_regB = 0x01;
    const unsigned char mode_reg = 0x02;
    const unsigned char data_reg = 0x03;  // first data reg to point
    uint32_t read_bytes = 0x06;

   // uint8_t  *buff=&a[0];
   writeReg(mode_reg,config_regB);
     delay_ms(10);


      // calculate 2's complement of databytes
      // writeReg(data_reg,NULL);  // point again  to first data register 0x03
       // delay of 67ms
        //end loop

    return true;
}

bool I2C_comp::loop()
{
    compass_data_t c_axes;
    uint8_t a[100];
         a[0]= readReg(0x03);
         a[1]= readReg(0x04);
         a[2]= readReg(0x05);
         a[3]= readReg(0x06);
         a[4]= readReg(0x07);
         a[5]= readReg(0x08);
         c_axes.x = (a[0]<<8)|a[1];
         c_axes.x= (~c_axes.x)+1;
         c_axes.y = (a[2]<<8)|a[3];
         c_axes.y= (~c_axes.x)+1;
         c_axes.z = (a[4]<<8)|a[5];
         c_axes.z= (~c_axes.z)+1;
        printf("x=%d\t y=%d \t z=%d \n",c_axes.x,c_axes.y,c_axes.z);

         delay_ms(100);
return true;
}
#endif
