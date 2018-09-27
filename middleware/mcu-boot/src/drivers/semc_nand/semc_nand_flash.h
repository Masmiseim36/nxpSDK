/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SEMC_NAND_FLASH_H__
#define __SEMC_NAND_FLASH_H__

#include "fsl_common.h"
#include "bootloader_common.h"
#include "semc/fsl_semc.h"
#include "nand_ecc/nand_ecc.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define NAND_MEM_BLANK_VALUE  (0xFFU)

enum
{
    kParallelNandConfigOption_Tag = 0x0d,
};

//! @brief SEMC Parallel NAND Status Group
/*
enum
{
    kStatusGroup_SemcNAND = 212,
};
*/

//! @brief Status for SEMC Parallel NAND driver
enum _semc_nand_status
{
    kStatus_SemcNAND_DeviceTimeout    = MAKE_STATUS(kStatusGroup_SemcNAND, 0),
    kStatus_SemcNAND_InvalidMemoryAddress    = MAKE_STATUS(kStatusGroup_SemcNAND, 1),
    kStatus_SemcNAND_NotEqualToOnePageSize    = MAKE_STATUS(kStatusGroup_SemcNAND, 2),
    kStatus_SemcNAND_MoreThanOnePageSize    = MAKE_STATUS(kStatusGroup_SemcNAND, 3),
    kStatus_SemcNAND_EccCheckFail    = MAKE_STATUS(kStatusGroup_SemcNAND, 4),
    kStatus_SemcNAND_InvalidOnfiParameter    = MAKE_STATUS(kStatusGroup_SemcNAND, 5),
    kStatus_SemcNAND_CannotEnableDeviceEcc    = MAKE_STATUS(kStatusGroup_SemcNAND, 6),
    kStatus_SemcNAND_SwitchTimingModeFailure    = MAKE_STATUS(kStatusGroup_SemcNAND, 7),
    kStatus_SemcNAND_ProgramVerifyFailure    = MAKE_STATUS(kStatusGroup_SemcNAND, 8),
    kStatus_SemcNAND_EraseVerifyFailure    = MAKE_STATUS(kStatusGroup_SemcNAND, 9),
    kStatus_SemcNAND_InvalidReadbackBuffer    = MAKE_STATUS(kStatusGroup_SemcNAND, 10),
    kStatus_SemcNAND_CannotDisableDeviceEcc    = MAKE_STATUS(kStatusGroup_SemcNAND, 11),

    kStatus_SemcNAND_InvalidCfgTag    = MAKE_STATUS(kStatusGroup_SemcNAND, 0x10),
    kStatus_SemcNAND_FailToUpdateFcb    = MAKE_STATUS(kStatusGroup_SemcNAND, 0x11),
    kStatus_SemcNAND_FailToUpdateDbbt    = MAKE_STATUS(kStatusGroup_SemcNAND, 0x12),
    kStatus_SemcNAND_DisallowOverwriteBcb    = MAKE_STATUS(kStatusGroup_SemcNAND, 0x13),
    kStatus_SemcNAND_OnlySupportOnfiDevice    = MAKE_STATUS(kStatusGroup_SemcNAND, 0x14),
    kStatus_SemcNAND_MoreThanMaxImageCopy    = MAKE_STATUS(kStatusGroup_SemcNAND, 0x15),
    kStatus_SemcNAND_DisorderedImageCopies    = MAKE_STATUS(kStatusGroup_SemcNAND, 0x16),
};

//! @brief SEMC Parallel NAND Flash vendor type
enum _semc_nand_vendor_type_index
{
    kSemcNandVendorType_Micron = 0U,
    kSemcNandVendorType_Spansion = 1U,
    kSemcNandVendorType_Samsung = 2U,
    kSemcNandVendorType_Winbond = 3U,
    kSemcNandVendorType_Hynix = 4U,
    kSemcNandVendorType_Toshiba = 5U,
    kSemcNandVendorType_Macronix = 6U,
    kSemcNandVendorType_Unknown = 7U,
};

//! @brief SEMC Parallel NAND Flash cell technology
enum _semc_nand_cell_technology
{
    kSemcNandCellTechnology_SLC = 0U,
    kSemcNandCellTechnology_MLC = 1U,
};

