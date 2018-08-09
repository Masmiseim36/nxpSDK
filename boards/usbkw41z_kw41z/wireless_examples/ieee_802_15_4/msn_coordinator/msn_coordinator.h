/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for MyStarNetwork Demo Coordinator application.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
