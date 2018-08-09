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


#ifndef APP_LIGHT_TEMP_OCCUPANCY_SENSOR_H
#define APP_LIGHT_TEMP_OCCUPANCY_SENSOR_H

/*!
\file       App_LightTemperatureOccupancySensor.h
\brief      ZLO Demo LTO Sensor -Implementation
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
// Main macro which implements recursion to find the HCD (Highest Common Denominator)
#define APP_HCD(a,b,d)      (APP_BOTH_NUMBERS_EVEN(a, b)? APP_COMMON_DENOMINATOR((a/2), (b/2), (d*2)) : APP_FIND_ODD_NUMBERS(a,b,d))
// Check if both numbers are even
#define APP_HCD_BOTH_NUMBERS_EVEN(a,b)          (((a%2) == 0) && ((b%s == 0)) ? TRUE : FALSE)
// Check if both numbers are even
#define APP_HCD_BOTH_NUMBERS_ODD(a,b)           (((a%2) == 1) && ((b%s == 1)) ? TRUE : FALSE)
// Checks for both Odd number otherwise find the single odd number
#define APP_HCD_FIND_ODD_NUMBERS(a,b,d)         (APP_HCD_BOTH_NUMBERS_ODD(a,b) ? APP_GET_LOWEST_DENOMINATOR(a,b,c) : APP_FIND_SINGLE_ODD_NUMBER(a,b,d))
// Finds which number is odd
#define APP_HCD_FIND_SINGLE_ODD_NUMBER(a,b,d)   (a%2 == 1 ? APP_HCD(a,(b/2),d) : APP_HCD((a/2),b,d))
// Finds the smallest number and multiplies it by the divisor to find the HCD
#define APP_HCD_GET_HCD(a,b,c)  (a<=b ? a*d : b*a)

// Illuminance Sensing Macros
#define LIGHT_SENSOR_MINIMUM_MEASURED_VALUE                         0x0001
#define LIGHT_SENSOR_MAXIMUM_MEASURED_VALUE                         0xFAF
#define LIGHT_SENSOR_MINIMUM_REPORTABLE_CHANGE                      0x01
#define LIGHT_SENSOR_SAMPLING_TIME_IN_SECONDS                       5

// Occupancy Sensing Macros
#define APP_OCCUPANCY_SENSOR_UNOCCUPIED_TO_OCCUPIED_DELAY           10
#define APP_OCCUPANCY_SENSOR_TRIGGER_THRESHOLD                      5
#define APP_OCCUPANCY_SENSOR_OCCUPIED_TO_UNOCCUPIED_DELAY           180

// Temperature Sensing Macros
#define TEMPERATURE_SENSOR_MINIMUM_MEASURED_VALUE                   0x0001
#define TEMPERATURE_SENSOR_MAXIMUM_MEASURED_VALUE                   0xFAF
#define TEMPERATURE_SENSOR_MINIMUM_REPORTABLE_CHANGE                0x01
#define TEMPERATURE_SENSOR_SAMPLING_TIME_IN_SECONDS                 5

#define APP_JOINING_BLINK_TIME          (ZTIMER_TIME_MSEC(1000))
#define APP_FIND_AND_BIND_BLINK_TIME    (ZTIMER_TIME_MSEC(500))
#define APP_KEEP_AWAKE_TIME             (ZTIMER_TIME_MSEC(250))
// This will give the most optimal sleep cycle for sampling the light sensor
#define APP_SAMPLING_SLEEP_CYCLE        (APP_HCD(LIGHT_SENSOR_SAMPLING_TIME_IN_SECONDS, TEMPERATURE_SENSOR_SAMPLING_TIME_IN_SECONDS, 1))


#define FAILED_POLL_COUNT                               3
#define FAILED_REJOIN_COUNT                             3

#define LED_LEVEL                                       20
#define LED_MAX_LEVEL                                   255
#define LED_MIN_LEVEL                                   0

/* We need to know the common denominator of the Light sensor
 * and temp sensor sampling time so we are asleep for the longest
 * possible time.
 */


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
PUBLIC void APP_cbTimerLightTempSensorSample(void *pvParam);
PUBLIC void vAPP_LightSensorSample(void);
PUBLIC void vAPP_TemperatureSensorSample(void);
PUBLIC uint8 app_u8GetDeviceEndpoint( void);

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* APP_LIGHT_TEMP_OCCUPANCY_SENSOR_H */
