/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This source file contains the code that enables the OTA Programming protocol
* to load an image received over the air into an external memory, using
* the format that the bootloader will understand
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
#include "EmbeddedTypes.h"
#include "OtaSupport.h"
#include "Eeprom.h"
#include "MemManager.h"
#include "FunctionLib.h"

#include "Flash_Adapter.h"
#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"

#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
#include "FsciInterface.h"
#include "FsciCommands.h"
#endif


/******************************************************************************
*******************************************************************************
* Private Macros
*******************************************************************************
******************************************************************************/

/* There are 2 flags stored in the internal Flash of the MCU that tells
  1. whether there is a bootable image present in the external EEPROM
  2. whether the load of a bootable image from external EEPROM to internal
Flash has been completed. This second flag is useful in case the MCU is reset
while the loading of image from external EEPROM to internal Flash is in progress
  These 2 flags are always located at a fixed address in the internal FLASH */
extern uint32_t __BootFlags_Start__[];
#define gBootImageFlagsAddress_c       ((uint32_t)__BootFlags_Start__)

#define gOtaVerifyWriteBufferSize_d (16) /* [bytes] */


/******************************************************************************
*******************************************************************************
* Private Prototypes
*******************************************************************************
******************************************************************************/
#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
static bool_t OtaSupportCalback(clientPacket_t* pData);
#endif

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/
/* Structure containing the 2 boot flags */
typedef PACKED_STRUCT
{
    uint8_t  newBootImageAvailable[gEepromParams_WriteAlignment_c];
    uint8_t  bootProcessCompleted[gEepromParams_WriteAlignment_c];
    uint8_t  version[2];
}bootInfo_t;


/******************************************************************************
*******************************************************************************
* Private Memory Declarations
*******************************************************************************
******************************************************************************/
#if (gEepromType_d != gEepromDevice_None_c)
/* Flag storing we are already in the process of writing an image received
OTA in the EEPROM or not */
static  bool_t    mLoadOtaImageInEepromInProgress = FALSE;
/* Total length of the OTA image that is currently being written in EEPROM */
static  uint32_t  mOtaImageTotalLength = 0;
/* The length of the OTA image that has being written in EEPROM so far */
static  uint32_t  mOtaImageCurrentLength = 0;
/* Current write address in the EEPROM */
static  uint32_t  mCurrentEepromAddress = 0;
/* When a new image is ready the flash flags will be write in idle task */
static  bool_t    mNewImageReady = FALSE;
#endif

#if gEnableOTAServer_d
/* The FSCI interface used to download an image */
static  uint8_t   mOtaFsciInterface = 0;
#endif

#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
/* The size of the image to be downloaded */
static  uint32_t  mTotalUpdateSize = 0;
#endif

#if gUpgradeImageOnCurrentDevice_d
static uint8_t mNextPushChunkSeq;
#endif

/* Variables used by the Bootloader */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "BootloaderFlags"
const bootInfo_t gBootFlags =
#elif defined(__GNUC__)
const bootInfo_t gBootFlags __attribute__ ((section(".BootloaderFlags"))) =
#else
const bootInfo_t gBootFlags =
#endif
{{gBootValueForFALSE_c}, {gBootValueForTRUE_c}, {0x00, 0x02}};

/* Contains Application Callbacks for packets received over the serial interface */
#if gEnableOTAServer_d
static otaServer_AppCB_t *mpOTA_AppCB;
#endif


/******************************************************************************
*******************************************************************************
* Public Memory
*******************************************************************************
******************************************************************************/
otaMode_t gUpgradeMode = gUpgradeImageOnCurrentDevice_c;

#if !gEnableOTAServer_d && !gUpgradeImageOnCurrentDevice_d
uint16_t gBootFlagsSectorBitNo;
#endif

/******************************************************************************
*******************************************************************************
* Public Functions
*******************************************************************************
******************************************************************************/
#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
/*****************************************************************************
*  OTA_Init
*
*  This function registers a calback function into the FSCI module.
*
*
*****************************************************************************/
otaResult_t OTA_RegisterToFsci( uint32_t fsciInterface, otaServer_AppCB_t *pCB)
{
    pfFSCI_OtaSupportCalback = OtaSupportCalback;
#if gEnableOTAServer_d
    mOtaFsciInterface = (uint8_t)fsciInterface;
    mpOTA_AppCB = pCB;
#endif
    return gOtaSucess_c;
}
#endif


