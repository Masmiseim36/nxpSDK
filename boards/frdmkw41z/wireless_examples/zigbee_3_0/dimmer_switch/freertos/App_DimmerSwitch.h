/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_DIMMER_SWITCH_H
#define APP_DIMMER_SWITCH_H

/*!
\file       App_DimmerSwitch.h
\brief      ZLO Controller & Switch Demo: Dimmer Switch Interface
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "dimmer_switch.h"
#include "zcl.h"

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsZLO_DimmerSwitchDevice sSwitch;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

void vAPP_ZCL_DeviceSpecific_Init(void);
teZCL_Status eApp_ZCL_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr);
PUBLIC void vAPP_ZCL_DeviceSpecific_UpdateIdentify(void);
PUBLIC void vAPP_ZCL_DeviceSpecific_SetIdentifyTime(uint16 u16Time);
PUBLIC void vAPP_ZCL_DeviceSpecific_IdentifyOff(void);
PUBLIC uint8 app_u8GetDeviceEndpoint( void);

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif
