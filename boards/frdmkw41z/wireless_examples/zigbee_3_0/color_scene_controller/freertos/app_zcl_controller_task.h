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
