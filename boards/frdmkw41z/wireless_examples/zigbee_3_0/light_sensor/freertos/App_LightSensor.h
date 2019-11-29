/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_LIGHT_SENSOR_H_
#define APP_LIGHT_SENSOR_H_

/*!
\file       App_LightSensor.h
\brief      ZLO Demo Light Sensor -Interface
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "light_sensor.h"
#include "zcl_options.h"
#include "app_reporting.h"

/****************************************************************************/
/***        Macro Definition                                              ***/
/****************************************************************************/
#define LIGHT_SENSOR_MINIMUM_MEASURED_VALUE                            0x0001
#define LIGHT_SENSOR_MAXIMUM_MEASURED_VALUE                            0xFAF
#define LIGHT_SENSOR_MINIMUM_REPORTABLE_CHANGE                         0x01
#define LIGHT_SENSOR_SAMPLING_TIME_IN_SECONDS                          5

#define APP_JOINING_BLINK_TIME          (ZTIMER_TIME_MSEC(1000))
#define APP_FIND_AND_BIND_BLINK_TIME    (ZTIMER_TIME_MSEC(500))
#define APP_KEEP_AWAKE_TIME             (ZTIMER_TIME_MSEC(250))

#define LED_LEVEL                                                      20
#define LED_MAX_LEVEL                                                  255
#define LED_MIN_LEVEL                                                  0

#define FAILED_POLL_COUNT                               3
#define FAILED_REJOIN_COUNT                             3
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZLO_LightSensorDevice sSensor;
extern tsReports asDefaultReports[];
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

void vAPP_ZCL_DeviceSpecific_Init(void);
teZCL_Status eApp_ZCL_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr);
PUBLIC void vAPP_ZCL_DeviceSpecific_UpdateIdentify(void);
PUBLIC void vAPP_ZCL_DeviceSpecific_SetIdentifyTime(uint16 u16Time);
PUBLIC void vAPP_ZCL_DeviceSpecific_IdentifyOff(void);
PUBLIC void APP_cbTimerLightSensorSample(void *pvParam);
PUBLIC void vAPP_LightSensorSample(void);
PUBLIC uint8 app_u8GetDeviceEndpoint( void);

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* APP_LIGHT_SENSOR_H_ */
