/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_DIMMABLE_LIGHT_H
#define APP_DIMMABLE_LIGHT_H

/*!
\file       App_DimmableLight.h
\brief      ZLO Demo: Dimmable Light Interface
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "dimmable_light.h"
#include "commission_endpoint.h"

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern tsZLO_DimmableLightDevice sLight;
extern tsCLD_ZllDeviceTable sDeviceTable;
extern tsReports asDefaultReports[];


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eApp_ZLO_RegisterEndpoint(tfpZCL_ZCLCallBackFunction fptr);
PUBLIC void vAPP_ZCL_DeviceSpecific_Init(void);
PUBLIC void vSetBulbState(bool bOn, uint8 u8Level);
PUBLIC void vStartEffect(uint8 u8Effect);
PUBLIC void vIdEffectTick( uint8 u8Endpoint);
PUBLIC void APP_vHandleIdentify(uint16 u16Time);
PUBLIC void vCreateInterpolationPoints(void);
PUBLIC uint8 app_u8GetDeviceEndpoint( void);
PUBLIC void vApp_ZCL_ResetDeviceStructure(void);
PUBLIC void vUpdateBulbFromZCL(bool_t bResetInterpolation);

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* APP_DIMMABLE_LIGHT_H */
