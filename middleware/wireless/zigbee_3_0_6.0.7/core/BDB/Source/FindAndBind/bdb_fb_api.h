/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
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
     
#ifndef BDB_FIND_AND_BIND_H
#define BDB_FIND_AND_BIND_H

/*!
\file       bdb_fb_api.h
\brief      BDB Find & Bind interface
*/

#if defined __cplusplus
extern "C" {
#endif


#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "zps_apl_aps.h"
#include "appZdpExtraction.h"
#include "bdb_api.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* The time in seconds that a device will wait before 
 * re broadcasting identify query request */
#ifndef BDB_FB_RESEND_IDENTIFY_QUERY_TIME
#define BDB_FB_RESEND_IDENTIFY_QUERY_TIME                       10
#endif

/* maximum number of target devices that the device is going to bind.*/
#ifndef BDB_FB_MAX_TARGET_DEVICES
#define BDB_FB_MAX_TARGET_DEVICES                               5
#endif

#ifndef BDB_FB_NUMBER_OF_ENDPOINTS
#define BDB_FB_NUMBER_OF_ENDPOINTS                              ZCL_NUMBER_OF_ENDPOINTS
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/*Find and Bind states */
typedef enum 
{
    E_FB_IDLE_STATE,
    E_FB_IN_PROGRESS_STATE,
    E_FB_WAIT_FOR_QUERY_RESPONSE_STATE,
    E_FB_WAIT_FOR_IEEE_RESPONSE_STATE,
    E_FB_WAIT_FOR_SIMPLE_DESCRIPTOR_RESPONSE_STATE,
    E_FB_WAIT_FOR_STOP_IDENTICIATION_STATE
}teFB_State;

/* Target Address Info */
typedef struct
{
    uint8  u8DstEndpoint;
    uint16 u16NwkAddr;
    uint64 u64IeeeAddr;
}tsFB_TargetInfo;

/* Find and Bind Structure */
typedef struct
{
    uint8                           u8Endpoint;
    teFB_State                      eFBState;
    uint8                           u8ReSendQueryTime;
    uint16                          u8CommTime;
    tsFB_TargetInfo                 asTargetInfo[BDB_FB_MAX_TARGET_DEVICES];
}tsFindAndBind;


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC BDB_teStatus BDB_eFbTriggerAsInitiator(uint8 u8SourceEndPointId);
PUBLIC void BDB_vFbHandleQueryResponse(tsZCL_CallBackEvent *pCallBackEvent);
PUBLIC void BDB_vFbZclHandler(uint8 u8EventType,tsZCL_CallBackEvent *psCallBackEvent);
PUBLIC void vFbZdpHandler(ZPS_tsAfEvent *pZPSevent);
PUBLIC BDB_teStatus BDB_eFbTriggerAsTarget(uint8 u8EndPointId);
PUBLIC void BDB_vFbExitAsInitiator();
PUBLIC void BDB_vFbExitAsTarget(uint8 u8SourceEndpoint);

PUBLIC void BDB_vFbTimerCb(void *pvParam);
PUBLIC void BDB_vFbHandleStopIdentification(tsZCL_CallBackEvent *pCallBackEvent);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC uint8 u8TimerBdbFb;

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /* BDB_FIND_AND_BIND_H */
