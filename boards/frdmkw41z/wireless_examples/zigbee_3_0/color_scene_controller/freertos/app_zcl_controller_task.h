/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/****************************************************************************/
/* Description.                                                             */
/* If you do not need this file to be parsed by doxygen then delete @file   */
/****************************************************************************/

/** @file
 * Add brief description here.
 * Add more detailed description here
 */

/****************************************************************************/
/* Description End                                                          */
/****************************************************************************/

#ifndef APP_ZCL_TASK_H_
#define APP_ZCL_TASK_H_

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_ZCL_vInitialise(void);
PUBLIC void APP_ZCL_vTask(void);
PUBLIC void APP_ZCL_vSendHAToggle(tsZCL_Address *sAddress, uint8 u8SwitchEndPoint, uint8 u8LightEndPoint);
PUBLIC void APP_ZCL_vSendHAOff(tsZCL_Address *sAddress, uint8 u8SwitchEndPoint, uint8 u8LightEndPoint);
PUBLIC void APP_ZCL_vSendHAOn(tsZCL_Address *sAddress, uint8 u8SwitchEndPoint, uint8 u8LightEndPoint);
PUBLIC void APP_ZCL_vEventHandler(ZPS_tsAfEvent *psStackEvent);
PUBLIC void APP_ZCL_vSendHAMoveUp(tsZCL_Address *psAddress, uint8 u8SwitchEndPoint, uint8 u8LightEndPoint, uint8 u8Rate);
PUBLIC void APP_ZCL_vSendHAMoveDown(tsZCL_Address *psAddress, uint8 u8SwitchEndPoint, uint8 u8LightEndPoint, uint8 u8Rate);
PUBLIC void APP_ZCL_vSendHAMoveStop(tsZCL_Address *psAddress, uint8 u8SwitchEndPoint, uint8 u8LightEndPoint);

PUBLIC bool_t APP_ZCL_bSendHAIdentifyQuery(tsZCL_Address *psAddress, uint8 u8SwitchEndPoint, uint8 u8LightEndPoint);
PUBLIC void APP_cbTimerZclTick(void *pvParam);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_ZCL_TASK_H_ */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
