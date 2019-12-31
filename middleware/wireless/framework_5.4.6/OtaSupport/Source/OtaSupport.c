/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This source file contains the code that enables the OTA Programming protocol
* to load an image received over the air into an external memory, using
* the format that the Bootloader will understand
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

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

#if defined(__CC_ARM)

  extern uint32_t Image$$BOOT_FLAGS$$Base[];
  #define gBootImageFlagsAddress_c       ((uint32_t)Image$$BOOT_FLAGS$$Base)
  
#else /* defined(__CC_ARM) */

  extern uint32_t __BootFlags_Start__[];
  #define gBootImageFlagsAddress_c       ((uint32_t)__BootFlags_Start__)

#endif /* defined(__CC_ARM) */

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

/******************************************************************************
*******************************************************************************
* Private Memory Declarations
*******************************************************************************
******************************************************************************/
#if (gEepromType_d != gEepromDevice_None_c) && (!gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d))
/*! Flag storing we are already in the process of writing an image received
OTA in the EEPROM or not */
static  bool_t    mLoadOtaImageInEepromInProgress = FALSE;
/*! Total length of the OTA image that is currently being written in EEPROM */
static  uint32_t  mOtaImageTotalLength = 0;
/*! The length of the OTA image that has being written in EEPROM so far */
static  uint32_t  mOtaImageCurrentLength = 0;
/*! Current write address in the EEPROM */
static  uint32_t  mCurrentEepromAddress = 0;
/*! When a new image is ready the flash flags will be write in idle task */
static  bool_t    mNewImageReady = FALSE;
#endif

#if gEnableOTAServer_d
/*! The FSCI interface used to download an image */
static  uint8_t   mOtaFsciInterface = 0;
#endif

#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
/*! The size of the image to be downloaded */
static  uint32_t  mTotalUpdateSize = 0;
#endif

#if gUpgradeImageOnCurrentDevice_d
static uint8_t mNextPushChunkSeq;
#endif

#if !gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d)
/*! Variables used by the Bootloader */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "BootloaderFlags"
const bootInfo_t gBootFlags =
#elif defined(__GNUC__)
const bootInfo_t gBootFlags __attribute__ ((section(".BootloaderFlags"))) =
#elif defined(__CC_ARM)
volatile const bootInfo_t gBootFlags __attribute__ ((section(".BootloaderFlags"))) =
#else
    #error "Compiler unknown!"
#endif
{
    {gBootFlagUnprogrammed_c},
    {gBootValueForTRUE_c},
    {0x00, 0x02},
    {gBootFlagUnprogrammed_c},
#if defined(CPU_K32W032S1M2VPJ_cm4) && (CPU_K32W032S1M2VPJ_cm4 == 1)
    {PLACEHOLDER_SBKEK},
    {BOOT_MAGIC_WORD}
#endif
};

#endif /* !gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d) */

/*! Contains Application Callbacks for packets received over the serial interface */
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
/*! *********************************************************************************
* \brief  Registers the OTA component to FSCI
*
* \param[in] fsciInterface  The Id of the FSCI interface used by the Otap Server
* \param[in] pCB            Pointer to a table of callback function.
*
* \return  error code.
*
********************************************************************************** */
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


/*! *********************************************************************************
* \brief  Starts the process of writing a new image to the external EEPROM.
*
* \param[in] length: the length of the image to be written in the EEPROM
*
* \return  
*  - gOtaInvalidParam_c: the intended length is bigger than the FLASH capacity
*  - gOtaInvalidOperation_c: the process is already started (can be cancelled)
*  - gOtaEepromError_c: can not detect external EEPROM
*
********************************************************************************** */
otaResult_t OTA_StartImage(uint32_t length)
{
    otaResult_t status = gOtaSucess_c;
#if (gEepromType_d != gEepromDevice_None_c) && (!gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d))
    /* Check if we already have an operation of writing an OTA image in the EEPROM
    in progress and if yes, deny the current request */
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
#if defined(FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE)
    gBootFlagsSectorBitNo = gBootImageFlagsAddress_c/(uint32_t)((uint8_t*)FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE);
#elif defined(FSL_FEATURE_FLASH_PAGE_SIZE_BYTES)
    gBootFlagsSectorBitNo = gBootImageFlagsAddress_c/(uint32_t)((uint8_t*)FSL_FEATURE_FLASH_PAGE_SIZE_BYTES);
#endif
#endif
#endif
    return status;
}


