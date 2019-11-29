/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       PriceCommandGetBlockPeriod.c
\brief      eSE_GetBlockPeriodCommand Send and Receive functions
==============================================================================*/
#include "zcl_options.h"

#ifdef CLD_PRICE
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "Price.h"
#include "Price_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"



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

/****************************************************************************
 **
 ** NAME:       eSE_PriceGetBlockPeriodSend
 **
 ** DESCRIPTION:
 ** Sends Get Block Period Request
 **
 ** PARAMETERS:         Name                           Usage
 ** uint8               u8SourceEndPointId             Source EP Id
 ** uint8               u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address      *psDestinationAddress           Destination Address
 ** uint8              *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsSE_PriceGetBlockPeriodCmdPayload *psGetBlockPeriodCmdPayload      Command Payload
 **
 ** RETURN:
 ** teSE_PriceStatus
 **
 ****************************************************************************/

PUBLIC  teSE_PriceStatus eSE_PriceGetBlockPeriodSend(
                    uint8                                      u8SourceEndPointId,
                    uint8                                      u8DestinationEndPointId,
                    tsZCL_Address                              *psDestinationAddress,
                    uint8                                      *pu8TransactionSequenceNumber,
                    tsSE_PriceGetBlockPeriodCmdPayload       *psGetBlockPeriodCmdPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1, E_ZCL_UTCT,  &psGetBlockPeriodCmdPayload->utctStartTime},
            {1, E_ZCL_UINT8,  &psGetBlockPeriodCmdPayload->u8NoOfEvents},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SE_CLUSTER_ID_PRICE,
                                  FALSE,
                                  E_SE_GET_BLOCK_PERIOD,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}



/****************************************************************************
 **
 ** NAME:       eSE_GetBlockPeriodsReceive
 **
 ** DESCRIPTION:
 ** handles scheduled block period events
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent            *pZPSevent                     Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** uint32                   *pu32StartTime                 start time
 ** uint8                    *pu8NumberOfEvents             number of events recieved
 **
 ** RETURN:
 ** teSE_PriceStatus
 **
 ****************************************************************************/

PUBLIC  teSE_PriceStatus eSE_GetBlockPeriodsReceive(
                    ZPS_tsAfEvent               *pZPSevent,
                    uint8                       *pu8TransactionSequenceNumber,
                    uint32                      *pu32StartTime,
                    uint8                       *pu8NumberOfEvents)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT32,   pu32StartTime},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    pu8NumberOfEvents},
                                                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_DISABLE_DEFAULT_RESPONSE);

}
#endif /* CLD_PRICE */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
