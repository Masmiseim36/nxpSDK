/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
