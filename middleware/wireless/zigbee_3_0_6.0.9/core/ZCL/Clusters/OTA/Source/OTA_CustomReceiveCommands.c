/*
* Copyright 2012-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!
\file       OTA_CustomReceiveCommands.c
\brief      Over The Air Upgrade custom receive commands
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "FunctionLib.h"
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"
#include "OTA.h"
#include "OTA_private.h"

#include <string.h>
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
#ifdef OTA_CLIENT
/****************************************************************************
 **
 ** NAME:       eOtaReceivedImageNotify
 **
 ** DESCRIPTION:
 ** receives image notify command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/
PUBLIC teZCL_Status eOtaReceivedImageNotify(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    uint8 u8TransactionSequenceNumber;
    uint16 u16ActualQuantity;
    uint8 u8ControlField;
    tsOTA_Common *psOTA_Common = ((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);
    teZCL_Status eStatus = E_ZCL_SUCCESS;

    uint8 u8ItemsInPayload = 0;
    
    tsZCL_RxPayloadItem asPayloadDefinition[] = {
                {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.ePayloadType},
                {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.u8QueryJitter},
                {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.u16ManufacturerCode},
                {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.u16ImageType},
                {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.u32NewFileVersion}

            };
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif


    u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
        u16Offset,
        E_ZCL_UINT8,
        &u8ControlField);

    switch(u8ControlField)
    {
        case E_CLD_OTA_ITYPE_MDID_FVERSION_JITTER:
        {
            u8ItemsInPayload = 5;
        }
        break;
        case E_CLD_OTA_ITYPE_MDID_JITTER:
        {
            u8ItemsInPayload = 4;
        }
        break;
        case E_CLD_OTA_MANUFACTURER_ID_AND_JITTER:
        {
            u8ItemsInPayload=3;
        }
        break;
        case E_CLD_OTA_QUERY_JITTER:
        {
            u8ItemsInPayload=2;
        }
        break;
        default:
            eStatus = E_ZCL_FAIL;
            break;

    }

    if(eStatus != E_ZCL_FAIL)
    {
        eStatus = eZCL_CustomCommandReceive(pZPSevent,
                  &u8TransactionSequenceNumber,
                  asPayloadDefinition,
                  u8ItemsInPayload,
                  E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);
    }


    if (eStatus == E_ZCL_SUCCESS)
    {
        if(
#ifndef ZCLIP_ENABLED
           !OTA_IS_UNICAST(psOTA_Common->sReceiveEventAddress.u8DstAddrMode, psOTA_Common->sReceiveEventAddress.uDstAddress.u16Addr) &&
#endif
           !(psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.u8QueryJitter >= 1 &&
           psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.u8QueryJitter <= 100))
        {
#ifndef ZCLIP_ENABLED
            eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
#endif
        }
        else
        {
#ifndef ZCLIP_ENABLED
            if(OTA_IS_UNICAST(psOTA_Common->sReceiveEventAddress.u8DstAddrMode, psOTA_Common->sReceiveEventAddress.uDstAddress.u16Addr))
            {
                psOTA_Common->sOTACallBackMessage.uMessage.sImageNotifyPayload.ePayloadType = E_CLD_OTA_QUERY_JITTER;
            }
#endif

            psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_IMAGE_NOTIFY;

            // fill in callback event structures
            eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

            psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);
            //post the incoming request to the state machine
            ((pFuncptr)(psOTA_Common->sOTACallBackMessage.sPersistedData.u32FunctionPointer))(psOTA_Common, psEndPointDefinition);
        }
    }
    else
    {
         if(OTA_IS_UNICAST(psOTA_Common->sReceiveEventAddress.u8DstAddrMode, psOTA_Common->sReceiveEventAddress.uDstAddress.u16Addr) )
         {
            eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
         }
    }

    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return eStatus;
}

/****************************************************************************
 **
 ** NAME:       eOtaReceivedQueryNextImageResponse
 **
 ** DESCRIPTION:
 ** receives query next image response command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaReceivedQueryNextImageResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    uint8 u8TransactionSequenceNumber, u8Status;
    uint16 u16ActualQuantity;
    tsOTA_Common *psOTA_Common =((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);

    uint8 u8ItemsInPayload = 0;
    
#ifdef ZCLIP_ENABLED
    uint8 otaFileUri[CLD_OTA_FILE_URI_MAX_LENGTH] = {0};
    psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.sFileUri.u8MaxLength = CLD_OTA_FILE_URI_MAX_LENGTH;
    psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.sFileUri.pu8Data = otaFileUri;

#endif

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
             {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.u8Status},
#ifndef ZCLIP_ENABLED
             {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.u16ManufacturerCode},
             {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.u16ImageType},
#endif
             {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.u32FileVersion},
             {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.u32ImageSize}
#ifdef ZCLIP_ENABLED
            ,{1, &u16ActualQuantity, E_ZCL_CSTRING,  &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImageResponsePayload.sFileUri}
#endif
            };

        // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
        u16Offset,
        E_ZCL_UINT8,
        &u8Status);

    if(u8Status == OTA_STATUS_SUCCESS)
    {
        u8ItemsInPayload=sizeof(asPayloadDefinition)/sizeof(tsZCL_RxPayloadItem);
    }
    else
    {
        u8ItemsInPayload=1;
    }
    u8Status = eZCL_CustomCommandReceive(pZPSevent,
                          &u8TransactionSequenceNumber,
                          asPayloadDefinition,
                          u8ItemsInPayload,
                          E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);
                              
    if(u8Status != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return u8Status;
    }

    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_QUERY_NEXT_IMAGE_RESPONSE;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);

    //post the incoming request to the state machine
   ((pFuncptr)(psOTA_Common->sOTACallBackMessage.sPersistedData.u32FunctionPointer))(psOTA_Common, psEndPointDefinition);

    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return E_ZCL_SUCCESS;
}


/****************************************************************************
 **
 ** NAME:       eOtaReceivedBlockResponse
 **
 ** DESCRIPTION:
 ** receives image block response command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaReceivedBlockResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    uint8 u8TransactionSequenceNumber;
    uint16 u16ActualQuantity;
    uint8 u8Status,u8BlockSize;
    tsOTA_Common *psOTA_Common =((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
        u16Offset,
        E_ZCL_UINT8,
        &u8Status);
    psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.pu8Data = psOTA_Common->sOTACallBackMessage.au8ReadOTAData;
    psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sWaitForData.u16BlockRequestDelayMs = 0;
    if(u8Status == OTA_STATUS_SUCCESS)
    {
        u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
            u16Offset+13,
            E_ZCL_UINT8,
            &u8BlockSize);
        {
            tsZCL_RxPayloadItem asPayloadDefinition[] = {
                {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.u8Status},
                {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u16ManufacturerCode},
                {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u16ImageType},
                {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u32FileVersion},
                {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u32FileOffset},
                {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.u8DataSize},
                {u8BlockSize, &u16ActualQuantity, E_ZCL_UINT8,   psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sBlockPayloadSuccess.pu8Data}
                };
            u8Status = eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                              E_ZCL_DISABLE_DEFAULT_RESPONSE);
        }
    }
    else if(u8Status == OTA_STATUS_WAIT_FOR_DATA)
    {
        tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.u8Status},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sWaitForData.u32CurrentTime},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sWaitForData.u32RequestTime},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sWaitForData.u16BlockRequestDelayMs}
            };
        u8Status = eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                              E_ZCL_ACCEPT_LESS|E_ZCL_DISABLE_DEFAULT_RESPONSE);

        if((u8Status == E_ZCL_SUCCESS)&&
           (psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sWaitForData.u32RequestTime < psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.uMessage.sWaitForData.u32CurrentTime))
        {
            eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
            #ifndef COOPERATIVE
                eZCL_ReleaseMutex(psEndPointDefinition);
            #endif

            return(E_ZCL_FAIL);
        }

    }
    else
    {
        tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sImageBlockResponsePayload.u8Status}
            };
        u8Status = eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                              E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);


    }
    if(u8Status != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return u8Status;
    }

    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_BLOCK_RESPONSE;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);

    //post the incoming request to the state machine
    ((pFuncptr)(psOTA_Common->sOTACallBackMessage.sPersistedData.u32FunctionPointer))(psOTA_Common, psEndPointDefinition);
    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return E_ZCL_SUCCESS;
}

/****************************************************************************
 **
 ** NAME:       eOtaQuerySpecificFileResponse
 **
 ** DESCRIPTION:
 ** receives Query specific file response command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaQuerySpecificFileResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    uint8 u8TransactionSequenceNumber;
    teZCL_Status eStatus;
    uint16 u16ActualQuantity;
    tsOTA_Common *psOTA_Common = ((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);

    uint8 u8ItemsInPayload = 0;

#ifdef ZCLIP_ENABLED
    uint8 otaFileUri[CLD_OTA_FILE_URI_MAX_LENGTH] = {0};
    psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.sFileUri.u8MaxLength = CLD_OTA_FILE_URI_MAX_LENGTH;
    psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.sFileUri.pu8Data = otaFileUri;
#endif

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
                    {1, &u16ActualQuantity, E_ZCL_UINT8,    &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.u8Status},
#ifndef ZCLIP_ENABLED
                    {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.u16ManufacturerCode},
                    {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.u16ImageType},
#endif
                    {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.u32FileVersion},
                    {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.u32ImageSize}
#ifdef ZCLIP_ENABLED
                   ,{1, &u16ActualQuantity, E_ZCL_CSTRING,  &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileResponsePayload.sFileUri}
#endif
                };


    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
        u16Offset,
        E_ZCL_UINT8,
        &eStatus);
    if(eStatus == OTA_STATUS_SUCCESS)
    {
        u8ItemsInPayload = 5;
    }
    else
    {
        u8ItemsInPayload = 1;
    }
    
    eStatus = eZCL_CustomCommandReceive(pZPSevent,
                          &u8TransactionSequenceNumber,
                          asPayloadDefinition,
                          u8ItemsInPayload,
                          E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

    if(eStatus != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return eStatus;
    }

    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_QUERY_SPECIFIC_FILE_RESPONSE;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);

    //post the incoming request to the state machine
    ((pFuncptr)(psOTA_Common->sOTACallBackMessage.sPersistedData.u32FunctionPointer))(psOTA_Common, psEndPointDefinition);

    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return eStatus;
}


/****************************************************************************
 **
 ** NAME:       eOtaReceivedUpgradeEndResponse
 **
 ** DESCRIPTION:
 ** receives Upgrade end response command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaReceivedUpgradeEndResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    uint8 u8TransactionSequenceNumber;
    uint16 u16ActualQuantity;
    tsOTA_Common *psOTA_Common = ((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);
    teZCL_Status eStatus = E_ZCL_SUCCESS;
    // get EP mutex

    tsZCL_RxPayloadItem asPayloadDefinition[] = {
                    {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeResponsePayload.u16ManufacturerCode},
                    {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeResponsePayload.u16ImageType},
                    {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeResponsePayload.u32FileVersion},
                    {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeResponsePayload.u32CurrentTime},
                    {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeResponsePayload.u32UpgradeTime},

                };
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    eStatus =eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                              E_ZCL_ACCEPT_EXACT);


    if(eStatus != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return eStatus;
    }

    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_UPGRADE_END_RESPONSE;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);

    //post the incoming request to the state machine
    if(eStatus == E_ZCL_SUCCESS)
    {
       ((pFuncptr)(psOTA_Common->sOTACallBackMessage.sPersistedData.u32FunctionPointer))(psOTA_Common, psEndPointDefinition);
    }
    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return eStatus;
}

/****************************************************************************
 **
 ** NAME:       vOtaReceivedDefaultResponse
 **
 ** DESCRIPTION:
 ** receives default response command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 **
 ** RETURN:
 ** None
 ****************************************************************************/

