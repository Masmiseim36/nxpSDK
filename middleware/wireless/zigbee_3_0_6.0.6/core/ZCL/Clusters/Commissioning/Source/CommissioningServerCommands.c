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
\file       CommissioningServerCommands.c
\brief      Send an Commissioning cluster command
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "Commissioning.h"
#include "Commissioning_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"

#include "dbg.h"

#ifdef DEBUG_CLD_COMMISSIONING
#define TRACE_COMMS    TRUE
#else
#define TRACE_COMMS    FALSE
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
#ifdef COMMISSIONING_SERVER
/****************************************************************************
 **
 ** NAME:       eCLD_CommissioningCommandResponseSend
 **
 ** DESCRIPTION:
 ** Builds and sends a commissioning cluster response command
 **
 ** PARAMETERS:                         Name                           Usage
 ** uint8                               u8SourceEndPointId             Source EP Id
 ** uint8                               u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address                       *psDestinationAddress           Destination Address
 ** uint8                               *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_Commissioning_ResponsePayload *psPayload                      Payload
 ** teCLD_Commissioning_Command          eCLD_Commissioning_Command     CommandID
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_CommissioningCommandResponseSend(
                                        uint8                               u8SourceEndPointId,
                                        uint8                               u8DestinationEndPointId,
                                        tsZCL_Address                       *psDestinationAddress,
                                        uint8                               *pu8TransactionSequenceNumber,
                                        tsCLD_Commissioning_ResponsePayload *psPayload,
                                        teCLD_Commissioning_Command          eCLD_Commissioning_Command)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                         E_ZCL_ENUM8,   &psPayload->u8Status},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  COMMISSIONING_CLUSTER_ID_COMMISSIONING,
                                  TRUE,
                                  eCLD_Commissioning_Command,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eCLD_CommissioningCommandRestartDeviceReceive
 **
 ** DESCRIPTION:
 ** handles rx of Restart Device command
 **
 ** PARAMETERS:                                 Name                          Usage
 ** ZPS_tsAfEvent                               *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition                    *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance                       *psClusterInstance             Cluster structure
 ** uint8                                       *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Commissioning_RestartDevicePayload    *psPayload                     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_CommissioningCommandRestartDeviceReceive(
                    ZPS_tsAfEvent                            *pZPSevent,
                    uint8                                    *pu8TransactionSequenceNumber,
                    tsCLD_Commissioning_RestartDevicePayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
    {1, &u16ActualQuantity, E_ZCL_BMAP8,   &psPayload->u8Options},
    {1, &u16ActualQuantity, E_ZCL_UINT8,   &psPayload->u8Delay},
    {1, &u16ActualQuantity, E_ZCL_UINT8,   &psPayload->u8Jitter},
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}

/****************************************************************************
 **
 ** NAME: eCLD_CommissioningCommandModifyStartupParamsReceive
 **
 ** DESCRIPTION:
 ** handles rx of Save, restore and Reset Startup Parameters command
 **
 ** PARAMETERS:                                 Name                          Usage
 ** ZPS_tsAfEvent                               *pZPSevent                     Zigbee stack event structure
 ** tsZCL_EndPointDefinition                    *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance                       *psClusterInstance             Cluster structure
 ** uint8                                       *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Commissioning_ModifyStartupParametersPayload    *psPayload                     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_CommissioningCommandModifyStartupParamsReceive(
                    ZPS_tsAfEvent                                       *pZPSevent,
                    uint8                                               *pu8TransactionSequenceNumber,
                    tsCLD_Commissioning_ModifyStartupParametersPayload  *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
    {1, &u16ActualQuantity, E_ZCL_BMAP8,   &psPayload->u8Options},
    {1, &u16ActualQuantity, E_ZCL_UINT8,   &psPayload->u8Index},
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}
#endif /* COMMISSIONING_SERVER */

/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
