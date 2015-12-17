#include "Motor_Speed_Precise_Directions/motor_speed.hpp"
#include "periodic_scheduler/MotorControl.hpp"
#include "io.hpp"
extern float pwm_mod;
extern int white_count;
int X;

#define set_speed_incline 8.0

//void maintain_speed()
//{
//    DC_Motor &dc_motor_instance = DC_Motor::getInstance();
//    X = AS.getX(); //get the elevation angle
//
//    if (X < 50) //flat ground
//    {
//        if (white_count < 10) //initial start condition
//        {
//            dc_motor_instance.setDriveMotor(pwm_mod);
//        }
//        else
//        {
//            dc_motor_instance.setDriveMotor(pwm_mod);
//        }
//    }
//    else //starting on inclination
//    {
//        if (white_count < 5) //initial start condition
//        {
//            pwm_mod = pwm_mod + 0.005;
//            dc_motor_instance.setDriveMotor(pwm_mod);
//        }
//        else if(white_count <=5 && white_count <10)
//        {
//            dc_motor_instance.setDriveMotor(pwm_mod);
//        }
//        else
//        {
//            pwm_mod = pwm_mod - 0.01;
//        }
//    }
//}

void maintain_speed()
{
    DC_Motor &dc_motor_instance = DC_Motor::getInstance();
    if(white_count < 5 )
    {
        dc_motor_instance.setDriveMotor(pwm_mod);
        pwm_mod = pwm_mod + 0.0001;
    }
    else if(white_count <=5 && white_count <10)
    {
        dc_motor_instance.setDriveMotor(pwm_mod);
    }
    else
    {
        pwm_mod = pwm_mod - 0.002;
        dc_motor_instance.setDriveMotor(pwm_mod);
        if(pwm_mod <=7.9)
        {
            pwm_mod = 7.92;
        }
    }
}
