#include "Motor_Speed_Precise_Directions/motor_speed.hpp"
#include "periodic_scheduler/MotorControl.hpp"
#include "io.hpp"
extern float pwm_mod;
extern int white_count;

#define set_speed 7.9

void maintain_speed()
{
    DC_Motor &dc_motor_instance = DC_Motor::getInstance();
    int X = AS.getX();
    if (X < 0) //flat ground
    {
        if (white_count < 5) //initial start condition
        {
            dc_motor_instance.setDriveMotor(set_speed);
        }
        else if ((white_count >= 5) && (white_count < 11))
        {
            pwm_mod = pwm_mod + 0.05;
            dc_motor_instance.setDriveMotor(pwm_mod);
        }
        else if ((white_count >= 11) && (white_count < 20)) //maintain this speed
        {
            dc_motor_instance.setDriveMotor(pwm_mod); //7.9 was here before
            //dc_motor_instance.setDriveMotor(7.5);
        }
        else if ((white_count >= 21) && (white_count < 30)) //going fast
        {
            pwm_mod = pwm_mod - 0.02;

            /*Becareful about the deadzones*/
            if (pwm_mod <= (set_speed))
            {
                dc_motor_instance.setDriveMotor(set_speed); //recovers the motors
                //pwm_mod = set_speed + 0.02;
            }
        }
        else if ((white_count >= 31) && (white_count < 40)) //super fast
        {
            pwm_mod = pwm_mod - 0.05;
            /*Becareful about the deadzones*/
            if (pwm_mod <= (set_speed))
            {
                dc_motor_instance.setDriveMotor(set_speed); //recovers the motors
                //pwm_mod = set_speed + 0.05;
            }
        }
        else if ((white_count >= 41) && (white_count < 50)) //super fast
        {
            pwm_mod = pwm_mod - 0.08;

            /*Becareful about the deadzones*/
            if (pwm_mod <= (set_speed))
            {
                dc_motor_instance.setDriveMotor(set_speed); //recovers the motors
                //pwm_mod = set_speed + 0.08;
            }
        }
        else
        {
            pwm_mod = set_speed;
        }
    }
    else //starting on inclination
    {
        if (white_count < 5) //initial start condition
        {
            pwm_mod = pwm_mod + 0.05;
            dc_motor_instance.setDriveMotor(set_speed);
        }
        else if ((white_count >= 5) && (white_count < 11))
        {
            pwm_mod = pwm_mod + 0.05;
            dc_motor_instance.setDriveMotor(pwm_mod);
        }
        else if ((white_count >= 11) && (white_count < 20)) //maintain this speed
        {
            dc_motor_instance.setDriveMotor(pwm_mod); //7.9 was here before
            //dc_motor_instance.setDriveMotor(7.5);
        }
        else if ((white_count >= 21) && (white_count < 30)) //going fast
        {
            pwm_mod = pwm_mod - 0.02;

            /*Becareful about the deadzones*/
            if (pwm_mod <= (set_speed))
            {
                dc_motor_instance.setDriveMotor(set_speed); //recovers the motors
                //pwm_mod = set_speed + 0.02;
            }
        }
        else if ((white_count >= 31) && (white_count < 40)) //super fast
        {
            pwm_mod = pwm_mod - 0.05;
            /*Becareful about the deadzones*/
            if (pwm_mod <= (set_speed))
            {
                dc_motor_instance.setDriveMotor(set_speed); //recovers the motors
                //pwm_mod = set_speed + 0.05;
            }
        }
        else if ((white_count >= 41) && (white_count < 50)) //super fast
        {
            pwm_mod = pwm_mod - 0.08;

            /*Becareful about the deadzones*/
            if (pwm_mod <= (set_speed))
            {
                dc_motor_instance.setDriveMotor(set_speed); //recovers the motors
                //pwm_mod = set_speed + 0.08;
            }
        }
        else
        {
            pwm_mod = set_speed;
        }
    }
}
