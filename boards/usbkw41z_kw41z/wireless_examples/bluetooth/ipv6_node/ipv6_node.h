/*! *********************************************************************************
 * \defgroup IPv6 Node
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the IPv6 Node application
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_H_
#define _APP_H_
   
#include "ble_abs_types.h"   
#include "gap_types.h"   

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */

#define gFastConnMinAdvInterval_c       32 /* 20 ms */
#define gFastConnMaxAdvInterval_c       48 /* 30 ms */

#define gReducedPowerMinAdvInterval_c   1600 /* 1 s */
#define gReducedPowerMaxAdvInterval_c   4000 /* 2.5 s */

#define gFastConnAdvTime_c              30  /* s */
#define gReducedPowerAdvTime_c          300 /* s */

#if gAppUseBonding_d
#define gFastConnWhiteListAdvTime_c     10 /* s */
#else
#define gFastConnWhiteListAdvTime_c     0
#endif

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

void BleAbs_DataReq(bleDataReq_t * pParam, uint32_t upperLayerInstanceId);


uint32_t   BleAbs_GetMaxMsduSize(bleDataReq_t *pParam);

void   BleAbs_GetBleAddress(bleAddr_t aOutAddress);

bleAbsRequests_t* BleApp_RegisterBleCallbacks(instanceId_t instanceId);

#ifdef __cplusplus
}
#endif 


#endif /* _APP_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
