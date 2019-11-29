/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_COLOR_SCENE_CONTROLLER_H
#define APP_COLOR_SCENE_CONTROLLER_H

#include "colour_scene_controller.h"
#include "commission_endpoint.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// Total number of group addresses required by all sub-device endpoints on the device
#define GROUPS_REQUIRED 4

/* Application device Version */
#define APPLICATION_DEVICE_VERSION      1
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZLO_ColourSceneControllerDevice sRemote;
extern tsCLD_ZllDeviceTable sDeviceTable;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

teZCL_Status eApp_ZLO_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr);
void vAPP_ZCL_DeviceSpecific_Init(void);
PUBLIC uint8 app_u8GetDeviceEndpoint( void);
PUBLIC void vApp_ZCL_ResetDeviceStructure(void);
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
#endif