/*****************************************************************************
*  OTA_StartImage
*
*  This function is called in order to start a session of writing a OTA image.
*
*
*****************************************************************************/
otaResult_t OTA_StartImage(uint32_t length)
{
    otaResult_t status = gOtaSucess_c;
#if (gEepromType_d != gEepromDevice_None_c)
    /* Check if we already have an operation of writing an OTA image in the EEPROM
    in progess and if yes, deny the current request */
    if(mLoadOtaImageInEepromInProgress)
    {
        status = gOtaInvalidOperation_c;
    }
    /* Check if the internal FLASH and the EEPROM have enough room to store
    the image */
    else if((length > gFlashParams_MaxImageLength_c) ||
       (length > (gEepromParams_TotalSize_c - gBootData_Image_Offset_c)))
    {
        status = gOtaImageTooLarge_c;
    }
    /* Try to initialize the EEPROM */
    else if(OTA_InitExternalMemory() != gOtaSucess_c)
    {
        status = gOtaExternalFlashError_c;
    }
    else
    {
        /* Save the total length of the OTA image */
        mOtaImageTotalLength = length;
        /* Init the length of the OTA image currently written */
        mOtaImageCurrentLength = 0;
        /* Init the current EEPROM write address */
        mCurrentEepromAddress = gBootData_Image_Offset_c;
        /* Mark that we have started loading an OTA image in EEPROM */
        mLoadOtaImageInEepromInProgress = TRUE;
    }

#if !gEnableOTAServer_d && !gUpgradeImageOnCurrentDevice_d
    gBootFlagsSectorBitNo = gBootImageFlagsAddress_c/(uint32_t)((uint8_t*)FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE);
#endif
#endif
    return status;
}


/*****************************************************************************
*  OTA_PushImageChunk
*
*  Put image chunk on external memory. The CRC will not be computed.
*****************************************************************************/
otaResult_t OTA_PushImageChunk(uint8_t* pData, uint16_t length, uint32_t* pImageLength, uint32_t *pImageOffset)
{
    otaResult_t status = gOtaSucess_c;
#if (gEepromType_d != gEepromDevice_None_c)
    /* Cannot add a chunk without a prior call to OTA_StartImage() */
    if( !mLoadOtaImageInEepromInProgress )
    {
        status = gOtaInvalidOperation_c;
    }
    /* Validate parameters */
    else if((length == 0) || (pData == NULL))
    {
        status = gOtaInvalidParam_c;
    }
    /* Check if the chunck does not extend over the boundaries of the image */
    else if(mOtaImageCurrentLength + length > mOtaImageTotalLength)
    {
        status = gOtaInvalidParam_c;
    }
    else
    {
        /* Received a chunk with offset */
        if(NULL != pImageOffset)
        {
            mCurrentEepromAddress = gBootData_Image_Offset_c + *pImageOffset;
        }

        /* Try to write the data chunk into the external EEPROM */
        if(EEPROM_WriteData(length, mCurrentEepromAddress, pData) != ee_ok)
        {
            status = gOtaExternalFlashError_c;
        }
#if gOtaVerifyWrite_d
        else
        {
            uint8_t readData[gOtaVerifyWriteBufferSize_d];
            uint16_t readLen;
            uint16_t i = 0;

            while(i < length)
            {
                readLen = length - i;

                if( readLen > sizeof(readData) )
                {
                    readLen = sizeof(readData);
                }

                EEPROM_ReadData(readLen, mCurrentEepromAddress + i, readData);

                if( !FLib_MemCmp(&pData[i], readData, readLen) )
                {
                    status = gOtaExternalFlashError_c;
                    break;
                }

                i += readLen;
            }

        }
#endif

        /* Data chunck successfully writtem into EEPROM
        Update operation parameters */
        mCurrentEepromAddress  += length;
        mOtaImageCurrentLength += length;

        /* Return the currenlty written length of the OTA image to the caller */
        if(pImageLength != NULL)
        {
            *pImageLength = mOtaImageCurrentLength;
        }
    }
#endif
    return status;
}

