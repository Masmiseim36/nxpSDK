/*!
* Copyright (c) 2013, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the header file containing the OTA packets structure used by the OTAP demos.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _OTA_MESSAGES_H_
#define _OTA_MESSAGES_H_

#include "EmbeddedTypes.h"

#ifdef __cplusplus
    extern "C" {
#endif

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

#define gHeaderStrLength_c                 (32)
#define gImageDataPacketMaxSize_c          (100)
#define gQueryJitterAllClients             (100)

/*        Tag Id Description
 +----------------+---------------------------+
 |Tag Identifiers |       Description         |
 +----------------+---------------------------+
 | 0x0000         | Upgrade Image             |
 +----------------+---------------------------+
 | 0x0001         | ECDSA Signature           |
 +----------------+---------------------------+
 | 0x0002         | ECDSA Signing Certificate |
 +----------------+---------------------------+
 | 0x0003-0xefff  | Reserved                  |
 +----------------+---------------------------+
 | 0xf000-0xffff  | Manufacturer Specific Use |
 +----------------+---------------------------+ */
#define gUpgradeImageTadId             0x0000
#define gSectorBitmapTagId             0xf000
#define gCRCTagId                      0xf100

#define gManufacturerCode_d            0x1004
#define gImgBlkReqNoIEEEaddr           0
#define gNextImgReqHWverPresent        1
        
#define IsManufacturerSpecificImage(img) (img <= 0xFFBF)
#define IsSecurityCredentialImage(img)   (img == 0xFFC0)
#define IsConfigurationImage(img)        (img == 0xFFC1)
#define IsLogImage(img)                  (img == 0xFFC2)

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/* OTA status messages */
typedef enum{
  gOtaStatusSuccess_c          = 0x00,
  gOtaStatusNotAuthorized_c    = 0x7E,
  gOtaStatusMalformedCommand   = 0x80,
  gOtaStatusUsuportedCommand   = 0x81,
  gOtaStatusAbort_c            = 0x95,
  gOtaStatusInvalidImage_c     = 0x96,
  gOtaStatusWaitForData_c      = 0x97,
  gOtaStatusNoImageAvailable_c = 0x98,
  gOtaStatusRequireMoreImage_c = 0x99
}otaStatus_t;

/*   Image Notify -> Payload Type
+--------------+-------------------------------------------------------------------------+
| Payload Type |                      Values	Description                              |
+--------------+-------------------------------------------------------------------------+
| 0x00         | Query jitter                                                            |
+--------------+-------------------------------------------------------------------------+
| 0x01         | Query jitter and manufacturer identifier                                |
+--------------+-------------------------------------------------------------------------+
| 0x02         | Query jitter, manufacturer identifier, and image type                   |
+--------------+-------------------------------------------------------------------------+
| 0x03         | Query jitter, manufacturer identifier, image type, and new file version |
+--------------+-------------------------------------------------------------------------+
| 0x04-0xFF    | Reserved                                                                |
+--------------+-------------------------------------------------------------------------+
*/
typedef enum {
  gPayloadQJitter,
  gPayloadQJitter_ManId,
  gPayloadQJitter_ManId_ImgType,
  gPayloadQJitter_ManId_ImgType_Version
}notifyPayloadType_t;

typedef enum {
  ImageNotify,
  QueryNextImageRequest,
  QueryNextImageResponse,
  ImageBlockRequest,
  ImagePageRequest,
  ImageBlockResponse,
  UpgradeEndRequest,
  UpgradeEndResponse
}otaCommandId_t;

typedef PACKED_STRUCT subElement_tag
{
  uint16_t tagId;
  uint32_t dataLen;
}subElement_t;

/********************************************
 * OTAP Header format
 *
 *  Field Name          Size    Type
 *                     (bytes)
 * +------------------+-------+-----------+
 * | Magic Number     |   4   | Mandatory |
 * +------------------+-------+-----------+
 * | Header Version   |   2   | Mandatory |
 * +------------------+-------+-----------+
 * | Header Length    |   2   | Mandatory |
 * +------------------+-------+-----------+
 * | Field Control    |   2   | Mandatory |
 * +------------------+-------+-----------+
 * | Manufacturer Id  |   2   | Mandatory |
 * +------------------+-------+-----------+
 * | Image Type       |   2   | Mandatory |
 * +------------------+-------+-----------+
 * | File Version     |   4   | Mandatory |
 * +------------------+-------+-----------+
 * | Stack Version    |   2   | Mandatory |
 * +------------------+-------+-----------+
 * | Header String    |   32  | Mandatory |
 * +------------------+-------+-----------+
 * | Total Image Size |   4   | Mandatory |
 * +------------------+-------+-----------+
 * | Security Version |   1   | Optional  |
 * +------------------+-------+-----------+
 * | File Destination |   8   | Optional  |
 * +------------------+-------+-----------+
 * | Min HW Version   |   2   | Optional  |
 * +------------------+-------+-----------+
 * | Max HW Version   |   2   | Optional  |
 * +------------------+-------+-----------+
 ********************************************/
