/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SEMC_NOR_FLASH_H__
#define __SEMC_NOR_FLASH_H__

#include "fsl_common.h"
#include "bootloader_common.h"
#include "semc/fsl_semc.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define NOR_MEM_BLANK_VALUE  (0xFFU)

enum
{
    kParallelNorConfigOption_Tag = 0x0d,
};

//! @brief SEMC Parallel NOR Status Group
/*
enum
{
    kStatusGroup_SemcNOR = 211,
};
*/

//! @brief Status for SEMC Parallel NOR driver
enum _semc_nor_status
{
    kStatus_SemcNOR_DeviceTimeout     = MAKE_STATUS(kStatusGroup_SemcNOR, 0),
    kStatus_SemcNOR_InvalidMemoryAddress    = MAKE_STATUS(kStatusGroup_SemcNOR, 1),
    kStatus_SemcNOR_unmatchedCommandSet    = MAKE_STATUS(kStatusGroup_SemcNOR, 2),
    kStatus_SemcNOR_AddressAlignmentError    = MAKE_STATUS(kStatusGroup_SemcNOR, 3),
    kStatus_SemcNOR_InvalidCfiSignature    = MAKE_STATUS(kStatusGroup_SemcNOR, 4),
    kStatus_SemcNOR_CommandErrorNoOpToSuspend    = MAKE_STATUS(kStatusGroup_SemcNOR, 5),
    kStatus_SemcNOR_CommandErrorNoInfoAvailable    = MAKE_STATUS(kStatusGroup_SemcNOR, 6),
    kStatus_SemcNOR_BlockEraseCommandFailure    = MAKE_STATUS(kStatusGroup_SemcNOR, 7),
    kStatus_SemcNOR_BufferProgramCommandFailure    = MAKE_STATUS(kStatusGroup_SemcNOR, 8),
    kStatus_SemcNOR_ProgramVerifyFailure    = MAKE_STATUS(kStatusGroup_SemcNOR, 9),
    kStatus_SemcNOR_EraseVerifyFailure    = MAKE_STATUS(kStatusGroup_SemcNOR, 10),

    kStatus_SemcNOR_InvalidCfgTag    = MAKE_STATUS(kStatusGroup_SemcNOR, 0x10),
};

//!@brief SEMC Parallel NOR Flash vendor type
enum _semc_nor_device_vendor_index
{
    kSemcNorVendorType_Micron = 0U,
    kSemcNorVendorType_Spansion = 1U,
    kSemcNorVendorType_Winbond = 2U,
    kSemcNorVendorType_Macronix = 3U,
    kSemcNorVendorType_Microchip = 4U,
    kSemcNorVendorType_Unknown = 5U,
};

//!@brief SEMC Parallel NOR Flash command set
enum _semc_nor_device_command_set
{
    // As Micron MT28EW Embedded Parallel NOR Standard Command Definitions
    kSemcNorDeviceCommandSet_EPSCD = 0U,
    // As Micron MT28GU StrataFlash Memory Command Definitions
    kSemcNorDeviceCommandSet_SFMCD = 1U,
};

//! @brief SEMC Parallel NOR AC timing mode (Async read mode)
enum _semc_nor_ac_timing_mode
{
    kSemcNorAcTimingMode_DefaultSafe = 0U,
    kSemcNorAcTimingMode_DefaultFast = 1U,
    kSemcNorAcTimingMode_UserDefined = 2U,
    kSemcNorAcTimingMode_Invalid = 3U,
};

//! @brief SEMC Parallel NOR Flash readback verify option
enum _semc_nand_readback_verify_option
{
    kSemcNorReadbackVerifyOption_Enabled = 0U,
    kSemcNorReadbackVerifyOption_Disabled = 1U,
};

//! @brief AC characteristics of MT38 device (Under 133MHz)
enum _semc_nor_device_ac_characteristics_133M
{
    // Address setup to ADV# HIGH
    kSemcNorDeviceAC_MT38_AsyncRead_min_tAVVH_ns = 5U,
    // Address hold from ADV# HIGH
    kSemcNorDeviceAC_MT38_AsyncRead_min_tVHAX_ns = 5U,
    // ADV# HIGH to OE# LOW (A/D MUX only)
    kSemcNorDeviceAC_MT38_AsyncRead_min_tVHGL_ns = 2U,
    // OE# LOW to output valid
    kSemcNorDeviceAC_MT38_AsyncRead_max_tGLQV_ns = 7U,
    // Output hold from first occurring address, CE#, or OE# change
    kSemcNorDeviceAC_MT38_AsyncRead_min_tOH_ns = 0U,
    // OE# HIGH to output in High-Z
    kSemcNorDeviceAC_MT38_AsyncRead_max_tGHQZ_ns = 7U,
    // CE# pulse width HIGH
    kSemcNorDeviceAC_MT38_AsyncRead_min_tEHEL_ns = 7U,
    // -Synchronous/asynchronous read
    //   -Synchronous burst reads: 133 MHz (266MB/sec)
    //   -Random access times: 96ns
    kSemcNorDeviceAC_MT38_RandomAccessTimes_ns = 96U,
};

//! @brief AC characteristics of S29 device (Under 108MHz)
enum _semc_nor_device_ac_characteristics_108M
{
    // Address Setup Time to Rising Edge of AVD
    kSemcNorDeviceAC_S29_AsyncRead_min_tAAVDS_ns = 4U,
    // Address hold from ADV# HIGH
    kSemcNorDeviceAC_S29_AsyncRead_min_tAAVDH_ns = 4U, // 3.5

    // Output Enable to Output Valid
    kSemcNorDeviceAC_S29_AsyncRead_max_tOE_ns = 18U,

    // Output Enable to High-Z
    kSemcNorDeviceAC_S29_AsyncRead_max_tOEZ_ns = 10U,

