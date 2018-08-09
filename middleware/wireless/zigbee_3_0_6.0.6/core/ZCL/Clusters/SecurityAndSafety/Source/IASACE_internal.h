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
