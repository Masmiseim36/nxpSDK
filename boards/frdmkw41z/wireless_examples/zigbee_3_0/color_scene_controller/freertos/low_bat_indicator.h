/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#if 0
#ifndef  LOWBAT_H_INCLUDED
#define  LOWBAT_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
/* Jennic include files */
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef MK_DK2
#define LHS_LED_DIO (1 << 16)
#define RHS_LED_DIO (1 << 17)
#else
#define LHS_LED_DIO (1 << 0)
#define RHS_LED_DIO (1 << 1)
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
	E_BO_TRIP_1V95 = 0,
	E_BO_TRIP_2V0  = 2,
	E_BO_TRIP_2V1  = 4,
	E_BO_TRIP_2V2  = 6,
	E_BO_TRIP_2V3  = 8,
	E_BO_TRIP_2V4  = 10,
	E_BO_TRIP_2V7  = 12,
	E_BO_TRIP_3V0  = 14
} teBrownOutTripVoltage;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vLowBatIndicationInit(uint32 u32LedMask,teBrownOutTripVoltage eBOTripValue);
PUBLIC void vCbSystemController(uint32 u32DeviceId, uint32 u32ItemBitmap);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


#if defined __cplusplus
}
#endif

#endif  /* LOWBAT_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif