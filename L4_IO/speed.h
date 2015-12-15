#include "io.hpp"
extern float pwm_mod;
extern int white_count;


void maintain_speed()
{
    if (white_count < 5) //initial start condition
    {
        DC_Motor &dc_motor_instance = DC_Motor::getInstance();
        dc_motor_instance.setDriveMotor(pwm_mod);
    }
    else if ((white_count > 5) && (white_count < 11))
    {
        pwm_mod = pwm_mod + 0.01;
        if (pwm_mod >= 8.0) //max car speed
        {
            pwm_mod = 8.0;
        }
        DC_Motor &dc_motor_instance = DC_Motor::getInstance();
        dc_motor_instance.setDriveMotor(pwm_mod);
    }
    else if ((white_count >= 11) && (white_count < 20)) //maintain this speed
    {
        DC_Motor &dc_motor_instance = DC_Motor::getInstance();
        dc_motor_instance.setDriveMotor(7.9);
        dc_motor_instance.setDriveMotor(7.5);

    }
    else if ((white_count >= 21) && (white_count < 30)) //going fast
    {
        pwm_mod = pwm_mod - 0.01;
        DC_Motor &dc_motor_instance = DC_Motor::getInstance();

        /*Becareful about the deadzones*/
        if (pwm_mod <= 7.9)
        {
            pwm_mod = 7.9; //recovers the motors
        }
    }
    else if ((white_count >= 31) && (white_count < 40)) //super fast
    {
        pwm_mod = pwm_mod - 0.02;
        DC_Motor &dc_motor_instance = DC_Motor::getInstance();
        /*Becareful about the deadzones*/
        if (pwm_mod <= 7.9)
        {
            pwm_mod = 7.9; //recovers the motors
        }
    }

}

