/*
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* SPDX-License-Identifier: BSD-3-Clause
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
PUBLIC uint32 zps_u32GetTsvVersion(void);
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

PUBLIC void ZPS_eTsvStopAllTimers(void);

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
