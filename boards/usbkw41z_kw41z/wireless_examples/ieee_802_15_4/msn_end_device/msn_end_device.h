/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for MyStarNetworkDemo-EndDevice application.
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

#define mMacExtendedAddress_c    (0xFFFFFFFFFFFFFFFF)

#ifdef gPHY_802_15_4g_d
#define mDefaultValueOfChannel_c (0x0001FFFF)
#else
#define mDefaultValueOfChannel_c (0x07FFF800)
#endif

/* Define wait interval in seconds before another associate retry */
#define mDefaultValueOfWaitInterval_c          7	

#define mDefaultValueOfPollInterval_c          200


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* All states in the applications state machine */
enum
{
  stateInit,
  stateScanActiveStart,  
  stateScanActiveWaitConfirm,
  stateAssociate,
  stateAssociateWaitConfirm,
  stateWaitInterval,
  stateListen
};

/* Events */
#define gAppEvtDummyEvent_c            (1 << 0)
#define gAppEvtMessageFromMLME_c       (1 << 1)
#define gAppEvtMessageFromMCPS_c       (1 << 2)
#define gAppEvtStartWait_c             (1 << 3)
#define gAppEvtSendMessage_c           (1 << 4)
#define gAppEvtSendSensorData_c        (1 << 5)
#define gAppEvtSendPollReq_c           (1 << 6)



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

/******************************************************************************
*******************************************************************************
* Public Prototypes
*******************************************************************************
******************************************************************************/

#ifdef __cplusplus
}
#endif

/**********************************************************************************/
#endif /* _MAPP_H_ */
