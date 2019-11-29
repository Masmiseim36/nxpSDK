/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef  IASACE_INTERNAL_H_INCLUDED
#define  IASACE_INTERNAL_H_INCLUDED

/*!
\file       IASACE_internal.h
\brief      The internal API for the IAS ACE Cluster
*/
     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "IASACE.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_IASACECommandHandler (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    tsZCL_EndPointDefinition                        *psEndPointDefinition,
                    tsZCL_ClusterInstance                           *psClusterInstance);

#ifdef IASACE_SERVER
PUBLIC teZCL_Status eCLD_IASACE_ArmRespSend (
                uint8                                       u8SourceEndPointId,
                uint8                                       u8DestinationEndPointId,
                tsZCL_Address                               *psDestinationAddress,
                uint8                                       *pu8TransactionSequenceNumber,
                tsCLD_IASACE_ArmRespPayload                 *psPayload);
                
PUBLIC teZCL_Status eCLD_IASACE_GetZoneIDMapRespSend (
                    uint8                                           u8SourceEndPointId,
                    uint8                                           u8DestinationEndPointId,
                    tsZCL_Address                                   *psDestinationAddress,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_GetZoneIDMapRespPayload            *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_GetZoneInfoRespSend (
                    uint8                                           u8SourceEndPointId,
                    uint8                                           u8DestinationEndPointId,
                    tsZCL_Address                                   *psDestinationAddress,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_GetZoneInfoRespPayload             *psPayload);
               
PUBLIC teZCL_Status eCLD_IASACE_BypassRespSend (
                uint8                                       u8SourceEndPointId,
                uint8                                       u8DestinationEndPointId,
                tsZCL_Address                               *psDestinationAddress,
                uint8                                       *pu8TransactionSequenceNumber,
                tsCLD_IASACE_BypassRespPayload              *psPayload);
                
PUBLIC teZCL_Status eCLD_IASACE_GetZoneStatusRespSend (
                uint8                                       u8SourceEndPointId,
                uint8                                       u8DestinationEndPointId,
                tsZCL_Address                               *psDestinationAddress,
                uint8                                       *pu8TransactionSequenceNumber,
                tsCLD_IASACE_GetZoneStatusRespPayload       *psPayload);

PUBLIC teZCL_Status eCLD_IASACEArmReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,                    
                    tsCLD_IASACE_ArmPayload                         *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_BypassReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,                    
                    tsCLD_IASACE_BypassPayload                      *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_EmergencyReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);
                    
PUBLIC teZCL_Status eCLD_IASACE_FireReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);

PUBLIC teZCL_Status eCLD_IASACE_PanicReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);

PUBLIC teZCL_Status eCLD_IASACE_GetZoneIDMapReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);

PUBLIC teZCL_Status eCLD_IASACE_GetZoneInfoReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_GetZoneInfoPayload                 *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_GetPanelStatusReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);

PUBLIC teZCL_Status eCLD_IASACE_GetBypassedZoneListReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);

PUBLIC teZCL_Status eCLD_IASACE_GetZoneStatusReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_GetZoneStatusPayload               *psPayload);
#endif

#ifdef IASACE_CLIENT
PUBLIC teZCL_Status eCLD_IASACE_ArmRespReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_ArmRespPayload                     *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_GetZoneIDMapRespReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_GetZoneIDMapRespPayload            *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_GetZoneInfoRespReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_GetZoneInfoRespPayload             *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_ZoneStatusChangedReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_ZoneStatusChangedPayload           *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_PanelStatusChangedOrGetPanelStatusRespReceive (
                    ZPS_tsAfEvent                                                       *pZPSevent,
                    uint8                                                               *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_PanelStatusChangedOrGetPanelStatusRespPayload          *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_SetBypassedZoneListReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_SetBypassedZoneListPayload         *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_BypassRespReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_BypassRespPayload                  *psPayload);

PUBLIC teZCL_Status eCLD_IASACE_GetZoneStatusRespReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_IASACE_GetZoneStatusRespPayload           *psPayload);
#endif                     
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* IASACE_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
