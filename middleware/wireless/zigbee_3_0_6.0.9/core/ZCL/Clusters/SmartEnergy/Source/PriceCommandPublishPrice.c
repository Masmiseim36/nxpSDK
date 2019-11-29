/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       PriceCommandPublishPrice.c
\brief      Send a publish price command
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
 ** NAME:       eSE_PricePublishPriceSend
 **
 ** DESCRIPTION:
 ** Builds and sends the Publish Price command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsSE_PricePublishPriceCmdPayload   *psPublishPriceCmdPayload       Message contents structure
 **
 ** RETURN:
 ** teSE_PriceStatus
 **
 ****************************************************************************/

PUBLIC  teSE_PriceStatus eSE_PricePublishPriceSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address              *psDestinationAddress,
                    uint8                      *pu8TransactionSequenceNumber,
                    tsSE_PricePublishPriceCmdPayload   *psPublishPriceCmdPayload)
{

    uint32 u32UTCTime;

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32ProviderId},
            {1, E_ZCL_OSTRING,  &psPublishPriceCmdPayload->sRateLabel},
            {1, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32IssuerEventId},
            {1, E_ZCL_UINT32,   &u32UTCTime},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8UnitOfMeasure},
            {1, E_ZCL_UINT16,   &psPublishPriceCmdPayload->u16Currency},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8PriceTrailingDigitAndPriceTier},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8NumberOfPriceTiersAndRegisterTiers},
            {1, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32StartTime},
            {1, E_ZCL_UINT16,   &psPublishPriceCmdPayload->u16DurationInMinutes},
            {1, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32Price},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8PriceRatio},
            {1, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32GenerationPrice},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8GenerationPriceRatio},
            {1, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32AlternateCostDelivered},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8AlternateCostUnit},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8AlternateCostTrailingDigit},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8NumberOfBlockThresholds},
            {1, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8PriceControl},
        };

    // Time must be synchronised as current time is sent in the publish message
    if(!bZCL_GetTimeHasBeenSynchronised())
    {
        return(E_ZCL_ERR_TIME_NOT_SYNCHRONISED);
    }

    // get time
    u32UTCTime = u32ZCL_GetUTCTime();
    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SE_CLUSTER_ID_PRICE,
                                  TRUE,
                                  E_SE_PUBLISH_PRICE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));
}

#endif//CLD_PRICE
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
