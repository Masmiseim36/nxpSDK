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
