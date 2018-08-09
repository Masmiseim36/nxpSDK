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
\file       ApplianceControlServerCommands.c
\brief      Appliance Control Server Commands
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_options.h"
#include "zcl_customcommand.h"

#include "ApplianceControl.h"
#include "ApplianceControl_internal.h"

#include "dbg.h"


#ifdef DEBUG_CLD_APPLIANCE_CONTROL
#define TRACE_APPLIANCE_CONTROL    TRUE
#else
#define TRACE_APPLIANCE_CONTROL    FALSE
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

#ifdef APPLIANCE_CONTROL_SERVER
/****************************************************************************
 **
 ** NAME:       eCLD_ACSignalStateResponseORSignalStateNotificationSend
 **
 ** DESCRIPTION:
 ** Sends an Execution of command
 **
 ** PARAMETERS:                                                         Name                               Usage
 ** uint8                                                               u8SourceEndPointId                 Source EP Id
 ** uint8                                                               u8DestinationEndPointId            Destination EP Id
 ** tsZCL_Address                                                       *psDestinationAddress              Destination Address
 ** uint8                                                               *pu8TransactionSequenceNumber      Sequence number Pointer
 **    teCLD_ApplianceControl_ServerCommandId                           eCommandId                         Command ID
 **    bool                                                             bApplianceStatusTwoPresent         Is ApplianceStatusTwo is presnet?
 ** tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload        *psPayload                         Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ACSignalStateResponseORSignalStateNotificationSend(
                    uint8                                                               u8SourceEndPointId,
                    uint8                                                               u8DestinationEndPointId,
                    tsZCL_Address                                                       *psDestinationAddress,
                    uint8                                                               *pu8TransactionSequenceNumber,
                    teCLD_ApplianceControl_ServerCommandId                              eCommandId,
                    bool_t                                                                bApplianceStatusTwoPresent,
                    tsCLD_AC_SignalStateResponseORSignalStateNotificationPayload        *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
    {1,                                     E_ZCL_UINT8,                            &psPayload->eApplianceStatus},
    {1,                                     E_ZCL_UINT8,                            &psPayload->u8RemoteEnableFlagAndDeviceStatusTwo},
    {1,                                     E_ZCL_UINT24,                           &psPayload->u24ApplianceStatusTwo},
                                                };
    
    uint8    u8PayloadSize;
    if(bApplianceStatusTwoPresent){
        u8PayloadSize = sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem);
    }else{
        u8PayloadSize = 2; 
    }
    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  APPLIANCE_MANAGEMENT_CLUSTER_ID_APPLIANCE_CONTROL,
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
 ** NAME:       eCLD_ACExecutionOfCommandReceive
 **
 ** DESCRIPTION:
 ** Receives Execution Of command
 **
 ** PARAMETERS:                                                     Name                               Usage
 ** ZPS_tsAfEvent                                                   *pZPSevent                         Event Received
 ** uint8                                                           *pu8TransactionSequenceNumber      Sequence number Pointer
 ** tsCLD_AC_ExecutionOfCommandPayload                              *psPayload                         Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ACExecutionOfCommandReceive(
                                                ZPS_tsAfEvent                                                       *pZPSevent,
                                                uint8                                                               *pu8TransactionSequenceNumber,
                                                tsCLD_AC_ExecutionOfCommandPayload                                  *psPayload)
{
    uint16 u16ActualQuantity;
    tsZCL_RxPayloadItem asPayloadDefinition[] = {
             {1,                                &u16ActualQuantity,         E_ZCL_UINT8,                           &(psPayload->eExecutionCommandId)}
                                                          };
    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_EXACT);

}

/****************************************************************************
 **
 ** NAME:       eCLD_ACSignalStateReceive
 **
 ** DESCRIPTION:
 ** Receives signal state command
 **
 ** PARAMETERS:                                                     Name                               Usage
 ** ZPS_tsAfEvent                                                   *pZPSevent                         Event Received
 ** uint8                                                           *pu8TransactionSequenceNumber      Sequence number Pointer
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ACSignalStateReceive(
                                                ZPS_tsAfEvent                                                       *pZPSevent,
                                                uint8                                                               *pu8TransactionSequenceNumber)
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
