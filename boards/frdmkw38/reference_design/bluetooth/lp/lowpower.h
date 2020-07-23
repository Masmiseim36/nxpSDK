/*! *********************************************************************************
 * \defgroup LowPowerRefDes
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the Temperature Sensor application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __LOWPOWER_H__
#define __LOWPOWER_H__

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

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
extern gapExtAdvertisingParameters_t gAppExtAdvParams;
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void BleApp_Init(void);

void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent);

#ifdef __cplusplus
}
#endif


#endif /* TEMPERATURE_SENSOR_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
