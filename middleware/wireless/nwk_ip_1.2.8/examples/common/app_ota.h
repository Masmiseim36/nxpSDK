/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _APP_OTA_H
#define _APP_OTA_H

/*!=================================================================================================
\file       app_ota.h
\brief      This is the header file for the ota module
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "app_init.h"
#include "coap.h"
/*==================================================================================================
Public macros
==================================================================================================*/
#define OTA_CLIENT_URI_PATH                     "/otaclient"
#define OTA_SERVER_URI_PATH                     "/otaserver"

#ifndef OTA_USE_NWK_DATA
    #define OTA_USE_NWK_DATA                        FALSE
#endif

#if OTA_USE_NWK_DATA
    #define OTA_ADDR_TYPE                       gRLOCAddr_c
#else
    #define OTA_ADDR_TYPE                       gMLEIDAddr_c
#endif

/* ota params */
#define gOtaFileIdentifierNo_c                  0x0BEEF11E
#define gOtaManufacturerCode_c                  0x04, 0x10
#define gOtaManufacturerCodeNo_c                0x1004
#define gOtaCurrentImageType_c                  0x00, 0x00
#define gOtaCurrentImageTypeNo_c                0x0000
#define gOtaCurrentFileVersion_c                0x05, 0x40, 0x03, 0x40
#define gOtaCurrentFileVersionNo_c              0x40034005
#define gOtaHardwareVersion_c                   0x21, 0x24
#define gOtaHardwareVersionNo_c                 0x2421

#define gOtaMaxBlockDataSize_c                  64 /* 60 bytes */

#define gOtaFileBitmapTagId_c                   0xF000
#define gOtaFileUpgradeImageTagId_c             0x0000
#define gOtaFileCrcTagId_c                      0xF100
#define gZclOtaFileIntegrityCodeTagId           0x0003

/* OTA server standalone mode -> requires an external memory or a reserved region of
   internal MCU flash to keep the client image(s) */
#ifndef gOtaServer_EnableStandaloneMode_c
    #define gOtaServer_EnableStandaloneMode_c       FALSE
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/

/* ota commands */
typedef enum
{
    gOtaCmd_ImageNotify_c          =       0x00,
    gOtaCmd_QueryImageReq_c,
    gOtaCmd_QueryImageRsp_c,
    gOtaCmd_BlockReq_c,
    gOtaCmd_BlockRsp_c,
    gOtaCmd_UpgradeEndReq_c,
    gOtaCmd_UpgradeEndRsp_c,
    gOtaCmd_ServerDiscovery_c,
    gOtaCmd_Invalid_c              =       THR_ALL_FFs8
} gOtaCmd_t;

/*! Ota status */
typedef enum otaStatus_tag
{
    gOtaStatus_Success_c          = 0x00,
    gOtaStatus_Failed_c           = 0x01,
    gOtaStatus_InvalidInstance_c  = 0x02,
    gOtaStatus_InvalidParam_c     = 0x03,
    gOtaStatus_NotPermitted_c     = 0x04,
    gOtaStatus_NotStarted_c       = 0x05,
    gOtaStatus_NoMem_c            = 0x06,
    gOtaStatus_UnsupportedAttr_c  = 0x07,
    gOtaStatus_EmptyEntry_c       = 0x08,
    gOtaStatus_InvalidValue_c     = 0x09,
    gOtaStatus_AlreadyStarted_c   = 0x0A,
    gOtaStatus_NoTimers_c         = 0x0B,
    gOtaStatus_NoUdpSocket_c      = 0x0C,
    gOtaStatus_EntryNotFound_c    = THR_ALL_FFs8
} otaStatus_t;

/* OTA File Status */
typedef enum otaFileStatus_tag
{
    gOtaFileStatus_Success_c          = 0x00, /* Success Operation */
    gOtaFileStatus_NotAuthorized_c    = 0x7E, /* Server is not authorized to upgrade the client. */
    gOtaFileStatus_Abort_c            = 0x95, /* Failed case when a client or a server decides to abort the upgrade process. */
    gOtaFileStatus_InvalidImage_c     = 0x96, /* Invalid OTA upgrade image. */
    gOtaFileStatus_ServerBusy_c       = 0x97, /* Server is busy, retry later. */
    gOtaFileStatus_NoImageAvailable_c = 0x98, /* No OTA upgrade image available for a particular client. */
    gOtaFileStatus_ImageTooLarge_c    = 0x99, /* Received OTA image is larger than the available storage space. */
    gOtaFileStatus_InvalidOperation_c = 0x9A, /* Client encountered an invalid operation error. */
    gOtaFileStatus_InvalidParameter_c = 0x9B, /* Client encountered an invalid parameter error. */
    gOtaFileStatus_ExtFlashError_c    = 0x9C, /* Client encountered an external flash error. */
    gOtaFileStatus_ClientError_c      = 0x9D /* Generic client error. */
} otaFileStatus_t;