/*! *********************************************************************************
* \brief  Places the next image chunk into the external FLASH. The CRC will not be computed.
*
* \param[in] pData          pointer to the data chunk
* \param[in] length         the length of the data chunk
* \param[in] pImageLength   if it is not null and the function call is successful,
*                           it will be filled with the current length of the image
* \param[in] pImageOffset   if it is not null contains the current offset of the image
*
* \return
*  - gOtaInvalidParam_c: pData is NULL or the resulting image would be bigger than the 
*       final image length specified with OTA_StartImage()
*  - gOtaInvalidOperation_c: the process is not started
*
********************************************************************************** */
otaResult_t OTA_PushImageChunk(uint8_t* pData, uint16_t length, uint32_t* pImageLength, uint32_t *pImageOffset)
{
    otaResult_t status = gOtaSucess_c;
#if (gEepromType_d != gEepromDevice_None_c) && (!gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d))
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
    /* Check if the chunk does not extend over the boundaries of the image */
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

        /* Data chunk successfully written into EEPROM
        Update operation parameters */
        mCurrentEepromAddress  += length;
        mOtaImageCurrentLength += length;

        /* Return the currently written length of the OTA image to the caller */
        if(pImageLength != NULL)
        {
            *pImageLength = mOtaImageCurrentLength;
        }
    }
#endif
    return status;
}


