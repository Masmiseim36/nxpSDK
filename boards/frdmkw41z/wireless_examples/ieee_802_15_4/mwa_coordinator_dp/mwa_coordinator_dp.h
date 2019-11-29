/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for MyWirelessApp Demo Coordinator application.
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
#ifdef gPHY_802_15_4g_d
  #define mDefaultValueOfChannel_c (0x0001FFFF)
  #define mDefaultMaxChannel_c     (0x11)
#else
  #define mDefaultValueOfChannel_c (0x07FFF800)
#endif

#define mMacExtendedAddress_c    (0x1111111111111111)

/* Set the Coordinator short address */ 
#define mDefaultValueOfShortAddress_c     0xCAFE

/* Set the Coordinator PanID */ 
#define mDefaultValueOfPanId_c            0xBEEF

#define mMacExtendedAddress_PAN1_c    (0x2222222222222222)

/* Set the Coordinator short address1 */ 
#define mDefaultValueOfShortAddress_PAN1_c     0xACE5

/* Set the Coordinator PanID1 */ 
#define mDefaultValueOfPanId_PAN1_c            0xF00D

/* Maximum number of outstanding packets */
#define mDefaultValueOfMaxPendingDataPackets_c 2

/*******************************************************************************
 *          Dual Pan Dwell settings
 *  +-----------------+---------------------+
 *  | PRESCALER       |    RANGE            |
 *  | bits [1:0]      |  bits [7:2]         |
 *  +------+----------+---------------------+
 *  |value | timebase |  min - max          |
 *  +------+----------+---------------------+
 *  |  00  |  0.5 ms  |  0.5 - 32  ms       |
 *  |  01  |  2.5 ms  |  2.5 - 160 ms       |
 *  |  10  |  10  ms  |   10 - 640 ms       |
 *  |  11  |  50  ms  |   50 - 3.2 seconds  |
 *  +------+----------+---------------------+
 *
 *******************************************************************************/

/* Dwell Time prescaller (0 to 3) */
#define mDefaultDualPanDwellPrescaller  0 // 0,5 ms
#define mDualPanDwellPrescallerMask     0x03

/* Dwell Time value (0 to 63) */
#define mDefaultDualPanDwellTime        6
#define mDualPanDwellTimeShift          2

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/* The various states of the applications state machines. */
enum {
  stateInit,
  stateScanEdStart,
  stateScanEdWaitConfirm,
  stateStartCoordinator,
  stateStartCoordinatorWaitConfirm,
  stateListen
};

/* Events that are passed to the application task. 
   Are defined as byte masks to make possible 
   send multiple events to the task */

#define gAppEvtDummyEvent_c            (1 << 0)
#define gAppEvtRxFromUart_c            (1 << 1)
#define gAppEvtMessageFromMLME_c       (1 << 2)
#define gAppEvtMessageFromMCPS_c       (1 << 3)
#define gAppEvtStartCoordinator_c      (1 << 4)

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
#endif /* _MAPP_H_ */
