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
     
#ifndef BDB_START_INCLUDED
#define BDB_START_INCLUDED

/*!
\file       bdb_start.h
\brief      BDB Network Initialisation API
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "bdb_api.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_INIT_IDLE,
    E_INIT_WAIT_REJOIN,
}teInitState;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void BDB_vInit(BDB_tsInitArgs *psInitArgs);
PUBLIC bool_t BDB_bIsBaseIdle(void);
PUBLIC void BDB_vStart(void);
PUBLIC void BDB_vInitStateMachine(BDB_tsZpsAfEvent *psZpsAfEvent);
PUBLIC uint8 BDB_u8PickChannel(uint32 u32ChannelMask);
PUBLIC void BDB_vRejoinCycle(bool_t bSkipDirectJoin);
PUBLIC void BDB_vRejoinSuccess(void);
PUBLIC void BDB_vRejoinTimerCb(void *pvParam);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
extern PUBLIC uint8 au8DefaultTCLinkKey[16];
extern PUBLIC uint8 au8DistributedLinkKey[16];
extern PUBLIC uint8 au8PreConfgLinkKey[16];
#if (defined JENNIC_CHIP_FAMILY_JN516x) || (defined JENNIC_CHIP_FAMILY_JN517x)
extern PUBLIC tsReg128 sTLMasterKey;
extern PUBLIC tsReg128 sTLCertKey;
#else
extern PUBLIC uint8 au8TLMasterKey[16];
extern PUBLIC uint8 au8TLCertKey[16];
#endif

extern PUBLIC teInitState eInitState;
extern PUBLIC bool_t bAssociationJoin;
extern PUBLIC uint8 u8RejoinCycles;
extern PUBLIC uint8 u8TimerBdbRejoin;

#if defined __cplusplus
}
#endif

#endif  /* BDB_START_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/






