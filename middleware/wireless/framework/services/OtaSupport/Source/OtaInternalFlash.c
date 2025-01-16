/*! *********************************************************************************
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the Source file for the EEPROM emulated inside the MCU's FLASH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#include "FunctionLib.h"
#include "OtaPrivate.h"
#include "fsl_adapter_flash.h"
#include "fwk_platform_ota.h"
#include <stdbool.h>

/******************************************************************************
*******************************************************************************
* Private Macros
*******************************************************************************
******************************************************************************/

#define RAISE_ERROR(x, val) \
    {                       \
        x = (val);          \
        break;              \
    }
/* Size of storage sector bit map array */
#define StorageBitmapSize \
    ((FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE - 1U) / (FSL_FEATURE_FLASH_PFLASH_SECTOR_SIZE * 32U) + 1U)

#if defined FSL_FEATURE_FLASH_PFLASH_PHRASE_SIZE
#define gEepromParams_WriteAlignment_c FSL_FEATURE_FLASH_PFLASH_PHRASE_SIZE
#endif

#ifndef gEepromParams_WriteAlignment_c
#define gEepromParams_WriteAlignment_c (1U)
#endif

/* Convert relative offset in internal storage into real address */
#define PHYS_ADDR(x) ((uint32_t)(x) + ota_internal_partition->start_offset)

#define IS_PHRASE_ALIGNED(addr)   (((addr) & (FSL_FEATURE_FLASH_PFLASH_PHRASE_SIZE - 1U)) == 0U)
#define IS_SECTOR_ALIGNED(addr)   (((addr) & (FSL_FEATURE_FLASH_PFLASH_SECTOR_SIZE - 1U)) == 0U)
#define CURRENT_SECTOR_ADDR(addr) (((addr) & ~(FSL_FEATURE_FLASH_PFLASH_SECTOR_SIZE - 1U)))
#define NEXT_SECTOR_ADDR(addr)    (CURRENT_SECTOR_ADDR(addr) + FSL_FEATURE_FLASH_PFLASH_SECTOR_SIZE)

/******************************************************************************
*******************************************************************************
* Private Prototypes
*******************************************************************************
******************************************************************************/
static ota_flash_status_t InternalFlash_PrepareForWrite(uint32_t NoOfBytes, uint32_t offs);
static ota_flash_status_t InternalFlash_EraseBlockBySectorNumber(uint32_t blk_nb);

static ota_flash_status_t InternalFlash_Init(void);
static ota_flash_status_t InternalFlash_PartitionErase(void);
static ota_flash_status_t InternalFlash_WriteData(uint32_t NoOfBytes, uint32_t offs, uint8_t *Outbuf);
static ota_flash_status_t InternalFlash_FlushWriteBuffer(void);
static ota_flash_status_t InternalFlash_ReadData(uint16_t NoOfBytes, uint32_t offs, uint8_t *inbuf);
static ota_flash_status_t InternalFlash_EraseArea(uint32_t *pOffs, int32_t *size, bool non_blocking);
static uint8_t            InternalFlash_isBusy(void);
#if defined               OtaDeprecatedFlashVerifyWrite_d && (OtaDeprecatedFlashVerifyWrite_d > 0)
static ota_flash_status_t InternalVerifyFlashProgram(uint8_t *pData, uint32_t offs, uint32_t length);
#endif
/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
******************************************************************************/

union physical_address
{
    uint32_t  value;
    uint32_t *pointer;
};

/******************************************************************************
*******************************************************************************
* Private Memory Declarations
*******************************************************************************
******************************************************************************/
static uint32_t mEepromEraseBitmap[StorageBitmapSize];
#if (gEepromParams_WriteAlignment_c > 1)
static uint8_t  mWriteBuff[gEepromParams_WriteAlignment_c];
static uint8_t  mWriteBuffLevel = 0U;
static uint32_t mWriteBuffOffs  = 0U;
#endif

/******************************************************************************
*******************************************************************************
* Public Memory
*******************************************************************************
******************************************************************************/

