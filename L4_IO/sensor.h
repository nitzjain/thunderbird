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


void trigger_LeftSensor();
void trigger_CentralSensor();
void trigger_RightSensor();
void trigger_all_sensors();
void leftSensorRiseEdge();
void leftSensorfallEdge();

#ifdef __cplusplus
}
#endif
#endif /* SENSOR_H_ */
