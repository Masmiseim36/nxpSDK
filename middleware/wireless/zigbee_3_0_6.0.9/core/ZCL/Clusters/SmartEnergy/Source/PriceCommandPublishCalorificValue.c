/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       PriceCommandPublishCalorificValue.c
\brief      eSE_PricePublishConversionFactor
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



#ifdef PRICE_CALORIFIC_VALUE
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
 ** NAME:       eSE_PricePublishConversionFactor
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

PUBLIC  teSE_PriceStatus eSE_PricePublishCalorificValueSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address              *psDestinationAddress,
                    uint8                      *pu8TransactionSequenceNumber,
                    tsSE_PricePublishCalorificValueCmdPayload   *psPublishCalorificValueCmdPayload)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1, E_ZCL_UINT32,   &psPublishCalorificValueCmdPayload->u32IssuerEventId},
            {1, E_ZCL_UINT32,   &psPublishCalorificValueCmdPayload->u32StartTime},
            {1, E_ZCL_UINT32,   &psPublishCalorificValueCmdPayload->u32CalorificValue},
            {1, E_ZCL_ENUM8,    &psPublishCalorificValueCmdPayload->e8CalorificValueUnit},
            {1, E_ZCL_BMAP8,   &psPublishCalorificValueCmdPayload->b8CalorificValueTrailingDigit},
            };

   if(!bZCL_GetTimeHasBeenSynchronised())
   {
        return(E_ZCL_ERR_TIME_NOT_SYNCHRONISED);
   }


    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SE_CLUSTER_ID_PRICE,
                                  TRUE,
                                  E_SE_PUBLISH_CALORIFIC_VALUE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));


}

#endif
#endif /* CLD_PRICE */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
