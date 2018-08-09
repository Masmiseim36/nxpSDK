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
\file       PriceCustomCommandHandler.c
\brief      Event handler for price cluster custom commands
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <string.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "Price.h"
#include "Price_internal.h"

#include "zcl_options.h"



/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#if defined(CLD_PRICE) && !defined(PRICE_SERVER) && !defined(PRICE_CLIENT)
#error You Must Have either PRICE_SERVER and/or PRICE_CLIENT defined in zcl_options.h
#endif

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
 ** NAME:       eSE_PriceCommandHandler
 **
 ** DESCRIPTION:
 ** Handles Price Cluster custom commands
 **
 ** PARAMETERS:               Name                      Usage
 ** ZPS_tsAfEvent              *pZPSevent                 Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition      EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance         Cluster structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC  teZCL_Status eSE_PriceCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance)
{

	tsZCL_HeaderParams sZCL_HeaderParams;
    // further error checking can only be done once we have interrogated the ZCL payload
    u16ZCL_ReadCommandHeader(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
                             &sZCL_HeaderParams);

    // store the incoming event details which can be used to send events back to user
    eZCL_SetReceiveEventAddressStructure(pZPSevent, &((tsSE_PriceCustomDataStructure *)psClusterInstance->pvEndPointCustomStructPtr)->sPrice_Common.sReceiveEventAddress);
    switch(psClusterInstance->bIsServer)
    {
#ifdef PRICE_SERVER
        case(TRUE):
        {
            switch(sZCL_HeaderParams.u8CommandIdentifier)
            {
                case(E_SE_GET_CURRENT_PRICE):
                {
                    vSE_HandleGetCurrentPrice(pZPSevent, psEndPointDefinition, psClusterInstance);
                    break;
                }
                case(E_SE_GET_SCHEDULED_PRICES):
                {
                    vSE_HandleGetSheduledPrices(pZPSevent, psEndPointDefinition, psClusterInstance);
                    break;
                }
                case (E_SE_PRICE_ACK):
                {
                    vSE_HandlePriceAck(pZPSevent, psEndPointDefinition, psClusterInstance);
                    break;
                }

#ifdef BLOCK_CHARGING
                case (E_SE_GET_BLOCK_PERIOD):
                {
                    vSE_HandleGetBlockPeriods(pZPSevent, psEndPointDefinition, psClusterInstance);
                    break;
                }
#endif /* BLOCK_CHARGING */

#ifdef PRICE_CONVERSION_FACTOR
                case(E_SE_GET_CONVERSION_FACTOR):
                {
                    vSE_HandleGetConversionFactor(pZPSevent,psEndPointDefinition, psClusterInstance);
                    break;
                }
#endif /*PRICE_CONVERSION_FACTOR */

#ifdef PRICE_CALORIFIC_VALUE
                case(E_SE_GET_CALORIFIC_VALUE):
                {
                    vSE_HandleGetCalorificValue(pZPSevent,psEndPointDefinition, psClusterInstance);
                    break;
                }
#endif /* PRICE_CALORIFIC_VALUE */

                default:
                {
                    return(E_ZCL_FAIL);
                    break;
                }
            }
            break;
        }
#endif

#ifdef PRICE_CLIENT
        case(FALSE):
        {
            switch(sZCL_HeaderParams.u8CommandIdentifier)
            {
                case(E_SE_PUBLISH_PRICE):
                {
                    vSE_HandlePublishPrice(pZPSevent, psEndPointDefinition, psClusterInstance);
                    break;
                }

#ifdef BLOCK_CHARGING
                case(E_SE_PUBLISH_BLOCK_PERIOD):
                {
                    vSE_HandlePublishBlockPeriod(pZPSevent, psEndPointDefinition, psClusterInstance);
                    break;
                }
#endif /* BLOCK_CHARGING */

#ifdef PRICE_CALORIFIC_VALUE
                case (E_SE_PUBLISH_CALORIFIC_VALUE):
                {
                    vSE_HandlePublishCalorificValue(pZPSevent,psEndPointDefinition,psClusterInstance);
                    break;
                }
#endif

#ifdef PRICE_CONVERSION_FACTOR
                case (E_SE_PUBLISH_CONVERSION_FACTOR):
                {
                    vSE_HandlePublishConversionFactor(pZPSevent,psEndPointDefinition,psClusterInstance);
                    break;
                }
#endif
                default:
                {
                    return(E_ZCL_FAIL);
                    break;
                }
            }
            break;
        }
#endif

       // ifdef failsafe
        default:
        {
            return(E_ZCL_FAIL);
            break;
        }
    }


#if defined(PRICE_SERVER) || defined(PRICE_CLIENT)
    // delete the i/p buffer on return
    return(E_ZCL_SUCCESS);
#endif

}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