PUBLIC  void vOtaReceivedDefaultResponse(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance)
{
    uint8 u8TransactionSequenceNumber,u8CommandId,u8StatusCode;
    uint16 u16ActualQuantity;
    tsOTA_Common *psOTA_Common = ((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);


    tsZCL_RxPayloadItem asPayloadDefinition[] = {
                    {1, &u16ActualQuantity, E_ZCL_UINT8,   &u8CommandId},
                    {1, &u16ActualQuantity, E_ZCL_UINT8,   &u8StatusCode}

                };
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    if(E_ZCL_SUCCESS == eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                              E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE))
    {

        if((E_ZCL_CMDS_SUCCESS != u8StatusCode)&&
                ((E_CLD_OTA_COMMAND_QUERY_NEXT_IMAGE_REQUEST == u8CommandId)||
                (E_CLD_OTA_COMMAND_BLOCK_REQUEST == u8CommandId)||
                (E_CLD_OTA_COMMAND_UPGRADE_END_REQUEST == u8CommandId)))
        {
            psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_INTERNAL_COMMAND_RCVD_DEFAULT_RESPONSE;
            //post the incoming request to the state machine
            ((pFuncptr)(psOTA_Common->sOTACallBackMessage.sPersistedData.u32FunctionPointer))(psOTA_Common, psEndPointDefinition);
        }
    }
    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return ;
}
#endif

