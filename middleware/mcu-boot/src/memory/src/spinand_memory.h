/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(__SPINAND_MEMORY_H__)
#define __SPINAND_MEMORY_H__

#include "flexspi_nand_flash.h"
#include "memory.h"

#if BL_FEATURE_SPINAND_MODULE
////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////
enum
{
    //! @brief Max page size used to create a page buffer
    kSpiNandMemory_MaxPageSize = 4096U,
    //! @brief Max items in DBBT
    kSpiNandMemory_MaxDBBTSize = 256U,
    //! @brief Max numbers of firmware defined in FCB
    kSpiNandMemory_MaxFirmware = 8U,
};

enum
{
    kTag_SPINAND_FCB = 0x4E464342U,  // ASCII: "NFCB"
    kTag_SPINAND_FCB_Version = 0x1,  // Version: 1.0
    kTag_SPINAND_DBBT = 0x44424254U, // ASICII: DBBT
    kTag_SPINAND_DBBT_Version = 0x1, // Version: 1.0
};

typedef enum _spinand_property
{
    kSPINANDProperty_InitStatus = 0,
    kSPINANDProperty_StartAddress = 1,            //!< Tag used to retrieve start address
    kSPINANDrProperty_TotalFlashSizeInKBytes = 2, //!< Tag used to retrieve total flash size in terms of KByte
    kSPINANDProperty_PageSize = 3,                //!< Tag used to retreive page size in terms of byte
    kSPINANDProperty_SectorSize = 4,              //!< Tag used to retrieve sector size in term of byte
    kSPINANDProperty_BlockSize = 5,               //!< Tag used to retrieve block size in terms of byte

    kSPINANDProperty_TotalFlashSize = 0x10, //!< Tag used to retrieve total flash size in terms of byte
} spinand_property_t;

typedef struct _firmware_info
{
    uint32_t startPage;
    uint32_t pagesInFirmware;
} firmware_info_t;

typedef struct _spinand_fcb
{
    uint32_t crcChecksum;                                      //!< [0x000-0x003]
    uint32_t fingerprint;                                      //!< [0x004-0x007]
    uint32_t version;                                          //!< [0x008-0x00b]
    uint32_t DBBTSerachAreaStartPage;                          //!< [0x00c-0x00f]
    uint16_t searchStride;                                     //!< [0x010-0x011] Not used by ROM
    uint16_t searchCount;                                      //!< [0x012-0x013] Not used by ROM
    uint32_t firmwareCopies;                                   //!< [0x014-0x017]
    uint32_t reserved0[10];                                    //!< [0x018-0x03f]
    firmware_info_t firmwareTable[kSpiNandMemory_MaxFirmware]; //!< [0x040-0x07f]
    uint32_t reserved1[32];                                    //!< [0x080-0x0ff];
#if BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    flexspi_nand_config_t config; //!< [0x100-0x2ff];
#else
#error "Unknown SPI NAND peripheral configuration."
#endif                      // BL_FEATURE_SPINAND_MODULE_PERIPHERAL_FLEXSPI
    uint32_t reserved2[64]; //!< [0x300-0x3ff];
} spinand_fcb_t;

typedef struct _spinand_dbbt
{
    uint32_t crcChecksum;                               //!< [0x000-0x003]
    uint32_t fingerprint;                               //!< [0x003-0x007]
    uint32_t version;                                   //!< [0x008-0x00b]
    uint32_t reserved0;                                 //!< [0x00c-0x00f]
    uint32_t badBlockNumber;                            //!< [0x010-0x013]
    uint32_t reserved1[3];                              //!< [0x014-0x01f]
    uint32_t badBlockTable[kSpiNandMemory_MaxDBBTSize]; //!< [0x020-0x41f]
} spinand_dbbt_t;

typedef struct _spinand_img_option
{
    union
    {
        struct
        {
            uint32_t size : 4;          //!< Size in longword
            uint32_t reserved0 : 4;     //!< Reserved for future use
            uint32_t address_type : 4;  //!< NAND Adress type: 0 - byte address, 1 - block address
            uint32_t reserved1 : 8;     //!< Reserved for future use
            uint32_t search_stride : 4; //!< Search Stride: 0-64 pages, 1-128 pages, 2-256 pages, 3-32 pages.
            uint32_t search_count : 4;  //!< 1 - 1, 2 -2 ,3 - 3, 4 - 4
            uint32_t tag : 4;           //!< tag, fixed to 0x0E
        } B;
        uint32_t U;
#if defined(BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL) && \
    BL_FEATURE_SPINAND_MODULE_PERIPHERAL_INSTANCE_RUNTIME_SEL
        struct
        {
            uint32_t cf9_field : 20; //!< CFG data field
            uint32_t magic : 12;     //!< Magic, must be 0xcf9 (standing for "CFG")
        } P;                         // Preconfig
#endif
    } option0;
    uint32_t nand_option_addr;
    struct
    {
        uint16_t block_count; //!!< Maximum block count for an application image
        uint16_t block_id;    //!< Image start block ID
    } image_info[kSpiNandMemory_MaxFirmware];
} spinand_img_option_t;

enum
{
    kNandImgOption_Tag = 0x0C,
    kNandImgOption_MinSize = ((uint32_t) & (((spinand_img_option_t *)0)->image_info[1])) / sizeof(uint32_t),
    kNandImgOption_MaxSize = sizeof(spinand_img_option_t) / sizeof(uint32_t),
};

#define MAGIC_NUMBER_SPINAND_PRECFG \
    (0xcf9 | (kNandImgOption_Tag << 8) & (kNandImgOption_Tag << 8))

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

//! @name SPI NAND memory
//@{

//! @brief Initialize SPI NAND memory
status_t spinand_mem_init(void);

//! @brief Configure SPI NAND memory
status_t spinand_mem_config(uint32_t *config);

//! @brief Read SPI NAND memory.
status_t spinand_mem_read(uint32_t address, uint32_t length, uint8_t *restrict buffer);

//! @brief Write SPI NAND memory.
status_t spinand_mem_write(uint32_t address, uint32_t length, const uint8_t *buffer);

//! @brief  Erase SPI NAND memory
status_t spinand_mem_erase(uint32_t address, uint32_t length);

//! @brief Erase all SPI NAND memory
status_t spinand_mem_erase_all(void);

//! @brief flush cached data to SPI NAND memory
status_t spinand_mem_flush(void);

//! @brief finalize the read/write operation of SPI NAND memory
status_t spinand_mem_finalize(void);

//@}

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // BL_FEATURE_SPINAND_MODULE
#endif // __SPINAND_MEMORY_INTERFACE_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
