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

/*!=============================================================================
\file       IdentifyClientCommands.c
\brief      Send an Identify cluster command
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "Identify.h"
#include "Identify_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"

#include "dbg.h"

#ifdef DEBUG_CLD_IDENTIFY
#define TRACE_IDENTIFY    TRUE
#else
#define TRACE_IDENTIFY    FALSE
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/
#ifdef IDENTIFY_CLIENT
/****************************************************************************
 **
 ** NAME:       eCLD_IdentifyCommandIdentifyRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends an identify cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_Identify_IdentifyRequestPayload *psPayload           Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_IdentifyCommandIdentifyRequestSend(
                                        uint8              u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_Identify_IdentifyRequestPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                         E_ZCL_UINT16,   &psPayload->u16IdentifyTime},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_IDENTIFY,
                                  FALSE,
                                  E_CLD_IDENTIFY_CMD_IDENTIFY,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}


/****************************************************************************
 **
 ** NAME:       eCLD_IdentifyCommandIdentifyQueryRequestSend
 **
 ** DESCRIPTION:
 ** Builds and sends an identify cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_IdentifyCommandIdentifyQueryRequestSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber)
{

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_IDENTIFY,
                                  FALSE,
                                  E_CLD_IDENTIFY_CMD_IDENTIFY_QUERY,
                                  pu8TransactionSequenceNumber,
                                  0,
                                  FALSE,
                                  0,
                                  0);

}

#ifdef  CLD_IDENTIFY_CMD_TRIGGER_EFFECT
/****************************************************************************
 **
 ** NAME:       eCLD_IdentifyCommandIdentifyTriggerEffectSend
 **
 ** DESCRIPTION:
 ** Builds and sends an identify cluster command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** uint8                      u8effectId
 ** uint8                      u8EffectVarient
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_IdentifyCommandTriggerEffectSend(
                                    uint8           u8SourceEndPointId,
                                    uint8           u8DestinationEndPointId,
                                    tsZCL_Address   *psDestinationAddress,
                                    uint8           *pu8TransactionSequenceNumber,
                                    teCLD_Identify_EffectId           u8EffectId,
                                    uint8           u8EffectVarient)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,  E_ZCL_UINT8,   &u8EffectId},
            {1,  E_ZCL_UINT8,   &u8EffectVarient},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_IDENTIFY,
                                  FALSE,
                                  E_CLD_IDENTIFY_CMD_TRIGGER_EFFECT,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));
}
#endif

/****************************************************************************
 **
 ** NAME:       eCLD_IdentifyCommandIdentifyQueryResponseReceive
 **
 ** DESCRIPTION:
 ** handles rx of identify query response
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Identify_IdentifyQueryResponsePayload *psPayload  Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_IdentifyCommandIdentifyQueryResponseReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_Identify_IdentifyQueryResponsePayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
    {1, &u16ActualQuantity, E_ZCL_UINT16,   &psPayload->u16Timeout},
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                        pu8TransactionSequenceNumber,
                                        asPayloadDefinition,
                                        sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                        E_ZCL_ACCEPT_EXACT);

}

#ifdef CLD_IDENTIFY_ATTR_COMMISSION_STATE
/****************************************************************************
 **
 ** NAME:       eCLD_IdentifyUpdateCommissionStateCommandSend
 **
 ** DESCRIPTION:
 ** Builds and sends an identify cluster command
 **
 ** PARAMETERS:                                 Name                           Usage
 ** uint8                                       u8SourceEndPointId              Source EP Id
 ** uint8                                       u8DestinationEndPointId         Destination EP Id
 ** tsZCL_Address                               *psDestinationAddress           Destination Address
 ** uint8                                       *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_Identify_UpdateCommissionStatePayload *psPayload                      Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_IdentifyUpdateCommissionStateCommandSend(
                                        uint8                                       u8SourceEndPointId,
                                        uint8                                       u8DestinationEndPointId,
                                        tsZCL_Address                               *psDestinationAddress,
                                        uint8                                       *pu8TransactionSequenceNumber,
                                        tsCLD_Identify_UpdateCommissionStatePayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                         E_ZCL_ENUM8,                &psPayload->u8Action},
            {1,                         E_ZCL_BMAP8,   &psPayload->u8CommissionStateMask},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_IDENTIFY,
                                  FALSE,
                                  E_CLD_IDENTIFY_CMD_UPDATE_COMMISSION_STATE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}
#endif
#endif /*IDENTIFY_CLIENT*/
/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