static const OtaFlashOps_t int_flash_ops = {
    .init           = &InternalFlash_Init,
    .format_storage = &InternalFlash_PartitionErase,
    .writeData      = &InternalFlash_WriteData,
    .readData       = &InternalFlash_ReadData,
    .isBusy         = &InternalFlash_isBusy,
    .eraseArea      = &InternalFlash_EraseArea,
    .flushWriteBuf  = &InternalFlash_FlushWriteBuffer,
};

static const OtaPartition_t *ota_internal_partition;

/******************************************************************************
*******************************************************************************
* Private Functions
*******************************************************************************
******************************************************************************/

/*! *********************************************************************************
 * \brief  Initialize External storage for OTA.
 *
 * \return   true range belongs to OTA partition false otherwise.
 *
 ***********************************************************************************/
static bool OtaCheckRangeBelongsToPartition(uint32_t offset, uint32_t range_sz)
{
    return ((offset + range_sz) <= ota_internal_partition->size);
}

/*! *********************************************************************************
 * \brief  Initialize internal storage for OTA.
 *
 * \return    kStatus_OTA_Flash_Success if successful, other values in case of error
 *
 ***********************************************************************************/
static ota_flash_status_t InternalFlash_Init(void)
{
    ota_flash_status_t status    = kStatus_OTA_Flash_Success;
    static bool        init_done = false;
    if (!init_done)
    {
        /*  Ensure that Storage BitMap size is sufficient to deal with the entire
            internal storage area : here the sector size is 8kB and the storage size
            is ~ 512kB */
        /* Wipe clean the EraseBitMap */
        FLib_MemSet(mEepromEraseBitmap, 0x00, StorageBitmapSize * 4U);
#if (gEepromParams_WriteAlignment_c > 1)
        FLib_MemSet(mWriteBuff, 0xff, gEepromParams_WriteAlignment_c);
        mWriteBuffLevel = 0;
        mWriteBuffOffs  = 0;
#endif

        status = (ota_flash_status_t)HAL_FlashInit();
    }
    return status;
}

/*! *********************************************************************************
 * \brief  Clean internal storage partition by erasing all sectors
 *
 * \return    kStatus_OTA_Flash_Success if successful, other values in case of error
 *
 ***********************************************************************************/
static ota_flash_status_t InternalFlash_PartitionErase(void)
{
    ota_flash_status_t status = kStatus_OTA_Flash_Success;
    uint32_t           i;

    for (i = 0; i < (ota_internal_partition->size / ota_internal_partition->sector_size); i++)
    {
        status = InternalFlash_EraseBlockBySectorNumber(i);
        if (kStatus_OTA_Flash_Success != status)
        {
            break;
        }
    }

    return status;
}

/*! *********************************************************************************
 * \brief  Writes a data buffer into internal storage, at a given address
 *
 * \param[in] NoOfBytes   Number of bytes to be written
 * \param[in] offs        offset address relative to start of Internal Storage
 * \param[in] Outbuf      pointer on buffer to be written
 *
 * \return    kStatus_OTA_Flash_Success if successful, other values in case of error
 ***********************************************************************************/
