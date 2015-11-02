/*
 * sensor.h
 *
 *  Created on: Oct 26, 2015
 *      Author: Rajashree
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

int GetLeftSensorReading();
int GetRightSensorReading();
int GetMidSensorReading();
#ifdef __cplusplus
}
#endif
#endif /* SENSOR_H_ */
