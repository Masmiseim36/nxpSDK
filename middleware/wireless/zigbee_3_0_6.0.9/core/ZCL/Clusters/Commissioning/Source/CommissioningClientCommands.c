/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       CommissioningClientCommands.c
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
#ifdef COMMISSIONING_CLIENT
/****************************************************************************
 **
 ** NAME:       eCLD_CommissioningCommandRestartDeviceSend
 **
 ** DESCRIPTION:
 ** Builds and sends a restart device cluster command
 **
 ** PARAMETERS:                                 Name                           Usage
 ** uint8                                       u8SourceEndPointId             Source EP Id
 ** uint8                                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address                               *psDestinationAddress          Destination Address
 ** uint8                                       *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Commissioning_RestartDevicePayload    *psPayload                     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_CommissioningCommandRestartDeviceSend(
                                        uint8                                       u8SourceEndPointId,
                                        uint8                                       u8DestinationEndPointId,
                                        tsZCL_Address                               *psDestinationAddress,
                                        uint8                                       *pu8TransactionSequenceNumber,
                                        tsCLD_Commissioning_RestartDevicePayload    *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                         E_ZCL_BMAP8,   &psPayload->u8Options},
            {1,                         E_ZCL_UINT8,   &psPayload->u8Delay},
            {1,                         E_ZCL_UINT8,   &psPayload->u8Jitter},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  COMMISSIONING_CLUSTER_ID_COMMISSIONING,
                                  FALSE,
                                  E_CLD_COMMISSIONING_CMD_RESTART_DEVICE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eCLD_CommissioningCommandModifyStartupParamsSend
 **
 ** DESCRIPTION:
 ** Builds and sends a Save, restore and Reset start up parameters cluster command
 **
 ** PARAMETERS:                                         Name                           Usage
 ** uint8                                               u8SourceEndPointId             Source EP Id
 ** uint8                                               u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address                                       *psDestinationAddress          Destination Address
 ** uint8                                               *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Commissioning_ModifyStartupParametersPayload  *psPayload                     Payload
 ** teCLD_Commissioning_Command                         eCLD_Commissioning_Command     CommandID
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_CommissioningCommandModifyStartupParamsSend(
                                        uint8                                               u8SourceEndPointId,
                                        uint8                                               u8DestinationEndPointId,
                                        tsZCL_Address                                       *psDestinationAddress,
                                        uint8                                               *pu8TransactionSequenceNumber,
                                        tsCLD_Commissioning_ModifyStartupParametersPayload  *psPayload,
                                        teCLD_Commissioning_Command                         eCLD_Commissioning_Command)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                         E_ZCL_BMAP8,   &psPayload->u8Options},
            {1,                         E_ZCL_UINT8,   &psPayload->u8Index},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  COMMISSIONING_CLUSTER_ID_COMMISSIONING,
                                  FALSE,
                                  eCLD_Commissioning_Command,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eCLD_CommissioningCommandResponseReceive
 **
 ** DESCRIPTION:
 ** handles rx of commissioning response
 **
 ** PARAMETERS:                         Name                           Usage
 ** ZPS_tsAfEvent                       *pZPSevent                     Zigbee stack event structure
 ** tsZCL_EndPointDefinition            *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance               *psClusterInstance             Cluster structure
 ** uint8                               *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Commissioning_ResponsePayload *psPayload                     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_CommissioningCommandResponseReceive(
                    ZPS_tsAfEvent                       *pZPSevent,
                    uint8                               *pu8TransactionSequenceNumber,
                    tsCLD_Commissioning_ResponsePayload *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
    {1, &u16ActualQuantity, E_ZCL_ENUM8,   &psPayload->u8Status},
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                        pu8TransactionSequenceNumber,
                                        asPayloadDefinition,
                                        sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                        E_ZCL_ACCEPT_EXACT);

}
#endif /*COMMISSIONING_CLIENT*/
/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
