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
\file       IASZoneCommands.c
\brief      Send IAS Zone cluster commands
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"
#include "zcl_options.h"

#include "IASZone.h"
#include "IASZone_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"
#include "dbg.h"

#ifdef DEBUG_CLD_IASZONE
#define TRACE_IASZONE   TRUE
#else
#define TRACE_IASZONE   FALSE
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
#ifdef IASZONE_SERVER

/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneEnrollReqSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Colour Control custom command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_IASZone_EnrollRequestPayload *psPayload       Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneEnrollReqSend (
                                uint8                             u8SourceEndPointId,
                                uint8                             u8DestinationEndPointId,
                                tsZCL_Address                     *psDestinationAddress,
                                uint8                             *pu8TransactionSequenceNumber,
                                tsCLD_IASZone_EnrollRequestPayload   *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
        {1, E_ZCL_ENUM16,   &psPayload->e16ZoneType},
        {1, E_ZCL_UINT16,  &psPayload->u16ManufacturerCode}
                                                };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SECURITY_AND_SAFETY_CLUSTER_ID_IASZONE,
                                  TRUE,
                                  E_CLD_IASZONE_CMD_ZONE_ENROLL_REQUEST,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneStatusChangeNotificationSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Colour Control custom command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_IASZone_StatusChangeNotificationPayload *psPayload       Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneStatusChangeNotificationSend (
                                uint8                             u8SourceEndPointId,
                                uint8                             u8DestinationEndPointId,
                                tsZCL_Address                     *psDestinationAddress,
                                uint8                             *pu8TransactionSequenceNumber,
                                tsCLD_IASZone_StatusChangeNotificationPayload   *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
        {1, E_ZCL_BMAP16,   &psPayload->b16ZoneStatus},
        {1, E_ZCL_BMAP8,    &psPayload->b8ExtendedStatus},
        {1, E_ZCL_UINT8,    &psPayload->u8ZoneId},
        {1, E_ZCL_UINT16,   &psPayload->u16Delay}
                                                };

    // handle sequence number get a new sequence number
    if(pu8TransactionSequenceNumber != NULL)
        *pu8TransactionSequenceNumber = u8GetTransactionSequenceNumber();
        
    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SECURITY_AND_SAFETY_CLUSTER_ID_IASZONE,
                                  TRUE,
                                  E_CLD_IASZONE_CMD_ZONE_STATUS_CHANGE_NOTIFICATION,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneEnrollRespReceive
 **
 ** DESCRIPTION:
 ** handles receive of a colour control custom command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_ColourControl_MoveToHueCommandPayload *psPayload  Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneEnrollRespReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_IASZone_EnrollResponsePayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
        {1, &u16ActualQuantity, E_ZCL_UINT8,   &psPayload->e8EnrollResponseCode},
        {1, &u16ActualQuantity, E_ZCL_ENUM8,   &psPayload->u8ZoneID}
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}

/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneNormalOperationModeReqReceive
 **
 ** DESCRIPTION:
 ** handles receive of a colour control custom command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneNormalOperationModeReqReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber)
{


    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     NULL,
                                     0,
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}

#ifdef CLD_IASZONE_CMD_INITIATE_TEST_MODE
/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneTestModeReqReceive
 **
 ** DESCRIPTION:
 ** handles receive of a colour control custom command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_IASZone_InitiateTestModeRequestPayload *psPayload    Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneTestModeReqReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_IASZone_InitiateTestModeRequestPayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_ENUM8,   &psPayload->u8TestModeDuration},
            {1, &u16ActualQuantity, E_ZCL_UINT8,   &psPayload->u8CurrentZoneSensitivityLevel},
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}
#endif
#endif

#ifdef IASZONE_CLIENT
/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneEnrollRespSend
 **
 ** DESCRIPTION:
 ** Builds and sends a IAS Zone cluster renroll response
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_IASZone_EnrollResponsePayload *psPayload             Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneEnrollRespSend (
                                uint8                             u8SourceEndPointId,
                                uint8                             u8DestinationEndPointId,
                                tsZCL_Address                     *psDestinationAddress,
                                uint8                             *pu8TransactionSequenceNumber,
                                tsCLD_IASZone_EnrollResponsePayload   *psPayload)

{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
        {1, E_ZCL_UINT8,   &psPayload->e8EnrollResponseCode},
        {1, E_ZCL_ENUM8,   &psPayload->u8ZoneID}
                                                };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SECURITY_AND_SAFETY_CLUSTER_ID_IASZONE,
                                  FALSE,
                                  E_CLD_IASZONE_CMD_ZONE_ENROLL_RESP,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

#ifdef CLD_IASZONE_CMD_INITIATE_NORMAL_OPERATION_MODE
/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneNormalOperationModeReqSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Colour Control custom command
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
PUBLIC teZCL_Status eCLD_IASZoneNormalOperationModeReqSend (
                                uint8                             u8SourceEndPointId,
                                uint8                             u8DestinationEndPointId,
                                tsZCL_Address                     *psDestinationAddress,
                                uint8                             *pu8TransactionSequenceNumber)
{

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SECURITY_AND_SAFETY_CLUSTER_ID_IASZONE,
                                  FALSE,
                                  E_CLD_IASZONE_CMD_INITIATE_NORMAL_OPERATION_MODE,
                                  pu8TransactionSequenceNumber,
                                  NULL,
                                  FALSE,
                                  0,
                                  0);

}
#endif

#ifdef CLD_IASZONE_CMD_INITIATE_TEST_MODE
/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneTestModeReqSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Colour Control custom command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_IASZone_InitiateTestModeRequestPayload *psPayload       Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneTestModeReqSend (
                                uint8                             u8SourceEndPointId,
                                uint8                             u8DestinationEndPointId,
                                tsZCL_Address                     *psDestinationAddress,
                                uint8                             *pu8TransactionSequenceNumber,
                                tsCLD_IASZone_InitiateTestModeRequestPayload    *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
        {1, E_ZCL_ENUM8,   &psPayload->u8TestModeDuration},
        {1, E_ZCL_UINT8,   &psPayload->u8CurrentZoneSensitivityLevel}
                                                };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SECURITY_AND_SAFETY_CLUSTER_ID_IASZONE,
                                  FALSE,
                                  E_CLD_IASZONE_CMD_INITIATE_TEST_MODE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}
#endif

/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneEnrollReqReceive
 **
 ** DESCRIPTION:
 ** handles receive of a colour control custom command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_IASZone_EnrollRequestPayload *psPayload    Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneEnrollReqReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_IASZone_EnrollRequestPayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_ENUM16,   &psPayload->e16ZoneType},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psPayload->u16ManufacturerCode}
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}

/****************************************************************************
 **
 ** NAME:       eCLD_IASZoneStatusChangeNotificationReceive
 **
 ** DESCRIPTION:
 ** handles receive of a colour control custom command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                   Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_ColourControl_MoveHueCommandPayload *psPayload    Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_IASZoneStatusChangeNotificationReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    tsCLD_IASZone_StatusChangeNotificationPayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_BMAP16,   &psPayload->b16ZoneStatus},
            {1, &u16ActualQuantity, E_ZCL_BMAP8,    &psPayload->b8ExtendedStatus},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPayload->u8ZoneId},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psPayload->u16Delay}
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
