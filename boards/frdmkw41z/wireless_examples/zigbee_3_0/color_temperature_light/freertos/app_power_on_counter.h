/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_POWER_ON_COUNTER_H_
#define APP_POWER_ON_COUNTER_H_

/*!
\file       app_power_on_counter.h
\brief      ZHA Demo : Stack <-> Light-App Interaction (Interface)
*/
     
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zcl.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define POWER_ON_COUNTER_DB_IN_MSEC 1750
#define POWER_ON_PRE_INIT_COUNTER_DB_IN_MSEC 250
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
	uint8 u8PowerOnCounter;
}tsPowerOnCounts;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vManagePowerOnCountLoadRecord(void);
PUBLIC void vManagePowerOnCountInit(void);
PUBLIC void vManagePowerOnCountReset(void);
PUBLIC void APP_cbTimerPowerCount( void * pvParam);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/

/****************************************************************************/
/****************************************************************************/

#endif //APP_POWER_ON_COUNTER_H_
