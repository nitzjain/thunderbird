
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
 *         - Right ranges - 7 to 10 - 7 being the center
 * CAUTION: 4 is extreme left and 10 is extreme right
 *
 */

typedef enum steer_values
{
    left_0 = 10,
    left_20 = 20,
    left_40 = 30,
    left_60 = 40,
    left_80 = 50,
    left_100 = 60,
    left_120 = 70,
    left_140 = 80,
    left_160 = 90,
    left_180 = 100,

    right_0 = 110,
    right_20 = 120,
    right_40 = 130,
    right_60 = 140,
    right_80 = 150,
    right_100 = 160,
    right_120 = 170,
    right_140 = 180,
    right_160 = 190,
    right_180 = 200,
} steer_turn;

void precise_steer(int command)
{
    Steer_Motor &steer = Steer_Motor::getInstance();
    /*Precise steer left*/
    if (left_0 == command)
    {
        steer.setSteerMotor(7.0);
    }
    else if (left_20 == command)
    {
        steer.setSteerMotor(6.7);
    }
    else if (left_40 == command)
    {
        steer.setSteerMotor(6.4);
    }
    else if (left_60 == command)
    {
        steer.setSteerMotor(6.1);
    }
    else if (left_80 == command)
    {
        steer.setSteerMotor(5.8);
    }
    else if (left_100 == command)
    {
        steer.setSteerMotor(5.5);
    }
    else if (left_120 == command)
    {
        steer.setSteerMotor(5.2);
    }
    else if (left_140 == command)
    {
        steer.setSteerMotor(4.9);
    }
    else if (left_160 == command)
    {
        steer.setSteerMotor(4.6);
    }
    else if (left_180 == command)
    {
        steer.setSteerMotor(4.3);
    }

    /*Precise steer right*/
    else if (right_0 == command)
    {
        steer.setSteerMotor(7.0);
    }
    else if (right_20 == command)
    {
        steer.setSteerMotor(7.3);
    }
    else if (right_40 == command)
    {
        steer.setSteerMotor(7.6);
    }
    else if (right_60 == command)
    {
        steer.setSteerMotor(7.9);
    }
    else if (right_80 == command)
    {
        steer.setSteerMotor(8.2);
    }
    else if (right_100 == command)
    {
        steer.setSteerMotor(8.5);
    }
    else if (right_120 == command)
    {
        steer.setSteerMotor(8.8);
    }
    else if (right_140 == command)
    {
        steer.setSteerMotor(9.1);
    }
    else if (right_160 == command)
    {
        steer.setSteerMotor(9.4);
    }
    else if (right_180 == command)
    {
        steer.setSteerMotor(9.7);
    }
}
