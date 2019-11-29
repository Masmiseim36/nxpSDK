/*! *********************************************************************************
 * \defgroup IPv6 Router
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the IPv6 Router application
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_H_
#define _APP_H_
  
#include "EmbeddedTypes.h"
#include "ble_abs_types.h"   
#include "gap_types.h"   

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/*! Defines the MTU supported by the L2cap Channel */
#ifndef	gL2capMtu_d
#define gL2capMtu_d                 1280
#endif

/*! Enables Debug Logging on L2CAP Credit Based Channels */
#ifndef gAppIpspDebugLog_d
#define gAppIpspDebugLog_d   0
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void BleApp_Init(void);
void BleApp_Start(void);
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent);
bleAbsRequests_t* BleApp_RegisterBleCallbacks(instanceId_t instanceId);

#ifdef __cplusplus
}
#endif 


#endif /* _APP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