static ota_flash_status_t InternalFlash_WriteData(uint32_t NoOfBytes, uint32_t offs, uint8_t *Outbuf)
{
    ota_flash_status_t status = kStatus_OTA_Flash_Fail;
    do
    {
        hal_flash_status_t st;
        if (!OtaCheckRangeBelongsToPartition(offs, NoOfBytes))
        {
            RAISE_ERROR(status, kStatus_OTA_Flash_InvalidArgument);
        }
        if (0U == NoOfBytes)
        {
            RAISE_ERROR(status, kStatus_OTA_Flash_InvalidArgument);
        }

        status = InternalFlash_PrepareForWrite(NoOfBytes, offs);
        if (kStatus_OTA_Flash_Success != status)
        {
            break;
        }
#if (gEepromParams_WriteAlignment_c > 1)
        if (mWriteBuffLevel != 0U)
        {
            if ((offs >= mWriteBuffOffs) && (offs < mWriteBuffOffs + gEepromParams_WriteAlignment_c))
            {
                uint32_t offset = offs - mWriteBuffOffs;
                uint32_t size   = gEepromParams_WriteAlignment_c - offset;

                FLib_MemCpy(&mWriteBuff[offset], Outbuf, size);
                offs += size;
                Outbuf += size;
                if (NoOfBytes > size)
                {
                    NoOfBytes -= size;
                }
                else
                {
                    NoOfBytes = 0U;
                }
            }
            else
            {
                uint8_t size = gEepromParams_WriteAlignment_c - mWriteBuffLevel;
                FLib_MemSet(&mWriteBuff[mWriteBuffLevel], 0xFF, (uint32_t)size);
            }

            st = HAL_FlashProgramUnaligned(PHYS_ADDR(mWriteBuffOffs), sizeof(mWriteBuff), mWriteBuff);
            if (kStatus_HAL_Flash_Success != st)
            {
                RAISE_ERROR(status, kStatus_OTA_Flash_Fail);
            }

#if defined OtaDeprecatedFlashVerifyWrite_d && (OtaDeprecatedFlashVerifyWrite_d > 0)
            /* Do flash program verification where we best know what we did write */
            status = InternalVerifyFlashProgram(mWriteBuff, mWriteBuffOffs, (uint32_t)sizeof(mWriteBuff));
            if (kStatus_OTA_Flash_Success != status)
            {
                break;
            }
#endif
            mWriteBuffLevel = 0U;
        }

        if ((offs & (gEepromParams_WriteAlignment_c - 1U)) != 0U)
        {
            RAISE_ERROR(status, kStatus_OTA_Flash_AlignmentError);
        }
        /* Store unaligned bytes for later processing */
        mWriteBuffLevel = (uint8_t)NoOfBytes % gEepromParams_WriteAlignment_c;
        NoOfBytes -= mWriteBuffLevel;
        mWriteBuffOffs = offs + NoOfBytes;
        FLib_MemCpy(mWriteBuff, &Outbuf[NoOfBytes], mWriteBuffLevel);
#endif

        /* Write data to FLASH */
        st = HAL_FlashProgramUnaligned(PHYS_ADDR(offs), NoOfBytes, Outbuf);
        if (kStatus_HAL_Flash_Success != st)
        {
            RAISE_ERROR(status, kStatus_OTA_Flash_Fail);
        }

#if defined OtaDeprecatedFlashVerifyWrite_d && (OtaDeprecatedFlashVerifyWrite_d > 0)
        status = InternalVerifyFlashProgram(Outbuf, offs, NoOfBytes);
        if (kStatus_OTA_Flash_Success != status)
        {
            break;
        }
#endif
    } while (false);
    return status;
}

/*! *********************************************************************************
 * \brief  Writes remainder of 16 byte buffer to flash when terminating FW update
 *
 * \return    kStatus_OTA_Flash_Success if successful, other values in case of error
 ***********************************************************************************/
static ota_flash_status_t InternalFlash_FlushWriteBuffer(void)
{
    ota_flash_status_t status;
#if (gEepromParams_WriteAlignment_c > 1)
    status = kStatus_OTA_Flash_Fail;
    do
    {
        hal_flash_status_t st;
        uint8_t            size;

        if (mWriteBuffLevel == 0U)
        {
            status = kStatus_OTA_Flash_Success;
            break;
        }
        size = gEepromParams_WriteAlignment_c - mWriteBuffLevel;
        /* Pad the remainder of write buffer with 0xff fill */
        FLib_MemSet(&mWriteBuff[mWriteBuffLevel], 0xffu, (uint32_t)size);
        /* the write RAM Write Buffer to FLASH */
        st = HAL_FlashProgramUnaligned(PHYS_ADDR(mWriteBuffOffs), sizeof(mWriteBuff), mWriteBuff);
        if (kStatus_HAL_Flash_Success != st)
        {
            RAISE_ERROR(status, kStatus_OTA_Flash_Fail);
        }
#if defined OtaDeprecatedFlashVerifyWrite_d && (OtaDeprecatedFlashVerifyWrite_d > 0)
        /* Do flash program verification where we best know what we did write */
        status = InternalVerifyFlashProgram(mWriteBuff, mWriteBuffOffs, (uint32_t)sizeof(mWriteBuff));
        if (kStatus_OTA_Flash_Success != status)
        {
            break;
        }
#endif
        mWriteBuffLevel = 0U;
    } while (false);
#else
    status = kStatus_OTA_Flash_Success;
#endif
    return status;
}

