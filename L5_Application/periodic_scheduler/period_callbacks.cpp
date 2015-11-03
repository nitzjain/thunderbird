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

/**
 * @file
 * This contains the period callback functions for the periodic scheduler
 *
 * @warning
 * These callbacks should be used for hard real-time system, and the priority of these
 * tasks are above everything else in the system (above the PRIORITY_CRITICAL).
 * The period functions SHOULD NEVER block and SHOULD NEVER run over their time slot.
 * For example, the 1000Hz take slot runs periodically every 1ms, and whatever you
 * do must be completed within 1ms.  Running over the time slot will reset the system.
 */

#include <stdint.h>
#include "stdio.h"
#include "io.hpp"
#include "periodic_callback.h"
#include "can.h"
#include "lpc_pwm.hpp"
#include "singleton_template.hpp"
#include "can_periodic/canperiodicext.hpp"
#include "motor_directions/motor_directions.hpp"

/// This is the stack size used for each of the period tasks
const uint32_t PERIOD_TASKS_STACK_SIZE_BYTES = (512 * 4);

//can message id
can_msg_t control;

unsigned int valueNeeded;

#define up    7.9
#define up_L  7.6
#define up_R  7.6
#define down  5.5
#define SL    5
#define SR    8

#if 0
class MotorController1: public SingletonTemplate<MotorController1>
{
    public:

    void setDriveMotor(float v)
    {
        mDriveMotor.set(v);
    }
    private:
    PWM mDriveMotor;

    MotorController1() :
    mDriveMotor(PWM::pwm1, 50)
    {

    }
    friend class SingletonTemplate<MotorController1>;
    ///< Friend class used for Singleton Tem

};

class MotorController2: public SingletonTemplate<MotorController2>
{
    public:
    void setSteerMotor(float v)
    {
        mSteerMotor.set(v);
    }

    private:
    PWM mSteerMotor;

    MotorController2() :
    mSteerMotor(PWM::pwm2, 50)
    {

    }
    friend class SingletonTemplate<MotorController2>;
    ///< Friend class used for Singleton Tem

};

#endif

class MotorController: public SingletonTemplate<MotorController>
{
    public:

        void setDriveMotor(float v)
        {
            mDriveMotor.set(v);
        }
    private:
        PWM mDriveMotor;
       // PWM mSteerMotor;

        MotorController() : mDriveMotor(PWM::pwm1)//, mSteerMotor(PWM::pwm2)
        {

        }

        friend class SingletonTemplate<MotorController> ;
        ///< Friend class used for Singleton Template
};

//MotorController &M = MotorController::getInstance();

class MotorController2: public SingletonTemplate<MotorController2>
{
    public:
    void setSteerMotor(float v)
    {
        mSteerMotor.set(v);
    }

    private:
    PWM mSteerMotor;

    MotorController2() : mSteerMotor(PWM::pwm2)
    {

    }
    friend class SingletonTemplate<MotorController2>;
    ///< Friend class used for Singleton Tem

};

void period_1Hz(void)
{
    //LE.off(1);
    //LE.off(2);
    //LE.off(3);
    //LE.off(4);
}

void period_10Hz(void)
{

}

void period_100Hz(void)
{
    MotorController &M = MotorController::getInstance();
    MotorController2 &M2 = MotorController2::getInstance();
    //should test in 1Hz
    static int flag = 0;

    if (flag == 0)
    {
        M.setDriveMotor(7.5); //Initialize the motor
        M2.setSteerMotor(7);
        printf("Initialzed");
        // M.setDriveMotor(up); //Move forward
        flag++;
    }

    /* Move forward - */
    if (CAN_rx(can1, &control, 1))
    {
        if (control.msg_id == forward) //go forward - 000
        {
            M.setDriveMotor(up); //Move forward
            M2.setSteerMotor(7);
             LE.toggle(1);
        }
        else if (control.msg_id == left) //go left 001, 011
        {
            M2.setSteerMotor(SL);
            M.setDriveMotor(up_L);
             LE.on(2);
        }
        else if (control.msg_id == right) //go right 010, 100, 110
        {
            M2.setSteerMotor(SR);
            M.setDriveMotor(up_R);
            LE.on(3);
        }
        else if (control.msg_id == reverse) //go back 111, 101
        {
            M.setDriveMotor(7.5);
            M.setDriveMotor(down);
            M2.setSteerMotor(SR);
            LE.toggle(4);
        }
        else if (control.msg_id == stop)
        {
            M.setDriveMotor(7.5);
            M2.setSteerMotor(7);
        }
    }
}

void period_1000Hz(void)
{

}
