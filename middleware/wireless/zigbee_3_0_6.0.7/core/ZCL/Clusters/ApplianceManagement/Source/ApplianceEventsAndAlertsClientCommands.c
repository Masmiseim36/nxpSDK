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
\file       ApplianceEventsAndAlertsClientCommands.c
\brief      Appliance Events And Alerts Cliend Commands
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

#ifdef APPLIANCE_EVENTS_AND_ALERTS_CLIENT
/****************************************************************************
 **
 ** NAME:       eCLD_AEAAGetAlertsSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Log Request command
 **
 ** PARAMETERS:                                                     Name                               Usage
 ** uint8                                                           u8SourceEndPointId                 Source EP Id
 ** uint8                                                           u8DestinationEndPointId            Destination EP Id
 ** tsZCL_Address                                                   *psDestinationAddress              Destination Address
 ** uint8                                                           *pu8TransactionSequenceNumber      Sequence number Pointer
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_AEAAGetAlertsSend(
                    uint8                                                       u8SourceEndPointId,
                    uint8                                                       u8DestinationEndPointId,
                    tsZCL_Address                                               *psDestinationAddress,
                    uint8                                                       *pu8TransactionSequenceNumber)
{

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  APPLIANCE_MANAGEMENT_CLUSTER_ID_APPLIANCE_EVENTS_AND_ALERTS,
                                  FALSE,
                                  E_CLD_APPLIANCE_EVENTS_AND_ALERTS_CMD_GET_ALERTS,
                                  pu8TransactionSequenceNumber,
                                  0,
                                  FALSE,
                                  0,
                                  0);
}

/****************************************************************************
 **
 ** NAME:       eCLD_AEAAGetAlertsResponseORAlertsNotificationReceive
 **
 ** DESCRIPTION:
 ** Recives a Get Alerts Response and Alerts Notification command
 **
 ** PARAMETERS:                                                     Name                               Usage
 ** ZPS_tsAfEvent                                                   *pZPSevent                         ZPS Event
 ** tsZCL_Address                                                   *psDestinationAddress              Destination Address
 ** uint8                                                           *pu8TransactionSequenceNumber      Sequence number Pointer
 ** tsCLD_AEAA_GetAlertsResponseORAlertsNotificationPayload         *psPayload                         Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_AEAAGetAlertsResponseORAlertsNotificationReceive(
                                                        ZPS_tsAfEvent                                                               *pZPSevent,
                                                        uint8                                                                       *pu8TransactionSequenceNumber,
                                                        tsCLD_AEAA_GetAlertsResponseORAlertsNotificationPayload                     *psPayload)
{
    uint16 u16ActualQuantity;
    tsZCL_RxPayloadItem asPayloadDefinition[] = {
             {1,                                                                &u16ActualQuantity,         E_ZCL_UINT8,            &(psPayload->u8AlertsCount)},
             {CLD_APPLIANCE_EVENTS_AND_ALERTS_MAXIMUM_NUM_OF_ALERTS,            &u16ActualQuantity,         E_ZCL_UINT24,           &(psPayload->au24AlertStructure[0])},
                                                          };
    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_LESS);

}
/****************************************************************************
 **
 ** NAME:       eCLD_AEAAEventNotificationReceive
 **
 ** DESCRIPTION:
 ** Recives a Get Alerts Response and Alerts Notification command
 **
 ** PARAMETERS:                                                                     Name                               Usage
 ** uint8                                                                           u8SourceEndPointId                 Source EP Id
 ** uint8                                                                           u8DestinationEndPointId            Destination EP Id
 ** tsZCL_Address                                                                      *psDestinationAddress              Destination Address
 ** uint8                                                                              *pu8TransactionSequenceNumber      Sequence number Pointer
 ** tsCLD_AEAA_EventNotificationPayload                         *psPayload                        Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_AEAAEventNotificationReceive(
                                                        ZPS_tsAfEvent                                                               *pZPSevent,
                                                        uint8                                                                       *pu8TransactionSequenceNumber,
                                                        tsCLD_AEAA_EventNotificationPayload                                         *psPayload)
{
    uint16 u16ActualQuantity;
    tsZCL_RxPayloadItem asPayloadDefinition[] = {
             {1,                                               &u16ActualQuantity,         E_ZCL_UINT8,            &(psPayload->u8EventHeader)},
             {1,                                               &u16ActualQuantity,         E_ZCL_UINT8,            &(psPayload->u8EventIdentification)},
                                                          };
    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT);

}

#endif
/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
