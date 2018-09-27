/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SEMC_NAND_MEMORY_H__
#define __SEMC_NAND_MEMORY_H__

#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
#include "semc/fsl_semc.h"
#include "semc_nand/semc_nand_flash.h"

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

#define SEMC_NAND_4K_SIZE        4096U
#define SEMC_NAND_PAGE_SIZE_2K   2048U
#define SEMC_NAND_PHYSICAL_PAGE_SIZE_MAX (8*1024)

#define SEMC_NAND_BAD_BLOCKS_MAX_NUM 256
#define SEMC_NAND_FW_MAX_NUM         8

#define SEMC_NAND_FCB_TAG                 0x4E464342U     //!< ASCII: "NFCB"
#define SEMC_NAND_FCB_VERSION             0x00000001      //!< Version: 1.0
#define SEMC_NAND_DBBT_TAG                0x44424254U     //!< ASCII: "DBBT"
#define SEMC_NAND_DBBT_VERSION            0x00000001      //!< Version: 1.0

typedef enum _semc_nand_property
{
    kSemcNandProperty_InitStatus = 0,
    kSemcNandProperty_StartAddress = 1,            //!< Tag used to retrieve start address
    kSemcNandProperty_TotalFlashSizeInKBytes = 2, //!< Tag used to retrieve total flash size in terms of KByte
    kSemcNandProperty_PageSize = 3,                //!< Tag used to retreive page size in terms of byte
    kSemcNandProperty_BlockSize = 5,               //!< Tag used to retrieve block size in terms of byte

    kSemcNandProperty_TotalFlashSize = 0x10, //!< Tag used to retrieve total flash size in terms of byte
} semc_nand_property_t;

typedef struct _nand_firmware_info
{
    uint32_t startPage;
    uint32_t pagesInFirmware;
}nand_firmware_info_t;

typedef struct _semc_bcb_header
{
    uint32_t crcChecksum;                           //!< [0x000-0x003]
    uint32_t fingerprint;                           //!< [0x004-0x007]
    uint32_t version;                               //!< [0x008-0x00b]
}semc_bcb_header_t;

typedef struct _semc_nand_fcb
{
    semc_bcb_header_t bcbHeader;                    //!< [0x000-0x00b]
    uint32_t DBBTSerachAreaStartPage;               //!< [0x00c-0x00f]
    uint16_t searchStride;                          //!< [0x010-0x011] Not used by ROM
    uint16_t searchCount;                           //!< [0x012-0x013] Not used by ROM
    uint32_t firmwareCopies;                        //!< [0x014-0x017]
    uint32_t reserved0[10];                         //!< [0x018-0x03f]
    nand_firmware_info_t firmwareTable[SEMC_NAND_FW_MAX_NUM];//!< [0x040-0x07f]
    uint32_t reserved1[32];                         //!< [0x080-0x0ff];
    semc_nand_config_t nandConfig;                  //!< [0x100-0x1ff];
    uint32_t reserved2[128];                        //!< [0x200-0x3ff];
}semc_nand_fcb_t;

//! @brief SEMC Parallel NAND bad block info
typedef struct  _semc_nand_dbbt
{
    semc_bcb_header_t bcbHeader;                          //!< [0x000-0x00b]
    uint32_t reserved0;                                   //!< [0x00c-0x00f]
    uint32_t badBlockNumber;                              //!< [0x010-0x013]
    uint32_t reserved1[3];                                //!< [0x014-0x01f]
    uint32_t badBlockTable[SEMC_NAND_BAD_BLOCKS_MAX_NUM]; //!< [0x020-0x41f]
}semc_nand_dbbt_t;

//! SEMC NAND image Option
typedef struct _semc_nand_img_option
{
    parallel_nand_config_option_t nandOption;
    union
    {
        struct
        {
            uint32_t searchCount : 4;       //!< Search Count, Minimum 1
            uint32_t reserved0 : 4;
            uint32_t searchStrideBlk : 8;   //!< Search Stride in terms of block.
            uint32_t imgCopies : 4;         //!< Image copies, Minimum 1
            uint32_t reserved1 : 4;
            uint32_t reserved2 : 8;
        } B;
        uint32_t U;
    } bcbOption;
    struct
    {
        uint16_t blockCount;    //!< Maximum block count for an application image
        uint16_t blockIndex;    //!< Image start block ID
    } imageInfo[SEMC_NAND_FW_MAX_NUM];
} semc_nand_img_option_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif

//! @name SPI NAND memory
//@{

//! @brief Initialize SEMC NAND memory
status_t semc_nand_mem_init(void);

//! @brief Configure SEMC NAND memory
status_t semc_nand_mem_config(uint32_t *config);

//! @brief Read SEMC NAND memory.
status_t semc_nand_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer);

//! @brief Write SEMC NAND memory.
status_t semc_nand_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer);

//! @brief  Erase SEMC NAND memory
status_t semc_nand_mem_erase(uint32_t address, uint32_t length);

//! @brief Erase all SEMC NAND memory
status_t semc_nand_mem_erase_all(void);

//! @brief flush cached data to SEMC NAND memory
status_t semc_nand_mem_flush(void);

//! @brief finalize the read/write operation of SEMC NAND memory
status_t semc_nand_mem_finalize(void);

status_t semc_nand_get_fcb_search_cfg(uint32_t *searchCount, uint32_t *searchStride);

status_t semc_nand_get_default_config_block(semc_nand_config_t *config);

//@}

#if defined(__cplusplus)
}
#endif
#endif /* #ifndef __SEMC_NAND_MEMORY_H__ */

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
