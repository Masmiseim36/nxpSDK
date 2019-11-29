/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/



#ifndef  COMMS_INTERNAL_H_INCLUDED
#define  COMMS_INTERNAL_H_INCLUDED

/*!
\file       Commissioning_internal.h
\brief      The internal API for the Commissioning Cluster
*/

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "Commissioning.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_CommissioningCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

#ifdef COMMISSIONING_SERVER 
PUBLIC teZCL_Status eCLD_CommissioningCommandRestartDeviceReceive(
                    ZPS_tsAfEvent                            *pZPSevent,
                    uint8                                    *pu8TransactionSequenceNumber,
                    tsCLD_Commissioning_RestartDevicePayload *psPayload);

PUBLIC teZCL_Status eCLD_CommissioningCommandModifyStartupParamsReceive(
                    ZPS_tsAfEvent                                       *pZPSevent,
                    uint8                                               *pu8TransactionSequenceNumber,
                    tsCLD_Commissioning_ModifyStartupParametersPayload  *psPayload);

PUBLIC teZCL_Status eCLD_CommissioningCommandResponseSend(
                    uint8                               u8SourceEndPointId,
                    uint8                               u8DestinationEndPointId,
                    tsZCL_Address                       *psDestinationAddress,
                    uint8                               *pu8TransactionSequenceNumber,
                    tsCLD_Commissioning_ResponsePayload *psPayload,
                    teCLD_Commissioning_Command         eCLD_Commissioning_Command);
#endif /* COMMISSIONING_SERVER */                    

#ifdef COMMISSIONING_CLIENT
PUBLIC teZCL_Status eCLD_CommissioningCommandResponseReceive(
                    ZPS_tsAfEvent                       *pZPSevent,
                    uint8                               *pu8TransactionSequenceNumber,
                    tsCLD_Commissioning_ResponsePayload *psPayload);

#endif /* COMMISSIONING_CLIENT */ 
         
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* COMMS_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