/*****************************************************************************
*  OTA_CommitImage
*
*  Check image length, set bitmap and upgrade image flags or
*  signature needed by bootloader
*****************************************************************************/
otaResult_t OTA_CommitImage(uint8_t* pBitmap)
{
    otaResult_t status = gOtaSucess_c;
#if (gEepromType_d != gEepromDevice_None_c)
    /* Cannot commit a image without a prior call to OTA_StartImage() */
    if( !mLoadOtaImageInEepromInProgress )
    {
        status = gOtaInvalidOperation_c;
    }
    /* If the currently written image length in EEPROM is not the same with
    the one initially set, commit operation fails */
    else if(mOtaImageCurrentLength != mOtaImageTotalLength)
    {
        status = gOtaInvalidOperation_c;
    }
    else
    {
        /* To write image length into the EEPROM */
        if(EEPROM_WriteData(sizeof(uint32_t), gBootData_ImageLength_Offset_c,(uint8_t *)&mOtaImageCurrentLength) != ee_ok)
        {
            status = gOtaExternalFlashError_c;
        }
        /* To write the sector bitmap into the EEPROM */
        else if(EEPROM_WriteData(gBootData_SectorsBitmap_Size_c, gBootData_SectorsBitmap_Offset_c, pBitmap) != ee_ok)
        {
            status = gOtaExternalFlashError_c;
        }
        else
        {
            /* Flash flags will be write in next instance of idle task */
            mNewImageReady = TRUE;
            /* End the load of OTA in EEPROM process */
            mLoadOtaImageInEepromInProgress = FALSE;
        }
    }
#endif
    return status;
}


/*****************************************************************************
*  OTA_SetNewImageFlag
*
*  It is called in idle task to write flags from flash
*****************************************************************************/
void OTA_SetNewImageFlag(void)
{
#if (gEepromType_d != gEepromDevice_None_c)
    /* OTA image successfully writen in EEPROM. Set the flag that indicates that at the next
    boot, the image from FLASH will be updated with the one from EEPROM */
    uint8_t newImageFlag[gEepromParams_WriteAlignment_c];
    uint32_t status;

    if( mNewImageReady )
    {
        NV_Init();

        FLib_MemSet(newImageFlag, gBootValueForTRUE_c, sizeof(newImageFlag));

        status = NV_FlashProgramUnaligned((uint32_t)&gBootFlags.newBootImageAvailable,
                                          sizeof(newImageFlag),
                                          newImageFlag);

        if( status == kStatus_FLASH_Success )
        {
            mNewImageReady = FALSE;
        }
    }
#endif
}


/*****************************************************************************
*  OTA_CancelImage
*
*  Cancel upgrade image write process
*****************************************************************************/
void OTA_CancelImage(void)
{
#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
    mTotalUpdateSize = 0;
#endif
#if (gEepromType_d != gEepromDevice_None_c)
    mLoadOtaImageInEepromInProgress = FALSE;
#endif
}


/************************************************************************************
*  Updates the CRC based on the received data to process.
*  Updates the global CRC value. This was determined to be optimal from a resource
*  consumption POV.
*
*  Input parameters:
*  - None
*  Return:
*  - None
************************************************************************************/
uint16_t OTA_CrcCompute(uint8_t *pData, uint16_t lenData, uint16_t crcValueOld)
{
    uint8_t i;

    while(lenData--)
    {
        crcValueOld ^= (uint16_t)((uint16_t)*pData++ << 8);
        for( i = 0; i < 8; ++i )
        {
            if( crcValueOld & 0x8000 )
            {
                crcValueOld = (crcValueOld << 1) ^ 0x1021U;
            }
            else
            {
                crcValueOld = crcValueOld << 1;
            }
        }
    }
    return crcValueOld;
}