/*! *********************************************************************************
 * \brief  Read data from an address pointing to internal flash to a RAM buffer
 *
 * \param[in] NoOfBytes   Number of bytes to be read
 * \param[in] offs        offset address relative to start of Internal Storage
 * \param[in] Outbuf      pointer on buffer to be write to
 *
 * \return    kStatus_OTA_Flash_Success if successful, other values in case of error
 ***********************************************************************************/
static ota_flash_status_t InternalFlash_ReadData(uint16_t NoOfBytes, uint32_t offs, uint8_t *inbuf)
{
    union physical_address read_address;
    ota_flash_status_t     status = kStatus_OTA_Flash_Success;

    if (!OtaCheckRangeBelongsToPartition(offs, NoOfBytes))
    {
        status = kStatus_OTA_Flash_InvalidArgument;
    }
    else
    {
        read_address.value = PHYS_ADDR(offs);
        FLib_MemCpy(inbuf, read_address.pointer, NoOfBytes);

#if (gEepromParams_WriteAlignment_c > 1)
        /* Copy data from write buffer if needed */
        for (uint32_t i = 0; i < mWriteBuffLevel; i++)
        {
            if ((mWriteBuffOffs + i) >= offs && (mWriteBuffOffs + i) < (offs + NoOfBytes))
            {
                inbuf[mWriteBuffOffs - offs + i] = mWriteBuff[i];
            }
        }
#endif
    }

    return status;
}

/*! *********************************************************************************
 * \brief  Return busy status
 *
 * \return    always false in the case of internal flash
 ***********************************************************************************/
static uint8_t InternalFlash_isBusy(void)
{
    return 0U;
}

/*! *********************************************************************************
 * \brief  Erase sector identified by its number in OTA partition.
 *
 * \return    kStatus_OTA_Flash_Success if operation successful,
              kStatus_OTA_Flash_Error  otherwise.

 ***********************************************************************************/
static ota_flash_status_t InternalFlash_EraseBlockBySectorNumber(uint32_t blk_nb)
{
    ota_flash_status_t status = kStatus_OTA_Flash_Error;
    hal_flash_status_t st;
    assert(blk_nb < (ota_internal_partition->size / ota_internal_partition->sector_size));
    st = HAL_FlashEraseSector(PHYS_ADDR(blk_nb * ota_internal_partition->sector_size),
                              ota_internal_partition->sector_size);
    if (kStatus_HAL_Flash_Success == st)
    {
        /* mark each erased sector as such in the erase bit map */
        /* If blocks are 8kBytes large, one byte of bitmap is required per 64kByte tranche */
        mEepromEraseBitmap[blk_nb / 32U] |= ((uint32_t)1U << (blk_nb % 32U));
        status = kStatus_OTA_Flash_Success;
    }

    return status;
}

/*! *********************************************************************************
 * \brief  Erase number of sectors required for an upcoming program operation
 *
 * \param[in] NoOfBytes   Number of bytes of area to be cleared already checked to be non 0
 *            will be rounded to a multiple of flash sectors
 * \param[in] offs        offset address relative to start of Internal Storage from which
 *            erase operation is required
 * \return    kStatus_OTA_Flash_Success if successful, other values in case of error
 ***********************************************************************************/
static ota_flash_status_t InternalFlash_PrepareForWrite(uint32_t NoOfBytes, uint32_t offs)
{
    ota_flash_status_t status = kStatus_OTA_Flash_Success;
    uint32_t           i;
    uint32_t           endBlk;
    uint32_t           end_offs;

    /* Obtain the first and last block that need to be erased */
    end_offs = (offs + NoOfBytes);
    if ((end_offs & (ota_internal_partition->sector_size - 1U)) != 0U)
    {
        /* If the address is in the middle of a block, round up to the next block
         * This gives the upper block limit, every blocks before this one will be erased but not this block
         * as it is outside the erase range */
        end_offs = ((end_offs / ota_internal_partition->sector_size) + 1U) * ota_internal_partition->sector_size;
    }
    endBlk = end_offs / ota_internal_partition->sector_size;

    for (i = offs / ota_internal_partition->sector_size; i < endBlk; i++)
    {
        /* Check if the block was previously erased */
        if ((mEepromEraseBitmap[i / 32U] & ((uint32_t)1U << (i % 32U))) == 0U)
        {
            status = InternalFlash_EraseBlockBySectorNumber(i);
            if (kStatus_OTA_Flash_Success != status)
            {
                break;
            }
        }
    }

    return status;
}

