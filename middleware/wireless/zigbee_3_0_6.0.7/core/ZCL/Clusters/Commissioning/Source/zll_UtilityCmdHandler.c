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
\file       zll_UtilityCmdHandler.c
\brief      ZigBee Light Link profile functions
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zps_apl.h"
#include "zcl_heap.h"
#include "zcl.h"
#include "zcl_options.h"
#include "zll_utility.h"
#include "zll_commission.h"



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
#ifdef ZLL_UTILITY_CLIENT
PRIVATE teZCL_Status eCLD_ZllUtilityHandleEndpointInfoCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset);
PRIVATE teZCL_Status eCLD_ZllUtilityHandleGroupIdRspCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset);
PRIVATE teZCL_Status eCLD_ZllUtilityHandleEndpointListRspCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset);


#endif

#ifdef ZLL_UTILITY_SERVER
PRIVATE teZCL_Status eCLD_ZllUtilityHandleGroupIdReqCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset);
PRIVATE teZCL_Status eCLD_ZllUtilityHandleEndpointListReqCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset);


#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:     eCLD_ZllUtilityCommandHandler
 **
 ** DESCRIPTION:
 ** sets address, based on Mode
 **
 ** PARAMETERS:                 Name                      Usage
 ** ZPS_tsAfEvent               *pZPSevent                Zigbee stack event structure
 ** tsZCL_EndPointDefinition    *psEndPointDefinition,    pointer to endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance,       pointer to cluster instance
 **
 ** RETURN:
 ** status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_ZllUtilityCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance)
{
    tsZCL_HeaderParams sZCL_HeaderParams;
    tsCLD_ZllCommissionCustomDataStructure *psCommon;
    
    sZCL_HeaderParams.u8TransactionSequenceNumber = 0;
    sZCL_HeaderParams.u8CommandIdentifier = 0xFF;
    /* Get pointer to custom data structure */
    psCommon = (tsCLD_ZllCommissionCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;


    #if (defined ZLL_UTILITY_SERVER) && (defined ZLL_UTILITY_CLIENT)
        uint16 u16Offset;
        // further error checking can only be done once we have interrogated the ZCL payload
        u16Offset = u16ZCL_ReadCommandHeader(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
                                 &sZCL_HeaderParams);
    #endif

    /* Store return address details */
    eZCL_SetReceiveEventAddressStructure(pZPSevent, &psCommon->sRxAddr);

    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif


    // fill in callback event structure
    eZCL_SetCustomCallBackEvent(&psCommon->sCustomCallBackEvent, pZPSevent, sZCL_HeaderParams.u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    /* Fill in message */
    psCommon->sCallBackMessage.u8CommandId = sZCL_HeaderParams.u8CommandIdentifier;


    /* Handle messages appropriate for the cluster type (Client/Server) */
    if(psClusterInstance->bIsServer)
    {
#ifdef ZLL_UTILITY_SERVER
        // SERVER
        switch(sZCL_HeaderParams.u8CommandIdentifier)
        {
        case E_CLD_UTILITY_CMD_GET_GROUP_ID_REQ_RSP:
            eCLD_ZllUtilityHandleGroupIdReqCommand(
                    pZPSevent,
                    psEndPointDefinition,
                    psClusterInstance,
                    u16Offset);
            break;

        case E_CLD_UTILITY_CMD_GET_ENDPOINT_LIST_REQ_RSP:
            eCLD_ZllUtilityHandleEndpointListReqCommand(
                    pZPSevent,
                    psEndPointDefinition,
                    psClusterInstance,
                    u16Offset);
            break;
            default:
            {
                // unlock
                //DBG_vPrintf(TRUE, "Zll unhandled server %d\n", u8CommandIdentifier);
                #ifndef COOPERATIVE
                    eZCL_ReleaseMutex(psEndPointDefinition);
                #endif

                return(E_ZCL_ERR_CUSTOM_COMMAND_HANDLER_NULL_OR_RETURNED_ERROR);
                break;
            }
        }
#endif
    }
    else
    {
#ifdef ZLL_UTILITY_CLIENT
        // CLIENT
        switch(sZCL_HeaderParams.u8CommandIdentifier)
        {
        case E_CLD_UTILITY_CMD_ENDPOINT_INFO:
            eCLD_ZllUtilityHandleEndpointInfoCommand(pZPSevent,
                    psEndPointDefinition,
                    psClusterInstance,
                    u16Offset);
            break;

        case E_CLD_UTILITY_CMD_GET_GROUP_ID_REQ_RSP:
            eCLD_ZllUtilityHandleGroupIdRspCommand(
                    pZPSevent,
                    psEndPointDefinition,
                    psClusterInstance,
                    u16Offset);
            break;

        case E_CLD_UTILITY_CMD_GET_ENDPOINT_LIST_REQ_RSP:
            eCLD_ZllUtilityHandleEndpointListRspCommand(
                    pZPSevent,
                    psEndPointDefinition,
                    psClusterInstance,
                    u16Offset);
            break;

            default:
                // unlock
                //DBG_vPrintf(TRUE, "Zll unhandled client %d\n", u8CommandIdentifier);
                #ifndef COOPERATIVE
                    eZCL_ReleaseMutex(psEndPointDefinition);
                #endif

                return(E_ZCL_ERR_CUSTOM_COMMAND_HANDLER_NULL_OR_RETURNED_ERROR);
                break;
        }
#endif
    }


    // unlock
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    // delete the i/p buffer on return
    return(E_ZCL_SUCCESS);
}


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

#ifdef ZLL_UTILITY_SERVER
PRIVATE  teZCL_Status eCLD_ZllUtilityHandleGroupIdReqCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset)
{
    teZCL_Status eStatus = E_ZCL_SUCCESS;
    tsCLD_ZllUtility_GetGroupIdReqCommandPayload sPayload;
    uint16 u16ActualQuantity;
    uint8   u8TransactionSequenceNumber;
    tsCLD_ZllCommissionCustomDataStructure *psCommon;

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
        {1, &u16ActualQuantity, E_ZCL_UINT8,  &sPayload.u8StartIndex}
    };

    eStatus = eZCL_CustomCommandReceive(pZPSevent,
            &u8TransactionSequenceNumber,
            asPayloadDefinition,
            sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
            E_ZCL_ACCEPT_EXACT);

    /* test status, and send reply */
    /* test status, and send reply */
    if (eStatus != E_ZCL_SUCCESS) {
        return eStatus;
    }



    psCommon = (tsCLD_ZllCommissionCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    tsZCL_Address  sDstAddr;

    sDstAddr.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
    sDstAddr.uAddress.u16DestinationAddress = psCommon->sRxAddr.uSrcAddress.u16Addr;

    eStatus = eCLD_ZllUtilityCommandGetGroupIdRspCommandSend(
                        pZPSevent->uEvent.sApsDataIndEvent.u8DstEndpoint,
                        pZPSevent->uEvent.sApsDataIndEvent.u8SrcEndpoint,
                        &sDstAddr,
                        &u8TransactionSequenceNumber,
                        sPayload.u8StartIndex);


    return eStatus;
}

/****************************************************************************
 **
 ** NAME:     eCLD_ZllUtilityHandleEndpointListReqCommand
 **
 ** DESCRIPTION:
 ** sets address, based on Mode
 **
 ** PARAMETERS:                 Name                      Usage
 ** ZPS_tsAfEvent               *pZPSevent                Zigbee stack event structure
 ** tsZCL_EndPointDefinition    *psEndPointDefinition     pointer to endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance        pointer to cluster instance
 ** uint16                      u16Offset                 Offset
 **
 ** RETURN:
 ** status
 **
 ****************************************************************************/
PRIVATE  teZCL_Status eCLD_ZllUtilityHandleEndpointListReqCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset)
{
    teZCL_Status eStatus = E_ZCL_SUCCESS;
    tsCLD_ZllUtility_GetEndpointListReqCommandPayload sPayload;
    uint16 u16ActualQuantity;
    uint8   u8TransactionSequenceNumber;
    tsCLD_ZllCommissionCustomDataStructure *psCommon;


    tsZCL_RxPayloadItem asPayloadDefinition[] = {
        {1, &u16ActualQuantity, E_ZCL_UINT8,  &sPayload.u8StartIndex}
    };

    eStatus = eZCL_CustomCommandReceive(pZPSevent,
            &u8TransactionSequenceNumber,
            asPayloadDefinition,
            sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
            E_ZCL_ACCEPT_EXACT);

    /* test status, and send reply */
    if (eStatus != E_ZCL_SUCCESS) {
        return eStatus;
    }

    psCommon = (tsCLD_ZllCommissionCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    tsZCL_Address  sDstAddr;

    sDstAddr.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
    sDstAddr.uAddress.u16DestinationAddress = psCommon->sRxAddr.uSrcAddress.u16Addr;

    eStatus = eCLD_ZllUtilityCommandGetEndpointListRspCommandSend(
                        pZPSevent->uEvent.sApsDataIndEvent.u8DstEndpoint,
                        pZPSevent->uEvent.sApsDataIndEvent.u8SrcEndpoint,
                        &sDstAddr,
                        &u8TransactionSequenceNumber,
                        sPayload.u8StartIndex);

    return eStatus;
}
#endif

#ifdef ZLL_UTILITY_CLIENT
/****************************************************************************
 **
 ** NAME:     eCLD_ZllUtilityHandleEndpointInfoCommand
 **
 ** DESCRIPTION:
 ** sets address, based on Mode
 **
 ** PARAMETERS:                 Name                      Usage
 ** ZPS_tsAfEvent               *pZPSevent                Zigbee stack event structure
 ** tsZCL_EndPointDefinition    *psEndPointDefinition     pointer to endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance        pointer to cluster instance
 ** uint16                      u16Offset                 Offset
 **
 ** RETURN:
 ** status
 **
 ****************************************************************************/
PRIVATE  teZCL_Status eCLD_ZllUtilityHandleEndpointInfoCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset)
{
    teZCL_Status eStatus = E_ZCL_SUCCESS;
    tsCLD_ZllUtility_EndpointInformationCommandPayload sPayload;
    uint16 u16ActualQuantity;
    uint8   u8TransactionSequenceNumber;


    tsZCL_RxPayloadItem asPayloadDefinition[] = {
        {1, &u16ActualQuantity, E_ZCL_UINT64,  &sPayload.u64IEEEAddr},
        {1, &u16ActualQuantity, E_ZCL_UINT16,  &sPayload.u16NwkAddr},
        {1, &u16ActualQuantity, E_ZCL_UINT8,  &sPayload.u8Endpoint},
        {1, &u16ActualQuantity, E_ZCL_UINT16,  &sPayload.u16ProfileID},
        {1, &u16ActualQuantity, E_ZCL_UINT16,  &sPayload.u16DeviceID},
        {1, &u16ActualQuantity, E_ZCL_UINT8,  &sPayload.u8Version}
    };

    eStatus = eZCL_CustomCommandReceive(pZPSevent,
            &u8TransactionSequenceNumber,
            asPayloadDefinition,
            sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
            E_ZCL_ACCEPT_EXACT);

    if (eStatus != E_ZCL_SUCCESS) {
        return eStatus;
    }

    tsCLD_ZllCommissionCustomDataStructure   *psCommon;


    /* Get pointer to custom data structure */
    psCommon = (tsCLD_ZllCommissionCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    /* Message data for callback */
    psCommon->sCallBackMessage.uMessage.psEndpointInfoPayload = &sPayload;

    /* call callback */
psEndPointDefinition->pCallBackFunctions(&psCommon->sCustomCallBackEvent);

    return E_ZCL_SUCCESS;
}


/****************************************************************************
 **
 ** NAME:     eCLD_ZllUtilityHandleGroupIdRspCommand
 **
 ** DESCRIPTION:
 ** sets address, based on Mode
 **
 ** PARAMETERS:                 Name                      Usage
 ** ZPS_tsAfEvent               *pZPSevent                Zigbee stack event structure
 ** tsZCL_EndPointDefinition    *psEndPointDefinition     pointer to endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance        pointer to cluster instance
 ** uint16                      u16Offset                 Offset
 **
 ** RETURN:
 ** status
 **
 ****************************************************************************/
PRIVATE  teZCL_Status eCLD_ZllUtilityHandleGroupIdRspCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset)
{
    tsCLD_ZllUtility_GetGroupIdRspCommandPayload sPayload;
    int i;
    uint8 * pu8Data = (uint8 *)(PDUM_pvAPduInstanceGetPayload(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst)) + u16Offset;

    sPayload.u8Total = *pu8Data++;
    sPayload.u8StartIndex = *pu8Data++;
    sPayload.u8Count = *pu8Data++;
    for (i=0; i<sPayload.u8Count; i++) {
        sPayload.asGroupRecords[i].u16GroupID = *pu8Data++;
        sPayload.asGroupRecords[i].u16GroupID |= (*pu8Data++) << 8;
        sPayload.asGroupRecords[i].u8GroupType = *pu8Data++;
    }

    tsCLD_ZllCommissionCustomDataStructure *psCommon;

    /* Get pointer to custom data structure */
    psCommon = (tsCLD_ZllCommissionCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    /* Message data for callback */
    psCommon->sCallBackMessage.uMessage.psGetGroupIdRspPayload = &sPayload;

    /* call callback */
    psEndPointDefinition->pCallBackFunctions(&psCommon->sCustomCallBackEvent);

    return E_ZCL_SUCCESS;




}


/****************************************************************************
 **
 ** NAME:     eCLD_ZllUtilityHandleEndpointListRspCommand
 **
 ** DESCRIPTION:
 ** sets address, based on Mode
 **
 ** PARAMETERS:                 Name                      Usage
 ** ZPS_tsAfEvent               *pZPSevent                Zigbee stack event structure
 ** tsZCL_EndPointDefinition    *psEndPointDefinition     pointer to endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance        pointer to cluster instance
 ** uint16                      u16Offset                 Offset
 **
 ** RETURN:
 ** status
 **
 ****************************************************************************/
PRIVATE  teZCL_Status eCLD_ZllUtilityHandleEndpointListRspCommand(
        ZPS_tsAfEvent               *pZPSevent,
        tsZCL_EndPointDefinition    *psEndPointDefinition,
        tsZCL_ClusterInstance       *psClusterInstance,
        uint16                      u16Offset)
{
    tsCLD_ZllUtility_GetEndpointListRspCommandPayload sPayload;
    int i;
    uint8 * pu8Data = (uint8 *)(PDUM_pvAPduInstanceGetPayload(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst)) + u16Offset;

    sPayload.u8Total = *pu8Data++;
    sPayload.u8StartIndex = *pu8Data++;
    sPayload.u8Count = *pu8Data++;
    for (i=0; i<sPayload.u8Count; i++) {
        sPayload.asEndpointRecords[i].u16NwkAddr = *pu8Data++;
        sPayload.asEndpointRecords[i].u16NwkAddr |= (*pu8Data++) << 8;
        sPayload.asEndpointRecords[i].u8Endpoint = *pu8Data++;
        sPayload.asEndpointRecords[i].u16ProfileID = *pu8Data++;
        sPayload.asEndpointRecords[i].u16ProfileID |= (*pu8Data++) << 8;
        sPayload.asEndpointRecords[i].u16DeviceID = *pu8Data++;
        sPayload.asEndpointRecords[i].u16DeviceID |= (*pu8Data++) << 8;
        sPayload.asEndpointRecords[i].u8Version = *pu8Data++;
    }

    tsCLD_ZllCommissionCustomDataStructure *psCommon;

    /* Get pointer to custom data structure */
    psCommon = (tsCLD_ZllCommissionCustomDataStructure*)psClusterInstance->pvEndPointCustomStructPtr;

    /* Message data for callback */
    psCommon->sCallBackMessage.uMessage.psGetEndpointListRspPayload = &sPayload;

    /* call callback */
    psEndPointDefinition->pCallBackFunctions(&psCommon->sCustomCallBackEvent);

    return E_ZCL_SUCCESS;
}
#endif


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

