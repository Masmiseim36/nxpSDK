/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_COLOR_TEMPERATURE_LIGHT_H
#define APP_COLOR_TEMPERATURE_LIGHT_H

/*!
\file       App_ColorTemperatureLight.h
\brief      ZLO Demo: Color Temperature Light Interface
*/
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "colour_temperature_light.h"
#include "commission_endpoint.h"

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZLO_ColourTemperatureLightDevice sLight;
extern tsCLD_ZllDeviceTable sDeviceTable;
extern tsReports asDefaultReports[];

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC teZCL_Status eApp_ZLO_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr);
PUBLIC void vApp_eCLD_ColourControl_GetRGB(uint8* pu8Red,uint8* pu8Green,uint8* pu8Blue);
PUBLIC void vAPP_ZCL_DeviceSpecific_Init(void);
PUBLIC void vStartEffect(uint8 u8Effect);
PUBLIC void vIdEffectTick(uint8 u8Endpoint);
PUBLIC void vRGBLight_Init(uint8 u8Prescale, uint8 u8Red, uint8 u8Green, uint8 u8Blue);
PUBLIC void vRGBLight_SetLevels(bool_t bOn, uint8 u8Level, uint8 u8Red, uint8 u8Green, uint8 u8Blue);
PUBLIC void APP_vHandleIdentify(uint16 u16Time);
PUBLIC void vTunableWhiteLightSetLevels(bool_t bOn, uint32 u32Level, int32 i32ColourTemperature);
PUBLIC uint8 app_u8GetDeviceEndpoint( void);
PUBLIC void vApp_ZCL_ResetDeviceStructure(void);
PUBLIC void vUpdateBulbFromZCL(bool_t bResetInterpolation);



/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* APP_COLOR_TEMPERATURE_LIGHT_H */
