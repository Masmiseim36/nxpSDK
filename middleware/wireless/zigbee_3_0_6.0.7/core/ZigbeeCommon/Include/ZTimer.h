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

#ifndef ZTIMER_H_
#define ZTIMER_H_

/*!
\file       ZTimer.h
\brief      Zigbee Timer module api
*/     
     
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define ZTIMER_TIME_SEC(v) ((uint32)(v) * 1000UL)
#define ZTIMER_TIME_MSEC(v) ((uint32)(v) * 1UL)

/* Flags for timer configuration */
#define ZTIMER_FLAG_ALLOW_SLEEP     0
#define ZTIMER_FLAG_PREVENT_SLEEP   (1 << 0)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_ZTIMER_STATE_CLOSED,
    E_ZTIMER_STATE_STOPPED,
    E_ZTIMER_STATE_RUNNING,
    E_ZTIMER_STATE_EXPIRED,    
} ZTIMER_teState;

typedef void (*ZTIMER_tpfCallback)(void *pvParam);

typedef struct
{
    uint8               u8Flags;
    ZTIMER_teState      eState;
    uint64              Time;
    void                *pvParameters;
    ZTIMER_tpfCallback  pfCallback;
} ZTIMER_tsTimer;

typedef enum
{
    E_ZTIMER_OK,
    E_ZTIMER_FAIL
} ZTIMER_teStatus;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC ZTIMER_teStatus ZTIMER_eInit(ZTIMER_tsTimer *psTimers, uint8 u8NumTimers);
PUBLIC void ZTIMER_vSleep(void);
PUBLIC void ZTIMER_vWake(void);
PUBLIC void ZTIMER_vTask(void);
PUBLIC ZTIMER_teStatus ZTIMER_eOpen(uint8 *pu8TimerIndex, ZTIMER_tpfCallback pfCallback, void *pvParams, uint8 u8Flags);
PUBLIC ZTIMER_teStatus ZTIMER_eClose(uint8 u8TimerIndex);
PUBLIC ZTIMER_teStatus ZTIMER_eStart(uint8 u8TimerIndex, uint32 u32Time);
PUBLIC ZTIMER_teStatus ZTIMER_eStop(uint8 u8TimerIndex);
PUBLIC ZTIMER_teState ZTIMER_eGetState(uint8 u8TimerIndex);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*ZTIMER_H_*/
