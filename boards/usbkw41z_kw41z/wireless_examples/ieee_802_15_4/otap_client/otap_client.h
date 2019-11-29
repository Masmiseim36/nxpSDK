/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for OTA Programming App Demo Client application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_H_
#define _APP_H_

#ifdef __cplusplus
    extern "C" {
#endif

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

#define mMacExtendedAddress_c    (0xFFFFFFFFFFFFFFFF)

#ifdef gPHY_802_15_4g_d
  #define mDefaultValueOfChannel_c (0x000000000003FFFF)
#else
  #define mDefaultValueOfChannel_c (0x07FFF800)
#endif

/* Maximum number of outstanding packets */
#define mDefaultValueOfMaxPendingDataPackets_c 2

/* The slow polling interval is used if the coordinator
   had no data last time we polled. */
#define mDefaultValueOfPollIntervalSlow_c 820 /* ~(0.001*mDefaultValueOfPollIntervalSlow_c) secs */

/* The fast polling interval is used if the coordinator had data last time we
   polled, thus we increase the band-width while data is available. */
#define mDefaultValueOfPollIntervalFast_c 82 /* ~(0.001*mDefaultValueOfPollIntervalFast_c) secs */

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

#define AppRelease_d   0x10
#define AppBuild_d     0x00

#define StackRelease_d 0x10
#define StackBuild_d   0x00
        
#define mActualFileVersion ((AppRelease_d   <<  0) | \
                            (AppBuild_d     <<  8) | \
                            (StackRelease_d << 16) | \
                            (StackBuild_d   << 24))

#define HwVersion_d    0x20
#define HwRevision_d   0x00

#define gOtaTimeout_c     (1000 * 10)  /* 10 seconds */
#define gOtaWaitTimeout_c (1000 * 60 * 2) /* 2 minnutes */


/* The various states of the applications state machines. */
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
#define gAppEvtRxFromComm_c            (1 << 1)
#define gAppEvtMessageFromMLME_c       (1 << 2)
#define gAppEvtMessageFromMCPS_c       (1 << 3)

#define gAppEvtQueryNextImageRequest_c  (1 << 4)
#define gAppEvtImageBlockRequest_c      (1 << 5)
#define gAppEvtUpgradeEndRequest_c      (1 << 6)


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
