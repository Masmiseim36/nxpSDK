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
\file       IASWDCommands.c
\brief      Send IAS WD cluster commands
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"
#include "zcl_options.h"

#include "IASWD.h"
#include "IASWD_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "dbg.h"

#ifdef DEBUG_CLD_IASWD
#define TRACE_IASWD   TRUE
#else
#define TRACE_IASWD   FALSE
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

#ifdef IASWD_CLIENT
/****************************************************************************
 **
 ** NAME:       eCLD_IASWDStartWarningReqSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Colour Control custom command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_IASWD_StartWarningReqPayload *psPayload       Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASWDStartWarningReqSend (
                                uint8                             u8SourceEndPointId,
                                uint8                             u8DestinationEndPointId,
                                tsZCL_Address                     *psDestinationAddress,
                                uint8                             *pu8TransactionSequenceNumber,
                                tsCLD_IASWD_StartWarningReqPayload   *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
        {1, E_ZCL_ENUM8,   &psPayload->u8WarningModeStrobeAndSirenLevel},
        {1, E_ZCL_UINT16,  &psPayload->u16WarningDuration},
        {1, E_ZCL_UINT8,   &psPayload->u8StrobeDutyCycle},
        {1, E_ZCL_ENUM8,   &psPayload->eStrobeLevel}        
                                                };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SECURITY_AND_SAFETY_CLUSTER_ID_IASWD,
                                  FALSE,
                                  E_CLD_IASWD_CMD_START_WARNING,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eCLD_IASWDSquawkReqSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Colour Control custom command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_IASWD_SquawkReqPayload *psPayload       Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASWDSquawkReqSend (
                                uint8                             u8SourceEndPointId,
                                uint8                             u8DestinationEndPointId,
                                tsZCL_Address                     *psDestinationAddress,
                                uint8                             *pu8TransactionSequenceNumber,
                                tsCLD_IASWD_SquawkReqPayload   *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
        {1, E_ZCL_ENUM8,   &psPayload->u8SquawkModeStrobeAndLevel}        
                                                };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SECURITY_AND_SAFETY_CLUSTER_ID_IASWD,
                                  FALSE,
                                  E_CLD_IASWD_CMD_SQUAWK,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}
#endif

#ifdef IASWD_SERVER
/****************************************************************************
 **
 ** NAME:       eCLD_IASWDStartWarningReqReceive
 **
 ** DESCRIPTION:
 ** handles receive of a colour control custom command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_IASWD_StartWarningReqPayload *psPayload    Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASWDStartWarningReqReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_IASWD_StartWarningReqPayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_ENUM8,   &psPayload->u8WarningModeStrobeAndSirenLevel},
            {1, &u16ActualQuantity, E_ZCL_UINT16,  &psPayload->u16WarningDuration},
            {1, &u16ActualQuantity, E_ZCL_UINT8,   &psPayload->u8StrobeDutyCycle},
            {1, &u16ActualQuantity, E_ZCL_ENUM8,   &psPayload->eStrobeLevel}
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}


/****************************************************************************
 **
 ** NAME:       eCLD_IASWDSquawkReqReceive
 **
 ** DESCRIPTION:
 ** handles receive of a colour control custom command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_IASWD_SquawkReqPayload *psPayload    Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASWDSquawkReqReceive(
                    ZPS_tsAfEvent                *pZPSevent,
                    uint8                        *pu8TransactionSequenceNumber,
                    tsCLD_IASWD_SquawkReqPayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_ENUM8,   &psPayload->u8SquawkModeStrobeAndLevel}
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}
#endif
/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
