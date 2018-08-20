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
            APP_vPrintfNullImpl((FORMAT), ## ARGS);                 \
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
