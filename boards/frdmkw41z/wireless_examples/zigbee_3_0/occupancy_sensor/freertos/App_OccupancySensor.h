/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
