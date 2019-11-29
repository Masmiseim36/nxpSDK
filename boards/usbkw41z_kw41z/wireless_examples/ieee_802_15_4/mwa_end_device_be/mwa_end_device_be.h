/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for MyWirelessApp Demo End Device application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_H_
#define _APP_H_

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */


/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

#define mMacExtendedAddress_c    (0xFFFFFFFFFFFFFFFF)

#ifdef gPHY_802_15_4g_d
  #define mDefaultValueOfChannel_c (0x000000000003FFFF)
  #define mEnterLowPowerWhenIdle_c (0)
#else
  #define mDefaultValueOfChannel_c (0x07FFF800)
  #define mEnterLowPowerWhenIdle_c (0)
#endif

/* Maximum number of outstanding packets */
#define mDefaultValueOfMaxPendingDataPackets_c 2

/* Scan duration must be equal to (Beacon order + 1)  */
#define mDefaultValueOfScanDuration_c 7

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* All states in the applications state machine */
enum {
  stateInit,
  stateScanPassiveStart,
  stateScanPassiveWaitConfirm,
  stateAssociate,
  stateAssociateWaitConfirm,
  stateListen
};

/* Events that are passed to the application task. 
   Are defined as byte masks to make possible 
   send multiple events to the task */

#define gAppEvtDummyEvent_c            (1 << 0)
#define gAppEvtRxFromUart_c            (1 << 1)
#define gAppEvtMessageFromMLME_c       (1 << 2)
#define gAppEvtMessageFromMCPS_c       (1 << 3)


/* Error codes */
enum {
  errorNoError,
  errorWrongConfirm,
  errorNotSuccessful,
  errorNoMessage,
  errorAllocFailed,
  errorInvalidParameter,
  errorNoScanResults
};

/******************************************************************************
*******************************************************************************
* Public Prototypes
*******************************************************************************
******************************************************************************/
#ifdef __cplusplus
    extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/**********************************************************************************/
#endif /* _APP_H_ */