#ifdef OTA_SERVER
/****************************************************************************
 **
 ** NAME:       eOtaReceivedQuerySpecificFileRequest
 **
 ** DESCRIPTION:
 ** receives query specific file request command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaReceivedQuerySpecificFileRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{

    teZCL_Status eStatus;
    uint8 u8TransactionSequenceNumber;
    uint16 u16ActualQuantity;

    tsOTA_Common *psOTA_Common =((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);



    tsZCL_RxPayloadItem asPayloadDefinition[] = {
           {1, &u16ActualQuantity, E_ZCL_UINT64,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileRequestPayload.u64RequestNodeAddress},
           {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileRequestPayload.u16ManufacturerCode},
           {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileRequestPayload.u16ImageType},
           {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileRequestPayload.u32FileVersion},
           {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQuerySpFileRequestPayload.u16CurrentZibgeeStackVersion},

            };

     // get EP mutex
     #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

     eStatus =  eZCL_CustomCommandReceive(pZPSevent,
                                  &u8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                  E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);


    if(eStatus != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

        return eStatus;
    }

    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_QUERY_SPECIFIC_FILE_REQUEST;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);
    vOtaUpgManServerCommandHandler(pZPSevent, psOTA_Common, psEndPointDefinition);
    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    return E_ZCL_SUCCESS;
}

/****************************************************************************
 **
 ** NAME:       eOtaReceivedQueryNextImage
 **
 ** DESCRIPTION:
 ** receives query next image request command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaReceivedQueryNextImage(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    teZCL_Status eStatus;
    uint8 u8TransactionSequenceNumber;
    uint16 u16ActualQuantity;
    uint8 u8FieldControl;
    tsOTA_Common *psOTA_Common =((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);

    uint8 u8ItemsInPayload = 0;
        tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT8,    &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImagePayload.u8FieldControl},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImagePayload.u16ManufacturerCode},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImagePayload.u16ImageType},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImagePayload.u32CurrentFileVersion},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sQueryImagePayload.u16HardwareVersion}
            };
                
    u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
        u16Offset,
        E_ZCL_UINT8,
        &u8FieldControl);
        // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif


    if(u8FieldControl & 0x1)
    {
        u8ItemsInPayload = 5;
    }
    else
    {
        u8ItemsInPayload = 4;
    }
    
    eStatus = eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              u8ItemsInPayload,
                              E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);
                                  
    if(eStatus != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return eStatus;
    }

    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_QUERY_NEXT_IMAGE_REQUEST;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);
    vOtaUpgManServerCommandHandler(pZPSevent, psOTA_Common, psEndPointDefinition);
    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    return E_ZCL_SUCCESS;
}
/****************************************************************************
 **
 ** NAME:       eOtaReceivedPageRequest
 **
 ** DESCRIPTION:
 ** receives page request command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/
PUBLIC  teZCL_Status eOtaReceivedPageRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{


#ifndef OTA_PAGE_REQUEST_SUPPORT
    eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_UNSUP_CLUSTER_COMMAND);
#else
    uint8 u8TransactionSequenceNumber;
    teZCL_Status eStatus;
    uint16 u16ActualQuantity;
    uint8 u8FieldControl;
    uint16 u16RspSpacing;
    tsOTA_Common *psOTA_Common =((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);
    tsZCL_RxPayloadItem asActPayloadDefinition[9] = {0};
    uint8 u8ItemsInPayload = 0;
    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u8FieldControl},
            {1, &u16ActualQuantity, E_ZCL_UINT16,  &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u16ManufactureCode},
            {1, &u16ActualQuantity, E_ZCL_UINT16,  &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u16ImageType},
            {1, &u16ActualQuantity, E_ZCL_UINT32,  &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u32FileVersion},
            {1, &u16ActualQuantity, E_ZCL_UINT32,  &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u32FileOffset},
            {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u8MaxDataSize},
            {1, &u16ActualQuantity, E_ZCL_UINT16,  &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u16PageSize},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u16ResponseSpacing},
            {1, &u16ActualQuantity, E_ZCL_UINT64,  &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u64RequestNodeAddress}
        };
    
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif


    u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
        u16Offset,
        E_ZCL_UINT8,
        &u8FieldControl);

    if(u8FieldControl & 0x1)
    {
        u8ItemsInPayload = 9;
    }
    else
    {
        u8ItemsInPayload = 8;
    }

    eStatus = eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              u8ItemsInPayload,
                              E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);    
    if(eStatus != E_ZCL_SUCCESS)
    {

        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return eStatus;
    }
    if(psOTA_Common->sOTACallBackMessage.sPageReqServerParams.u16DataSent !=0)
    {
        tsOTA_ImageBlockResponsePayload sWaitForData;
        tsZCL_Address sZCL_Address;
        sWaitForData.u8Status = OTA_STATUS_WAIT_FOR_DATA;
        sWaitForData.uMessage.sWaitForData.u32CurrentTime = u32ZCL_GetUTCTime();
        sWaitForData.uMessage.sWaitForData.u32RequestTime = u32ZCL_GetUTCTime()+10;
        sWaitForData.uMessage.sWaitForData.u16BlockRequestDelayMs = 0;

        // build address structure
        eZCL_BuildTransmitAddressStructure(pZPSevent, &sZCL_Address);

        eOTA_ServerImageBlockResponse(pZPSevent->uEvent.sApsDataIndEvent.u8DstEndpoint,
                            pZPSevent->uEvent.sApsDataIndEvent.u8SrcEndpoint,
                            &sZCL_Address,
                            &sWaitForData,
                            0,
                            u8TransactionSequenceNumber);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return E_ZCL_SUCCESS;
    }
    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_PAGE_REQUEST;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);
    /* store page request payload information so that multiple blocks can be sent without any further request */
    FLib_MemCpy(&psOTA_Common->sOTACallBackMessage.sPageReqServerParams.sReceiveEventAddress, &psOTA_Common->sReceiveEventAddress, sizeof(tsZCL_ReceiveEventAddress));

    if(OTA_PAGE_REQ_RESPONSE_SPACING >= psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u16ResponseSpacing)
    {
        u16RspSpacing = OTA_PAGE_REQ_RESPONSE_SPACING;
        psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u16ResponseSpacing = OTA_PAGE_REQ_RESPONSE_SPACING;
    }
    else
    {
        u16RspSpacing = psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload.u16ResponseSpacing;
    }
    psOTA_Common->sOTACallBackMessage.sPageReqServerParams.bPageReqRespSpacing = TRUE;
    FLib_MemCpy(&psOTA_Common->sOTACallBackMessage.sPageReqServerParams.sPageReq, &psOTA_Common->sOTACallBackMessage.uMessage.sImagePageRequestPayload, sizeof(tsOTA_ImagePageRequest));
    psOTA_Common->sOTACallBackMessage.sPageReqServerParams.u16DataSent = 0;
    psOTA_Common->sOTACallBackMessage.sPageReqServerParams.u8TransactionNumber = u8TransactionSequenceNumber;
    eZCL_UpdateMsTimer(psEndPointDefinition, TRUE,u16RspSpacing);

    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