typedef PACKED_STRUCT basicImageHeader_tag
{
  uint32_t  magicNumber;
  uint16_t  headerVersion;
  uint16_t  headerLength;
  uint16_t  fieldControl;
  uint16_t  manufacturerId;
  uint16_t  imageType;
  uint32_t  fileVersion;
  uint16_t  stackVersion;
  uint8_t   headerString[gHeaderStrLength_c];
  uint32_t  totalImageSize;
  uint16_t  minHwVersion;
  uint16_t  maxHwVersion;
}basicImageHeader_t;

typedef PACKED_STRUCT fullImageHeader_tag
{
  uint32_t  magicNumber;
  uint16_t  headerVersion;
  uint16_t  headerLength;
  uint16_t  fieldControl;
  uint16_t  manufacturerId;
  uint16_t  imageType;
  uint32_t  fileVersion;
  uint16_t  stackVersion;
  uint8_t   headerString[gHeaderStrLength_c];
  uint32_t  totalImageSize;
//  uint8_t   securityVersion;
//  uint8_t   fileDestination[8];
  uint16_t  minHwVersion;
  uint16_t  maxHwVersion;
}fullImageHeader_t;

/* Short Response Cmd */
typedef PACKED_STRUCT shortResponseCmd_tag
{
  otaCommandId_t  cmdId;
  uint8_t         status;
}shortResponseCmd_t;

/* Image Notify - 0x03 payload type*/
typedef PACKED_STRUCT imageNotifyCmd_tag
{
  otaCommandId_t       cmdId;
  notifyPayloadType_t  payloadType;
  uint8_t              queryJitter;
  uint16_t             manufacturerId;
  uint16_t             imageType;
  uint32_t             fileVersion;
}imageNotifyCmd_t;

/* Query Next Image Request */
typedef PACKED_STRUCT queryNextImageRequestCmd_tag
{
  otaCommandId_t  cmdId;
  PACKED_STRUCT {
      uint8_t     HwVerPresent :1;
      uint8_t     reserved     :7;
  }fieldControl;
  uint16_t        manufacturerId;
  uint16_t        imageType;
  uint32_t        fileVersion;
  uint16_t        hwVersion;
}queryNextImageRequestCmd_t;

/* Query Next Image Response */
typedef PACKED_STRUCT queryNextImageResponseCmd_tag
{
  otaCommandId_t  cmdId;
  uint8_t         status;
  uint16_t        manufacturerId;
  uint16_t        imageType;
  uint32_t        fileVersion;
  uint32_t        imageSize;
}queryNextImageResponseCmd_t;

/* Image Block Request */
typedef PACKED_STRUCT imageBlockRequestCmd_tag
{
  otaCommandId_t  cmdId;
  PACKED_STRUCT {
      uint8_t     ieeeAddrPresent :1;
      uint8_t     reserved        :7;
  }fieldControl;
  uint16_t        manufacturerId;
  uint16_t        imageType;
  uint32_t        fileVersion;
  uint32_t        fileOffset;
  uint8_t         maxDataSize;
}imageBlockRequestCmd_t;

/* Image Block Response */
typedef PACKED_STRUCT imageBlockResponseCmd_tag
{
  otaCommandId_t  cmdId;
  uint8_t         status;
  uint16_t        manufacturerId;
  uint16_t        imageType;
  uint32_t        fileVersion;
  uint32_t        fileOffset;
  uint8_t         dataSize;
}imageBlockResponseCmd_t;

/* Upgrade End Request */
typedef PACKED_STRUCT upgradeEndRequestCmd_tag
{
  otaCommandId_t  cmdId;
  uint8_t         status;
  uint16_t        manufacturerId;
  uint16_t        imageType;
  uint32_t        fileVersion;
}upgradeEndRequestCmd_t;

/* Upgrade End Response */
typedef PACKED_STRUCT upgradeEndResponseCmd_tag
{
  otaCommandId_t  cmdId;
  uint16_t        manufacturerId;
  uint16_t        imageType;
  uint32_t        fileVersion;
  uint32_t        currentTime;
  uint32_t        upgradeTime;
}upgradeEndResponseCmd_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _OTA_MESSAGES_H_ */
