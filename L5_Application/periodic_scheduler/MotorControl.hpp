/*
 * MotorControl.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: ajosyabhatla
 */

#include "singleton_template.hpp"
#include "can.h"
#include "lpc_pwm.hpp"

#ifndef L5_APPLICATION_PERIODIC_SCHEDULER_MOTORCONTROL_HPP_
#define L5_APPLICATION_PERIODIC_SCHEDULER_MOTORCONTROL_HPP_

class DC_Motor: public SingletonTemplate<DC_Motor>
{
    public:

        void setDriveMotor(float v)
        {
            mDriveMotor.set(v);
        }
    private:
        PWM mDriveMotor;

        DC_Motor() :
                mDriveMotor(PWM::pwm1) //, mSteerMotor(PWM::pwm2)
        {

        }

        friend class SingletonTemplate<DC_Motor> ;
        ///< Friend class used for Singleton Template
};

class Steer_Motor: public SingletonTemplate<Steer_Motor>
{
    public:
        void setSteerMotor(float v)
        {
            mSteerMotor.set(v);
        }

    private:
        PWM mSteerMotor;

        Steer_Motor() :
                mSteerMotor(PWM::pwm2)
        {

        }
        friend class SingletonTemplate<Steer_Motor> ;
        ///< Friend class used for Singleton Template

};

#endif /* L5_APPLICATION_PERIODIC_SCHEDULER_MOTORCONTROL_HPP_ */
