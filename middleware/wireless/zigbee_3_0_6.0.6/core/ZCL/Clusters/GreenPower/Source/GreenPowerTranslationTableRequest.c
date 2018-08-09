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
\file       GreenPowerTranslationTableRequest.c
\brief      ZGP Translation table request command send and receive functions
*/

#ifdef CLD_GREENPOWER

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "GreenPower.h"
#include "GreenPower_internal.h"

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
#ifndef GP_DISABLE_TRANSLATION_TABLE_REQ_CMD
/****************************************************************************
 **
 ** NAME:       eGP_ZgpTranslationTableRequestSend
 **
 ** DESCRIPTION:
 ** Sends ZGP Translation table request command
 **
 ** PARAMETERS:         Name                           Usage
 ** uint8               u8SourceEndPointId             Source EP Id
 ** uint8               u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address      *psDestinationAddress           Destination Address
 ** uint8              *pu8TransactionSequenceNumber   Sequence number Pointer
 ** uint8              u8StartIndex                    starting index of translation table
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC teZCL_Status eGP_ZgpTranslationTableRequestSend(
                    uint8                                  u8SourceEndPointId,
                    uint8                                  u8DestinationEndPointId,
                    tsZCL_Address                          *psDestinationAddress,
                    uint8                                  *pu8TransactionSequenceNumber,
                    uint8                                  *pu8StartIndex)
{

    uint8 u8ItemsInPayload = 1;

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1,  E_ZCL_UINT8,   pu8StartIndex}
           };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                              u8DestinationEndPointId,
                              psDestinationAddress,
                              GREENPOWER_CLUSTER_ID,
                              FALSE,
                              E_GP_ZGP_TRANSLATION_TABLE_REQUEST,
                              pu8TransactionSequenceNumber,
                              asPayloadDefinition,
                              FALSE,
                              0,
                              u8ItemsInPayload);

}

/****************************************************************************
 **
 ** NAME:       eGP_ZgpTranslationTableRequestReceive
 **
 ** DESCRIPTION:
 ** Handles ZGP Translation table request command
 **
 ** PARAMETERS:               Name                          Usage
 ** ZPS_tsAfEvent            *pZPSevent                     Zigbee stack event structure
 ** uint8                    u8StartIndex,                  starting index of translation table
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/

PUBLIC teZCL_Status eGP_ZgpTranslationTableRequestReceive(
                    ZPS_tsAfEvent                          *pZPSevent,
                    uint8                                  *pu8StartIndex)
{

    uint8 u8TransactionSequenceNumber;
    uint16 u16ActualQuantity;
    uint8  u8ItemsInPayload = 1;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT8,   pu8StartIndex}
           };

    return eZCL_CustomCommandReceive(pZPSevent,
                                 &u8TransactionSequenceNumber,
                                 asPayloadDefinition,
                                 u8ItemsInPayload,
                                 E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

}
#endif

#endif //#ifdef CLD_GREENPOWER
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
