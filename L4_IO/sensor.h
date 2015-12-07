/*
 * sensor.h
 *
 *  Created on: Nov 26, 2015
 *      Author: Nitesh
 */

#ifndef SENSOR_H_
#define SENSOR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "eint.h"


void trigger_LeftSensor();
void trigger_MidSensor();
void trigger_RightSensor();
void trigger_BackSensor();


void eintCallbackback_Rise();
void eintCallbackback_Fall();
void initPWback(eint_intr_t eintType);


void eintCallbackleft_Rise();
void eintCallbackleft_Fall();
void initPWleft(eint_intr_t eintType);

void eintCallbackright_Rise();
void eintCallbackright_Fall();
void initPWright(eint_intr_t eintType);

void eintCallbackmid_Rise();
void eintCallbackmid_Fall();
void initPWmid(eint_intr_t eintType);

void InitInterruptLeft();
void InitInterruptRight();
void InitInterruptMid();
void InitInterruptBack();

void sendsensorvalues(uint32_t,uint32_t,uint32_t,uint32_t);
//int GetLeftSensorReading();
//int GetRightSensorReading();
//int GetMidSensorReading();

#ifdef __cplusplus
}
#endif
#endif /* SENSOR_H_ */