/*****************************************************************************
*  OTA_InitExternalMemory
*
*  This function is called in order to initialize all elements needed for
*  external memory access
*
*****************************************************************************/
otaResult_t OTA_InitExternalMemory(void)
{
#if (gEepromType_d != gEepromDevice_None_c)
    otaResult_t status = gOtaSucess_c;
    uint32_t err;

    if(EEPROM_Init() != ee_ok)
    {
        status = gOtaExternalFlashError_c;
    }
    else
    {
        /* Update the offset to the internal storage if necessary */
        err = NV_ReadHWParameters(&gHardwareParameters);
        if( err || (gHardwareParameters.gInternalStorageAddr != gEepromParams_StartOffset_c) )
        {
            gHardwareParameters.gInternalStorageAddr = gEepromParams_StartOffset_c;
            NV_WriteHWParameters(&gHardwareParameters);
        }
    }

    return status;
#else
    return gOtaExternalFlashError_c;
#endif
}


/*****************************************************************************
*  OTA_EraseMemory
*
*  Function used when external memory must be erased before writing
*
*****************************************************************************/
otaResult_t OTA_EraseExternalMemory(void)
{
#if (gEepromType_d != gEepromDevice_None_c)
    otaResult_t status = gOtaSucess_c;
    if (EEPROM_ChipErase() != ee_ok)
    {
        status = gOtaExternalFlashError_c;
    }

    return status;
#else
    return gOtaExternalFlashError_c;
#endif
}

/*****************************************************************************
*  OTA_EraseBlock
*
*  This function is called in order to erase a block of memory (flash sector)
*
*****************************************************************************/
otaResult_t OTA_EraseBlock(uint32_t address)
{
#if (gEepromType_d != gEepromDevice_None_c)
    otaResult_t status = gOtaSucess_c;

    if( EEPROM_EraseBlock(address, gEepromParams_SectorSize_c)!= ee_ok )
    {
        status = gOtaExternalFlashError_c;
    }

    return status;
#else
    return gOtaExternalFlashError_c;
#endif
}

/*****************************************************************************
*  OTA_ReadExternalMemory
*
*  This function is called in order to read from external flash or eeprom
*
*****************************************************************************/
otaResult_t OTA_ReadExternalMemory(uint8_t* pData, uint16_t length, uint32_t address)
{
#if (gEepromType_d != gEepromDevice_None_c)
    otaResult_t status = gOtaSucess_c;

    if(ee_ok != EEPROM_ReadData(length, address, pData))
    {
        status = gOtaExternalFlashError_c;
    }

    return status;
#else
    return gOtaExternalFlashError_c;
#endif
}


/*****************************************************************************
*  OTA_WriteFlash
*
*  This function is called in order to write in external flash or eeprom
*
*****************************************************************************/
otaResult_t OTA_WriteExternalMemory(uint8_t* pData, uint16_t length, uint32_t address)
{
#if (gEepromType_d != gEepromDevice_None_c)
    otaResult_t status = gOtaSucess_c;
    if( ee_ok != EEPROM_WriteData(length, address, pData) )
    {
        status = gOtaExternalFlashError_c;
    }

#if gOtaVerifyWrite_d
    else
    {
        uint8_t readData[gOtaVerifyWriteBufferSize_d];
        uint16_t readLen;
        uint16_t i = 0;

        while(i < length)
        {
            readLen = length - i;

            if( readLen > sizeof(readData) )
            {
                readLen = sizeof(readData);
            }

            EEPROM_ReadData(readLen, address + i, readData);

            if( !FLib_MemCmp(&pData[i], readData, readLen) )
            {
                status = gOtaExternalFlashError_c;
                break;
            }

            i += readLen;
        }

    }
#endif
    return status;
#else
    return gOtaExternalFlashError_c;
#endif
}


#if gEnableOTAServer_d
/*****************************************************************************
*  OTA_QueryImageReq
*
*
*
*****************************************************************************/
void OTA_QueryImageReq(uint16_t devId, uint16_t manufacturer, uint16_t imgType, uint32_t fileVersion)
{
    uint8_t idx;
    clientPacket_t *pPkt;

    /* compute payload len */
    idx = sizeof(devId) + sizeof(manufacturer) + sizeof(imgType) + sizeof(fileVersion);
    pPkt = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + idx + 2);

    if( NULL == pPkt )
    {
        return;
    }

    pPkt->structured.header.opGroup = gFSCI_ReqOpcodeGroup_c;
    pPkt->structured.header.opCode = mFsciOtaSupportQueryImageReq_c;
    pPkt->structured.header.len = idx;

    // Copy data into the payload buffer
    idx = 0;
    FLib_MemCpy(&pPkt->structured.payload[idx], &devId, sizeof(devId));
    idx +=sizeof(devId);
    FLib_MemCpy(&pPkt->structured.payload[idx], &manufacturer, sizeof(manufacturer));
    idx +=sizeof(manufacturer);
    FLib_MemCpy(&pPkt->structured.payload[idx], &imgType, sizeof(imgType));
    idx +=sizeof(imgType);
    FLib_MemCpy(&pPkt->structured.payload[idx], &fileVersion, sizeof(fileVersion));

    FSCI_transmitFormatedPacket(pPkt, mOtaFsciInterface);
}


