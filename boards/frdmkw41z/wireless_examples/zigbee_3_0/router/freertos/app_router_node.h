/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_ROUTER_NODE_H_
#define APP_ROUTER_NODE_H_

#include "zcl_options.h"

#ifdef CLD_OTA
#include "OTA.h"
#endif
#include "app_common.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitialiseRouter(void);
PUBLIC void APP_vFactoryResetRecords(void);
PUBLIC void APP_taskRouter(void);
PUBLIC void APP_vOobcSetRunning(void);
PUBLIC uint8 app_u8GetDeviceEndpoint(void);

#ifdef CLD_OTA
PUBLIC tsOTA_PersistedData sGetOTACallBackPersistdata(void);
#endif
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern tsNodeState sNodeState;
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_ROUTER_NODE_H_*/
