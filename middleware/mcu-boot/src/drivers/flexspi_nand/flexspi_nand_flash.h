/*
 * Copyright 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FLEXSPI_NAND_FLASH_H__
#define __FLEXSPI_NAND_FLASH_H__

#include "fsl_common.h"
#include "bootloader_common.h"
#include "flexspi/fsl_flexspi.h"

/**********************************************************************************************************************
 * Definitions
 *********************************************************************************************************************/

/*! @brief Status for FLEXSPI NAND driver */
enum _serial_nand_status
{
    kStatus_FlexSPINAND_ReadPageFail = MAKE_STATUS(kStatusGroup_FlexSPINAND, 0), /*! < FLEXSPI NAND Read Page Failure */
    kStatus_FlexSPINAND_ReadCacheFail =
        MAKE_STATUS(kStatusGroup_FlexSPINAND, 1), /*! < FLEXSPI NAND Read Cache Failure */
    kStatus_FlexSPINAND_EccCheckFail = MAKE_STATUS(kStatusGroup_FlexSPINAND, 2), /*! < FLEXSPI NAND ECC Cehck Failure */
    kStatus_FlexSPINAND_PageLoadFail = MAKE_STATUS(kStatusGroup_FlexSPINAND, 3), /*! < FLEXSPI NAND Page Load Failure */
    kStatus_FlexSPINAND_PageExecuteFail =
        MAKE_STATUS(kStatusGroup_FlexSPINAND, 4), /*! < FLEXSPI NAND Page Execute Failure */
    kStatus_FlexSPINAND_EraseBlockFail =
        MAKE_STATUS(kStatusGroup_FlexSPINAND, 5), /*! < FLEXSPI NAND Erase Block Failure */
    kStatus_FlexSPINAND_WaitTimeout = MAKE_STATUS(kStatusGroup_FlexSPINAND, 6), /*! < FLEXSPI NAND Write Timeout */

    kStatus_FlexSPINAND_NotSupported = MAKE_STATUS(kStatusGroup_FlexSPINAND, 7), // PageSize over the max buffer size
    kStatus_FlexSPINAND_FcbUpdateFail = MAKE_STATUS(kStatusGroup_FlexSPINAND, 8),
    kStatus_FlexSPINAND_DbbtUpdateFail = MAKE_STATUS(kStatusGroup_FlexSPINAND, 9),
    kStatus_FlexSPINAND_WriteAlignmentError = MAKE_STATUS(kStatusGroup_FlexSPINAND, 10),
    kStatus_FlexSPINAND_NotFound = MAKE_STATUS(kStatusGroup_FlexSPINAND, 11),
};

/*
 *  NAND Command index used for customized LUT entries.
 */
#define NAND_CMD_INDEX_READCHACHE CMD_INDEX_READ
#define NAND_CMD_INDEX_READSTATUS CMD_INDEX_READSTATUS
#define NAND_CMD_INDEX_WRITEENABLE CMD_INDEX_WRITEENABLE
#define NAND_CMD_INDEX_ERASEBLOCK 3
#define NAND_CMD_INDEX_PROGRAMLOAD CMD_INDEX_WRITE
#define NAND_CMD_INDEX_READPAGE 5
#define NAND_CMD_INDEX_READECCSTAT 6
#define NAND_CMD_INDEX_PROGRAMEXECUTE 7
#define NAND_CMD_INDEX_READCACHE_ODD 8
#define NAND_CMD_INDEX_PROGRAMLOAD_ODD 9

/*
 * NAND LUT sequence index used for default LUT assignment
 * NOTE:
 *      The will take effect if the lut sequences are not customized.
 */
