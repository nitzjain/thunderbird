#include "precise_steer.hpp"
#include "periodic_scheduler/MotorControl.hpp"
/*
 * Author: Dheeraj
 * Description: PWM valus for precise steer
 *
 * **********Steering controlled by Compass************
 * Servo - Takes values from Compass - Sectors 1 to 17
 * Sectors - Take right - Sector 0 to 9
 *         - Take left - Sectors 10 to 17
 * PWM     - Left ranges - 4 to 7
 *         - Right ranges - 7 to 10 - 6.8 being the center
 * CAUTION: 4 is extreme left and 10 is extreme right
 *
 */

typedef enum steer_values
{
    angle_0 = 0,
    angle_20 = 20,
    angle_40 = 40,
    angle_60 = 60,
    angle_80 = 80,
    angle_100 = 100,
    angle_120 = 120,
    angle_140 = 140,
    angle_160 = 160,
    angle_180 = 180,
} steer_turn;

void precise_steer_left(int command)
{
    Steer_Motor &steer = Steer_Motor::getInstance();
    /*Precise steer left*/
    if (angle_0 == command)
    {
        steer.setSteerMotor(6.8);
    }
    else if (angle_20 == command)
    {
        steer.setSteerMotor(6.5);
    }
    else if (angle_40 == command)
    {
        steer.setSteerMotor(6.2);
    }
    else if (angle_60 == command)
    {
        steer.setSteerMotor(5.9);
    }
    else if (angle_80 == command)
    {
        steer.setSteerMotor(5.6);
    }
    else if (angle_100 == command)
    {
        steer.setSteerMotor(5.3);
    }
    else if (angle_120 == command)
    {
        steer.setSteerMotor(5.0);
    }
    else if (angle_140 == command)
    {
        steer.setSteerMotor(4.7);
    }
    else if (angle_160 == command)
    {
        steer.setSteerMotor(4.4);
    }
    else if (angle_180 == command)
    {
        steer.setSteerMotor(4.1);
    }
}

void precise_steer_right(int command)
{
    Steer_Motor &steer = Steer_Motor::getInstance();
    /*Precise steer right*/
    if (angle_0 == command)
    {
        steer.setSteerMotor(6.8);
    }
    else if (angle_20 == command)
    {
        steer.setSteerMotor(7.1);
    }
    else if (angle_40 == command)
    {
        steer.setSteerMotor(7.4);
    }
    else if (angle_60 == command)
    {
        steer.setSteerMotor(7.7);
    }
    else if (angle_80 == command)
    {
        steer.setSteerMotor(8.0);
    }
    else if (angle_100 == command)
    {
        steer.setSteerMotor(8.3);
    }
    else if (angle_120 == command)
    {
        steer.setSteerMotor(8.6);
    }
    else if (angle_140 == command)
    {
        steer.setSteerMotor(8.9);
    }
    else if (angle_160 == command)
    {
        steer.setSteerMotor(9.2);
    }
    else if (angle_180 == command)
    {
        steer.setSteerMotor(9.5);
    }
}