//! @brief Parallel NAND Flash ONFI Version
enum _nand_onfi_version
{
    kNandOnfiVersion_None = 0U,
    kNandOnfiVersion_1p0 = 1U,
    kNandOnfiVersion_2p0 = 2U,
    kNandOnfiVersion_3p0 = 3U,
    kNandOnfiVersion_4p0 = 4U,
};

//! @brief Parallel NAND Flash AC timing mode
enum _nand_ac_timing_table_index
{
    kNandAcTimingTableIndex_UserDefined = 0U,
    kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz = 1U,
    kNandAcTimingTableIndex_ONFI_1p0_Mode1_20MHz = 2U,
    kNandAcTimingTableIndex_ONFI_1p0_Mode2_28MHz = 3U,
    kNandAcTimingTableIndex_ONFI_1p0_Mode3_33MHz = 4U,
    kNandAcTimingTableIndex_ONFI_1p0_Mode4_40MHz = 5U,
    kNandAcTimingTableIndex_ONFI_1p0_Mode5_50MHz = 6U,
    kNandAcTimingTableIndex_ONFI_1p0_FastestMode = 7U,
};

//! @brief SEMC Parallel NAND Flash ecc check option
enum _semc_nand_ecc_check_option
{
    kSemcNandEccCheckOption_Enabled = 0U,
    kSemcNandEccCheckOption_Disabled = 1U,
};

//! @brief SEMC Parallel NAND Flash ecc check type
enum _semc_nand_ecc_check_type
{
    kSemcNandEccCheckType_SoftwareECC = 0U,
    kSemcNandEccCheckType_DeviceECC = 1U,
};

//! @brief SEMC Parallel NAND Flash device ecc status
typedef enum _semc_nand_device_ecc_status
{
    kSemcNandDeviceEccStatus_Enabled = 0U,
    kSemcNandDeviceEccStatus_Disabled = 1U,
} semc_nand_device_ecc_status_t;

//! @brief SEMC Parallel NAND Flash sw ecc algorithm
enum _semc_nand_sw_ecc_algorithm
{
    kSemcNandSwEccAlgorithm_SEC_HammingCode = 0U,
    kSemcNandSwEccAlgorithm_SECDED_HsiaoCode = 1U,
    kSemcNandSwEccAlgorithm_SECDEDSBD_ReddyCode = 2U,
    kSemcNandSwEccAlgorithm_SBCDBD_FiniteFieldCode = 3U,
    kSemcNandSwEccAlgorithm_DECTED_BoseChaudhuriHocquenghemCode = 4U,
    kSemcNandSwEccAlgorithm_BCH40 = 5U,
    kSemcNandSwEccAlgorithm_ReedSolomon = 6U,
};

//! @brief SEMC Parallel NAND Flash Ready check option
typedef enum _semc_nand_ready_check_option
{
    kSemcNandReadyCheckOption_SR = 0U,    // Via Status Register
    kSemcNandReadyCheckOption_RB = 1U,    // Via R/B# signal
} semc_nand_ready_check_option_t;

//! @brief SEMC Parallel NAND Flash Status Command Type
typedef enum _semc_nand_status_command_type
{
    kSemcNandStatusCommandType_Common = 0U,
    kSemcNandStatusCommandType_Enhanced = 1U,
} semc_nand_status_command_type_t;

//! @brief SEMC Parallel NAND Flash readback verify option
enum _semc_nand_readback_verify_option
{
    kSemcNandReadbackVerifyOption_Enabled = 0U,
    kSemcNandReadbackVerifyOption_Disabled = 1U,
};

typedef struct __nand_ac_timing_parameter
{
    uint16_t min_tCS_ns;
    uint16_t min_tCH_ns;
    uint16_t min_tCEITV_ns;
    uint16_t min_tWP_ns;
    uint16_t min_tWH_ns;
    uint16_t min_tRP_ns;
    uint16_t min_tREH_ns;
    uint16_t min_tTA_ns;
    uint16_t min_tWHR_ns;
    uint16_t min_tRHW_ns;
    uint16_t min_tADL_ns;
    uint16_t min_tRR_ns;
    uint16_t max_tWB_ns;
    uint16_t reserved;
} nand_ac_timing_parameter_t;

