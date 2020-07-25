/*! *********************************************************************************
 * \defgroup Extended Advertising Peripheral
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2020 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the extended advertising peripheral application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef ADV_EXT_PERIPHERAL_H
#define ADV_EXT_PERIPHERAL_H

/*************************************************************************************
**************************************************************************************
* Public macros
**************************************************************************************
*************************************************************************************/

/* Profile Parameters */

#define gReducedPowerMinAdvInterval_c   1600 /* 1 s */
#define gReducedPowerMaxAdvInterval_c   4000 /* 2.5 s */

#define gGoToSleepAfterDataTime_c       5 /* 5 s*/

#ifndef gAppDeepSleepMode_c
#define gAppDeepSleepMode_c 1
#endif

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
/* Legacy Advertising Parameters */
extern gapExtAdvertisingParameters_t gExtAdvParamsLegacy;
/* Extended Scannable Advertising Parameters */
extern gapExtAdvertisingParameters_t gExtAdvParamsScannable;
/* Extended Connectable Advertising Parameters */
extern gapExtAdvertisingParameters_t gExtAdvParamsConnectable;
/* Extended Non Connectable Non Scannable Advertising Parameters */
extern gapExtAdvertisingParameters_t gExtAdvParamsNonConnNonScann;
extern gapAdvertisingData_t gAppExtAdvDataScannable;
extern gapAdvertisingData_t gAppExtAdvDataConnectable;
extern gapAdvertisingData_t gAppExtAdvDataId1NonConnNonScan;
extern gapAdvertisingData_t gAppExtAdvDataId2NonConnNonScan;
extern gapAdvertisingData_t gAppExtAdvDataId1Periodic;
extern gapAdvertisingData_t gAppExtAdvDataId2Periodic;
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void BleApp_Init(void);
void BleApp_Start(uint8_t mode);
void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent);

#ifdef __cplusplus
}
#endif


#endif /* ADV_EXT_PERIPHERAL_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
