/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
