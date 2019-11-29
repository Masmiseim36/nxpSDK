/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_LI_H
#define APP_LI_H

/*!
\file       app_buttons.h
\brief      Light Bulb application: ZCL Linear Interpolation - Implementation
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vLI_SetCurrentValues(uint32 u32Level, uint32 u32Red, uint32 u32Green, uint32 u32Blue, uint32 u32ColTemp);
PUBLIC void vLI_Start(uint32 u32Level,uint32 u32Red, uint32 u32Green, uint32 u32Blue, uint32 u32ColTemp);
PUBLIC void vLI_Stop(void);
PUBLIC void vLI_CreatePoints(void);
PUBLIC void vLI_UpdateDriver(void);
PUBLIC void vSaveVars(void);
PUBLIC void vRestoreVars(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

#endif /* APP_LI_H */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
