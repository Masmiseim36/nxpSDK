/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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


PUBLIC  teZCL_Status eCLD_LevelControlCommandMoveToClosestFreqCommandReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_LevelControl_MoveToClosestFreqCommandPayload *psPayload);
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
