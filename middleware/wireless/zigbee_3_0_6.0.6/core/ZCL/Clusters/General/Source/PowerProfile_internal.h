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


#ifndef  POLLCONTROL_INTERNAL_H_INCLUDED
#define  POLLCONTROL_INTERNAL_H_INCLUDED

/*!
\file       PowerProfile_internal.h
\brief      The internal API for the Power Profile
*/     
#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "zcl.h"
#include "PowerProfile.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_PPCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);
                    
PUBLIC teZCL_Status eCLD_PPPowerProfileReqReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_PP_PowerProfileReqPayload                 *psPayload);                    

PUBLIC teZCL_Status eCLD_PPEnergyPhasesScheduleReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_PP_EnergyPhasesSchedulePayload            *psPayload,
                    uint16                                          u16HeaderOffset);                    
#ifdef PP_CLIENT
PUBLIC teZCL_Status eCLD_PPGetOverallSchedulePriceReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);
PUBLIC teZCL_Status eCLD_PPPowerProfileScheduleConstraintsReceive   (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_PP_PowerProfileScheduleConstraintsPayload *psPayload);
PUBLIC teZCL_Status eCLD_PPGetPowerProfilePriceExtendedReceive   (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_PP_GetPowerProfilePriceExtendedPayload    *psPayload);
PUBLIC teZCL_Status eCLD_PPGetPowerProfilePriceOrExtendedRspSend  (
                    uint8                                            u8SourceEndPointId,
                    uint8                                            u8DestinationEndPointId,
                    tsZCL_Address                                    *psDestinationAddress,
                    uint8                                            *pu8TransactionSequenceNumber,
                    teCLD_PP_ServerReceivedCommandID                 eCommandId,
                    tsCLD_PP_GetPowerProfilePriceRspPayload         *psPayload);
PUBLIC teZCL_Status eCLD_PPGetOverallSchedulePriceRspSend(
                    uint8                                             u8SourceEndPointId,
                    uint8                                             u8DestinationEndPointId,
                    tsZCL_Address                                     *psDestinationAddress,
                    uint8                                             *pu8TransactionSequenceNumber,
                    tsCLD_PP_GetOverallSchedulePriceRspPayload       *psPayload);
                
#endif

#ifdef PP_SERVER
PUBLIC teZCL_Status eCLD_PPPowerProfileStateReqReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber);
PUBLIC teZCL_Status eCLD_PPGetPowerProfilePriceOrExtendedRspReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_PP_GetPowerProfilePriceRspPayload         *psPayload);    
PUBLIC teZCL_Status eCLD_PPGetOverallSchedulePriceRspReceive (
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_PP_GetOverallSchedulePriceRspPayload      *psPayload);    
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* PP_INTERNAL_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
