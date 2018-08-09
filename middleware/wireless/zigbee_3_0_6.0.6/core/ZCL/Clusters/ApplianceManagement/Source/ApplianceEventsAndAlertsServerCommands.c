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
\file       ApplianceEventsAndAlertsServerCommands.c
\brief      Appliance Events And Alerts Server Commands
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "ApplianceEventsAndAlerts.h"
#include "ApplianceEventsAndAlerts_internal.h"

#include "dbg.h"


#ifdef DEBUG_CLD_APPLIANCE_EVENTS_AND_ALERTS
#define TRACE_APPLIANCE_EVENTS_AND_ALERTS    TRUE
#else
#define TRACE_APPLIANCE_EVENTS_AND_ALERTS    FALSE
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

#ifdef APPLIANCE_EVENTS_AND_ALERTS_SERVER
/****************************************************************************
 **
 ** NAME:       eCLD_AEAAGetAlertsResponseORAlertsNotificationSend
 **
 ** DESCRIPTION:
 ** Builds and sends an Alerts notification & Get Alerts Response command
 **
 ** PARAMETERS:                                                                     Name                               Usage
 ** uint8                                                                           u8SourceEndPointId                 Source EP Id
 ** uint8                                                                           u8DestinationEndPointId            Destination EP Id
 ** tsZCL_Address                                                                   *psDestinationAddress              Destination Address
 ** uint8                                                                           *pu8TransactionSequenceNumber      Sequence number Pointer
 ** teCLD_ApplianceEventsAndAlerts_CommandId                                        eCommandId                        Command Id
 ** tsCLD_AEAA_GetAlertsResponseORAlertsNotificationPayload                         *psPayload                        Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_AEAAGetAlertsResponseORAlertsNotificationSend(
                                        uint8                                                                       u8SourceEndPointId,
                                        uint8                                                                       u8DestinationEndPointId,
                                        tsZCL_Address                                                               *psDestinationAddress,
                                        uint8                                                                       *pu8TransactionSequenceNumber,
                                        teCLD_ApplianceEventsAndAlerts_CommandId                                    eCommandId,
                                        tsCLD_AEAA_GetAlertsResponseORAlertsNotificationPayload                     *psPayload)
{
    uint8    u8PayloadSize;
    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT8,                                       &(psPayload->u8AlertsCount)},
            {(psPayload->u8AlertsCount & 0x0F),     E_ZCL_UINT24,                                      psPayload->au24AlertStructure},
                                                          };
    if((psPayload->u8AlertsCount & 0x0F)){
        u8PayloadSize = sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem);        
    }else {
        u8PayloadSize = 1;
    }
    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  APPLIANCE_MANAGEMENT_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS,
                                  TRUE,
                                  eCommandId,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  u8PayloadSize);

}

/****************************************************************************
 **
 ** NAME:       eCLD_AEAAEventNotificationSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Events Notification command
 **
 ** PARAMETERS:                                                 Name                               Usage
 ** uint8                                                       u8SourceEndPointId                 Source EP Id
 ** uint8                                                       u8DestinationEndPointId            Destination EP Id
 ** tsZCL_Address                                               *psDestinationAddress              Destination Address
 ** uint8                                                       *pu8TransactionSequenceNumber      Sequence number Pointer
 ** tsCLD_AEAA_EventNotificationPayload                         *psPayload                         Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_AEAAEventNotificationSend(
                                        uint8                                                                       u8SourceEndPointId,
                                        uint8                                                                       u8DestinationEndPointId,
                                        tsZCL_Address                                                               *psDestinationAddress,
                                        uint8                                                                       *pu8TransactionSequenceNumber,
                                        tsCLD_AEAA_EventNotificationPayload                                         *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                                     E_ZCL_UINT8,                                       &(psPayload->u8EventHeader)},
            {1,                                     E_ZCL_UINT8,                                       &(psPayload->u8EventIdentification)},
                                                          };
    // handle sequence number get a new sequence number
    if(pu8TransactionSequenceNumber != NULL)
        *pu8TransactionSequenceNumber = u8GetTransactionSequenceNumber();
    
    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  APPLIANCE_MANAGEMENT_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS,
                                  TRUE,
                                  E_CLD_APPLIANCE_EVENTS_AND_ALERTS_CMD_EVENT_NOTIFICATION,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eCLD_AEAAGetAlertsReceive
 **
 ** DESCRIPTION:
 ** handles rx of log request
 **
 ** PARAMETERS:                                       Name                           Usage
 ** ZPS_tsAfEvent                                    *pZPSevent                     Zigbee stack event structure
 ** tsZCL_EndPointDefinition                         *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance                            *psClusterInstance             Cluster structure
 ** uint8                                            *pu8TransactionSequenceNumber  Sequence number Pointer
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_AEAAGetAlertsReceive(
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber)
{
    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     0,
                                     0,
                                     E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}

#endif
/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
