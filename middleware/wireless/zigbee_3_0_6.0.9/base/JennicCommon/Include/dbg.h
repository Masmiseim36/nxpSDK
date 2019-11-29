/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef DBG_H_
#define DBG_H_

/*!
\file       dbg.h
\brief      Zigbee Debug
*/

#include "fsl_common.h"
#include "app_zb_utils.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifndef DBG_vPrintf
#define DBG_vPrintf(STREAM, FORMAT, ARGS...)\
    do {                                                            \
        if(STREAM)                                                  \
            APP_vPrintf((FORMAT), ## ARGS);                         \
        else                                                        \
            APP_vPrintfNullImpl((STREAM), ## ARGS);                 \
    } while(0)
#endif

#ifndef DBG_vAssert
#define DBG_vAssert(STREAM, ASSERTION)                                \
    do {                                                              \
        bool_t bEval = (!(ASSERTION) && (STREAM) ) ?                    \
                APP_vPrintf(#ASSERTION), *((uint32*)0x800000FB) = 0x1, TRUE : FALSE; \
        bEval = bEval;                                                  \
    } while(0)
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef struct
{
    void (*prInitHardwareCb)(void);
    void (*prPutchCb)       (char c);
    void (*prFlushCb)       (void);
    void (*prFailedAssertCb)(void);
    int  (*prGetCharCb)     (void);
} tsDBG_FunctionTbl;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

void DBG_vInit(tsDBG_FunctionTbl *psFunctionTbl);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*DBG_H_*/