#endif
    return E_ZCL_SUCCESS;

}

/****************************************************************************
 **
 ** NAME:       eOtaReceivedBlockRequest
 **
 ** DESCRIPTION:
 ** receives image block request command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaReceivedBlockRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    uint8 u8TransactionSequenceNumber;
    teZCL_Status eStatus;
    uint16 u16ActualQuantity;
    uint8 u8FieldControl;
    tsOTA_Common *psOTA_Common =((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);

    uint8 u8ItemsInPayload = 0;
    
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u16BlockRequestDelay = 0xFFFF;
    u16ZCL_APduInstanceReadNBO(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
        u16Offset,
        E_ZCL_UINT8,
        &u8FieldControl);
    
    tsZCL_RxPayloadItem asPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u8FieldControl},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u16ManufactureCode},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u16ImageType},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u32FileVersion},
            {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u32FileOffset},
            {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u8MaxDataSize},
            {1, &u16ActualQuantity, E_ZCL_UINT64,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u64RequestNodeAddress},
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u16BlockRequestDelay}
        };

    if((u8FieldControl & 0x1)&&(u8FieldControl & 0x2))
    {
        u8ItemsInPayload=8;
    }
    else if(u8FieldControl & 0x2)
    {
        tsZCL_RxPayloadItem asActPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u16BlockRequestDelay}
        };
        FLib_MemCpy(&asPayloadDefinition[6],asActPayloadDefinition,sizeof(asActPayloadDefinition));
        u8ItemsInPayload = 7;
    }
    else if(u8FieldControl & 0x1)
    {
        tsZCL_RxPayloadItem asActPayloadDefinition[] = {
            {1, &u16ActualQuantity, E_ZCL_UINT64,   &psOTA_Common->sOTACallBackMessage.uMessage.sBlockRequestPayload.u64RequestNodeAddress}
        };

        u8ItemsInPayload = 7;
        FLib_MemCpy(&asPayloadDefinition[6],asActPayloadDefinition,sizeof(asActPayloadDefinition));
    }
    else
    {
        u8ItemsInPayload = 6;
    }

    eStatus = eZCL_CustomCommandReceive(pZPSevent,
          &u8TransactionSequenceNumber,
          asPayloadDefinition,
          u8ItemsInPayload,
          E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);
                  
    if(eStatus != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return eStatus;
    }
#if OTA_MAX_SIMULTANEOUS_CLIENTS
    if(vOtaUpgManServerCheckClient(psOTA_Common->sReceiveEventAddress.uSrcAddress.u16Addr, u32ZCL_GetUTCTime()))
    {
#endif
        psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_BLOCK_REQUEST;
        
        // fill in callback event structures
        eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);
        
        psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);
        vOtaUpgManServerCommandHandler(pZPSevent,psOTA_Common, psEndPointDefinition);
#if OTA_MAX_SIMULTANEOUS_CLIENTS
    }
    else
    {
        /*Send Block Response with WAIT_FOR_DATA status */
        tsZCL_Address sZCL_Address;
        tsOTA_ImageBlockResponsePayload sBlockResponse;
        
