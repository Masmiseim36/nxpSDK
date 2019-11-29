/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef  DRIVER_SHIM_H_INCLUDED
#define  DRIVER_SHIM_H_INCLUDED

/*!
\file       DriverBulb_Shim.h
\brief      Shim functions to provide generic interface to upper layer
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
/* SDK includes */
#include <jendefs.h>
#include "DriverBulb.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Public Function Prototypes                                    ***/
/****************************************************************************/

/* Interface functions */

PUBLIC void vBULB_Init(void);
PUBLIC void vBULB_SetOnOff(bool_t bOn);
PUBLIC void vBULB_SetLevel(uint32 u32Level);
PUBLIC void vBULB_SetColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue);
PUBLIC void vBULB_SetColourTemperature(uint32 u32ColTemp);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* DRIVER_SHIM_H_INCLUDED */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
