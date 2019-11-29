/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_GREEN_POWER_H
#define APP_GREEN_POWER_H

/*!
\file       app_green_power.h
\brief      Light Bulb application - Green power - Implementation
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "gp.h"


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

void vApp_GP_RegisterDevice(tfpZCL_ZCLCallBackFunction fptrEPCallBack);
void vApp_GP_EnterCommissioningMode(void);
void vAPP_GP_LoadPDMData(void);
void vHandleGreenPowerEvent(tsGP_GreenPowerCallBackMessage *psGPMessage);
void vAPP_GP_ResetData(void);
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* APP_GREEN_POWER_H */
