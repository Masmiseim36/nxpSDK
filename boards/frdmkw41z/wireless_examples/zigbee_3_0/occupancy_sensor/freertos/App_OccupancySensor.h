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

#ifndef APP_OCCUPANCY_SENSOR_H_
#define APP_OCCUPANCY_SENSOR_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "occupancy_sensor.h"
#include "zcl_options.h"
#include "app_reporting.h"
/****************************************************************************/
/***        Macro definition                                              ***/
/****************************************************************************/

#define APP_OCCUPANCY_SENSOR_UNOCCUPIED_TO_OCCUPIED_DELAY           10
#define APP_OCCUPANCY_SENSOR_TRIGGER_THRESHOLD                      5
#define APP_OCCUPANCY_SENSOR_OCCUPIED_TO_UNOCCUPIED_DELAY           180

#define APP_JOINING_BLINK_TIME	      (ZTIMER_TIME_MSEC(1000))
#define APP_FIND_AND_BIND_BLINK_TIME  (ZTIMER_TIME_MSEC(500))
#define APP_KEEP_AWAKE_TIME           (ZTIMER_TIME_MSEC(250))

#define FAILED_POLL_COUNT             3
#define FAILED_REJOIN_COUNT           3

/* Only one reportable attribute that is Occupancy attribute */
#define OCCUPANCY_NUMBER_OF_REPORTS   1
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZLO_OccupancySensorDevice sSensor;
extern tsReports asDefaultReports[];
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

void vAPP_ZCL_DeviceSpecific_Init(void);
teZCL_Status eApp_ZLO_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr);
PUBLIC void vAPP_ZCL_DeviceSpecific_UpdateIdentify(void);
PUBLIC void vAPP_ZCL_DeviceSpecific_SetIdentifyTime(uint16 u16Time);
PUBLIC void vAPP_ZCL_DeviceSpecific_IdentifyOff(void);
PUBLIC uint8 app_u8GetDeviceEndpoint( void);

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* APP_OCCUPANCY_SENSOR_H_ */
