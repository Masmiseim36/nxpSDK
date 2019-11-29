/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_ZCL_SENSOR_TASK_H_
#define APP_ZCL_SENSOR_TASK_H_

/*!
\file       app_zcl_sensor_task.h
\brief      ZLO Light Sensor Behavior (Interface)
*/


/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zcl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define ZCL_TICK_TIME                      ZTIMER_TIME_MSEC(1000)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_ZCL_vInitialise(void);
PUBLIC void APP_ZCL_vEventHandler(ZPS_tsAfEvent* psStackEvent);
PUBLIC void vAPP_ZCL_Tick(void);
PUBLIC void APP_cbTimerZclTick( void *pvParam);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_ZCL_SENSOR_TASK_H_ */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
