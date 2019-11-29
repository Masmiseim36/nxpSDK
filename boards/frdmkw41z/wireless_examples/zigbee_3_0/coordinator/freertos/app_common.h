/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_COMMON_H_
#define APP_COMMON_H_

#include "zcl_options.h"

/*!
\file       app_common.h
\brief      Base Device application - common includes selector
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define NETWORK_RESTART_TIME    ZTIMER_TIME_MSEC(1000)
#define POLL_TIME               ZTIMER_TIME_MSEC(5000)
#define POLL_TIME_FAST          ZTIMER_TIME_MSEC(250)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum
{
    E_STARTUP,
    E_RUNNING
} teNodeState;

typedef struct {
    teNodeState eNodeState;
#ifdef CLD_OTA
    bool bValid;
    uint64 u64IeeeAddrOfServer;
    uint16 u16NwkAddrOfServer;
    uint8 u8OTAserverEP;
#endif
}tsNodeState;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC void vTAM_MLME_RxInCca(bool_t bEnable);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_COMMON_H_*/