typedef enum otaTransferType_tag
{
    gOtaUnicast_c = 0x00,
    gOtaMulticast_c
} otaTransferType_t;

/* ota command format */
typedef struct otaCommand_tag
{
    uint8_t commandId;
    uint8_t pPayload[1];
} otaCommand_t;

/* ota image notify command format*/
typedef struct otaServerCmd_ImageNotify_tag
{
    uint8_t commandId;
    uint8_t transferType;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t imageSize[4];
    uint8_t fileSize[4];
    uint8_t fileVersion[4];
    uint8_t serverDownloadPort[2];
    uint8_t fragmentSize[2];
} otaServerCmd_ImageNotify_t;

/* ota query image req command format*/
typedef struct otaCmd_QueryImageReq_tag
{
    uint8_t commandId;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t hardwareVersion[2];
} otaCmd_QueryImageReq_t;

/* ota query image rsp - success */
typedef struct otaCmd_QueryImageRspSuccess_tag
{
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t fileSize[4];
    uint8_t serverDownloadPort[2];
} otaCmd_QueryImageRspSuccess_t;

/* ota query image rsp - wait */
typedef struct otaCmd_QueryImageRspWait_tag
{
    uint8_t currentTime[4];
    uint8_t requestTime[4];
} otaCmd_QueryImageRspWait_t;

/* ota query image rsp command format*/
typedef struct otaCmd_QueryImageRsp_tag
{
    uint8_t commandId;
    uint8_t status;
    union
    {
        otaCmd_QueryImageRspSuccess_t success;
        otaCmd_QueryImageRspWait_t wait;
    } data;
} otaCmd_QueryImageRsp_t;

/* ota block req command format*/
typedef struct otaCmd_BlockReq_tag
{
    uint8_t commandId;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t fileOffset[4];
    uint8_t maxDataSize;
} otaCmd_BlockReq_t;

/* ota block rsp - success */
typedef struct otaCmd_BlockRspSuccess_tag
{
    uint8_t fileVersion[4];
    uint8_t fileOffset[4];
    uint8_t dataSize;
    uint8_t pData[1];
} otaCmd_BlockRspSuccess_t;

/* ota block rsp - wait for data */
typedef struct otaCmd_BlockRspWaitForData_tag
{
    uint8_t currentTime[4];
    uint8_t requestTime[4];
} otaCmd_BlockRspWaitForData_t;

/* ota block rsp command format*/
typedef struct otaCmd_BlockRsp_tag
{
    uint8_t commandId;
    uint8_t status;
    union
    {
        otaCmd_BlockRspSuccess_t success;
        otaCmd_BlockRspWaitForData_t wait;
    } data;
} otaCmd_BlockRsp_t;

/* ota upgrade end req command format*/
typedef struct otaCmd_UpgradeEndReq_tag
{
    uint8_t commandId;
    uint8_t status;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
} otaCmd_UpgradeEndReq_t;

/* ota upgrade end rsp - success */
typedef struct otaCmd_UpgradeEndRspSuccess_tag
{
    uint8_t currentTime[4];     /* milliseconds */
    uint8_t upgradeTime[4];     /* milliseconds */
    uint8_t fileVersion[4];
} otaCmd_UpgradeEndRspSuccess_t;

/* ota upgrade end rsp - wait for data */
typedef struct otaCmd_UpgradeEndRspWaitForData_tag
{
    uint8_t currentTime[4];
    uint8_t requestTime[4];
} otaCmd_UpgradeEndRspWaitForData_t;

/* ota upgrade end rsp command format*/
typedef struct otaCmd_UpgradeEndRsp_tag
{
    uint8_t commandId;
    uint8_t status;
    union
    {
        otaCmd_UpgradeEndRspSuccess_t success;
        otaCmd_UpgradeEndRspWaitForData_t wait;
    } data;
} otaCmd_UpgradeEndRsp_t;

