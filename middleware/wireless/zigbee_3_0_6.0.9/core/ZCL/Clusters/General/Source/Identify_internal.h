/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  IDENTIFY_INTERNAL_H_INCLUDED
#define  IDENTIFY_INTERNAL_H_INCLUDED

/*!
\file       Identify_internal.h
\brief      The internal API for the Identify Cluster
*/     

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "Identify.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_IdentifyCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

#ifdef IDENTIFY_SERVER 
PUBLIC teZCL_Status eCLD_IdentifyCommandIdentifyRequestReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_Identify_IdentifyRequestPayload *psPayload);

PUBLIC teZCL_Status eCLD_IdentifyCommandIdentifyQueryRequestReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber);

PUBLIC teZCL_Status eCLD_IdentifyCommandIdentifyQueryResponseSend(
                    uint8           u8SourceEndPointId,
                    uint8           u8DestinationEndPointId,
                    tsZCL_Address   *psDestinationAddress,
                    uint8           *pu8TransactionSequenceNumber,
                    tsCLD_Identify_IdentifyQueryResponsePayload *psPayload);

#ifdef CLD_IDENTIFY_CMD_TRIGGER_EFFECT
PUBLIC teZCL_Status eCLD_IdentifyCommandTriggerEffectRequestReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_Identify_TriggerEffectRequestPayload *psPayload);
#endif 
 
#ifdef CLD_IDENTIFY_ATTR_COMMISSION_STATE
PUBLIC teZCL_Status eCLD_IdentifyUpdateCommissionStateCommandReceive(
                    ZPS_tsAfEvent                               *pZPSevent,
                    uint8                                       *pu8TransactionSequenceNumber,
                    tsCLD_Identify_UpdateCommissionStatePayload *psPayload);
#endif 
#endif /* IDENTIFY_SERVER */                    

#ifdef IDENTIFY_CLIENT
PUBLIC teZCL_Status eCLD_IdentifyCommandIdentifyQueryResponseReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_Identify_IdentifyQueryResponsePayload *psPayload);

#endif /* IDENTIFY_CLIENT */ 

#ifdef CLD_IDENTIFY_CMD_EZ_MODE_INVOKE                  
PUBLIC teZCL_Status eCLD_IdentifyEZModeInvokeCommandReceive(
                    ZPS_tsAfEvent                               *pZPSevent,
                    uint8                                       *pu8TransactionSequenceNumber,
                    tsCLD_Identify_EZModeInvokePayload          *psPayload);                
#endif
         
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* IDENTIFY_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
