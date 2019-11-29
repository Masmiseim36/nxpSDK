/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for MyStarNetwork Demo Coordinator application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_H_
#define _APP_H_


/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

#define mMacExtendedAddress_c    (0xFFFFFFFFFFFFFFFF)

#ifdef gPHY_802_15_4g_d
#define mDefaultValueOfChannel_c (0x0001FFFF)
#define mDefaultMaxChannel_c     (0x11)
#else
#define mDefaultValueOfChannel_c (0x07FFF800)
#endif

/*Set the Coordinator short address */ 
/* We want the coordinators short address to be 0xCAFE. */
#define mDefaultValueOfShortAddress_c          0xCAFE

/*Set the PanID */ 
/* We want the coordinators short address to be 0xBEEF. */
#define mDefaultValueOfPanId_c                 0xBEEF

/* Define for basic time interval between each transmition of a new message to the
   end devices ( in seconds ). */
#define mDefaultValueOfTimeInterval_c          3


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* Error codes */
enum
{
  errorNoError,
  errorWrongConfirm,
  errorNotSuccessful,
  errorNoMessage,
  errorAllocFailed,
  errorInvalidParameter,
  errorNoScanResults
};

/* The various states of the application state machine. */
enum 
{
  stateInit,
  stateScanEdStart,
  stateScanEdWaitConfirm,
  stateStartCoordinator,
  stateStartCoordinatorWaitConfirm,
  stateListen
};

/* Events */
#define gAppEvtDummyEvent_c            (1 << 0)
#define gAppEvtStartCoordinator_c      (1 << 1)
#define gAppEvtMessageFromMLME_c       (1 << 2)
#define gAppEvtMessageFromMCPS_c       (1 << 3)
#define gAppEvtRxFromComm_c            (1 << 4)


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
