/*! *********************************************************************************
 * \defgroup Beacon
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Beacon application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef BEACON_H
#define BEACON_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

#ifndef gIBeaconAdvData_c
#define gIBeaconAdvData_c 0
#endif

#if defined(gIBeaconAdvData_c) && (gIBeaconAdvData_c)
#define gAdvUuidOffset_c        4U
#else
#define gAdvUuidOffset_c        3U
#endif /* gIBeaconAdvData_c */

#ifndef gBeaconAE_c
#define gBeaconAE_c 0
#endif
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

extern const gapAdvertisingParameters_t gAppAdvParams;
#if gBeaconAE_c
extern gapAdvertisingData_t             gAppExtAdvertisingNoData;
extern gapExtAdvertisingParameters_t    gExtAdvParams;
extern gapAdvertisingData_t             gAppExtAdvertisingData;
extern gapPeriodicAdvParameters_t       gPeriodicAdvParams;
#endif

extern gapAdvertisingData_t             gAppAdvertisingData;
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


#endif /* BEACON_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
