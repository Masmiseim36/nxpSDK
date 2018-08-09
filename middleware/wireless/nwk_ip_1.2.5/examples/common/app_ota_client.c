/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

/*!=================================================================================================
\file       app_ota_client.c
\brief      This is a public source file for the ota client module
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
/* Framework */
#include "Panic.h"
#include "LED.h"

/* Application */
#include "sockets.h"
#include "session.h"
#include "app_stack_config.h"
#include "app_ota.h"
#include "OtaSupport.h"
#include "coap.h"
#include "thread_utils.h"
/*==================================================================================================
Private macros
==================================================================================================*/
/* The client port for image download socket */
#ifndef OTA_CLIENT_DEFAULT_PORT
    #define OTA_CLIENT_DEFAULT_PORT (61630)
#endif

/* Max. number of retreies until the process is stoped */
#ifndef gOtaClientMaxRetries_c
    #define gOtaClientMaxRetries_c                         0xA00
#endif

#ifndef gOtaClientValueMinForNextDiscoveryReqMs_c
    #define gOtaClientValueMinForNextDiscoveryReqMs_c      15000   /* 15 seconds */
#endif

#ifndef gOtaClientValueMaxForNextDiscoveryReqMs_c
    #define gOtaClientValueMaxForNextDiscoveryReqMs_c      30000   /* 30 seconds */
#endif

#ifndef gOtaMinClientValueForNextBlockReqMs_c
    #define gOtaMinClientValueForNextBlockReqMs_c          10     /*    10 ms */
#endif

#ifndef gOtaMaxClientValueForNextBlockReqMs_c
    #define gOtaMaxClientValueForNextBlockReqMs_c          40    /*    40 ms */
#endif

#ifndef gOtaMinClientValueForNextReqMs_c
    #define gOtaMinClientValueForNextReqMs_c               1000   /*   1 second */
#endif

#ifndef gOtaMaxClientValueForNextReqMs_c
    #define gOtaMaxClientValueForNextReqMs_c               2000   /*   1 seconds */
#endif

#ifndef gOtaClientQueryMinImgReqMs_c
    #define gOtaClientQueryMinImgReqMs_c                   1000   /*   1 second */
#endif

#ifndef gOtaClientQueryMaxImgReqMs_c
    #define gOtaClientQueryMaxImgReqMs_c                   15000   /*   1 second */
#endif

#ifndef gOtaClientDeclareErrorMs_c
    #define gOtaClientDeclareErrorMs_c                     3000     /* 3 seconds */
#endif

/* Wait timeout until the client switch to unicast mode for resynchronization */
#ifndef gOtaClientMulticastWaitMs_c
    #define gOtaClientMulticastWaitMs_c                    60000     /* 60 seconds */
#endif

#ifndef gOtaClientMulticastWaitJitterMs_c
    #define gOtaClientMulticastWaitJitterMs_c              10000     /* 10 seconds */
#endif
/* Enable or disable ota activity led */
#ifndef gOtaClientEnableLedActivity_c
    #define gOtaClientEnableLedActivity_c      1
#endif

/* Ota Multicast chunk bitmask length */
#ifndef gOtaClientChunkBitmaskLength_c
    #define gOtaClientChunkBitmaskLength_c      1000
#endif

/*==================================================================================================
Private type definitions
==================================================================================================*/
/* OTA client session state */
/* To define new client error states, use enums greater than gOtaClientState_Error_c. The enums 
   lower than gOtaClientState_Error_c are for normal OTA client application states */
typedef enum
{
    gOtaClientState_NotInit_c = 0,
    gOtaClientState_Idle_c,
    gOtaClientState_ServerDiscovery_c,
    gOtaClientState_QueryImage_c,
    gOtaClientState_BlockReq_c,
    gOtaClientState_Reboot_c,
    gOtaClientState_Error_c,
    gOtaClientState_ErrorOperation_c,
    gOtaClientState_ErrorParameter_c,
    gOtaClientState_ErrorFlash_c,
    gOtaClientState_ErrorImage_c
} otaClientSessionState_t;

typedef enum
{
    gOtaClientBlockReqState_Idle_c = 0,
    gOtaClientBlockReqState_StartDownload_c,
    gOtaClientBlockReqState_ProcessHeader_c,
    gOtaClientBlockReqState_UpgradeImg_c,
    gOtaClientBlockReqState_ProcessSubElement_c,
    gOtaClientBlockReqState_ValidateSignature_c,
    gOtaClientBlockReqState_GetBitmap_c,
    gOtaClientBlockReqState_ComputeImageSignature_c
} otaClientSessionBlockReqState_t;

typedef enum
{
    gOtaClientMulticastState_Idle_c = 0,
    gOtaClientMulticastState_ReceiveImage_c,
    gOtaClientMulticastState_SyncChunks_c
} otaClientSessionMulticastState_t;

typedef struct otaClientSignature_tag
{
    uint32_t crcValue;
} otaClientSignature_t;

/* ota client session format: */
typedef struct otaClientSession_tag
{
    uint32_t fileLength;
    uint32_t fileOffset;
    uint32_t downloadFileVersion;
    uint32_t bytesPerState;
    uint32_t remainingBytes;
    otaClientSessionState_t state;
    otaClientSessionBlockReqState_t blockReqState;
    otaClientSessionMulticastState_t multicastState;
    ipAddr_t serverIpAddr;
    uint16_t serverDownloadPort;
    uint16_t noOfRetries;
    uint8_t *pTempBuffer;
    uint8_t *pMulticastTempBuffer;
    uint8_t *pBitmap;
    otaTransferType_t transferType;
    uint32_t imageSize;
    uint16_t fragmentSize;
    uint8_t chunkBitmask[gOtaClientChunkBitmaskLength_c];
    uint16_t chunkToRxNo;
    uint16_t chunkMaxToRxNo;
    otaClientSignature_t signature;
} otaClientSession_t;

typedef struct otaClientSetup_tag
{
    instanceId_t thrInstId;
    uint8_t coapInstanceId;
    int32_t mOtaUdpSrvSockFd;
    tmrTimerID_t otaClientTmr;
} otaClientSetup_t;

typedef struct otaserverInfo_tag
{
    ipAddr_t remoteAddr;
    uint32_t dataLen;
    uint8_t  pData[1];
} otaServerInfo_t;

typedef enum
{
    gOtaLedSet_c,
    gOtaLedToggle_c,
    gOtaLedOff_c
} otaClientLedCmd_t;

/*==================================================================================================
Private function prototypes
==================================================================================================*/
/* OTA Client Coap callback*/
static void OtaClient_CoapCb(coapSessionStatus_t sessionStatus, void *pData,
                             coapSession_t *pSession, uint32_t dataLen);

/* OTA Client Udp Socket Callback*/
static void OtaClient_UdpServerService(void *pInData);

/* OTA Client Coap command handlers */
static void OtaClient_CoapMsgProcess(void *param);
static otaStatus_t OtaClient_ImageNotifyHandler(otaServerInfo_t *pOtaServerInfo);
static otaStatus_t OtaClient_QueryImageRspHandler(otaServerInfo_t *pOtaServerInfo);
static otaStatus_t OtaClient_BlockRspHandler(otaServerInfo_t *pOtaServerInfo);
static otaStatus_t OtaClient_UpgradeEndRspHandler(otaServerInfo_t *pOtaServerInfo);

/* OTA Client Coap commands */
static void OtaClient_CoapSendServerDiscovery(void *pParam);
static void OtaClient_CoapSendQueryImageReq(void *pParam);
static void OtaClient_SendBlockReq(void *pParam);
static void OtaClient_CoapSendUpgradeEndReq(void *pParam);
static nwkStatus_t OtaClient_CoapSendReq(coapSession_t *pSession, uint8_t *pData, uint32_t dataLen);

/* OTA Client utility functions */
static void OtaClient_ProcessBlock(void *pParam);
static void OtaClient_InitSignature(void);
static void OtaClient_ComputeSignature(uint8_t *pData, uint32_t dataLen);
static otaResult_t OtaClient_StartDownload(uint8_t *pData, uint32_t bytesToProcess, uint32_t *pBytesProcessed);
static otaResult_t OtaClient_ProcessHeader(uint8_t *pData, uint32_t bytesToProcess, uint32_t *pBytesProcessed);
static otaResult_t OtaClient_UpgradeImage(uint8_t *pData, uint32_t bytesToProcess, uint32_t *pBytesProcessed);
static otaResult_t OtaClient_ProcessSubElement(uint8_t *pData, uint32_t bytesToProcess, uint32_t *pBytesProcessed);
static otaResult_t OtaClient_GetBitmap(uint8_t *pData, uint32_t bytesToProcess, uint32_t *pBytesProcessed);
static otaResult_t OtaClient_ValidateSignature(uint8_t *pData, uint32_t bytesToProcess, uint32_t *pBytesProcessed);
static otaStatus_t OtaClient_HandleBlockSocket(bool_t onOff);
static otaStatus_t OtaClient_ComputeImageSignature(void);

/* OTA Client MCU Reset Callback for sleepy end devices*/
#if THR_DEFAULT_IS_POLLING_END_DEVICE
static void OtaClient_SleepyEndDeviceMCUResetCb(void * param);
#endif

/* OTA Client sync functions */
static void OtaClient_TimerCallback(void *pParam);
static void OtaClient_SendNextReq(void *pParam);
static otaStatus_t OtaClient_PrepareNextReq(uint8_t otaStatus, uint32_t timeout);
static void OtaClient_AbortSession(void);
static uint32_t OtaClient_GetJitter(void);
static void OtaClient_ErrorTimerCallback(void *pParam);
static void OtaClient_ErrorHandler(void *pParam);

/* OTA Client led activity function */
static void OtaClient_ActivityLed(otaClientLedCmd_t otaClientLedCmd);

/* OTA Client Multicast functions */
static void OtaClientMulticast_ProcessBlock(void *pParam);
static void OtaClientMulticast_TimerHandler(void *pParam);
static void OtaClientMulticast_ProcessImage(void *pParam);
/*==================================================================================================
Private global variables declarations
==================================================================================================*/
/* Pointer to task message queue */
static taskMsgQueue_t *mpOtaClientMsgQueue = NULL;

static otaClientSession_t mOtaClientSession = {.state = gOtaClientState_NotInit_c,
                                               .multicastState = gOtaClientMulticastState_Idle_c
                                              };

static otaClientSetup_t mOtaClientSetup = {.mOtaUdpSrvSockFd = -1, .otaClientTmr = gTmrInvalidTimerID_c};

static const bool_t bStart = TRUE;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
const coapUriPath_t gOTA_CLIENT_URI_PATH = {SizeOfString(OTA_CLIENT_URI_PATH), (uint8_t *)OTA_CLIENT_URI_PATH};
const coapUriPath_t gOTA_SERVER_URI_PATH = {SizeOfString(OTA_SERVER_URI_PATH), (uint8_t *)OTA_SERVER_URI_PATH};

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     otaStatus_t OtaClientInit(instanceId_t thrInstId, taskMsgQueue_t *pMsgQueue)
\brief  Initialize OTA client application.

\param  [in]    thrInstId      Thread Instance Id
\param  [in]    pMsgQueue      Pointer to task message queue

