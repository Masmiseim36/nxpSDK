/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       app_zcl_event_handler.h
\brief      ZigbeeNodeControlBridge
==============================================================================*/

/****************************************************************************/
/* Description End                                                          */
/****************************************************************************/

#ifndef APP_ZCL_ZB_CONTROLBRIDGE_H_
#define APP_ZCL_ZB_CONTROLBRIDGE_H_

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
PUBLIC void APP_ZCL_vInitialise ( void );
PUBLIC void APP_ZCL_vSendHAToggle ( tsZCL_Address*    psAddress,
                                    uint8             u8SwitchEndPoint,
                                    uint8             u8LightEndPoint );

PUBLIC void APP_ZCL_vSendHAOff ( tsZCL_Address*    psAddress,
                                 uint8             u8SwitchEndPoint,
                                 uint8             u8LightEndPoint );
PUBLIC void APP_ZCL_vSendHAOn ( tsZCL_Address*    psAddress,
                                uint8             u8SwitchEndPoint,
                                uint8 u8LightEndPoint );

PUBLIC void APP_ZCL_vSendHAMoveUp ( tsZCL_Address*    psAddress,
                                    uint8             u8SwitchEndPoint,
                                    uint8             u8LightEndPoint,
                                    uint8             u8Rate );

PUBLIC void APP_ZCL_vSendHAMoveDown ( tsZCL_Address*    psAddress,
                                      uint8             u8SwitchEndPoint,
                                      uint8             u8LightEndPoint,
                                      uint8             u8Rate );
PUBLIC void APP_ZCL_vSendHAMoveStop ( tsZCL_Address*    psAddress,
                                      uint8             u8SwitchEndPoint,
                                      uint8             u8LightEndPoint );

PUBLIC bool_t APP_ZCL_bSendHAIdentifyQuery ( tsZCL_Address*    psAddress,
                                             uint8             u8SwitchEndPoint,
                                             uint8 u8LightEndPoint );
teZCL_Status eApp_ZLL_RegisterEndpoint ( tfpZCL_ZCLCallBackFunction    fptr );
void vAPP_ZCL_DeviceSpecific_Init ( void );

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_ZCL_ZB_CONTROLBRIDGE_H_ */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