static ota_flash_status_t InternalFlash_EraseArea(uint32_t *pOffs, int32_t *size, bool non_blocking)
{
    ota_flash_status_t status;

    /* The erase operation in internal flash is necessarily blocking */
    NOT_USED(non_blocking);

    do
    {
        uint32_t remain_sz  = (uint32_t)*size;
        uint32_t erase_offs = *pOffs;
        if (!IS_SECTOR_ALIGNED(erase_offs))
        {
            RAISE_ERROR(status, kStatus_OTA_Flash_AlignmentError);
        }
        if (!OtaCheckRangeBelongsToPartition(erase_offs, remain_sz))
        {
            RAISE_ERROR(status, kStatus_OTA_Flash_InvalidArgument);
        }

        if (remain_sz != 0U)
        {
            status = InternalFlash_PrepareForWrite(remain_sz, erase_offs);
            if (status != kStatus_OTA_Flash_Success)
            {
                break;
            }
            erase_offs += remain_sz;
            /* In the case of internal flash the entire erase operation must
             * complete till the end
             */
            remain_sz = 0U;
        }
        *pOffs = NEXT_SECTOR_ADDR(erase_offs);
        *size  = (int32_t)remain_sz;
        status = kStatus_OTA_Flash_Success;
    } while (false);

    return status;
}
#if defined               OtaDeprecatedFlashVerifyWrite_d && (OtaDeprecatedFlashVerifyWrite_d > 0)
static ota_flash_status_t InternalVerifyFlashProgram(uint8_t *pData, uint32_t offs, uint32_t length)
{
    ota_flash_status_t     status = kStatus_OTA_Flash_Success;
    union physical_address verify_address;
    verify_address.value = PHYS_ADDR(offs);
    if (!FLib_MemCmp(pData, verify_address.pointer, length))
    {
        status = kStatus_OTA_Flash_Fail;
    }
    return status;
}
#endif

otaResult_t OTA_SelectInternalStoragePartition(void)
{
    otaResult_t    status = gOtaInternalFlashError_c;
    OtaStateCtx_t *hdl    = &mOtaHdl;
    do
    {
        hal_flash_status_t flashInitStatus;

        if (hdl->FwUpdImageState == OtaImgState_Acquiring)
        {
            RAISE_ERROR(status, gOtaInvalidOperation_c);
        }

        flashInitStatus = HAL_FlashInit();

        if (flashInitStatus != kStatus_HAL_Flash_Success)
        {
            RAISE_ERROR(status, gOtaError_c);
        }

        OTA_DEBUG_TRACE("Select Internal flash\r\n");

        ota_internal_partition = PLATFORM_OtaGetOtaInternalPartitionConfig();
        if (ota_internal_partition == NULL)
        {
            RAISE_ERROR(status, gOtaInvalidParam_c);
        }
        hdl->ota_partition = ota_internal_partition;
        hdl->FlashOps      = &int_flash_ops;
        /* This might happen if target is not generated with gUseInternalStorageLink_d=1 */
        assert(0UL != hdl->ota_partition->size && 0UL != hdl->ota_partition->sector_size);

        hdl->ImageOffset       = PLATFORM_OtaGetImageOffset();
        hdl->MaxImageLength    = hdl->ota_partition->size - hdl->ImageOffset;
        hdl->ErasedUntilOffset = 0U;

        status = gOtaSuccess_c;

        /* Try to initialize the OTA Storage */
        if (hdl->FlashOps->init() != kStatus_OTA_Flash_Success)
        {
            RAISE_ERROR(status, gOtaInternalFlashError_c);
        }

    } while (false);

    return status;
}
