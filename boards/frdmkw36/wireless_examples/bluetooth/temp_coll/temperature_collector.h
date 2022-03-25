/*! *********************************************************************************
 * \defgroup Temperature Collector
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Temperature Collector application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef TEMPERATURE_COLLECTOR_H
#define TEMPERATURE_COLLECTOR_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/
#define gScanningTime_c                30

#define gWaitForDataTime_c             5

#ifndef gAppDeepSleepMode_c
#define gAppDeepSleepMode_c            1
#endif

#ifndef gAppPrintLePhyEvent_c
#define gAppPrintLePhyEvent_c    0
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern gapConnectionRequestParameters_t gConnReqParams;
extern gapScanningParameters_t          gScanParams;
extern gapPairingParameters_t           gPairingParameters;

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

#ifdef __cplusplus
}
#endif


#endif /* TEMPERATURE_COLLECTOR_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