/* ota upgrade server discovery command format */
typedef struct otaCmd_ServerDiscovery_tag
{
    uint8_t commandId;
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
} otaCmd_ServerDiscovery_t;

/* ota file header */
typedef struct otaFileHeader_tag
{
    uint8_t fileIdentifier[4];
    uint8_t headerVersion[2];
    uint8_t headerLength[2];
    uint8_t fieldControl[2];
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t stackVersion[2];
    uint8_t headerString[32];
    uint8_t totalImageSize[4];
    uint8_t minHwVersion[2];
    uint8_t maxHwVersion[2];
} otaFileHeader_t;

typedef struct otaFileSubElement_tag
{
    uint8_t id[2];
    uint8_t length[4];
} otaFileSubElement_t;

/* OTA Server Serial Protocol */

/* thci image notify command format */
typedef struct thciOtaServer_ImageNotify_tag
{
    uint8_t deviceId[2];
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t imageSize[4];
    uint8_t fileSize[4];
    uint8_t fileVersion[4];
} thciOtaServer_ImageNotify_t;

/* thci query image command format */
typedef struct thciOtaServer_QueryImage_tag
{
    uint8_t imageStatus;
    uint8_t deviceId[2];
    uint8_t manufacturerCode[2];
    uint8_t imageType[2];
    uint8_t fileVersion[4];
    uint8_t totalImgSize[4];
} thciOtaServer_QueryImage_t;

/* thci block rsp command format */
typedef struct thciOtaServer_BlockRsp_tag
{
    uint8_t dataLen;
    uint8_t pData[1];
} thciOtaServer_BlockRsp_t;

/* ota server operation mode */
typedef enum
{
    gOtaServerOpMode_Reserved_c = 0,
    gOtaServerOpMode_Standalone_c,           /* requires an external memory or a reserved region of internal MCU flash to keep the client image */
    gOtaServerOpMode_Dongle_c,               /* without internal/external memory capacity */
} otaServerOpMode_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

extern const coapUriPath_t gOTA_CLIENT_URI_PATH;
extern const coapUriPath_t gOTA_SERVER_URI_PATH;

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\public
\fn  otaStatus_t OtaServerInit(taskMsgQueue_t *pMsgQueue)
\brief  Initialize OTA server application

\param  [in]    pMsgQueue   Pointer to task message queue

\return   otaStatus_t
 ***************************************************************************************************/
otaStatus_t OtaServerInit(taskMsgQueue_t *pMsgQueue);

/*!*************************************************************************************************
\public
\fn  otaStatus_t OtaClientInit(instanceId_t thrInstId, taskMsgQueue_t *pMsgQueue);
\brief  Initialize OTA server application

\param  [in]    thrInstId   Thread instance id
\param  [in]    pMsgQueue   Pointer to task message queue

\return   otaStatus_t
 ***************************************************************************************************/
otaStatus_t OtaClientInit(instanceId_t thrInstId, taskMsgQueue_t *pMsgQueue);

/*!*************************************************************************************************
\public
\fn   otaStatus_t OtaClient_StartServerDiscovery(void* param)
\brief  This function is used to start Server Discovery procedure

\param [in]   param    not used

\return   otaStatus_t
 ***************************************************************************************************/
otaStatus_t OtaClient_StartServerDiscovery(void *param);

/*!*************************************************************************************************
\fn     void OtaClient_SetServerIp6Addr(uint16_t otaServerShort)
\brief  This function sets the IPv6 (RLOC) address of the OTA server.
\param  [in]  otaServerShort    OTA server's short address.
***************************************************************************************************/
void OtaClient_SetServerIp6Addr(uint16_t otaServerShort);

/*!*************************************************************************************************
\public
\fn   otaStatus_t OtaClient_StartServerImageNotify(void* param)
\brief  This function is used to send a server multicast image notify

\param [in]   param    not used

\return   otaStatus_t
 ***************************************************************************************************/
otaStatus_t OtaClient_StartServerImageNotify(void *param);

/* OTA Client Coap callback*/
void OtaClient_CoapCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
                             coapSession_t *pSession, uint32_t dataLen);

/* OTA Server Coap callback */
void OtaServer_CoapCb(coapSessionStatus_t sessionStatus, uint8_t *pData,
                             coapSession_t *pSession, uint32_t dataLen);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/
#endif  /*  _APP_OTA_H */
