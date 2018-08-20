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


#ifndef  LEVEL_CONTROL_INTERNAL_H_INCLUDED
#define  LEVEL_CONTROL_INTERNAL_H_INCLUDED

/*!
\file       LevelControl_internal.h
\brief      The internal API for the Level Control Cluster
*/
     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "LevelControl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* OffWithEffect based on (EffectId | EffectVariant) (6.6.1.4.4)*/
typedef enum 
{
    CLD_ONOFF_OFF_WITH_EFFECT_FADE_OFF            = 0x0000,
    CLD_ONOFF_OFF_WITH_EFFECT_NO_FADE             = 0x0001,
    CLD_ONOFF_OFF_WITH_EFFECT_DIM_DOWN_FADE_OFF   = 0x0002,
    CLD_ONOFF_OFF_WITH_EFFECT_DIM_UP_FADE_OFF     = 0x0100,
    CLD_ONOFF_OFF_WITH_EFFECT_NONE                = 0xFFFF
} teCLD_OnOff_OffWithEffect;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_LevelControlCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

PUBLIC teZCL_Status eCLD_LevelControlCommandMoveToLevelCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_LevelControl_MoveToLevelCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_LevelControlCommandMoveCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_LevelControl_MoveCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_LevelControlCommandStepCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_LevelControl_StepCommandPayload *psPayload);

PUBLIC  teZCL_Status eCLD_LevelControlCommandStopCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_LevelControl_StopCommandPayload *psPayload);

PUBLIC teZCL_Status eCLD_LevelControlSetOnOffState(
                    uint8                       u8SourceEndPointId,
                    bool_t                      bOn,
                    teCLD_OnOff_OffWithEffect   eCLD_OnOff_OffWithEffect);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* LEVEL_CONTROL_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
