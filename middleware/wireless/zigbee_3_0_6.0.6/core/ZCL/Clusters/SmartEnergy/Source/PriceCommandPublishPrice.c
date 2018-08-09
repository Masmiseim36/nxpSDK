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
\file       PriceCommandPublishPrice.c
\brief      Send a publish price command
==============================================================================*/


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

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
