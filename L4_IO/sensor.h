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
void trigger_CentralSensor();
void trigger_RightSensor();
void trigger_all_sensors();
int Left_run();
int Right_run();
int Mid_run();
void initPWDummy(eint_intr_t eintType);
void eintCallbackDummy();
int Dummy_run();
int GetDummyReading();

void initPWleft(eint_intr_t eintType);
void eintCallbackleft();
void initPWright(eint_intr_t eintType);
void eintCallbackright();
void initPWmid(eint_intr_t eintType);
void eintCallbackmid();
int GetLeftSensorReading();
int GetRightSensorReading();
int GetMidSensorReading();
#ifdef __cplusplus
}
#endif
#endif /* SENSOR_H_ */
