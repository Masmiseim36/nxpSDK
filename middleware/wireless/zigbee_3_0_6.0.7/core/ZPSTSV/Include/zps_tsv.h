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

#ifndef ZPS_TSV_H_
#define ZPS_TSV_H_

/*!
\file       zps_tsv.h
\brief      ZPS Timer server shim layer header
*/
     
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"
#ifndef CPU_MKW41Z512VHT4
#include "tsv_pub.h"
#else
#include "mac_sap.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define ZPS_TSV_TIME_SEC(v) ((uint32)(v) * 64000UL)
#define ZPS_TSV_TIME_MSEC(v) ((uint32)(v) * 64UL)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef TSV_ResultCode_e ZPS_teTsvResultCode;
typedef void (*ZPS_prTsvExpireCb)(void *, void *);

typedef struct
{
    ZPS_prTsvExpireCb prCallback;
    ZPS_prTsvExpireCb pvContext;
    ZPS_prTsvExpireCb pvParam;
} ZPS_tsTsvTimerInfo;

typedef struct
{
    TSV_Timer_s sTsvTimer;
    ZPS_tsTsvTimerInfo sTimerInfo;
} ZPS_tsTsvTimer;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

void ZPS_vTsvInit(void (*prExpireFn)(ZPS_tsTsvTimerInfo *));

PUBLIC ZPS_teTsvResultCode ZPS_eTsvOpen(
    ZPS_tsTsvTimer *psTimer,
    ZPS_prTsvExpireCb prExpireCb,
    void *pvContext,
    void *pvParam);

PUBLIC ZPS_teTsvResultCode ZPS_eTsvClose(
    ZPS_tsTsvTimer* psTimer,
    bool_t bInvokeCBIfRunning);

PUBLIC ZPS_teTsvResultCode ZPS_eTsvStart(
    ZPS_tsTsvTimer* psTimer,
    uint32 u32Value);

PUBLIC ZPS_teTsvResultCode ZPS_eTsvStop(ZPS_tsTsvTimer* psTimer);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

extern PUBLIC const uint32 ZPS_g_u32TsvVersion;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#ifdef __cplusplus
};
#endif

#endif /* ZPS_TSV_H_ */
