/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