#if OTA_ACKS_ON == TRUE
        sZCL_Address.eAddressMode = E_ZCL_AM_SHORT;
#else
        sZCL_Address.eAddressMode = E_ZCL_AM_SHORT_NO_ACK;
#endif
        sZCL_Address.uAddress.u16DestinationAddress = psOTA_Common->sReceiveEventAddress.uSrcAddress.u16Addr;
        
        sBlockResponse.u8Status = OTA_STATUS_WAIT_FOR_DATA;
        sBlockResponse.uMessage.sWaitForData.u32CurrentTime = 0;
        sBlockResponse.uMessage.sWaitForData.u32RequestTime = OTA_CLIENT_DELAY_FOR_NEXT_REQUEST;
        sBlockResponse.uMessage.sWaitForData.u16BlockRequestDelayMs = 0;
        
        eOTA_ServerImageBlockResponse(psOTA_Common->sReceiveEventAddress.u8DstEndpoint,
                        psOTA_Common->sReceiveEventAddress.u8SrcEndpoint,
                        &sZCL_Address,
                        &sBlockResponse,
                        0,
                        psOTA_Common->sOTACustomCallBackEvent.u8TransactionSequenceNumber);
    }
#endif //OTA_MAX_SIMULTANEOUS_CLIENTS
    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return E_ZCL_SUCCESS;
}

