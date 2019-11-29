/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_ZCL_TASK_H_
#define APP_ZCL_TASK_H_

/*!
\file       app_zcl_light_task.h
\brief      ZLO Demo: ZCL to Light App Binding - Interface
*/
     
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define BULB_SAVE_DELAY_SEC     (2)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_ZCL_vInitialise(void);
PUBLIC void APP_ZCL_vSetIdentifyTime(uint16 u16Time);
PUBLIC void APP_cbTimerZclTick(void *pvParam);
PUBLIC void APP_cbTimerZclMsTick(void *pvParam);
PUBLIC void APP_cbTimerGPZclTick(void *pvParam);
PUBLIC void APP_ZCL_vTask(void);
PUBLIC void APP_ZCL_vEventHandler(ZPS_tsAfEvent *psStackEvent);
PUBLIC void vResetZCLRecords( void);
PUBLIC void APP_vRemoveAllGroupsAndScenes( void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern uint8  u8StateChangeTick;

#endif /* APP_ZCL_TASK_H_ */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
