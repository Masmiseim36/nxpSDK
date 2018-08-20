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
\file       PriceCommandPublishPriceReceive.c
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
 ** NAME:       eSE_PublicPriceReceive
 **
 ** DESCRIPTION:
 ** checks validity of a user EP structure
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent              *pZPSevent                     Zigbee stack event structure
 ** tsSE_PricePublishPriceCmdPayload *psPublishPriceCmdPayload      Price Table record
 ** uint8                    *pu8TransactionSequenceNumber  Sequence number Pointer
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC  teSE_PriceStatus eSE_PublishPriceReceive(
                            ZPS_tsAfEvent                *pZPSevent,
                            tsSE_PricePublishPriceCmdPayload   *psPublishPriceCmdPayload,
                            uint8                      *pu8TransactionSequenceNumber,
                            uint32                     *pu32CurrentTimeInMessage)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32ProviderId},
            {1, &u16ActualQuantity, E_ZCL_OSTRING,  &psPublishPriceCmdPayload->sRateLabel},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32IssuerEventId},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   pu32CurrentTimeInMessage},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8UnitOfMeasure},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psPublishPriceCmdPayload->u16Currency},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8PriceTrailingDigitAndPriceTier},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8NumberOfPriceTiersAndRegisterTiers},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32StartTime},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psPublishPriceCmdPayload->u16DurationInMinutes},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32Price},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8PriceRatio},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32GenerationPrice},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8GenerationPriceRatio},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psPublishPriceCmdPayload->u32AlternateCostDelivered},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8AlternateCostUnit},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8AlternateCostTrailingDigit},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8NumberOfBlockThresholds},
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psPublishPriceCmdPayload->u8PriceControl},
    };

    return eZCL_CustomCommandReceive(pZPSevent,
                                     pu8TransactionSequenceNumber,
                                     asPayloadDefinition,
                                     sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                     E_ZCL_ACCEPT_LESS  | E_ZCL_DISABLE_DEFAULT_RESPONSE);

}

#endif//CLD_PRICE
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
