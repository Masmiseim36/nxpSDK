/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This header file is for MyStarNetworkDemo-EndDevice application.
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