/****************************************************************************
 **
 ** NAME:       eOtaReceivedUpgradeEndRequest
 **
 ** DESCRIPTION:
 ** receives upgrade end request command
 **
 ** PARAMETERS:                  Name                           Usage
 ** ZPS_tsAfEvent               *pZPSevent                     ZPS event
 ** tsZCL_EndPointDefinition    *psEndPointDefinition          Endpoint definition
 ** tsZCL_ClusterInstance       *psClusterInstance             cluster instance
 ** uint16                       u16Offset                     offset
 **
 ** RETURN:
 ** teZCL_Status
 ****************************************************************************/

PUBLIC  teZCL_Status eOtaReceivedUpgradeEndRequest(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint16                       u16Offset)
{
    uint8 u8TransactionSequenceNumber;
    teZCL_Status eStatus;
    uint16 u16ActualQuantity;
    tsOTA_Common *psOTA_Common =((tsOTA_Common *)psClusterInstance->pvEndPointCustomStructPtr);
    tsZCL_RxPayloadItem asPayloadDefinition[] = {
             {1, &u16ActualQuantity, E_ZCL_UINT8,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeEndRequestPayload.u8Status},
             {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeEndRequestPayload.u16ManufacturerCode},
             {1, &u16ActualQuantity, E_ZCL_UINT16,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeEndRequestPayload.u16ImageType},
             {1, &u16ActualQuantity, E_ZCL_UINT32,   &psOTA_Common->sOTACallBackMessage.uMessage.sUpgradeEndRequestPayload.u32FileVersion}
            };
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif


    eStatus = eZCL_CustomCommandReceive(pZPSevent,
                              &u8TransactionSequenceNumber,
                              asPayloadDefinition,
                              sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                              E_ZCL_ACCEPT_EXACT|E_ZCL_DISABLE_DEFAULT_RESPONSE);

    if(eStatus != E_ZCL_SUCCESS)
    {
        eZCL_SendDefaultResponse(pZPSevent, E_ZCL_CMDS_MALFORMED_COMMAND);
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        return eStatus;
    }

    psOTA_Common->sOTACallBackMessage.eEventId = E_CLD_OTA_COMMAND_UPGRADE_END_REQUEST;

    // fill in callback event structures
    eZCL_SetCustomCallBackEvent(&psOTA_Common->sOTACustomCallBackEvent, pZPSevent, u8TransactionSequenceNumber, psEndPointDefinition->u8EndPointNumber);

    psEndPointDefinition->pCallBackFunctions(&psOTA_Common->sOTACustomCallBackEvent);
    vOtaUpgManServerCommandHandler(pZPSevent,psOTA_Common, psEndPointDefinition);
    // release mutex
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif


    return E_ZCL_SUCCESS;
}
#endif
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