/*! *********************************************************************************
* \brief  Finishes the writing of a new image to the permanent storage.
*         It will write the image header (signature and length) and footer (sector copy bitmap).
*
* \param[in] bitmap   pointer to a  byte array indicating the sector erase pattern for the
*                     internal FLASH before the image update.
*
* \return
*  - gOtaInvalidOperation_c: the process is not started,
*  - gOtaEepromError_c: error while trying to write the EEPROM 
*
********************************************************************************** */
otaResult_t OTA_CommitImage(uint8_t* pBitmap)
{
    otaResult_t status = gOtaSucess_c;
#if (gEepromType_d != gEepromDevice_None_c) && (!gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d))
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
#if (gEepromType_d == gEepromDevice_InternalFlash_c)
        uint8_t start_marker[gBootData_Marker_Size_c] = {gBootData_StartMarker_Value_c};
        
        /* Write the Start marker at the beginning of the internal storage. */
        if(EEPROM_WriteData(gBootData_Marker_Size_c, gBootData_StartMarker_Offset_c, start_marker) != ee_ok)
        {
            status = gOtaExternalFlashError_c;
        }
        else
#endif
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


/*! *********************************************************************************
* \brief  Set the boot flags, to trigger the Bootloader at the next CPU reset.
*
********************************************************************************** */
void OTA_SetNewImageFlag(void)
{
#if (gEepromType_d != gEepromDevice_None_c) && (!gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d))
    /* OTA image successfully written into the non-volatile storage. 
       Set the boot flag to trigger the Bootloader at the next CPU Reset. */
    union{ 
        uint32_t value;
        uint8_t aValue[FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE];
    }bootFlag;
#if defined(CPU_K32W032S1M2VPJ_cm4) && (CPU_K32W032S1M2VPJ_cm4 == 1)
    uint8_t defaultSBKEK[SBKEK_SIZE] = {DEFAULT_DEMO_SBKEK};
#endif
    uint32_t status;

    if( mNewImageReady )
    {
        NV_Init();

        bootFlag.value = gBootValueForTRUE_c;

        status = NV_FlashProgramUnaligned((uint32_t)&gBootFlags.newBootImageAvailable,
                                          sizeof(bootFlag),
                                          bootFlag.aValue);
        
        if( (status == kStatus_FLASH_Success) && 
             FLib_MemCmpToVal(gBootFlags.internalStorageAddr, 0xFF, sizeof(gBootFlags.internalStorageAddr)) )
        {
            bootFlag.value = gEepromParams_StartOffset_c + gBootData_ImageLength_Offset_c;
            status = NV_FlashProgramUnaligned((uint32_t)&gBootFlags.internalStorageAddr,
                                              sizeof(bootFlag),
                                              bootFlag.aValue);
        }

#if defined(CPU_K32W032S1M2VPJ_cm4) && (CPU_K32W032S1M2VPJ_cm4 == 1)
        if( status == kStatus_FLASH_Success )
        {
            /* Write the default SBKEK for secured OTA */
            status = NV_FlashProgramUnaligned((uint32_t)&gBootFlags.sbkek,
                                              SBKEK_SIZE,
                                              defaultSBKEK);
        }
#endif
        
        if( status == kStatus_FLASH_Success )
        {
            mNewImageReady = FALSE;
        }
    }
#endif
}


/*! *********************************************************************************
* \brief  Cancels the process of writing a new image to the external EEPROM.
*
********************************************************************************** */
void OTA_CancelImage(void)
{
#if gEnableOTAServer_d || gUpgradeImageOnCurrentDevice_d
    mTotalUpdateSize = 0;
#endif
#if (gEepromType_d != gEepromDevice_None_c) && (!gEnableOTAServer_d || (gEnableOTAServer_d && gUpgradeImageOnCurrentDevice_d))
    mLoadOtaImageInEepromInProgress = FALSE;
#endif
}

/*! *********************************************************************************
* \brief  Compute CRC over a data chunk.
*
* \param[in] pData        pointer to the data chunk
* \param[in] length       the length of the data chunk
* \param[in] crcValueOld  current CRC value
*
* \return  computed CRC. 
*
********************************************************************************** */
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


/*! *********************************************************************************
* \brief  Initializes the image storage (external memory or internal flash)
*
* \return  error code.
*
********************************************************************************** */
otaResult_t OTA_InitExternalMemory(void)
{
#if (gEepromType_d != gEepromDevice_None_c)
    otaResult_t status = gOtaSucess_c;

    if(EEPROM_Init() != ee_ok)
    {
        status = gOtaExternalFlashError_c;
    }

    return status;
#else
    return gOtaExternalFlashError_c;
#endif
}


/*! *********************************************************************************
* \brief  This function is called in order to erase the image storage 
*         (external memory or internal flash)
*
* \return  error code. 
*
********************************************************************************** */
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

/*! *********************************************************************************
* \brief  Read from the image storage (external memory or internal flash)
*
* \param[in] pData    pointer to the data chunk
* \param[in] length   the length of the data chunk
* \param[in] address  image storage address
*
* \return  error code. 
*
********************************************************************************** */
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


/*! *********************************************************************************
* \brief  Write into the image storage (external memory or internal flash)
*
* \param[in] pData    pointer to the data chunk
* \param[in] length   the length of the data chunk
* \param[in] address  image storage address
*
* \return  error code. 
*
********************************************************************************** */
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
/*! *********************************************************************************
* \brief  This function sends a request for a new image chunk over the serial interface
*
* \param[in] offset  image offset
* \param[in] len     requested chunk length
* \param[in] devId   Id of the device
*
********************************************************************************** */
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

    /* Copy data into the payload buffer */
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


/*! *********************************************************************************
* \brief  This function sends over the serial interface a query for a specific image
*
* \param[in] devId          Id of the device
* \param[in] manufacturer   The manufacturer code
* \param[in] imgType        Type of the image
* \param[in] fileVersion    Minimum version of the image
*
********************************************************************************** */
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

    /* Copy data into the payload buffer */
    idx = 0;
    FLib_MemCpy(&pPkt->structured.payload[idx], &devId, sizeof(devId));
    idx +=sizeof(devId);
    FLib_MemCpy(&pPkt->structured.payload[idx], &offset, sizeof(offset));
    idx +=sizeof(offset);
    FLib_MemCpy(&pPkt->structured.payload[idx], &len, sizeof(len));

    FSCI_transmitFormatedPacket(pPkt, mOtaFsciInterface);
}


/*! *********************************************************************************
* \brief  
*
* \param[in] pClientAddr    Pointer to the client address (16 bytes)
* \param[in] offset         
* \param[in] devId          Id of the device
*
********************************************************************************** */
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

      /* Copy data into the payload buffer */
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

/*! *********************************************************************************
* \brief  This function handles the FSCI messages for the OTA Server
*
* \param[in] pData    pointer to the received FSCI packet
*
* \return  TRUE if the received message should be recycled. 
*
********************************************************************************** */
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
                status = OTA_PushImageChunk(&pData->structured.payload[1], pData->structured.header.len-1, NULL, NULL);
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