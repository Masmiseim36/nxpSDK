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
\file       DRLCCommandCancelLoadControlEvent.c
\brief      Cancel Load Event Code
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>
#include "zcl.h"
#include "zcl_customcommand.h"
#include "DRLC.h"
#include "DRLC_internal.h"
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
 ** NAME:       eSE_DRLCCancelLoadControlEventSend
 **
 ** DESCRIPTION:
 ** Sends Cancel Load Control Event Cmd
 **
 ** PARAMETERS:                         Name                                Usage
 ** uint8                               u8SourceEndPointId                  Source EP Id
 ** uint8                               u8DestinationEndPointId             Destination EP Id
 ** tsZCL_Address                       *psDestinationAddress               Destination Address
 ** tsSE_DRLCCancelLoadControlEvent     *psCancelLoadControlEvent           Cancel Data Structure
 ** uint8                               *pu8TransactionSequenceNumber       Sequence number Pointer
 **
 ** RETURN:
 ** teSE_DRLCStatus
 **
 ****************************************************************************/

PUBLIC  teSE_DRLCStatus eSE_DRLCCancelLoadControlEventSend(
                    uint8                           u8SourceEndPointId,
                    uint8                           u8DestinationEndPointId,
                    tsZCL_Address                   *psDestinationAddress,
                    tsSE_DRLCCancelLoadControlEvent *psCancelLoadControlEvent,
                    uint8                           *pu8TransactionSequenceNumber)
{

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
    {1, E_ZCL_UINT32,   &psCancelLoadControlEvent->u32IssuerId},
    {1, E_ZCL_UINT16,   &psCancelLoadControlEvent->u16DeviceClass},
    {1, E_ZCL_UINT8,    &psCancelLoadControlEvent->u8UtilityEnrolmentGroup},
    {1, E_ZCL_UINT8,    &psCancelLoadControlEvent->eCancelControl},
    {1, E_ZCL_UINT32,   &psCancelLoadControlEvent->u32effectiveTime},
                                              };

    return eZCL_CustomCommandSend(u8SourceEndPointId,
                              u8DestinationEndPointId,
                              psDestinationAddress,
                              SE_CLUSTER_ID_DEMAND_RESPONSE_AND_LOAD_CONTROL,
                              TRUE,
                              SE_DRLC_LOAD_CONTROL_EVENT_CANCEL,
                              pu8TransactionSequenceNumber,
                              asPayloadDefinition,
                              FALSE,
                              0,
                              sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));

}

/****************************************************************************
 **
 ** NAME:       eSE_DRLCCancelLoadControlEventReceive
 **
 ** DESCRIPTION:
 ** Handles Cancel Load Control Event
 **
 ** PARAMETERS:                     Name                            Usage
 ** ZPS_tsAfEvent                   *pZPSevent                      Zigbee stack event structure
 ** tsZCL_EndPointDefinition        *psEndPointDefinition           EP structure
 ** tsSE_DRLCCancelLoadControlEvent *psCancelLoadControlEvent       Cancel Data Structure
 ** uint8                           *pu8TransactionSequenceNumber   Sequence number Pointer
 **
 ** RETURN:
 ** teSE_DRLCStatus
 **
 ****************************************************************************/

PUBLIC  teSE_DRLCStatus eSE_DRLCCancelLoadControlEventReceive(
                    ZPS_tsAfEvent                   *pZPSevent,
                    tsZCL_EndPointDefinition        *psEndPointDefinition,
                    tsSE_DRLCCancelLoadControlEvent *psCancelLoadControlEvent,
                    uint8                           *pu8TransactionSequenceNumber)
{

    uint16 u16ActualQuantity;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
        {1, &u16ActualQuantity, E_ZCL_UINT32,   &psCancelLoadControlEvent->u32IssuerId},
        {1, &u16ActualQuantity, E_ZCL_UINT16,   &psCancelLoadControlEvent->u16DeviceClass},
        {1, &u16ActualQuantity, E_ZCL_UINT8,    &psCancelLoadControlEvent->u8UtilityEnrolmentGroup},
        {1, &u16ActualQuantity, E_ZCL_UINT8,    &psCancelLoadControlEvent->eCancelControl},
        {1, &u16ActualQuantity, E_ZCL_UINT32,   &psCancelLoadControlEvent->u32effectiveTime},
                                                };

    if(eZCL_CustomCommandDRLCReceive(pZPSevent,
                                 psEndPointDefinition,
                                 pu8TransactionSequenceNumber,
                                 asPayloadDefinition,
                                 sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                 E_ZCL_DISABLE_DEFAULT_RESPONSE) != E_ZCL_SUCCESS)
    {
        return E_ZCL_FAIL;
    }

    // If struct not packed - mask off all but lower 8 bits
    psCancelLoadControlEvent->eCancelControl &= 0xff;

    return E_ZCL_SUCCESS;

}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