\return         otaStatus_t    Status of the operation
 ***************************************************************************************************/
otaStatus_t OtaClientInit
(
    instanceId_t thrInstId,
    taskMsgQueue_t *pMsgQueue
)
{
    otaStatus_t otaStatus = gOtaStatus_Success_c;

    if (gTmrInvalidTimerID_c == mOtaClientSetup.otaClientTmr)
    {
        mOtaClientSetup.otaClientTmr = TMR_AllocateTimer();

        if (gTmrInvalidTimerID_c == mOtaClientSetup.otaClientTmr)
        {
            panic(0, (uint32_t)OtaClientInit, 0, 0);
            return gOtaStatus_NoTimers_c;
        }
    }

    if (gOtaClientState_NotInit_c == mOtaClientSession.state)
    {
        mOtaClientSetup.thrInstId = thrInstId;
        coapRegCbParams_t cbParams[] =  {{OtaClient_CoapCb, (coapUriPath_t *) &gOTA_SERVER_URI_PATH}};
        coapStartUnsecParams_t coapParams = {COAP_DEFAULT_PORT, AF_INET6};

        /* Register Services in COAP */
        mOtaClientSetup.coapInstanceId = COAP_CreateInstance(NULL, &coapParams, gIpIfSlp0_c,
                                         (coapRegCbParams_t *)cbParams, NumberOfElements(cbParams));
        FLib_MemSet(&mOtaClientSession, 0, sizeof(otaClientSession_t));

        mOtaClientSession.state = gOtaClientState_Idle_c;
        mOtaClientSession.blockReqState = gOtaClientBlockReqState_Idle_c;
        mpOtaClientMsgQueue = pMsgQueue;
    }
    else
    {
        otaStatus = gOtaStatus_AlreadyStarted_c;
    }

    return otaStatus;
}

/*!*************************************************************************************************
\fn   otaStatus_t OtaClient_StartServerDiscovery(void *param)
\brief  This function is used to start Server Discovery procedure.

\param [in]   param          Not used

\return       otaStatus_t    Status of the operation
 ***************************************************************************************************/
otaStatus_t OtaClient_StartServerDiscovery
(
    void *param
)
{
    otaStatus_t otaStatus = gOtaStatus_Success_c;
    uint32_t delayMs = NWKU_GetRandomNoFromInterval(0, gOtaClientValueMinForNextDiscoveryReqMs_c);

    if ((gOtaClientState_Idle_c == mOtaClientSession.state) || (gOtaClientState_ServerDiscovery_c == mOtaClientSession.state))
    {
        if (IP_IsAddrEqual(&mOtaClientSession.serverIpAddr, &in6addr_any))
        {
            mOtaClientSession.state = gOtaClientState_ServerDiscovery_c;
        }
        else
        {
            mOtaClientSession.state = gOtaClientState_QueryImage_c;
        }

        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, delayMs, OtaClient_TimerCallback, NULL);
    }
    else
    {
        otaStatus = gOtaStatus_AlreadyStarted_c;
    }

    return otaStatus;
}

#if OTA_USE_NWK_DATA
/*!*************************************************************************************************
\fn     void OtaClient_SetServerIp6Addr(uint16_t otaServerShort)
\brief  This function sets the IPv6 (RLOC) address of the OTA server.
\param  [in]  otaServerShort    OTA server's short address.
***************************************************************************************************/
void OtaClient_SetServerIp6Addr
(
    uint16_t otaServerShort
)
{
    /* Check if ML prefix has been received */
    if (!IP_IsAddrEqual(&gpaThrActiveAttr[mOtaClientSetup.thrInstId]->MLprefix.prefix, &in6addr_any))
    {
        THR_BuildNodeIpAddr(mOtaClientSetup.thrInstId, gRLOCAddr_c, &otaServerShort, &mOtaClientSession.serverIpAddr);
    }
}
#endif /* OTA_USE_NWK_DATA */

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     static void void OtaClient_CoapCb(coapSessionStatus_t sessionStatus, void *pData,
                                          coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for CoAP message.

\param  [in]    sessionStatus    Status for CoAP session
\param  [in]    pData            Pointer to CoAP message payload
\param  [in]    pSession         Pointer to CoAP session
\param  [in]    dataLen          Length of CoAP payload
***************************************************************************************************/
static void OtaClient_CoapCb
(
    coapSessionStatus_t sessionStatus,
    void *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    if ((pData) && (sessionStatus == gCoapSuccess_c))
    {
        otaServerInfo_t *pOtaServerInf = MEM_BufferAlloc(sizeof(otaServerInfo_t) + dataLen);

        if (NULL != pOtaServerInf)
        {
            /* Save client info params */
            FLib_MemCpy(&pOtaServerInf->remoteAddr, &pSession->remoteAddr, sizeof(ipAddr_t));
            FLib_MemCpy(&pOtaServerInf->pData, pData, dataLen);
            pOtaServerInf->dataLen = dataLen;

            /* Send nwk msg to process client request */
            if (FALSE == NWKU_SendMsg(OtaClient_CoapMsgProcess, pOtaServerInf, mpOtaClientMsgQueue))
            {
                MEM_BufferFree(pOtaServerInf);
            }
        }
    }
}

/*!*************************************************************************************************
\fn     static void OtaClient_UdpServerService(void *pInData)
\brief  This function is the callback function for Ota client socket.

\param  [in]    pInData    Pointer to the received packet
***************************************************************************************************/
static void OtaClient_UdpServerService
(
    void *pInData
)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t *)pInData;
    uint8_t otaCommand = *(uint8_t *)pSessionPacket->pData;

    if (gOtaCmd_BlockRsp_c == otaCommand)
    {
        otaServerInfo_t *pOtaServerInf = MEM_BufferAlloc(sizeof(otaServerInfo_t) + pSessionPacket->dataLen);

        if (NULL != pOtaServerInf)
        {
            /* Save server info params */
            FLib_MemCpy(&pOtaServerInf->remoteAddr, &pSessionPacket->remAddr, sizeof(ipAddr_t));
            FLib_MemCpy(&pOtaServerInf->pData, pSessionPacket->pData, pSessionPacket->dataLen);
            pOtaServerInf->dataLen = pSessionPacket->dataLen;

            /* Drop packet if source address isn't from server */
            if (IP_IsAddrEqual(&mOtaClientSession.serverIpAddr, &pOtaServerInf->remoteAddr))
            {
                TMR_StopTimer(mOtaClientSetup.otaClientTmr);
                (void)OtaClient_BlockRspHandler(pOtaServerInf);
            }
            else
            {
                MEM_BufferFree(pOtaServerInf);
            }
        }
    }

    MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_CoapMsgProcess(void *param)
\brief  This function is the callback function for CoAP message.

\param  [in]    param    Pointer to pOtaServerInfo
***************************************************************************************************/
static void OtaClient_CoapMsgProcess
(
    void *param
)
{
    otaServerInfo_t *pOtaServerInf = (otaServerInfo_t *)param;
    uint8_t otaCommand = 0;
    otaStatus_t status = gOtaStatus_Failed_c;

    otaCommand = *(uint8_t *)pOtaServerInf->pData;
    TMR_StopTimer(mOtaClientSetup.otaClientTmr);

    /* Check if IP address is valid. Before the ImageNotify command comes from the server, the
        mOtaClientSession structure is not initialized with data from the server and the serverIpAddr
        will contain only 0's, namely it will be equal to in6addr_any. In this case, this structure
        needs to be initialized and the packet cannot be dropped.
        Also, check if client is not in an error state */
    if ((!(IP_IsAddrEqual(&mOtaClientSession.serverIpAddr, &pOtaServerInf->remoteAddr) ||
           IP_IsAddrEqual(&mOtaClientSession.serverIpAddr, &in6addr_any))) &&
           (mOtaClientSession.state >= gOtaClientState_Error_c))
    {
        MEM_BufferFree(pOtaServerInf);
        return;
    }

    switch (otaCommand)
    {
        case gOtaCmd_ImageNotify_c:
            status = OtaClient_ImageNotifyHandler(pOtaServerInf);
            break;

        case gOtaCmd_QueryImageRsp_c:
            status = OtaClient_QueryImageRspHandler(pOtaServerInf);
            break;

        case gOtaCmd_BlockRsp_c:
            status = OtaClient_BlockRspHandler(pOtaServerInf);
            break;

        case gOtaCmd_UpgradeEndRsp_c:
            status = OtaClient_UpgradeEndRspHandler(pOtaServerInf);
            break;

        default:
            MEM_BufferFree(pOtaServerInf);
            OtaClient_SendNextReq(NULL);
            break;
    }

    if(status != gOtaStatus_Success_c)
    {
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, gOtaClientDeclareErrorMs_c,
                                 OtaClient_ErrorTimerCallback, NULL);
    }
}

