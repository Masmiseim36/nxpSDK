/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       PriceCommandPublishConversionFactorReceive.c
\brief      Receive a publish price command
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

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"



#ifdef PRICE_CONVERSION_FACTOR

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
 ** NAME:       eSE_PublishBlockPeriodPriceReceive
 **
 ** DESCRIPTION:
 ** Receives the Publish Block Period command from ZCL.
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent            *pZPSevent                     Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition          EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 ** tsSE_PricePublishBlockPeriodCmdPayload *psPublishBlockPeriodCmdPayload      Block Period Table record
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** uint32                   *pu32CurrentTimeInMessage      Timestamp
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC  teSE_PriceStatus eSE_PublishConversionFactorReceive(
                            ZPS_tsAfEvent              *pZPSevent,
                            tsSE_PricePublishConversionCmdPayload   *psPublishConversionFactorCmdPayload,
                            uint8                      *pu8TransactionSequenceNumber,
                            uint32                     *pu32CurrentTimeInMessage)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
                {1,&u16ActualQuantity, E_ZCL_UINT32,   &psPublishConversionFactorCmdPayload->u32IssuerEventId},
                {1,&u16ActualQuantity, E_ZCL_UINT32,   &psPublishConversionFactorCmdPayload->u32StartTime},
                {1,&u16ActualQuantity, E_ZCL_UINT32,   &psPublishConversionFactorCmdPayload->u32ConversionFactor},
                {1,&u16ActualQuantity, E_ZCL_BMAP8,   &psPublishConversionFactorCmdPayload->b8ConversionFactorTrailingDigit},
                };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_LESS);



}

#endif
#endif /* CLD_PRICE */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