/* LUT sequence index for READ cache sequence  */
#define NAND_CMD_LUT_SEQ_IDX_READCACHE CMD_LUT_SEQ_IDX_READ //!< 0
/* LUT sequence index for Read Status sequence */
#define NAND_CMD_LUT_SEQ_IDX_READSTATUS CMD_LUT_SEQ_IDX_READSTATUS //!< 1
/* LUT sequence index for write enable sequence */
#define NAND_CMD_LUT_SEQ_IDX_WRITEENABLE CMD_LUT_SEQ_IDX_WRITEENABLE //!< 3
/* LUT sequence index for Read cache for odd blocks */
#define NAND_CMD_LUT_SEQ_IDX_READCACHE_ODD 4 //!< 4
/* LUT sequence index for erase block */
#define NAND_CMD_LUT_SEQ_IDX_ERASEBLOCK 5 //!< 5
/* LUT sequence index for program load */
#define NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD CMD_LUT_SEQ_IDX_WRITE //!< 9
/* LUT sequence index for program load for odd blocks */
#define NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD_ODD 10 //!< 10
/**/
#define NAND_CMD_LUT_SEQ_IDX_READPAGE 11 //!< 11
/* LUT sequence index for read ecc status  */
#define NAND_CMD_LUT_SEQ_IDX_READECCSTAT 13 //!< 13
/* LUT sequence index for program execute */
#define NAND_CMD_LUT_SEQ_IDX_PROGRAMEXECUTE 14 //!< 14

//!@brief Serial NAND Command Definitions
enum
{
    kSerialNandCmd_WriteEnable = 0x06,
    kSerialNandCmd_SetFeatures = 0x1F,
    kSerialNandCmd_GetFeatures = 0x0F,
    kSerialNandCmd_PageRead = 0x13,
    kSerialNandCmd_ReadFromCache = 0x0B,
    kSerialNandCmd_ReadFromCacheX4 = 0x6B,
    kSerialNandCmd_ReadId = 0x9F,
    kSerialNandCmd_BlockErase = 0xD8,
    kSerialNandCmd_ProgramLoad = 0x02,
    kSerialNandCmd_ProgramLoadX4 = 0x32,
    kSerialNandCmd_ProgramExecute = 0x10,
};

/*
 *   NAND flash config block over FlexSPI
 */
typedef struct _flexspi_nand_config
{
    flexspi_mem_config_t memConfig; //!< [0x000-0x1bf] Generic Memory configuration block over FlexSPI
    uint32_t pageDataSize;          //!< [0x1c0-0x1c3] Data Size in one page, usually it is 2048 or 4096
    uint32_t pageTotalSize;   //!< [0x1c4-0x1c7] Total size in one page, usually, it equals 2 ^ width of column address
    uint32_t pagesPerBlock;   //!< [0x1c8-0x1cb] Pages per block
    uint8_t bypassReadStatus; //!< [0x1cc-0x1cc] Bypass Read Status
    uint8_t bypassEccRead;    //!< [0x1cd-0x1cd] Bypass ECC check
    uint8_t hasMultiPlanes;   //!< [0x1ce-0x1ce] Has multiple planes
    uint8_t skipOddBlocks;    //!< [0x1cf-0x1cf] Skip odd blocks
    uint8_t eccCheckCustomEnable; //!< [0x1d0-0x1d0] Determine if use the customized ECC check related masks
    uint8_t ipCmdSerialClkFreq;   //!< [0x1d1-0x1d1] IP Command Serial Clock Frequency, keep 0
    uint16_t readPageTimeUs;  //!< [0x1d2-0x1d3] Time needed for read page to cache, effective when bypassReadStatus=1
    uint32_t eccStatusMask;   //!< [0x1d4-0x1d7] ECC status mask
    uint32_t eccFailureMask;  //!< [0x1d8-0x1db] ECC failure mask
    uint32_t blocksPerDevice; //!< [0x1dc-0x1df] Blocks per device, not used in ROM
    uint32_t reserved1[8];    //!< [0x1e0-0x1ff] Reserved for future use.
} flexspi_nand_config_t;

enum
{
    kSerialNandPagesPerBlock_64 = 0,
    kSerialNandPagesPerBlock_128 = 1,
    kSerialNandPagesPerBlock_256 = 2,
    kSerialNandPagesPerBlock_32 = 3,
};

enum
{
    kSerialNandSearchStridePages_64 = 0,
    kSerialNandSearchStridePages_128 = 1,
    kSerialNandSearchStridePages_256 = 2,
    kSerialNandSearchStridePages_32 = 3,
};

enum
{
    kSerialNandPageSize_2KB = 2,
    kSerialNandPageSize_4KB = 4,
};

enum
{
    kSerialNandOption_Tag = 0x0C,
};