/*!*************************************************************************************************
\private
\fn     static otaStatus_t OtaClient_ImageNotifyHandler(otaServerInfo_t *pOtaServerInfo)
\brief  This function is used to process an Image Notify command.

\param  [in]    pOtaServerInfo    Pointer to OTA server information structure

\return         otaStatus_t       Status of the operation
***************************************************************************************************/
static otaStatus_t OtaClient_ImageNotifyHandler
(
    otaServerInfo_t *pOtaServerInfo
)
{
    otaStatus_t status = gOtaStatus_Failed_c;

    /* Validate session state */
    if ((mOtaClientSession.state == gOtaClientState_Idle_c) ||
            (mOtaClientSession.state == gOtaClientState_ServerDiscovery_c))
    {
        status = gOtaStatus_Success_c;

        /* Validate data length */
        if (pOtaServerInfo->dataLen >= sizeof(otaServerCmd_ImageNotify_t))
        {
            otaServerCmd_ImageNotify_t *pImgNotify = (otaServerCmd_ImageNotify_t *)pOtaServerInfo->pData;
            uint16_t manufacturerCode, imageType;
            uint32_t fileVersion;

            FLib_MemCpy(&manufacturerCode, pImgNotify->manufacturerCode, sizeof(manufacturerCode));
            FLib_MemCpy(&imageType, pImgNotify->imageType, sizeof(imageType));
            FLib_MemCpy(&fileVersion, pImgNotify->fileVersion, sizeof(fileVersion));

            /* Validate manufacturer code, image type and file version */
            if ((imageType == gOtaCurrentImageTypeNo_c) &&
                    (manufacturerCode == gOtaManufacturerCodeNo_c) &&
                    (fileVersion != gOtaCurrentFileVersionNo_c))
            {
                status = gOtaStatus_Success_c;
                mOtaClientSession.state = gOtaClientState_QueryImage_c;
                FLib_MemCpy(&mOtaClientSession.fileLength, &pImgNotify->fileSize, sizeof(uint32_t));
                FLib_MemCpy(&mOtaClientSession.serverIpAddr, &pOtaServerInfo->remoteAddr, sizeof(ipAddr_t));
                FLib_MemCpy(&mOtaClientSession.serverDownloadPort, &pImgNotify->serverDownloadPort[0], sizeof(uint16_t));
                mOtaClientSession.transferType = (otaTransferType_t)pImgNotify->transferType;
#if THR_DEFAULT_IS_POLLING_END_DEVICE
                /* Set RX on for SED during OTA upgrade */
                THR_SetPollingDevice(mOtaClientSetup.thrInstId, FALSE);
#endif

                /* For a multicast transfer - prepare to wait the ota image */
                if (gOtaMulticast_c == mOtaClientSession.transferType)
                {
                    bool_t bStartWaitTimer = TRUE;
                    otaResult_t ota_status = gOtaSucess_c;

                    FLib_MemCpy(&mOtaClientSession.fileLength, &pImgNotify->fileSize, sizeof(uint32_t));
                    FLib_MemCpy(&mOtaClientSession.imageSize, &pImgNotify->imageSize, sizeof(uint32_t));

                    /* This buffer is used for file elements that are not loaded in flash */
                    mOtaClientSession.pMulticastTempBuffer = MEM_BufferAllocForever(mOtaClientSession.fileLength - mOtaClientSession.imageSize, 0);
                    ota_status = OTA_StartImage(mOtaClientSession.imageSize);

                    if ((NULL != mOtaClientSession.pMulticastTempBuffer) &&
                            (gOtaStatus_Success_c == OtaClient_HandleBlockSocket(TRUE)) &&
                            (gOtaSucess_c == ota_status))
                    {
                        /* Prepare the first block request */
                        FLib_MemCpy(&mOtaClientSession.fragmentSize, &pImgNotify->fragmentSize, sizeof(uint16_t));
                        FLib_MemSet(&mOtaClientSession.chunkBitmask[0], 0, gOtaClientChunkBitmaskLength_c);
                        FLib_MemSet(mOtaClientSession.pMulticastTempBuffer, 0, mOtaClientSession.fileLength - mOtaClientSession.imageSize);
                        FLib_MemCpy(&mOtaClientSession.downloadFileVersion, &pImgNotify->fileVersion, sizeof(uint32_t));
                        mOtaClientSession.chunkMaxToRxNo = mOtaClientSession.fileLength / mOtaClientSession.fragmentSize;

                        if (mOtaClientSession.fileLength % mOtaClientSession.fragmentSize)
                        {
                            mOtaClientSession.chunkMaxToRxNo += 1;
                        }

                        mOtaClientSession.chunkToRxNo = mOtaClientSession.chunkMaxToRxNo;
                        mOtaClientSession.fileOffset = 0;
                        mOtaClientSession.state = gOtaClientState_BlockReq_c;
                        mOtaClientSession.blockReqState = gOtaClientBlockReqState_StartDownload_c;
                        mOtaClientSession.multicastState = gOtaClientMulticastState_ReceiveImage_c;
                        OtaClient_ActivityLed(gOtaLedSet_c);
                        OtaClientMulticast_TimerHandler(&bStartWaitTimer);
                    }
                    else
                    {
                        switch (ota_status)
                        {
                            case gOtaInvalidOperation_c:
                                mOtaClientSession.state = gOtaClientState_ErrorOperation_c;
                                break;

                            case gOtaExternalFlashError_c:
                                mOtaClientSession.state = gOtaClientState_ErrorFlash_c;
                                break;

                            case gOtaImageTooLarge_c:
                                mOtaClientSession.state = gOtaClientState_ErrorImage_c;
                                break;

                            default:
                                mOtaClientSession.state = gOtaClientState_Error_c;
                                break;
                        }

                        if (mOtaClientSession.pMulticastTempBuffer)
                        {
                            MEM_BufferFree(mOtaClientSession.pMulticastTempBuffer);
                            mOtaClientSession.pMulticastTempBuffer = NULL;
                        }

                        OTA_CancelImage();

                        if (ota_status != gOtaSucess_c)
                        {
                            status = gOtaStatus_Failed_c;
                        }
                    }
                }
                else if (gOtaUnicast_c == mOtaClientSession.transferType)
                {
                    OtaClient_SendNextReq(NULL);
                }
            }
        }
    }

    MSG_Free(pOtaServerInfo);

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaStatus_t OtaClient_QueryImageRspHandler(otaServerInfo_t *pOtaServerInfo)
\brief  This function is used to process an Query Image Rsp command.

\param  [in]    pOtaServerInfo    Pointer to OTA server information structure

\return         otaStatus_t       Status of the operation
***************************************************************************************************/
static otaStatus_t OtaClient_QueryImageRspHandler
(
    otaServerInfo_t *pOtaServerInfo
)
{
    otaStatus_t status = gOtaStatus_Failed_c;
    otaCmd_QueryImageRsp_t *pQueryImgRsp = (otaCmd_QueryImageRsp_t *)pOtaServerInfo->pData;

    /* Validate session state */
    if (mOtaClientSession.state != gOtaClientState_QueryImage_c)
    {
        /* Ignore the message */
        MSG_Free(pOtaServerInfo);
        return gOtaStatus_NotPermitted_c;
    }

    /* Validate status, manufacturer code, image type */
    if (pQueryImgRsp->status == gOtaFileStatus_Success_c)
    {
        uint16_t imageType, manufacturerCode;

        FLib_MemCpy(&imageType, &pQueryImgRsp->data.success.imageType[0], sizeof(uint16_t));
        FLib_MemCpy(&manufacturerCode, &pQueryImgRsp->data.success.manufacturerCode[0], sizeof(uint16_t));

        if ((imageType == gOtaCurrentImageTypeNo_c) &&
                (manufacturerCode == gOtaManufacturerCodeNo_c))
        {
            if (gOtaStatus_Success_c == OtaClient_HandleBlockSocket(TRUE))
            {
                /* Prepare the first block request */
                mOtaClientSession.fileOffset = 0;
                FLib_MemCpy(&mOtaClientSession.downloadFileVersion, pQueryImgRsp->data.success.fileVersion, sizeof(uint32_t));
                FLib_MemCpy(&mOtaClientSession.fileLength, pQueryImgRsp->data.success.fileSize, sizeof(uint32_t));
                mOtaClientSession.state = gOtaClientState_BlockReq_c;
                mOtaClientSession.blockReqState = gOtaClientBlockReqState_StartDownload_c;
                FLib_MemCpy(&mOtaClientSession.serverIpAddr, &pOtaServerInfo->remoteAddr, sizeof(ipAddr_t));
                FLib_MemCpy(&mOtaClientSession.serverDownloadPort, &pQueryImgRsp->data.success.serverDownloadPort[0], sizeof(uint16_t));
                OtaClient_SendNextReq(NULL);
                OtaClient_ActivityLed(gOtaLedSet_c);
                status = gOtaStatus_Success_c;
            }
            else
            {
                status = OtaClient_PrepareNextReq(pQueryImgRsp->status, gOtaClientValueMaxForNextDiscoveryReqMs_c);
            }
        }
    }
    else
    {
        uint32_t requestTime = 0, currentTime = 0;

        if (pQueryImgRsp->status == gOtaFileStatus_ServerBusy_c)
        {
            mOtaClientSession.noOfRetries = 0;
            FLib_MemCpy(&requestTime, pQueryImgRsp->data.wait.requestTime, sizeof(uint32_t));
            FLib_MemCpy(&currentTime, pQueryImgRsp->data.wait.currentTime, sizeof(uint32_t));
        }

        /* Status: gOtaFileStatus_NoImageAvailable_c, gOtaFileStatus_Abort_c,  gOtaFileStatus_WaitForData_c */
        status = OtaClient_PrepareNextReq(pQueryImgRsp->status, requestTime - currentTime);
    }

    MSG_Free(pOtaServerInfo);

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaStatus_t OtaClient_BlockRspHandler(otaServerInfo_t *pOtaServerInfo)
\brief  This function is used to process a BlockRsp command.

\param  [in]    pOtaServerInfo    Pointer to OTA server information structure

\return         otaStatus_t       Status of the operation
***************************************************************************************************/
static otaStatus_t OtaClient_BlockRspHandler
(
    otaServerInfo_t *pOtaServerInfo
)
{
    otaStatus_t status = gOtaStatus_Success_c;
    otaCmd_BlockRsp_t *pBlockRsp = (otaCmd_BlockRsp_t *)pOtaServerInfo->pData;
    uint32_t receivedOffset = 0;

    /* Protection just for coap messages */
    if (gOtaClientState_BlockReq_c != mOtaClientSession.state)
    {
        mOtaClientSession.noOfRetries++;
        status = gOtaStatus_InvalidParam_c;
    }
    else
    {
        if (pBlockRsp->status != gOtaFileStatus_Success_c)
        {
            uint32_t requestTime = 0, currentTime = 0;

            mOtaClientSession.noOfRetries++;

            if (pBlockRsp->status == gOtaFileStatus_ServerBusy_c)
            {
                mOtaClientSession.noOfRetries = 0;
                FLib_MemCpy(&requestTime, pBlockRsp->data.wait.requestTime, sizeof(uint32_t));
                FLib_MemCpy(&currentTime, pBlockRsp->data.wait.currentTime, sizeof(uint32_t));
            }

            if (mOtaClientSession.noOfRetries <= gOtaClientMaxRetries_c)
            {
                /* status: gOtaFileStatus_NoImageAvailable_c, gOtaFileStatus_Abort_c,  gOtaFileStatus_ServerBusy_c */
                status = OtaClient_PrepareNextReq(pBlockRsp->status, (requestTime - currentTime));
            }
        }
        else
        {
            uint32_t fileVersion = 0;

            /* Get received offset */
            FLib_MemCpy(&receivedOffset, pBlockRsp->data.success.fileOffset, sizeof(uint32_t));
            /* Get received fileVersion */
            FLib_MemCpy(&fileVersion, pBlockRsp->data.success.fileVersion, sizeof(uint32_t));

            if (fileVersion != mOtaClientSession.downloadFileVersion)
            {
                mOtaClientSession.noOfRetries++;
                status = gOtaStatus_InvalidParam_c;
            }

            /* Process unicast transfer */
            if (((gOtaUnicast_c == mOtaClientSession.transferType) ||
                    ((gOtaMulticast_c == mOtaClientSession.transferType) &&
                     (gOtaClientMulticastState_SyncChunks_c == mOtaClientSession.multicastState))) &&
                     (status == gOtaStatus_Success_c))
            {
                if ((((receivedOffset != mOtaClientSession.fileOffset) ||
                        (pBlockRsp->data.success.dataSize > gOtaMaxBlockDataSize_c) ||
                        (pBlockRsp->data.success.dataSize + mOtaClientSession.fileOffset > mOtaClientSession.fileLength))))
                {
                    mOtaClientSession.noOfRetries++;
                    status = gOtaStatus_InvalidParam_c;
                }
            }

            if (status == gOtaStatus_Success_c)
            {
                bool_t bstatus = FALSE;

                if (gOtaMulticast_c == mOtaClientSession.transferType)
                {
                    bstatus = NWKU_SendMsg(OtaClientMulticast_ProcessBlock, pOtaServerInfo, mpOtaClientMsgQueue);
                }
                else
                {
                    bstatus = NWKU_SendMsg(OtaClient_ProcessBlock, pOtaServerInfo, mpOtaClientMsgQueue);
                }

                if (bstatus == TRUE)
                {
                    pOtaServerInfo = NULL;
                    OtaClient_ActivityLed(gOtaLedToggle_c);
                    mOtaClientSession.noOfRetries = 0;
                }
                else
                {
                    mOtaClientSession.noOfRetries++;
                    status = gOtaStatus_Failed_c;
                }
            }
        }
    }

    if (mOtaClientSession.noOfRetries > gOtaClientMaxRetries_c)
    {
        (void)NWKU_SendMsg(OtaClient_CoapSendUpgradeEndReq, NULL, mpOtaClientMsgQueue);
        status = gOtaStatus_Failed_c;
    }

    if (status != gOtaStatus_Success_c)
    {
        OtaClient_SendNextReq(NULL);
    }

    if (NULL != pOtaServerInfo)
    {
        MSG_Free(pOtaServerInfo);
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaStatus_t OtaClient_UpgradeEndRspHandler(coapSession_t *pOtaServerInfo)
\brief  This function is used to process an UpgradeEndRsp command.

\param  [in]    pOtaServerInfo    Pointer to OTA server information structure

\return         otaStatus_t       Status of the operation
***************************************************************************************************/
static otaStatus_t OtaClient_UpgradeEndRspHandler
(
    otaServerInfo_t *pOtaServerInfo
)
{
    otaCmd_UpgradeEndRsp_t *pUpgradeEndRsp;
    otaStatus_t status = gOtaStatus_Success_c;

    pUpgradeEndRsp = (otaCmd_UpgradeEndRsp_t *)pOtaServerInfo->pData;

    /* When in multicast mode, an Upgrade End response with abort status may be sent by the server asynchronously,
    and the mOtaClientSession.state may not be in gOtaClientState_Reboot_c state but in gOtaClientState_BlockReq_c for example */
    if (pUpgradeEndRsp->status == gOtaFileStatus_Abort_c)
    {
        OtaClient_AbortSession();
        MSG_Free(pOtaServerInfo);
        return gOtaStatus_Failed_c;
    }

    if (mOtaClientSession.state != gOtaClientState_Reboot_c)
    {
        /* Ignore the message */
        OtaClient_SendNextReq(NULL);
        status = gOtaStatus_InvalidParam_c;
    }
    else
    {
        if (pUpgradeEndRsp->status != gOtaFileStatus_Success_c)
        {
            uint32_t requestTime = 0, currentTime = 0;

            mOtaClientSession.noOfRetries++;

            if (pUpgradeEndRsp->status == gOtaFileStatus_ServerBusy_c)
            {
                mOtaClientSession.noOfRetries = 0;
                FLib_MemCpy(&requestTime, pUpgradeEndRsp->data.wait.requestTime, sizeof(uint32_t));
                FLib_MemCpy(&currentTime, pUpgradeEndRsp->data.wait.currentTime, sizeof(uint32_t));
            }

            if (mOtaClientSession.noOfRetries <= gOtaClientMaxRetries_c)
            {
                /* Status: gOtaFileStatus_NoImageAvailable_c, gOtaFileStatus_Abort_c,  gOtaFileStatus_ServerBusy_c */
                status = OtaClient_PrepareNextReq(pUpgradeEndRsp->status, (requestTime - currentTime));
            }
        }
        else
        {
            uint32_t upgradeTime = 0, currentTime = 0;

            FLib_MemCpy(&upgradeTime, pUpgradeEndRsp->data.success.upgradeTime, sizeof(uint32_t));
            FLib_MemCpy(&currentTime, pUpgradeEndRsp->data.success.currentTime, sizeof(uint32_t));
            /* Set new image flag */
            OTA_SetNewImageFlag();

            /* Reset MCU to upgrade the new image  */
            /* THR_TimeoutResetMcu function relies on a timer that is deactivated when the MCU enters low power which leads to incorrect timing in this situation.
                Therefore in the case of LPEDs we need to start a low power timer, that stays activated even when the MCU enters low power state,
                and reset the MCU when this timer expires. */
#if THR_DEFAULT_IS_POLLING_END_DEVICE
                TMR_StartTimer(mOtaClientSetup.otaClientTmr, gTmrLowPowerSingleShotMillisTimer_c, (upgradeTime - currentTime),
                   OtaClient_SleepyEndDeviceMCUResetCb, NULL); 
#else
                THR_TimeoutResetMcu((upgradeTime - currentTime), FALSE);
#endif
        }
    }

#if THR_DEFAULT_IS_POLLING_END_DEVICE
    /* Set RX off for SED at the end of OTA upgrade */
    THR_SetPollingDevice(mOtaClientSetup.thrInstId, TRUE);
#endif
    MSG_Free(pOtaServerInfo);

    return status;
}

/*!*************************************************************************************************
\private
\fn     static nwkStatus_t OtaClient_CoapSendReq(coapSession_t *pSession, uint8_t *pData, uint32_t dataLen)
\brief  This function is used to send a ota coap request to the OTA server node.

\param  [in]   pSession       Pointer to CoAP session
\param  [in]   pData          Pointer to data
\param  [in]   dataLen        Payload length

\return        nwkStatus_t    Status of the operation
 ***************************************************************************************************/
static nwkStatus_t OtaClient_CoapSendReq
(
    coapSession_t *pSession,
    uint8_t *pData,
    uint32_t dataLen
)
{
    nwkStatus_t status = gNwkStatusFail_c;

    if (pSession)
    {
        pSession->ipIfId = gIpIfSlp0_c;
        pSession->pCallback = NULL;
        FLib_MemCpy(&pSession->remoteAddr, &mOtaClientSession.serverIpAddr, sizeof(ipAddr_t));
        (void)THR_GetIP6Addr(0, OTA_ADDR_TYPE, &pSession->sourceAddr, NULL);
        pSession->msgType = gCoapNonConfirmable_c;
        pSession->code = gCoapGET_c;
        COAP_SetUriPath(pSession, (coapUriPath_t *)&gOTA_CLIENT_URI_PATH);

        /* send coap message */
        status = COAP_SendMsg(pSession, pData, dataLen);
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_CoapSendQueryImageReq(void *pParam)
\brief  This function is used to send a Query Image req command to the OTA server node.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_CoapSendQueryImageReq
(
    void *pParam
)
{
    coapSession_t *pSession = NULL;

    /* Used for delaying the command */
    if (pParam)
    {
        pSession = COAP_OpenSession(mOtaClientSetup.coapInstanceId);
    }

    if (pSession)
    {
        otaCmd_QueryImageReq_t queryImageReq = {gOtaCmd_QueryImageReq_c, gOtaManufacturerCode_c, gOtaCurrentImageType_c,
                                                gOtaCurrentFileVersion_c, gOtaHardwareVersion_c
                                               };

        OtaClient_CoapSendReq(pSession, (uint8_t *)&queryImageReq, sizeof(otaCmd_QueryImageReq_t));
    }
    else
    {
        /* Start timer and wait for the response */
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, OtaClient_GetJitter(),
                                 OtaClient_TimerCallback, (void *)&bStart);
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_SendBlockReq(void *pParam)
\brief  This function is used to send a Block Req command to the OTA server node.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_SendBlockReq
(
    void *pParam
)
{
    /* Used for delaying the command */
    if ((pParam) && (-1 != mOtaClientSetup.mOtaUdpSrvSockFd))
    {
        sockaddrStorage_t portAddr;
        otaCmd_BlockReq_t blockReq = {gOtaCmd_BlockReq_c, gOtaManufacturerCode_c, gOtaCurrentImageType_c,
                                      gOtaCurrentFileVersion_c, {0x00, 0x00, 0x00, 0x00}, gOtaMaxBlockDataSize_c
                                     };

        FLib_MemCpy(&blockReq.fileVersion, &mOtaClientSession.downloadFileVersion, sizeof(uint32_t));
        /* Set remote address and local port */
        FLib_MemSet(&portAddr, 0, sizeof(sockaddrStorage_t));
        ((sockaddrIn6_t *)&portAddr)->sin6_family = AF_INET6;
        ((sockaddrIn6_t *)&portAddr)->sin6_port = mOtaClientSession.serverDownloadPort;
        IP_AddrCopy(&((sockaddrIn6_t *)&portAddr)->sin6_addr, &mOtaClientSession.serverIpAddr);
        ((sockaddrIn6_t *)&portAddr)->sin6_flowinfo = BSDS_SET_TX_SEC_FLAGS(1, 5);
        ((sockaddrIn6_t *)&portAddr)->sin6_scope_id = gIpIfSlp0_c;

        if (mOtaClientSession.transferType == gOtaMulticast_c)
        {
            blockReq.maxDataSize = mOtaClientSession.fragmentSize;
        }

        FLib_MemCpy(&blockReq.fileOffset, &mOtaClientSession.fileOffset, sizeof(uint32_t));
        (void)sendto(mOtaClientSetup.mOtaUdpSrvSockFd, (void *)&blockReq, sizeof(otaCmd_BlockReq_t), 0,
                     &portAddr, sizeof(portAddr));
    }
    else
    {
#if THR_DEFAULT_IS_POLLING_END_DEVICE
        /* turn rx on for SED */
        THR_SetPollingDevice(mOtaClientSetup.thrInstId, FALSE);
#endif
        /* Start timer and wait for the response */
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, OtaClient_GetJitter(),
                                 OtaClient_TimerCallback, (void *)&bStart);
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_CoapSendUpgradeEndReq(void *pParam)
\brief  This function is used to send a Upgrade End Req command to the OTA server node.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_CoapSendUpgradeEndReq
(
    void *pParam
)
{
    coapSession_t *pSession = NULL;

    /* Used for delaying the command */
    if (pParam)
    {
        pSession = COAP_OpenSession(mOtaClientSetup.coapInstanceId);
    }

    if (pSession)
    {
        otaCmd_UpgradeEndReq_t upgradeEndReq = {gOtaCmd_UpgradeEndReq_c, gOtaFileStatus_Abort_c,
                                                gOtaManufacturerCode_c, gOtaCurrentImageType_c,
                                                {0x00, 0x00, 0x00, 0x00}
                                               };

        htonal(upgradeEndReq.fileVersion, mOtaClientSession.downloadFileVersion);

        if (mOtaClientSession.state < gOtaClientState_Error_c)
        {
            switch (mOtaClientSession.state)
            {
                case gOtaClientState_Reboot_c:
                    upgradeEndReq.status = gOtaFileStatus_Success_c;
                    break;

                case gOtaClientState_BlockReq_c:
                    if(mOtaClientSession.blockReqState == gOtaClientBlockReqState_ValidateSignature_c)
                    {
                        upgradeEndReq.status = gOtaFileStatus_InvalidImage_c;
                        mOtaClientSession.state = gOtaClientState_Idle_c;
                    }
                    break;

                default:
                    break;
            }

            OtaClient_CoapSendReq(pSession, (uint8_t *)&upgradeEndReq, sizeof(otaCmd_UpgradeEndReq_t));
        }
        else
        {
            /* Set the error message for the server */
            switch (mOtaClientSession.state)
            {
                case gOtaClientState_ErrorOperation_c:
                    upgradeEndReq.status = gOtaFileStatus_InvalidOperation_c;
                    break;

                case gOtaClientState_ErrorParameter_c:
                    upgradeEndReq.status = gOtaFileStatus_InvalidParameter_c;
                    break;

                case gOtaClientState_ErrorFlash_c:
                    upgradeEndReq.status = gOtaFileStatus_ExtFlashError_c;
                    break;

                case gOtaClientState_ErrorImage_c:
                    upgradeEndReq.status = gOtaFileStatus_ImageTooLarge_c;
                    break;

                case gOtaClientState_Error_c:
                    upgradeEndReq.status = gOtaFileStatus_ClientError_c;
                    break;

                default:
                    break;
            }

            OtaClient_CoapSendReq(pSession, (uint8_t *)&upgradeEndReq, sizeof(otaCmd_UpgradeEndReq_t));
            OtaClient_AbortSession();
        }
    }
    else
    {
        /* Start timer and wait for the response */
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, OtaClient_GetJitter(),
                                 OtaClient_TimerCallback, (void *)&bStart);
    }
}

/*!*************************************************************************************************
\private
\fn     void OtaClient_CoapSendServerDiscovery(void *pParam)
\brief  This function is used to send a Server Discovery command to the all thread nodes.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_CoapSendServerDiscovery
(
    void *pParam
)
{
    coapSession_t *pSession = COAP_OpenSession(mOtaClientSetup.coapInstanceId);

    if (pSession)
    {
        otaCmd_ServerDiscovery_t serverDiscoveryCmd = {gOtaCmd_ServerDiscovery_c, gOtaManufacturerCode_c, gOtaCurrentImageType_c};

        pSession->ipIfId = gIpIfSlp0_c;
        pSession->pCallback = NULL;
        FLib_MemCpy(&pSession->remoteAddr, &in6addr_realmlocal_allthreadnodes, sizeof(ipAddr_t));
        COAP_SetUriPath(pSession, (coapUriPath_t *)&gOTA_CLIENT_URI_PATH);
        (void)THR_GetIP6Addr(0, OTA_ADDR_TYPE, &pSession->sourceAddr, NULL);
        pSession->msgType = gCoapNonConfirmable_c;
        pSession->code = gCoapGET_c;
        /* Send command */
        (void)COAP_Send(pSession, gCoapMsgTypeNonPost_c, &serverDiscoveryCmd, sizeof(otaCmd_ServerDiscovery_t));
    }

    (void)pParam;
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_ProcessBlock(void *pParam)
\brief  This function is used to process block received.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_ProcessBlock
(
    void *pParam
)
{
    otaResult_t status = gOtaSucess_c;
    otaCmd_BlockRsp_t *pBlockRsp = (otaCmd_BlockRsp_t *)((otaServerInfo_t *)pParam)->pData;
    uint32_t bytesToProcess = pBlockRsp->data.success.dataSize;
    uint8_t *pData = pBlockRsp->data.success.pData;
    uint32_t bytesProcessed = 0;

    while ((bytesToProcess) && (status == gOtaSucess_c))
    {
        switch (mOtaClientSession.blockReqState)
        {
            case gOtaClientBlockReqState_StartDownload_c:
                status = OtaClient_StartDownload(pData, bytesToProcess, &bytesProcessed);
                break;

            case gOtaClientBlockReqState_ProcessHeader_c:
                status = OtaClient_ProcessHeader(pData, bytesToProcess, &bytesProcessed);
                break;

            case gOtaClientBlockReqState_UpgradeImg_c:
                status = OtaClient_UpgradeImage(pData, bytesToProcess, &bytesProcessed);
                break;

            case gOtaClientBlockReqState_ProcessSubElement_c:
                status = OtaClient_ProcessSubElement(pData, bytesToProcess, &bytesProcessed);
                break;

            case gOtaClientBlockReqState_GetBitmap_c:
                status = OtaClient_GetBitmap(pData, bytesToProcess, &bytesProcessed);
                break;

            case gOtaClientBlockReqState_ValidateSignature_c:
                status = OtaClient_ValidateSignature(pData, bytesToProcess, &bytesProcessed);
                break;

            default:
                break;
        }

        if (status == gOtaSucess_c)
        {
            pData += bytesProcessed;
            bytesToProcess -= bytesProcessed;
            mOtaClientSession.fileOffset += bytesProcessed;
        }
        else
        {
            /* User can define particular behaviors for the different errors (gOtaNoImage_c, gOtaError_c, gOtaImageTooLarge_c, etc.).
               Based on these states, the client can send specific messages to the server. */
            switch (status)
            {
                case gOtaInvalidOperation_c:
                    mOtaClientSession.state = gOtaClientState_ErrorOperation_c;
                    break;

                case gOtaInvalidParam_c:
                    mOtaClientSession.state = gOtaClientState_ErrorParameter_c;
                    break;

                case gOtaExternalFlashError_c:
                    mOtaClientSession.state = gOtaClientState_ErrorFlash_c;
                    break;

                case gOtaImageTooLarge_c:
                    mOtaClientSession.state = gOtaClientState_ErrorImage_c;
                    break;

                case gOtaError_c:
                default:
                    mOtaClientSession.state = gOtaClientState_Error_c;
                    break;
            }
        }
    }

    MEM_BufferFree(pParam);

    if (status != gOtaSucess_c)
    {
        (void)NWKU_SendMsg(OtaClient_CoapSendUpgradeEndReq, NULL, mpOtaClientMsgQueue);
    }
    else
    {
        OtaClient_SendNextReq(NULL);
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClientMulticast_ProcessImage(void *pParam)
\brief  This function is used to process a multicast received image.

\param  [in]   pParam    Not used
 ***************************************************************************************************/
static void OtaClientMulticast_ProcessImage
(
    void *pParam
)
{
    otaResult_t status = gOtaSucess_c;
    uint32_t bytesToProcess = sizeof(otaFileHeader_t);
    uint8_t *pData = mOtaClientSession.pMulticastTempBuffer;
    uint32_t bytesProcessed = 0;

    while ((bytesToProcess) && (status == gOtaSucess_c))
    {
        switch (mOtaClientSession.blockReqState)
        {
            case gOtaClientBlockReqState_StartDownload_c:
                OtaClient_InitSignature();
                mOtaClientSession.blockReqState = gOtaClientBlockReqState_ProcessHeader_c;
                FLib_MemCpy(&mOtaClientSession.bytesPerState, pData + sizeof(uint32_t) + sizeof(uint16_t), sizeof(uint16_t));
                break;

            case gOtaClientBlockReqState_ProcessHeader_c:
                status = OtaClient_ProcessHeader(pData, bytesToProcess, &bytesProcessed);
                OtaClient_ComputeSignature((uint8_t *)mOtaClientSession.pMulticastTempBuffer + sizeof(otaFileHeader_t), sizeof(otaFileSubElement_t));
                pData = mOtaClientSession.pMulticastTempBuffer + sizeof(otaFileHeader_t) + sizeof(otaFileSubElement_t);
                bytesProcessed = 0;
                bytesToProcess = mOtaClientSession.fileLength - mOtaClientSession.imageSize - sizeof(otaFileHeader_t) - sizeof(otaFileSubElement_t);
                mOtaClientSession.blockReqState = gOtaClientBlockReqState_ComputeImageSignature_c;
                break;

            case gOtaClientBlockReqState_ComputeImageSignature_c:
                OtaClient_ComputeImageSignature();
                mOtaClientSession.blockReqState = gOtaClientBlockReqState_ProcessSubElement_c;
                break;

            case gOtaClientBlockReqState_ProcessSubElement_c:
                status = OtaClient_ProcessSubElement(pData, bytesToProcess, &bytesProcessed);
                break;

            case gOtaClientBlockReqState_GetBitmap_c:
                status = OtaClient_GetBitmap(pData, bytesToProcess, &bytesProcessed);
                break;

            case gOtaClientBlockReqState_ValidateSignature_c:
                status = OtaClient_ValidateSignature(pData, bytesToProcess, &bytesProcessed);
                break;

            default:
                break;
        }

        if (status == gOtaSucess_c)
        {
            pData += bytesProcessed;
            bytesToProcess -= bytesProcessed;
        }
    }

    MEM_BufferFree(mOtaClientSession.pMulticastTempBuffer);
    mOtaClientSession.pMulticastTempBuffer = NULL;

    if (status != gOtaSucess_c)
    {
        (void)NWKU_SendMsg(OtaClient_CoapSendUpgradeEndReq, NULL, mpOtaClientMsgQueue);
    }
    else
    {
        OtaClient_SendNextReq(NULL);
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClientMulticast_ProcessBlock(void *pParam)
\brief  This function is used to process multicast block received.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClientMulticast_ProcessBlock
(
    void *pParam
)
{
    otaCmd_BlockRsp_t *pBlockRsp = (otaCmd_BlockRsp_t *)((otaServerInfo_t *)pParam)->pData;
    uint8_t *pData = pBlockRsp->data.success.pData;
    uint8_t *pDataFlash = NULL;
    uint16_t bitNumber = 0, dataFlashLen = 0;
    uint32_t fileOffset = 0;
    uint32_t bytesToProcess = pBlockRsp->data.success.dataSize;
    uint32_t startImageLength = sizeof(otaFileHeader_t) + sizeof(otaFileSubElement_t);
    uint32_t endImageLength = startImageLength + mOtaClientSession.imageSize;

    FLib_MemCpy(&fileOffset, pBlockRsp->data.success.fileOffset, sizeof(uint32_t));
    bitNumber = fileOffset / mOtaClientSession.fragmentSize;

    if ((NWKU_GetBit(bitNumber, mOtaClientSession.chunkBitmask)) &&
            (mOtaClientSession.multicastState == gOtaClientMulticastState_ReceiveImage_c))
    {
        /* Already received this chunk */
        bool_t bStartWaitTimer = TRUE;

        MEM_BufferFree(pParam);
        OtaClientMulticast_TimerHandler(&bStartWaitTimer);
        return;
    }

    /* Check where to place the chunk */
    while ((fileOffset < startImageLength) && (bytesToProcess))
    {
        *(mOtaClientSession.pMulticastTempBuffer + fileOffset) = *pData;
        pData++;
        bytesToProcess--;
        fileOffset++;
    }

    if (bytesToProcess)
    {
        if ((fileOffset + bytesToProcess) < endImageLength)
        {
            pDataFlash = pData;
            dataFlashLen = bytesToProcess;
            pData = NULL;
            bytesToProcess = 0;
        }
        else if (fileOffset < endImageLength)
        {
            dataFlashLen = endImageLength - fileOffset;
            pDataFlash = pData;
            pData = pData + dataFlashLen;
            bytesToProcess -= dataFlashLen;
        }
    }
    else
    {
        pData = NULL;
    }

    /* Write in external flash */
    if (pDataFlash)
    {
        uint32_t flashFileOffset = fileOffset - startImageLength;
        otaResult_t otaResult = OTA_PushImageChunk(pDataFlash, dataFlashLen, NULL, &flashFileOffset);

        if (gOtaSucess_c != otaResult)
        {
            /* Stop here */
            panic(0, (uint32_t)OtaClientMulticast_ProcessBlock, 0, 0);
        }

        fileOffset += dataFlashLen;
    }

    /* Write in temp. buffer */
    if (pData)
    {
        uint32_t tempFileOffset = fileOffset - mOtaClientSession.imageSize;

        FLib_MemCpy(mOtaClientSession.pMulticastTempBuffer + tempFileOffset, pData, bytesToProcess);
    }

    /* Finished with this buffer */
    MEM_BufferFree(pParam);

    NWKU_SetBit(bitNumber, mOtaClientSession.chunkBitmask);
    mOtaClientSession.chunkToRxNo--;

    if (mOtaClientSession.chunkToRxNo)
    {
        /* Wait for chunks or make a request*/
        if (gOtaClientMulticastState_SyncChunks_c == mOtaClientSession.multicastState)
        {
            /* Get next offset */
            bitNumber = mOtaClientSession.fileOffset / mOtaClientSession.fragmentSize;

            bitNumber = NWKU_GetFirstBitValueInRange(mOtaClientSession.chunkBitmask,
                        bitNumber, mOtaClientSession.chunkMaxToRxNo, 0);
            mOtaClientSession.fileOffset = bitNumber * mOtaClientSession.fragmentSize;
            OtaClient_SendNextReq(NULL);
        }
        else
        {
            /* Wait for next chunk */
            bool_t bStartWaitTimer = TRUE;

            OtaClientMulticast_TimerHandler(&bStartWaitTimer);
        }
    }
    else
    {
        mOtaClientSession.multicastState =  gOtaClientMulticastState_SyncChunks_c;
        /* Received all the imagefile */
        (void)NWKU_SendMsg(OtaClientMulticast_ProcessImage, NULL, mpOtaClientMsgQueue);
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_InitSignature(void)
\brief  This function is used to initialize image signature.
 ***************************************************************************************************/
static void OtaClient_InitSignature
(
    void
)
{
    mOtaClientSession.signature.crcValue = 0;
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_ComputeSignature(uint8_t *pData, uint32_t dataLen)
\brief  This function is used to compute image signature based on current block received.

\param  [in]   pData      Pointer to data
\param  [in]   dataLen    Data length
 ***************************************************************************************************/
static void OtaClient_ComputeSignature
(
    uint8_t *pData,
    uint32_t dataLen
)
{
    mOtaClientSession.signature.crcValue = OTA_CrcCompute(pData, dataLen, mOtaClientSession.signature.crcValue);
}

/*!*************************************************************************************************
\private
\fn     static otaResult_t OtaClient_StartDownload(uint8_t *pData, uint32_t bytesToProcess,
                                                   uint32_t *pBytesProcessed)
\brief  This function is used to process start download state.

\param  [in]   pData              Pointer to data
\param  [in]   bytesToProcess     Number of bytes to process
\param  [out]  pBytesProcessed    Number of bytes processed

\return        otaResult_t        Result of the operation
 ***************************************************************************************************/
static otaResult_t OtaClient_StartDownload
(
    uint8_t *pData,
    uint32_t bytesToProcess,
    uint32_t *pBytesProcessed
)
{
    otaResult_t status = gOtaSucess_c;

    pData = pData + sizeof(uint32_t) + sizeof(uint16_t);
    OtaClient_InitSignature();

    /* The next sub state is process header */
    mOtaClientSession.state = gOtaClientState_BlockReq_c;
    mOtaClientSession.blockReqState = gOtaClientBlockReqState_ProcessHeader_c;
    FLib_MemCpy(&mOtaClientSession.bytesPerState, pData, sizeof(uint16_t));
    mOtaClientSession.remainingBytes = mOtaClientSession.bytesPerState;
    *pBytesProcessed = 0;

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaResult_t OtaClient_ProcessHeader(uint8_t *pData, uint32_t bytesToProcess,
                                                   uint32_t *pBytesProcessed)
\brief  This function is used to process header file.

\param  [in]   pData              Pointer to data
\param  [in]   bytesToProcess     Number of bytes to process
\param  [out]  pBytesProcessed    Number of bytes processed

\return        otaResult_t        Result of the operation
 ***************************************************************************************************/
static otaResult_t OtaClient_ProcessHeader
(
    uint8_t *pData,
    uint32_t bytesToProcess,
    uint32_t *pBytesProcessed
)
{
    otaResult_t status = gOtaError_c;
    otaFileHeader_t *pHeader = NULL;
    bool_t processData = FALSE;

    if (!mOtaClientSession.pTempBuffer)
    {
        if (mOtaClientSession.bytesPerState <= bytesToProcess)
        {
            processData = TRUE;
            *pBytesProcessed = mOtaClientSession.bytesPerState;
            pHeader = (otaFileHeader_t *)pData;
        }
        else
        {
            mOtaClientSession.pTempBuffer = MEM_BufferAlloc(mOtaClientSession.bytesPerState);

            if (mOtaClientSession.pTempBuffer)
            {
                /* require extra bytes, keep current information */
                FLib_MemCpy(mOtaClientSession.pTempBuffer, pData, bytesToProcess);
                mOtaClientSession.remainingBytes -= bytesToProcess;
                *pBytesProcessed = bytesToProcess;
                status = gOtaSucess_c;
            }
        }
    }
    else
    {
        uint32_t index = mOtaClientSession.bytesPerState - mOtaClientSession.remainingBytes;

        if (mOtaClientSession.remainingBytes <= bytesToProcess)
        {
            pHeader = (otaFileHeader_t *)mOtaClientSession.pTempBuffer;
            processData = TRUE;
            *pBytesProcessed = mOtaClientSession.remainingBytes;
            mOtaClientSession.remainingBytes = 0;
        }
        else
        {
            /* require extra bytes, keep current information */
            mOtaClientSession.remainingBytes -= bytesToProcess;
            *pBytesProcessed = bytesToProcess;
        }

        FLib_MemCpy(&mOtaClientSession.pTempBuffer[index], pData, *pBytesProcessed);
    }

    if (processData)
    {
        uint32_t otaFileIdentifier = gOtaFileIdentifierNo_c;
        uint16_t otaFileImageType  = gOtaCurrentImageTypeNo_c;
        uint16_t otaFileManufacturerCode = gOtaManufacturerCodeNo_c;

        /* Check the header for consistency */
        if (FLib_MemCmp((uint8_t *)pHeader->fileIdentifier, &otaFileIdentifier, sizeof(uint32_t)) &&
                FLib_MemCmp((uint8_t *)pHeader->imageType, &otaFileImageType, sizeof(uint16_t)) &&
                FLib_MemCmp((uint8_t *)pHeader->manufacturerCode, &otaFileManufacturerCode, sizeof(uint16_t)) &&
                FLib_MemCmp((uint8_t *)pHeader->fileVersion, &mOtaClientSession.downloadFileVersion, sizeof(uint16_t)))
        {
            mOtaClientSession.remainingBytes = 0;
            OtaClient_ComputeSignature((uint8_t *)pHeader, mOtaClientSession.bytesPerState);
            /* If we got here it means we are ready for the upgrade image tag.
               Next state: process sub-element */
            mOtaClientSession.blockReqState = gOtaClientBlockReqState_ProcessSubElement_c;
            mOtaClientSession.bytesPerState = sizeof(otaFileSubElement_t);
            mOtaClientSession.remainingBytes = mOtaClientSession.bytesPerState;
            status = gOtaSucess_c;
        }

        if (mOtaClientSession.pTempBuffer)
        {
            MEM_BufferFree(mOtaClientSession.pTempBuffer);
            mOtaClientSession.pTempBuffer = NULL;
        }
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaResult_t OtaClient_UpgradeImage(uint8_t *pData, uint32_t bytesToProcess,
                                                  uint32_t *pBytesProcessed)
\brief  This function is used to process start download state.

\param  [in]   pData              Pointer to data
\param  [in]   bytesToProcess     Number of bytes to process
\param  [out]  pBytesProcessed    Number of bytes processed

\return        otaResult_t        Result of the operation
 ***************************************************************************************************/
static otaResult_t OtaClient_UpgradeImage
(
    uint8_t *pData,
    uint32_t bytesToProcess,
    uint32_t *pBytesProcessed
)
{
    otaResult_t status = gOtaSucess_c;

    if (mOtaClientSession.remainingBytes)
    {
        *pBytesProcessed = (mOtaClientSession.remainingBytes > bytesToProcess) ?
                           bytesToProcess : mOtaClientSession.remainingBytes;

        if (*pBytesProcessed > THR_ALL_FFs8)
        {
            *pBytesProcessed = THR_ALL_FFs8;
        }

        OtaClient_ComputeSignature((uint8_t *)pData, *pBytesProcessed);
        status = OTA_PushImageChunk(pData, *pBytesProcessed, NULL, NULL);

        if (status == gOtaSucess_c)
        {
            mOtaClientSession.remainingBytes -= *pBytesProcessed;
        }
    }
    else
    {
        /* state completed. Go to the next state - subElement state */
        mOtaClientSession.blockReqState = gOtaClientBlockReqState_ProcessSubElement_c;
        mOtaClientSession.bytesPerState = sizeof(otaFileSubElement_t);
        mOtaClientSession.remainingBytes = mOtaClientSession.bytesPerState;
        status = gOtaSucess_c;
        *pBytesProcessed  = 0;
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaResult_t OtaClient_ProcessSubElement(uint8_t *pData, uint32_t bytesToProcess,
                                                       uint32_t *pBytesProcessed)
\brief  This function is used to process subElement information.

\param  [in]   pData              Pointer to data
\param  [in]   bytesToProcess     Number of bytes to process
\param  [out]  pBytesProcessed    Number of bytes processed

\return        otaResult_t        Result of the operation
 ***************************************************************************************************/
static otaResult_t OtaClient_ProcessSubElement
(
    uint8_t *pData,
    uint32_t bytesToProcess,
    uint32_t *pBytesProcessed
)
{
    otaResult_t status = gOtaSucess_c;
    bool_t processData = FALSE;
    otaFileSubElement_t *pOtaSubElement = NULL;

    if (!mOtaClientSession.pTempBuffer)
    {
        if (mOtaClientSession.bytesPerState <= bytesToProcess)
        {
            processData = TRUE;
            pOtaSubElement = (otaFileSubElement_t *)pData;
            *pBytesProcessed = mOtaClientSession.bytesPerState;
        }
        else
        {
            status =  gOtaError_c;
            mOtaClientSession.pTempBuffer = MEM_BufferAlloc(sizeof(otaFileSubElement_t));

            if (mOtaClientSession.pTempBuffer)
            {
                /* require extra bytes, keep current information */
                FLib_MemCpy(mOtaClientSession.pTempBuffer, pData, bytesToProcess);
                mOtaClientSession.remainingBytes -= bytesToProcess;
                *pBytesProcessed = bytesToProcess;
                status = gOtaSucess_c;
            }
        }
    }
    else
    {
        uint32_t index = mOtaClientSession.bytesPerState - mOtaClientSession.remainingBytes;

        if (mOtaClientSession.remainingBytes <= bytesToProcess)
        {
            pOtaSubElement = (otaFileSubElement_t *)mOtaClientSession.pTempBuffer;
            processData = TRUE;
            *pBytesProcessed = mOtaClientSession.remainingBytes;
            mOtaClientSession.remainingBytes = 0;
        }
        else
        {
            /* require extra bytes, keep current information */
            mOtaClientSession.remainingBytes -= bytesToProcess;
            *pBytesProcessed = bytesToProcess;
        }

        FLib_MemCpy(&mOtaClientSession.pTempBuffer[index], pData, *pBytesProcessed);
    }

    if (processData)
    {
        uint16_t subElementId;
        uint32_t subElementLength;

        FLib_MemCpy(&subElementId, pOtaSubElement->id, sizeof(uint16_t));
        FLib_MemCpy(&subElementLength, pOtaSubElement->length, sizeof(uint32_t));

        if (subElementId != gOtaFileCrcTagId_c)
        {
            OtaClient_ComputeSignature((uint8_t *)pOtaSubElement, mOtaClientSession.bytesPerState);
        }

        switch (subElementId)
        {
            case gOtaFileUpgradeImageTagId_c:
            {
                mOtaClientSession.blockReqState = gOtaClientBlockReqState_UpgradeImg_c;
                status = OTA_StartImage(subElementLength);
            }
            break;

            case gOtaFileBitmapTagId_c:
            {
                mOtaClientSession.blockReqState = gOtaClientBlockReqState_GetBitmap_c;
            }
            break;

            case gOtaFileCrcTagId_c:
            case gZclOtaFileIntegrityCodeTagId:
            {
                mOtaClientSession.blockReqState = gOtaClientBlockReqState_ValidateSignature_c;
            }
            break;

            default:
                break;
        }

        mOtaClientSession.bytesPerState = subElementLength;
        mOtaClientSession.remainingBytes = mOtaClientSession.bytesPerState;

        if (mOtaClientSession.pTempBuffer)
        {
            MEM_BufferFree(mOtaClientSession.pTempBuffer);
            mOtaClientSession.pTempBuffer = NULL;
        }
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaResult_t OtaClient_GetBitmap(uint8_t *pData, uint32_t bytesToProcess,
                                               uint32_t *pBytesProcessed)
\brief  This function is used to get bitmap information.

\param  [in]   pData              Pointer to data
\param  [in]   bytesToProcess     Number of bytes to process
\param  [out]  pBytesProcessed    Number of bytes processed

\return        otaResult_t        Result of the operation
 ***************************************************************************************************/
static otaResult_t OtaClient_GetBitmap
(
    uint8_t *pData,
    uint32_t bytesToProcess,
    uint32_t *pBytesProcessed
)
{
    otaResult_t status = gOtaSucess_c;
    bool_t stateCompleted = FALSE;

    if (!mOtaClientSession.pBitmap)
    {
        status = gOtaError_c;
        mOtaClientSession.pBitmap = MEM_BufferAllocForever(mOtaClientSession.bytesPerState, AppPoolId_d);

        if (mOtaClientSession.pBitmap)
        {
            status =  gOtaSucess_c;

            if (mOtaClientSession.bytesPerState <= bytesToProcess)
            {
                *pBytesProcessed = mOtaClientSession.bytesPerState;
                stateCompleted = TRUE;
                FLib_MemCpy(mOtaClientSession.pBitmap, pData, mOtaClientSession.bytesPerState);
            }
        }
    }

    if (!stateCompleted && (status == gOtaSucess_c))
    {
        uint32_t index = mOtaClientSession.bytesPerState - mOtaClientSession.remainingBytes;

        if (mOtaClientSession.remainingBytes <= bytesToProcess)
        {
            stateCompleted = TRUE;
            *pBytesProcessed = mOtaClientSession.remainingBytes;
        }
        else
        {
            /* require extra bytes, keep current information */
            mOtaClientSession.remainingBytes -= bytesToProcess;
            *pBytesProcessed = bytesToProcess;
        }

        FLib_MemCpy(&mOtaClientSession.pBitmap[index], pData,  *pBytesProcessed);
    }

    if (stateCompleted)
    {
        OtaClient_ComputeSignature((uint8_t *)mOtaClientSession.pBitmap, mOtaClientSession.bytesPerState);
        /* state completed. Go to the next state - subElement state */
        mOtaClientSession.blockReqState = gOtaClientBlockReqState_ProcessSubElement_c;
        mOtaClientSession.bytesPerState = sizeof(otaFileSubElement_t);
        mOtaClientSession.remainingBytes = mOtaClientSession.bytesPerState;
        /* KDS compile protection */
#if !gEnableOTAServer_d && !gUpgradeImageOnCurrentDevice_d

        /* Protection for bootloader flags bitmap */
        if (!NWKU_GetBit(gBootFlagsSectorBitNo, mOtaClientSession.pBitmap))
        {
            status = gOtaError_c;
            MEM_BufferFree(mOtaClientSession.pBitmap);
            mOtaClientSession.pBitmap = NULL;
        }

#endif
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static otaResult_t OtaClient_ValidateSignature(uint8_t *pData, uint32_t bytesToProcess,
                                                       uint32_t *pBytesProcessed)
\brief  This function is used to validate file signature.

\param  [in]   pData              Pointer to data
\param  [in]   bytesToProcess     Number of bytes to process
\param  [out]  pBytesProcessed    Number of bytes processed

\return        otaResult_t        Result of the operation
 ***************************************************************************************************/
static otaResult_t OtaClient_ValidateSignature
(
    uint8_t *pData,
    uint32_t bytesToProcess,
    uint32_t *pBytesProcessed
)
{
    otaResult_t status = gOtaSucess_c;
    bool_t processData = FALSE;
    uint8_t *pSignature = NULL;

    if (!mOtaClientSession.pTempBuffer)
    {
        if (mOtaClientSession.bytesPerState <= bytesToProcess)
        {
            processData = TRUE;
            *pBytesProcessed = mOtaClientSession.bytesPerState;
            pSignature = pData;
        }
        else
        {
            status =  gOtaError_c;
            mOtaClientSession.pTempBuffer = MEM_BufferAlloc(sizeof(otaFileSubElement_t));

            if (mOtaClientSession.pTempBuffer)
            {
                /* Require extra bytes, keep current information */
                FLib_MemCpy(mOtaClientSession.pTempBuffer, pData, bytesToProcess);
                mOtaClientSession.remainingBytes -= bytesToProcess;
                *pBytesProcessed = bytesToProcess;
                status = gOtaSucess_c;
            }
        }
    }
    else
    {
        uint32_t index = mOtaClientSession.bytesPerState - mOtaClientSession.remainingBytes;

        if (mOtaClientSession.remainingBytes <= bytesToProcess)
        {
            processData = TRUE;
            *pBytesProcessed = mOtaClientSession.remainingBytes;
            pSignature = mOtaClientSession.pTempBuffer;
        }
        else
        {
            /* Require extra bytes, keep current information */
            mOtaClientSession.remainingBytes -= bytesToProcess;
            *pBytesProcessed = bytesToProcess;
        }

        FLib_MemCpy(&mOtaClientSession.pTempBuffer[index], pData, *pBytesProcessed);
    }

    if (processData && pSignature)
    {
        status =  gOtaCrcError_c;

        if (FLib_MemCmp(pSignature, &mOtaClientSession.signature, mOtaClientSession.bytesPerState))
        {
            status = OTA_CommitImage(mOtaClientSession.pBitmap);

            if (status == gOtaSucess_c)
            {
                mOtaClientSession.state = gOtaClientState_Reboot_c;
            }
        }

        if (mOtaClientSession.pTempBuffer)
        {
            MEM_BufferFree(mOtaClientSession.pTempBuffer);
            mOtaClientSession.pTempBuffer = NULL;
        }
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_AbortSession(void)
\brief  This function is used to abort the OTA session.
 ***************************************************************************************************/
static void OtaClient_AbortSession
(
    void
)
{
    if (mOtaClientSession.pBitmap)
    {
        MEM_BufferFree(mOtaClientSession.pBitmap);
        mOtaClientSession.pBitmap = NULL;
    }

    if (mOtaClientSession.pTempBuffer)
    {
        MEM_BufferFree(mOtaClientSession.pTempBuffer);
        mOtaClientSession.pTempBuffer = NULL;
    }

    if (mOtaClientSession.pMulticastTempBuffer)
    {
        MEM_BufferFree(mOtaClientSession.pMulticastTempBuffer);
        mOtaClientSession.pMulticastTempBuffer = NULL;
    }

    OtaClient_ActivityLed(gOtaLedOff_c);
    FLib_MemSet(&mOtaClientSession, 0, sizeof(otaClientSession_t));
    mOtaClientSession.state = gOtaClientState_Idle_c;
    OtaClient_HandleBlockSocket(FALSE);
    OTA_CancelImage();
#if THR_DEFAULT_IS_POLLING_END_DEVICE
    /* Set RX off for SED at the end of OTA upgrade */
    THR_SetPollingDevice(mOtaClientSetup.thrInstId, TRUE);
#endif
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_SendNextReq(void *pParam)
\brief  Callback for sending next request.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_SendNextReq
(
    void *pParam
)
{
    bool_t startTimer = TRUE;

    if ((gOtaUnicast_c == mOtaClientSession.transferType) ||
            ((gOtaMulticast_c == mOtaClientSession.transferType) &&
             (gOtaClientMulticastState_SyncChunks_c == mOtaClientSession.multicastState)) ||
             (gOtaClientState_Error_c <= mOtaClientSession.state))
    {
        switch (mOtaClientSession.state)
        {
            case gOtaClientState_ServerDiscovery_c:
            {
                /* Send server discovery command */
                (void)OtaClient_CoapSendServerDiscovery(pParam);
            }
            break;

            case gOtaClientState_QueryImage_c:
            {
                /* Send query image request command */
                OtaClient_CoapSendQueryImageReq(pParam);
            }
            break;

            case gOtaClientState_BlockReq_c:
            {
                if (mOtaClientSession.blockReqState == gOtaClientBlockReqState_ValidateSignature_c)
                {
                    /* Send upgrade end request command */
                    OtaClient_CoapSendUpgradeEndReq(pParam);
                }
                else
                {
                    /* Send a block request command */
                    OtaClient_SendBlockReq(pParam);
                }
            }
            break;

            case gOtaClientState_Reboot_c:
            case gOtaClientState_Error_c:
            case gOtaClientState_ErrorOperation_c:
            case gOtaClientState_ErrorParameter_c:
            case gOtaClientState_ErrorFlash_c:
            case gOtaClientState_ErrorImage_c:
            {
                /* Send upgrade end request command */
                OtaClient_CoapSendUpgradeEndReq(pParam);
            }
            break;

            default:
            {
                startTimer = FALSE;
            }
            break;
        }

        if ((TRUE == startTimer) && (!TMR_IsTimerActive(mOtaClientSetup.otaClientTmr)))
        {
            TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, gOtaClientDeclareErrorMs_c,
                                     OtaClient_ErrorTimerCallback, NULL);
        }
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_ErrorTimerCallback(void *pParam)
\brief  Callback for OTA error timer.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_ErrorTimerCallback
(
    void *pParam
)
{
    if (FALSE == NWKU_SendMsg(OtaClient_ErrorHandler, pParam, mpOtaClientMsgQueue))
    {
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, gOtaClientDeclareErrorMs_c,
                                 OtaClient_ErrorTimerCallback, pParam);
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_ErrorHandler(void *pParam)
\brief  Callback for error handle.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_ErrorHandler
(
    void *pParam
)
{
    if ((mOtaClientSession.noOfRetries > gOtaClientMaxRetries_c) ||
            (mOtaClientSession.state == gOtaClientState_Idle_c))
    {
        /* abort current session */
        OtaClient_PrepareNextReq(gOtaFileStatus_Abort_c, 0);
    }
    else
    {
        mOtaClientSession.noOfRetries++;
        OtaClient_SendNextReq(NULL);
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClientMulticast_TimerHandler(void *pParam)
\brief  Callback for multicast timer handler.

\param  [in]   pParam    If NULL, then the multicast ended, else wait next fragment
 ***************************************************************************************************/
static void OtaClientMulticast_TimerHandler
(
    void *pParam
)
{
    tmrTimeInMilliseconds_t timeInMilliseconds = 0;

    if (pParam)
    {
        /* All ok, wait for next pack */
        timeInMilliseconds = NWKU_GetRandomNoFromInterval(gOtaClientMulticastWaitMs_c,
                             gOtaClientMulticastWaitMs_c + gOtaClientMulticastWaitJitterMs_c);
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, timeInMilliseconds,
                                 OtaClientMulticast_TimerHandler, NULL);
    }
    else
    {
        uint16_t bitNumber = NWKU_GetFirstBitValueInRange(mOtaClientSession.chunkBitmask, 0,
                             mOtaClientSession.chunkMaxToRxNo, 0);

        mOtaClientSession.multicastState = gOtaClientMulticastState_SyncChunks_c;
        mOtaClientSession.fileOffset = bitNumber * mOtaClientSession.fragmentSize;

        /* The server did not sent anything for a period, let's resink */
        if (FALSE == NWKU_SendMsg(OtaClient_SendNextReq, pParam, mpOtaClientMsgQueue))
        {
            /* Retray after a second */
            TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, 1000,
                                     OtaClientMulticast_TimerHandler, NULL);
        }
    }
}

/*!*************************************************************************************************
\private
\fn     static void OtaClient_TimerCallback(void *pParam)
\brief  Callback for OTA client timer.

\param  [in]   pParam    Pointer to data
 ***************************************************************************************************/
static void OtaClient_TimerCallback
(
    void *pParam
)
{
    if (FALSE == NWKU_SendMsg(OtaClient_SendNextReq, pParam, mpOtaClientMsgQueue))
    {
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, gOtaMaxClientValueForNextReqMs_c,
                                 OtaClient_TimerCallback, pParam);
    }
}

/*!*************************************************************************************************
\private
\fn     static otaStatus_t OtaClient_PrepareNextReq(uint8_t otaStatus, uint32_t timeout)
\brief  Prepare the next ota client request.

\param  [in]   otaStatus      OTA status
\param  [in]   timeout        Timeout for next request, used for gOtaFileStatus_ServerBusy_c status [miliseconds]

\return        otaStatus_t    Status of the operation
 ***************************************************************************************************/
static otaStatus_t OtaClient_PrepareNextReq
(
    uint8_t otaStatus,
    uint32_t timeout
)
{
    uint32_t delayMs = gOtaMaxClientValueForNextReqMs_c;

    if (otaStatus == gOtaFileStatus_ServerBusy_c)
    {
        delayMs = timeout;
#if THR_DEFAULT_IS_POLLING_END_DEVICE

        /* turn rx off for SED if the delay is too high */
        if (delayMs > THR_GetAttr_SedFastPollInterval(mOtaClientSetup.thrInstId))
        {
            THR_SetPollingDevice(mOtaClientSetup.thrInstId, TRUE);
        }

#endif
        /* prepare the next request after...miliseconds */
        TMR_StartSingleShotTimer(mOtaClientSetup.otaClientTmr, delayMs, OtaClient_TimerCallback, NULL);
    }
    else if (otaStatus != gOtaFileStatus_Success_c)
    {
        OtaClient_AbortSession();
    }

    return gOtaStatus_Success_c;
}

/*!*************************************************************************************************
\private
\fn     static otaStatus_t OtaClient_HandleBlockSocket(bool_t onOff)
\brief  This function is used to handle block sockets.

\param  [in]   onOff          If TRUE, create and bind and if FALSE, shutdown socket

\return        otaStatus_t    Status of the operation
 ***************************************************************************************************/
static otaStatus_t OtaClient_HandleBlockSocket
(
    bool_t onOff
)
{
    otaStatus_t status = gOtaStatus_Success_c;
    sockaddrStorage_t portAddr;

    if (TRUE == onOff)
    {
        if (mOtaClientSetup.mOtaUdpSrvSockFd == -1)
        {
            /* Set Ota Client local address and local port */
            FLib_MemSet(&portAddr, 0, sizeof(sockaddrStorage_t));
            ((sockaddrIn6_t *)&portAddr)->sin6_family = AF_INET6;
            ((sockaddrIn6_t *)&portAddr)->sin6_port = OTA_CLIENT_DEFAULT_PORT;
            IP_AddrCopy(&((sockaddrIn6_t *)&portAddr)->sin6_addr, &in6addr_any);
            /* Open Ota Client UDP socket */
            mOtaClientSetup.mOtaUdpSrvSockFd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

            if (mOtaClientSetup.mOtaUdpSrvSockFd != -1)
            {
                (void)bind(mOtaClientSetup.mOtaUdpSrvSockFd, &portAddr, sizeof(sockaddrStorage_t));
                Session_RegisterCb(mOtaClientSetup.mOtaUdpSrvSockFd, OtaClient_UdpServerService, mpOtaClientMsgQueue);
            }
            else
            {
                status = gOtaStatus_Failed_c;
            }
        }
        else
        {
            status = gOtaStatus_Success_c;
        }
    }
    else
    {
        if (mOtaClientSetup.mOtaUdpSrvSockFd != -1)
        {
            Session_UnRegisterCb(mOtaClientSetup.mOtaUdpSrvSockFd);
            (void)shutdown(mOtaClientSetup.mOtaUdpSrvSockFd, 0);
            mOtaClientSetup.mOtaUdpSrvSockFd = -1;
        }
    }

    return status;
}

/*!*************************************************************************************************
\private
\fn      static otaStatus_t OtaClient_ComputeImageSignature(void)
\brief   Compute ota image signature.

\return    otaStatus_t    Status of the operation
 ***************************************************************************************************/
static otaStatus_t OtaClient_ComputeImageSignature
(
    void
)
{
    otaStatus_t otaStatus = gOtaStatus_Failed_c;
    uint8_t *pImageBuffer = MEM_BufferAlloc(128);
    uint32_t otaImageIndex = 0;

    if (pImageBuffer)
    {
        for (otaImageIndex = 0; otaImageIndex < mOtaClientSession.imageSize / 128; otaImageIndex++)
        {
            if (gOtaSucess_c != OTA_ReadExternalMemory(pImageBuffer, 128, (otaImageIndex * 128) + gBootData_Image_Offset_c))
            {
                break;
            }

            OtaClient_ComputeSignature((uint8_t *)pImageBuffer, 128);
        }

        if (gOtaSucess_c == OTA_ReadExternalMemory(pImageBuffer, mOtaClientSession.imageSize % 128,
                (otaImageIndex * 128) + gBootData_Image_Offset_c))
        {
            otaStatus = gOtaStatus_Success_c;
            OtaClient_ComputeSignature((uint8_t *)pImageBuffer, mOtaClientSession.imageSize % 128);
        }
    }

    if (pImageBuffer)
    {
        MEM_BufferFree(pImageBuffer);
    }

    return otaStatus;
}

/*!*************************************************************************************************
\private
\fn     static uint32_t OtaClient_GetJitter(void)
\brief  Return a timeot value for next request.

\return       uin32_t   Jitter value
 ***************************************************************************************************/
static uint32_t OtaClient_GetJitter
(
    void
)
{
    tmrTimeInMilliseconds_t timeInMilliseconds = gOtaMaxClientValueForNextReqMs_c;

    switch (mOtaClientSession.state)
    {
        case gOtaClientState_QueryImage_c:
        {
            /* Send query image request command */
            timeInMilliseconds = NWKU_GetRandomNoFromInterval(gOtaClientQueryMinImgReqMs_c,
                                 gOtaClientQueryMaxImgReqMs_c);
        }
        break;

        case gOtaClientState_ServerDiscovery_c:
        {
            /* Send server discovery command */
            timeInMilliseconds = NWKU_GetRandomNoFromInterval(gOtaClientValueMinForNextDiscoveryReqMs_c,
                                 gOtaClientValueMaxForNextDiscoveryReqMs_c);
        }
        break;

        case gOtaClientState_Reboot_c:
        {
            /* Send upgrade end request command */
            timeInMilliseconds = NWKU_GetRandomNoFromInterval(gOtaMinClientValueForNextReqMs_c,
                                 gOtaMaxClientValueForNextReqMs_c);
        }
        break;

        case gOtaClientState_BlockReq_c:
        {
            /* Send a block request command */
            timeInMilliseconds = NWKU_GetRandomNoFromInterval(gOtaMinClientValueForNextBlockReqMs_c,
                                 gOtaMaxClientValueForNextBlockReqMs_c);
        }
        break;

        default:
        {
            timeInMilliseconds = NWKU_GetRandomNoFromInterval(gOtaMinClientValueForNextReqMs_c,
                                 gOtaMaxClientValueForNextReqMs_c);
        }
        break;
    }

    return timeInMilliseconds;
}

/*!*************************************************************************************************
\private
\fn     static uint32_t OtaClient_ActivityLed(otaClientLedCmd_t otaClientLedCmd)
\brief  Led control for OTA activity.

\param  [in]   otaClientLedCmd    OTA client LED command
 ***************************************************************************************************/
static void OtaClient_ActivityLed
(
    otaClientLedCmd_t otaClientLedCmd
)
{
#if gOtaClientEnableLedActivity_c

    switch (otaClientLedCmd)
    {
        case gOtaLedSet_c:
#if defined(FRDM_KW24) || defined(FRDM_KW41Z)
            LED_SetRgbLed(LED_RGB, THR_ALL_FFs8, 0, 0);
#endif
            break;

        case gOtaLedToggle_c:
#if defined(FRDM_KW24) || defined(FRDM_KW41Z)
            LED_ToggleLed(LED_RGB);
#endif
            break;

        case gOtaLedOff_c:
#if defined(FRDM_KW24) || defined(FRDM_KW41Z)
            LED_TurnOffLed(LED_RGB);
#endif
            break;

        default:
            break;
    }

#else
    (void)otaClientLedCmd;
#endif
}

/*!*************************************************************************************************
\private
\fn     void OtaClient_SleepyEndDeviceMCUResetCb(void * param)
\brief  MCU Reset Timer callback.

\param  [in]   void * param    Input params. Not used for this function.
 ***************************************************************************************************/
#if THR_DEFAULT_IS_POLLING_END_DEVICE
static void OtaClient_SleepyEndDeviceMCUResetCb(void * param)
{
	(void)param;
	THR_TimeoutResetMcu(0UL, FALSE);
}
#endif
/*==================================================================================================
Private debug functions
==================================================================================================*/
