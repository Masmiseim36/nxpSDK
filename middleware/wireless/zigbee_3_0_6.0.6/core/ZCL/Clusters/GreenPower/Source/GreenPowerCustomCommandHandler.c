/*
* The Clear BSD License
* Copyright 2012-2017 NXP
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

/*!
\file       GreenPowerCustomCommandHandler.c
\brief      handles green power custom commands
*/

#ifdef CLD_GREENPOWER

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <string.h>
#include "zcl.h"
#include "zcl_options.h"
#include "GreenPower.h"
#include "GreenPower_internal.h"
#include "dbg.h"
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
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 **
 ** NAME:       eGP_GreenPowerCommandHandler
 **
 ** DESCRIPTION:
 ** Green Power command handler
 **
 ** PARAMETERS:                    Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                  ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition       endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance          cluster instance
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/
PUBLIC teZCL_Status eGP_GreenPowerCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance)
{
	tsZCL_HeaderParams sZCL_HeaderParams;
	uint16 u16HeaderSize;
    teZCL_Status eStatus = E_ZCL_SUCCESS;

    // further error checking can only be done once we have interrogated the ZCL payload
    u16HeaderSize =     u16ZCL_ReadCommandHeader(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
                             &sZCL_HeaderParams);

    switch (psClusterInstance->bIsServer)
    {
        case(FALSE):
        {
           /* as per spec A ZGP infrastructure device shall silently drop any received
              ZGP command it does not support.It shall not send the ZCL Default Response command.*/
            switch(sZCL_HeaderParams.u8CommandIdentifier)
            {
            case E_GP_ZGP_SINK_TABLE_RESPONSE:
            	eStatus = eGP_HandleSinkTableResponse(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
            	break;
            case E_GP_ZGP_PROXY_TABLE_REQUEST:
            	eStatus = eGP_HandleProxyTableRequest(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
            	break;

                case E_GP_ZGP_PAIRING:
                        eStatus = eGP_HandleZgpPairing(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
                break;

                case E_GP_ZGP_PROXY_COMMOSSIONING_MODE:
#ifndef GP_DISABLE_PROXY_COMMISSION_MODE_CMD
                     eStatus = eGP_HandleProxyCommissioningMode(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
#else
                     eStatus = E_ZCL_SUCCESS;
#endif
                break;

                case E_GP_ZGP_RESPONSE:
#ifndef GP_DISABLE_ZGP_RESPONSE_CMD

                    eStatus = eGP_HandleZgpReponse(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);;

#endif
                    break;

                case E_GP_ZGP_TRANSLATION_TABLE_RESPONSE:
#ifndef GP_DISABLE_TRANSLATION_TABLE_RSP_CMD
#if((defined GP_COMBO_BASIC_DEVICE) || (defined GP_TEST_HARNESS))
                    eStatus = eGP_HandleZgpTranslationResponse(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
#else
                    eStatus = E_ZCL_SUCCESS;
#endif
#endif
                break;
                case E_GP_ZGP_NOTIFICATION:
                        eStatus = eGP_HandleZgpNotification(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);

                    break;
                default:
                    eStatus = E_ZCL_ERR_CUSTOM_COMMAND_HANDLER_NULL_OR_RETURNED_ERROR;
                break;
            }
        }
        break;
        case(TRUE):
        {  /* as per spec A ZGP infrastructure device shall silently drop any received
              ZGP command it does not support.It shall not send the ZCL Default Response command.*/
            switch(sZCL_HeaderParams.u8CommandIdentifier)
            {
#ifdef GP_COMBO_BASIC_DEVICE
            	case E_GP_ZGP_SINK_TABLE_REQUEST:
            		eStatus = eGP_HandleSinkTableRequest(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
            	break;
#endif
            	case E_GP_ZGP_PROXY_TABLE_RESPONSE:
            		eStatus = eGP_HandleProxyTableResponse(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
            	break;

                case E_GP_ZGP_NOTIFICATION:
                        eStatus = eGP_HandleZgpNotification(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);

                    break;

                case E_GP_ZGP_PAIRING_SEARCH:
#ifndef GP_DISABLE_PAIRING_SEARCH_CMD
#ifdef GP_COMBO_BASIC_DEVICE
                        eStatus = eGP_HandleZgpPairingSearch(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
#else
                        eStatus = E_ZCL_SUCCESS;
#endif
#endif
                    break;

                case E_GP_ZGP_TUNNELING_STOP:
                    eStatus = E_ZCL_SUCCESS;
                    break;

                case E_GP_ZGP_COMMOSSIONING_NOTIFICATION:

#ifndef GP_DISABLE_COMMISSION_NOTIFICATION_CMD
                       eStatus = eGP_HandleZgpCommissioningNotification(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);

#else
                        eStatus = E_ZCL_SUCCESS;
#endif
                    break;

                case E_GP_ZGP_TRANSLATION_TABLE_UPDATE:

#ifdef GP_COMBO_BASIC_DEVICE
                        eStatus = eGP_HandleZgpTranslationTableUpdate(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
#else
                        eStatus = E_ZCL_SUCCESS;
#endif
                    break;

                case E_GP_ZGP_TRANSLATION_TABLE_REQUEST:
#ifndef GP_DISABLE_TRANSLATION_TABLE_REQ_CMD
#ifdef GP_COMBO_BASIC_DEVICE
                        eStatus = eGP_HandleZgpTranslationRequest(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
#else
                        eStatus = E_ZCL_SUCCESS;
#endif
#endif
                    break;
                case E_GP_ZGP_PAIRING_CONFIGURATION:

#ifdef GP_COMBO_BASIC_DEVICE
                        eStatus = eGP_HandleZgpPairingConfig(pZPSevent, psEndPointDefinition, psClusterInstance, u16HeaderSize);
#else
                       eStatus = E_ZCL_SUCCESS;
#endif
                    break;
                default:

                   return(E_ZCL_ERR_CUSTOM_COMMAND_HANDLER_NULL_OR_RETURNED_ERROR);
                break;

            }
        }
        break;
        default:
            eStatus = E_ZCL_FAIL;
            break;
    }

    return eStatus;
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#endif //#ifdef CLD_GREENPOWER
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
