/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       ThermostatCommands.c
\brief      Send/Receive thermostat cluster command
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "Thermostat.h"
#include "Thermostat_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"

#include "dbg.h"

#ifdef DEBUG_CLD_THERMOSTAT
#define TRACE_THERMOSTAT    TRUE
#else
#define TRACE_THERMOSTAT    FALSE
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#ifdef CLD_THERMOSTAT
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

#ifdef THERMOSTAT_CLIENT
/****************************************************************************
 **
 ** NAME:       eCLD_ThermostatCommandSetpointRaiseOrLowerSend
 **
 ** DESCRIPTION:
 ** Builds and sends an thermostat cluster command
 **
 ** PARAMETERS:                                     Name                           Usage
 ** uint8                                           u8SourceEndPointId             Source EP Id
 ** uint8                                           u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address                                   *psDestinationAddress           Destination Address
 ** uint8                                           *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsCLD_Thermostat_SetpointRaiseOrLowerPayload    *psPayload                      Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ThermostatCommandSetpointRaiseOrLowerSend(
                                        uint8           u8SourceEndPointId,
                                        uint8           u8DestinationEndPointId,
                                        tsZCL_Address   *psDestinationAddress,
                                        uint8           *pu8TransactionSequenceNumber,
                                        tsCLD_Thermostat_SetpointRaiseOrLowerPayload *psPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,                         E_ZCL_ENUM8,   &psPayload->eMode},
            {1,                         E_ZCL_INT8,    &psPayload->i8Amount},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  HVAC_CLUSTER_ID_THERMOSTAT,
                                  FALSE,
                                  E_CLD_THERMOSTAT_CMD_SETPOINT_RAISE_LOWER,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}
#endif

#ifdef THERMOSTAT_SERVER

/****************************************************************************
 **
 ** NAME:       eCLD_ThermostatCommandSetpointRaiseOrLowerReceive
 **
 ** DESCRIPTION:
 ** handles rx of identify query response
 **
 ** PARAMETERS:                                     Name                          Usage
 ** ZPS_tsAfEvent                                   *pZPSevent                     Zigbee stack event structure
 ** uint8                                           *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Thermostat_SetpointRaiseOrLowerPayload    *psPayload                     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ThermostatCommandSetpointRaiseOrLowerReceive(
                    ZPS_tsAfEvent                                   *pZPSevent,
                    uint8                                           *pu8TransactionSequenceNumber,
                    tsCLD_Thermostat_SetpointRaiseOrLowerPayload    *psPayload)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1,                             &u16ActualQuantity,             E_ZCL_ENUM8,            &psPayload->eMode},
            {1,                             &u16ActualQuantity,             E_ZCL_INT8,             &psPayload->i8Amount},
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                        pu8TransactionSequenceNumber,
                                        asPayloadDefinition,
                                        sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                        E_ZCL_ACCEPT_EXACT);

}

#endif
#endif /* CLD_THERMOSTAT */
