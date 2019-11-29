/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