//! @brief SEMC Parallel NAND Config block structure
typedef struct __semc_nand_config
{
    semc_mem_config_t memConfig;             //!< [0x000-0x04f]
    uint8_t vendorType;                      //!< [0x050-0x050]
    uint8_t cellTechnology;
    uint8_t onfiVersion;
    uint8_t acTimingTableIndex;
    uint8_t enableEccCheck;                  //!< [0x054-0x054]
    uint8_t eccCheckType;
    uint8_t deviceEccStatus;
    uint8_t swEccAlgorithm;
    uint32_t swEccBlockBytes;                //!< [0x058-0x05b]
    uint8_t readyCheckOption;                //!< [0x05c-0x05c]
    uint8_t statusCommandType;               //!< [0x05d-0x05d]
    uint16_t readyCheckTimeoutInMs;          //!< [0x05e-0x05f]
    uint16_t readyCheckIntervalInUs;         //!< [0x060-0x061]
    uint8_t reserved0[30];                   //!< [0x062-0x07f]
    uint8_t userOnfiAcTimingModeCode;        //!< [0x080-0x080]
    uint8_t reserved1[31];                   //!< [0x081-0x09f]
    uint32_t bytesInPageDataArea;            //!< [0x0a0-0x0a3]
    uint32_t bytesInPageSpareArea;
    uint32_t pagesInBlock;
    uint32_t blocksInPlane;                  //!< [0x0ac-0x0af]
    uint32_t planesInDevice;                 //!< [0x0b0-0x0b3]
    uint32_t reserved2[11];                  //!< [0x0b4-0x0df]
    uint8_t enableReadbackVerify;            //!< [0x0e0-0x0e0]
    uint8_t reserved3[3];                    //!< [0x0e1-0x0e3]
    uint32_t readbackPageBufferAddress;      //!< [0x0e4-0x0e7]
    uint8_t reserved4[24];                   //!< [0x0e8-0x0ff]
} semc_nand_config_t;

//! Parallel NAND Configuration Option
typedef struct _parallel_nand_config_option
{
    union
    {
        struct
        {
            uint32_t onfiVersion : 3;       //!< ONFI version
            uint32_t edoMode : 1;           //!< EDO mode, 0 - disable, 1 - enable
            uint32_t onfiTimingMode : 3;    //!< ONFI timing mode
            uint32_t reserved0 : 1;

            uint32_t ioPortDiv8 : 2;        //!< IO port size, Minimum 1
            uint32_t reserved1 : 2;
            uint32_t pcsSelection : 3;      //!< PCS selection
            uint32_t reserved2 : 1;

            uint32_t eccType : 1;           //!< ECC type, 0 - SW, 1 - HW
            uint32_t eccStatus : 1;         //!< ECC status (device)
            uint32_t reserved3 : 6;

            uint32_t reserved4 : 4;
            uint32_t tag : 4;               //!< Tag
        } B;
        uint32_t U;
    } option;
} parallel_nand_config_option_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

//!@brief Initialize Parallel NAND devices via SEMC
status_t semc_nand_flash_init(semc_nand_config_t *config);

//!@brief Read page data from Parallel NAND via SEMC
status_t semc_nand_flash_read_page(semc_nand_config_t *config,
                                   uint32_t pageIndex,
                                   uint8_t *buffer,
                                   uint32_t length);

//!@brief Program page data to Parallel NAND via SEMC
status_t semc_nand_flash_page_program(semc_nand_config_t *config,
                                      uint32_t pageIndex,
                                      uint8_t *src,
                                      uint32_t length);

//!@brief Erase blocks of the Parallel NAND devices
status_t semc_nand_flash_erase_block(semc_nand_config_t *config,
                                     uint32_t blockIndex);

//!@brief Verify erase on Parallel NAND device
status_t semc_nand_flash_verify_erase(semc_nand_config_t *config,
                                      uint32_t pageIndex,
                                      uint32_t pageCount);

//!@brief Verify program on Parallel NAND device
status_t semc_nand_flash_verify_page_program(semc_nand_config_t *config,
                                             uint32_t pageIndex,
                                             const uint8_t *src,
                                             uint32_t lengthInBytes);

#ifdef __cplusplus
}
#endif

#endif // __SEMC_NAND_FLASH_H__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