    // Speed Option (MHz) 108
    // Max. Synch. Latency, ns (tIACC) 80
    // Max. Synch. Burst Access, ns (tBACC) 7.6
    // Max. Asynch. Access Time, ns (tACC) 80
    // Max OE# Access Time, ns (tOE) 15
    kSemcNorDeviceAC_S29_RandomAccessTimes_ns = 80U,
};

//! @brief AC characteristics by default
enum _semc_nor_device_ac_characteristics_default_133MHz
{
    kSemcNorDeviceAC_Comm_AsyncRead_min_tCES_ns = 0U,
    kSemcNorDeviceAC_Comm_AsyncRead_min_tAS_ns = 5U,
    kSemcNorDeviceAC_Comm_AsyncRead_min_tAH_ns = 5U,
    kSemcNorDeviceAC_Comm_AsyncRead_min_tTA_ns = 0U,
    kSemcNorDeviceAC_Comm_AsyncRead_min_tREL_ns = 96U,
    kSemcNorDeviceAC_Comm_AsyncRead_max_tREH_ns = 7U,
    kSemcNorDeviceAC_Comm_AsyncRead_min_tCEH_ns = 0U,
    kSemcNorDeviceAC_Comm_AsyncRead_min_tCEITV_ns = 7U,

    kSemcNorDeviceAC_Comm_AsyncWrite_min_tWEL_ns = 40U,
    kSemcNorDeviceAC_Comm_AsyncWrite_min_tWEH_ns = 20U,
    kSemcNorDeviceAC_Comm_AsyncWrite_min_tAWDH_ns = 5U,
};

typedef struct __nor_ac_timing_parameter
{
    uint16_t min_tCES_ns;
    uint16_t min_tCEH_ns;
    uint16_t min_tCEITV_ns;
    uint16_t min_tAS_ns;
    uint16_t min_tAH_ns;
    uint16_t min_tTA_ns;
    uint16_t min_tWEL_ns;
    uint16_t min_tWEH_ns;
    uint16_t min_tAWDH_ns;
    uint16_t min_tREL_ns;
    uint16_t max_tREH_ns;
    uint16_t reserved;
} nor_ac_timing_parameter_t;

//! @brief SEMC Parallel NOR Config block structure
typedef struct __semc_nor_config
{
    semc_mem_config_t memConfig;      //!< [0x000-0x04f]
    uint8_t vendorType;               //!< [0x050-0x050]
    uint8_t acTimingMode;             //!< [0x051-0x051]
    uint8_t deviceCommandSet;         //!< [0x052-0x052]
    uint8_t reserved0[77];            //!< [0x053-0x09f]
    uint32_t pageSizeInBytes;         //!< [0x0a0-0x0a3]
    uint32_t blockSizeInBytes;        //!< [0x0a4-0x0a7]
    uint32_t blockCount;              //!< [0x0a8-0x0ab]
    uint32_t reserved1[13];           //!< [0x0ac-0x0df]
    uint8_t enableReadbackVerify;     //!< [0x0e0-0x0e0]
    uint8_t reserved2[31];            //!< [0x0e1-0x0ff]
} semc_nor_config_t;

//!@brief Parallel NOR AC cycle option
typedef enum _nor_ac_cycle_set_option
{
    kNorAcCycleSetOption_Min          = 0x00U,
    kNorAcCycleSetOption_Max          = 0x01U,
} nor_ac_cycle_set_option_t;

//! Parallel NOR Configuration Option
typedef struct _parallel_nor_config_option
{
    union
    {
        struct
        {
            uint32_t commandSet : 2;        //!< Cmd Set, 0 - EPSCD, 1 - SFMCD, others - reserved
            uint32_t acTimingMode : 2;      //!< AC timing mode
            uint32_t reserved0 : 4;

            uint32_t dataIoPortDiv8 : 2;    //!< IO port size, Minimum 1
            uint32_t advPortPolarity : 1;   //!< ADV# polarity, 0 - low, 1 - high
            uint32_t reserved1 : 1;
            uint32_t pcsSelection : 3;      //!< PCS selection
            uint32_t reserved2 : 1;

            uint32_t reserved3 : 8;

            uint32_t reserved4 : 4;
            uint32_t tag : 4;               //!< Tag
        } B;
        uint32_t U;
    } option;
    nor_ac_timing_parameter_t acTable;
} parallel_nor_config_option_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

//!@brief Initialize Parallel NOR devices via SEMC
status_t semc_nor_flash_init(semc_nor_config_t *config);

//!@brief Program page data to Parallel NOR via SEMC
status_t semc_nor_flash_page_program(semc_nor_config_t *config,
                                         uint32_t dstAddr,
                                         const uint8_t *src,
                                         uint32_t length);

//!@brief Erase all the Parallel NOR devices connected on SEMC
status_t semc_nor_flash_erase_all(semc_nor_config_t *config);

//!@brief Erase one sector specified by address
status_t semc_nor_flash_erase_block(semc_nor_config_t *config, uint32_t blockIndex);

/* Verify erase on Parallel NOR Flash device*/
status_t semc_nor_flash_verify_erase(uint32_t startAddr, uint32_t lengthInBytes);

/* Verify program on Parallel NOR Flash device*/
status_t semc_nor_flash_verify_program(uint32_t startAddr, const uint8_t * src, uint32_t lengthInBytes);

/* Calculate min/max AC timing cycle for SEMC according to AC time*/
uint8_t semc_nor_get_specific_ac_cycles(semc_nor_config_t *config, nor_ac_cycle_set_option_t acSetOption, uint32_t requiredTime_ns);

#ifdef __cplusplus
}
#endif

#endif // __SEMC_NOR_FLASH_H__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