typedef struct
{
    union
    {
        struct
        {
            uint32_t max_freq : 4;        //!< Device specific definitions, see System Boot Chapter for more details
            uint32_t page_size_in_kb : 4; //!< 2 - 2048bytes, 4 - 4096 Bytes
            uint32_t pages_per_block : 4; //!< 0 - 64, 1 - 128, 2 - 256, 3 - 32
            uint32_t has_multiplanes : 4; //!< 0 - no , 1 - yes
            uint32_t flash_size : 4;      //!< 0 - 512Mbit, 1 - 1Gbit, 2 - 2Gbit, 4 - 4Gbit, 8 - 8Gbit
            uint32_t reserved : 4;        //!< Reserved for future use
            uint32_t option_size : 4;     //!< Option size : (option_size + 1) * 4 bytes
            uint32_t tag : 4;             //!< Fixed to 0x0E
        } B;
        uint32_t U;
    } option0;

    union
    {
        struct
        {
            uint32_t manufacturer_id : 8; //!< Manufacturer ID
            uint32_t eccFailureMask : 8;  //!< Ecc Failure Mask
            uint32_t eccCheckMask : 8;    //!< Ecc Check Mask
            uint32_t reserved : 8;        //!< Reserved for future use.
        } B;
        uint32_t U;
    } option1;
} serial_nand_config_option_t;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * API
 *********************************************************************************************************************/

/*
 * !@brief Initialize Serial NAND connected via FlexSPI
 *
 * This function intializes the Serial NAND deices with specified config parameters
 *
 * @param instance FlexSPI instance
 * @param config Serial NAND configuration paramters via FlexSPI
 */
status_t flexspi_nand_init(uint32_t instance, flexspi_nand_config_t *config);

/*
 * !@brief Read content from specified page via FlexSPI
 *
 * This function reads content from specified page
 *
 * @param instance FlexSPI instance
 * @param config Serial NAND configuration paramters via FlexSPI
 * @param pageId Specified page Index
 * @param buffer destination buffer for page read
 * @param length specified length of data to be read
 */
status_t flexspi_nand_read_page(
    uint32_t instance, flexspi_nand_config_t *config, uint32_t pageId, uint32_t *buffer, uint32_t length);

/*
 * !@brief Erase a block from Serial NAND via FlexSPI
 *
 * This function erase specified block from Serial NAND
 *
 * @param instance FlexSPI instance
 * @param config Serial NAND configuration paramters via FlexSPI
 * @param blockId Specified block Index
 */
//!@brief Erase a Serial NAND Block via FlexSPI
status_t flexspi_nand_erase_block(uint32_t instance, flexspi_nand_config_t *config, uint32_t blockId);

/*
 * !@brief Erase several pages from Serial NAND via FlexSPI
 *
 * This function erase specified block from Serial NAND
 *
 * @param instance FlexSPI instance
 * @param config Serial NAND configuration paramters via FlexSPI
 * @param pageStart Specified the first page to be erased
 * @param pages Total page counts to be erased.
 */
status_t flexspi_nand_erase(uint32_t instance, flexspi_nand_config_t *config, uint32_t pageStart, uint32_t pages);

/*
 * !@brief Program data to specified page via FlexSPI
 *
 * This function program content to specified page
 *
 * @param instance FlexSPI instance
 * @param config Serial NAND configuration paramters via FlexSPI
 * @param pageId Specified page Index
 * @param src Start pointer for data to be programmed.
 * @param length specified length of data to be programmed
 */
//!@brief Program data to specified Serial NAND page via FlexSPI
status_t flexspi_nand_program_page(
    uint32_t instance, flexspi_nand_config_t *config, uint32_t pageId, uint32_t *src, uint32_t length);

//
status_t flexspi_nand_get_fcb_search_cfg(uint32_t *searchCount, uint32_t *searchStride);

status_t flexspi_nand_get_default_cfg_blk(flexspi_nand_config_t *config);

status_t flexspi_nand_get_config(uint32_t instance, flexspi_nand_config_t *config, serial_nand_config_option_t *option);

#ifdef __cplusplus
}
#endif

#endif // __FLEXSPI_NAND_FLASH_H__