/*****************************************************************************
*  OTA_ImageChunkReq
*
*
*
*****************************************************************************/
void OTA_ImageChunkReq(uint32_t offset, uint16_t len, uint16_t devId)
{
    uint8_t idx;
    clientPacket_t *pPkt;

    idx = sizeof(offset) + sizeof(len) + sizeof(devId);
    pPkt = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + idx + 2);

    if( NULL == pPkt )
    {
        return;
    }

    pPkt->structured.header.opGroup = gFSCI_ReqOpcodeGroup_c;
    pPkt->structured.header.opCode = mFsciOtaSupportImageChunkReq_c;
    pPkt->structured.header.len = idx;

    // Copy data into the payload buffer
    idx = 0;
    FLib_MemCpy(&pPkt->structured.payload[idx], &devId, sizeof(devId));
    idx +=sizeof(devId);
    FLib_MemCpy(&pPkt->structured.payload[idx], &offset, sizeof(offset));
    idx +=sizeof(offset);
    FLib_MemCpy(&pPkt->structured.payload[idx], &len, sizeof(len));

    FSCI_transmitFormatedPacket(pPkt, mOtaFsciInterface);
}

/*****************************************************************************
*  OTA_ClientInfoReq
*
*
*
*****************************************************************************/
void OTA_ClientInfoCnf(uint8_t* pClientAddr, uint32_t offset, uint16_t devId)
{
    clientPacket_t *pPkt;
    uint8_t idx;

    if (pClientAddr)
    {
      pPkt = MEM_BufferAlloc(sizeof(clientPacketHdr_t) + 16 + sizeof(uint16_t) + sizeof(uint32_t) + 2);

      if( NULL == pPkt )
      {
          return;
      }

      pPkt->structured.header.opGroup = gFSCI_CnfOpcodeGroup_c;
      pPkt->structured.header.opCode = mFsciOtaSupportGetClientInfo_c;
      pPkt->structured.header.len = 16 + sizeof(uint32_t) + sizeof(uint16_t);

      // Copy data into the payload buffer
      idx = 0;
      FLib_MemCpy(&pPkt->structured.payload[idx], pClientAddr, 16);
      idx += 16;
      FLib_MemCpy(&pPkt->structured.payload[idx], &offset, sizeof(uint32_t));
      idx += sizeof(uint32_t);
      FLib_MemCpy(&pPkt->structured.payload[idx], &devId, sizeof(devId));

      FSCI_transmitFormatedPacket(pPkt, mOtaFsciInterface);
    }
}
#endif


