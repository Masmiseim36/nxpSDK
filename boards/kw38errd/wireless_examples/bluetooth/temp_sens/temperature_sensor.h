/*! *********************************************************************************
 * \defgroup Temperature Sensor
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Temperature Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */

#define gReducedPowerMinAdvInterval_c   1600 /* 1 s */
#define gReducedPowerMaxAdvInterval_c   4000 /* 2.5 s */

#define gAdvTime_c                      30 /* 30 s*/
#define gGoToSleepAfterDataTime_c       5 /* 5 s*/

#ifndef gAppDeepSleepMode_c
#define gAppDeepSleepMode_c 1
#endif

#ifndef gAppPrintLePhyEvent_c
#define gAppPrintLePhyEvent_c    0
#endif

/* The application starts advertising without the need to press a button when this define is set*/
#ifndef gAppStartAfterReset_d
#define gAppStartAfterReset_d             0
#endif

/* When gAppStartAfterReset_d is set, this define configures the time the device is disallowed to enter low power 
after the start of the advertising. This delay must be long enough to allow the 32KHZ oscillator to start. 
The 32KHZ oscillator is known to have a slow start (about 400ms). Without it running the LL is not able to enter low power */
#ifndef gAppStartAfterResetToLowPowerDelayMs_d
#define gAppStartAfterResetToLowPowerDelayMs_d             (500)        
#endif
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern gapAdvertisingData_t         gAppAdvertisingData;
extern gapScanResponseData_t        gAppScanRspData;
extern gapAdvertisingParameters_t   gAdvParams;
extern gapSmpKeys_t                 gSmpKeys;
extern gapPairingParameters_t       gPairingParameters;
extern gapDeviceSecurityRequirements_t deviceSecurityRequirements;

extern const gapAdStructure_t advScanStruct[3];
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


#endif /* TEMPERATURE_SENSOR_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
