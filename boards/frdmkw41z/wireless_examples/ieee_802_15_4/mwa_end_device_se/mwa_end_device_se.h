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

#ifdef __cplusplus
    extern "C" {
#endif

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

#define mMacExtendedAddress_c    (0x2222222222222222)

#ifdef gPHY_802_15_4g_d
  #define mDefaultValueOfChannel_c (0x000000000003FFFF)
  #define mEnterLowPowerWhenIdle_c (0)
#else
  #define mDefaultValueOfChannel_c (0x07FFF800)
  #define mEnterLowPowerWhenIdle_c (0)
#endif

/* Maximum number of outstanding packets */
#define mDefaultValueOfMaxPendingDataPackets_c 2

/* The slow polling interval is used if the coordinator
   had no data last time we polled. */
#define mDefaultValueOfPollIntervalSlow_c 820 /* ~(0.001*mDefaultValueOfPollIntervalSlow_c) secs */

/* The fast polling interval is used if the coordinator had data last time we
   polled, thus we increase the band-width while data is available. */
#define mDefaultValueOfPollIntervalFast_c 82 /* ~(0.001*mDefaultValueOfPollIntervalFast_c) secs */

/* The security level */
#define mDefaultValueOfSecurityLevel_c	gMacSecurityEncMic64_c

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* All states in the applications state machine */
enum {
  stateInit,
  stateScanActiveStart,
  stateScanActiveWaitConfirm,
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
}
#endif

/**********************************************************************************/
#endif /* _APP_H_ */