#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static bool_t OtaSupportCalback( clientPacket_t* pData )
{
    otaResult_t status = gOtaInvalidOperation_c;

    switch( pData->structured.header.opCode )
    {
    case mFsciOtaSupportStartImageReq_c:
        if( mTotalUpdateSize )
        {
            break;
        }

        FLib_MemCpy(&mTotalUpdateSize, pData->structured.payload, sizeof(mTotalUpdateSize));
        pData->structured.header.len = 3*sizeof(uint8_t);
        pData->structured.payload[0] = gOtaSucess_c;
        pData->structured.payload[1] = gOtaVersion_c;
        /* Check if image storage is available */
        if( mTotalUpdateSize > gEepromParams_TotalSize_c )
            pData->structured.payload[2] = FALSE; /* The available storage cannot be used */
        else
            pData->structured.payload[2] = (gEepromType_d != gEepromDevice_None_c);
#if gUpgradeImageOnCurrentDevice_d
        mNextPushChunkSeq = 0;
#endif
        return TRUE;

    case mFsciOtaSupportSetModeReq_c:
        gUpgradeMode = (otaMode_t)pData->structured.payload[0];

        if( gUpgradeMode > gDoNotUseExternalMemoryForOtaUpdate_c )
        {
            status = gOtaInvalidParam_c;
        }
#if (gEepromType_d == gEepromDevice_None_c)
        else if( gUpgradeMode == gUseExternalMemoryForOtaUpdate_c )
        {
            status = gOtaInvalidOperation_c;
        }
#endif
#if gUpgradeImageOnCurrentDevice_d
        else if( gUpgradeMode == gUpgradeImageOnCurrentDevice_c )
        {
            status = OTA_StartImage(mTotalUpdateSize);
        }
#endif
#if gEnableOTAServer_d
        else if( mpOTA_AppCB && mpOTA_AppCB->otaServerSetModeCnf )
        {
            status = mpOTA_AppCB->otaServerSetModeCnf(pData->structured.payload, pData->structured.header.len);
        }
#endif
        break;

    case mFsciOtaSupportPushImageChunkReq_c:
        if( !mTotalUpdateSize )
        {
            status = gOtaInvalidOperation_c;
        }
#if gUpgradeImageOnCurrentDevice_d
        else if( gUpgradeMode == gUpgradeImageOnCurrentDevice_c )
        {
            if( mNextPushChunkSeq == pData->structured.payload[0] )
            {
                status = OTA_PushImageChunk(&pData->structured.payload[1], pData->structured.header.len-1, NULL);
                if( gOtaSucess_c == status )
                {
                    mNextPushChunkSeq++;
                }
            }
        }
#endif
#if gEnableOTAServer_d
        else if( mpOTA_AppCB && mpOTA_AppCB->otaServerPushChunkCnf )
        {
            status = mpOTA_AppCB->otaServerPushChunkCnf(pData->structured.payload, pData->structured.header.len);
        }
#endif
        break;

    case mFsciOtaSupportCommitImageReq_c:
#if gUpgradeImageOnCurrentDevice_d
        mTotalUpdateSize = 0;

        if( gUpgradeMode == gUpgradeImageOnCurrentDevice_c )
        {
            status = OTA_CommitImage(pData->structured.payload);
            OTA_SetNewImageFlag();
        }
#endif
        break;

    case mFsciOtaSupportCancelImageReq_c:
#if gEnableOTAServer_d
        if( mpOTA_AppCB && mpOTA_AppCB->otaServerCancelImgCnf )
        {
            status = mpOTA_AppCB->otaServerCancelImgCnf(pData->structured.payload, pData->structured.header.len);
        }
#endif
        OTA_CancelImage();
        break;

#if gEnableOTAServer_d
    case mFsciOtaSupportQueryImageRsp_c:
        if( mpOTA_AppCB && mpOTA_AppCB->otaServerQueryImageCnf )
        {
            status = mpOTA_AppCB->otaServerQueryImageCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;

    case mFsciOtaSupportImageNotifyReq_c:
        if( mpOTA_AppCB && mpOTA_AppCB->otaServerImgNotifyCnf )
        {
            status = mpOTA_AppCB->otaServerImgNotifyCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;
    case mFsciOtaSupportSetFileVerPoliciesReq_c:
        if( mpOTA_AppCB && mpOTA_AppCB->otaServerSetFileVersPoliciesCnf )
        {
            status = mpOTA_AppCB->otaServerSetFileVersPoliciesCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;
    case mFsciOtaSupportAbortOTAUpgradeReq_c:
        if( mpOTA_AppCB && mpOTA_AppCB->otaServerAbortProcessCnf )
        {
            status = mpOTA_AppCB->otaServerAbortProcessCnf(pData->structured.payload, pData->structured.header.len);
        }
        break;
    case mFsciOtaSupportGetClientInfo_c:
         if( mpOTA_AppCB && mpOTA_AppCB->otaServerClientInfoCnf)
         {
            status = mpOTA_AppCB->otaServerClientInfoCnf(pData->structured.payload, pData->structured.header.len);
         }
        break;
#endif
    }

    /* Default response */
    pData->structured.header.len = sizeof(uint8_t);
    pData->structured.payload[0] = status;
    return TRUE;
}
#endif