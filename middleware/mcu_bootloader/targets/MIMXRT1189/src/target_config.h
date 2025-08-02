/*
 * Copyright 2018 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(__TARGET_CONFIG_H__)
#define __TARGET_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
//! FPGA frequency definition
#define FPGA_FREQ_CORE (24000000UL)
#define FPGA_FREQ_BUS (12000000UL)
#define FPGA_FREQ_UART (12000000UL)

//! @brief microseconds configuration.
#define LPIT LPIT1
#define kCLOCK_Lpit kCLOCK_Lpit1

#define SxMU MU_RT_S3MUA
#define SxMU_Type S3MU_Type

//! @brief Constants for FlexSPI features.
enum
{
    kFlexSpi_AhbMemoryMaxSizeMB = (256u * 1024u * 1024u),
};

//!@brief Unique ID constants
enum _uid_constrants
{
    kUniqueId_SizeInBytes = 16,
};

//! @brief Version constants for the target.
enum _target_version_constants
{
    kTarget_Version_Name = 'T',
    kTarget_Version_Major = 1,
    kTarget_Version_Minor = 0,
    kTarget_Version_Bugfix = 0
};

//!@brief PIT backward compatible defintion
#define kCLOCK_Pit kCLOCK_Pit1

//!@brief FLEXSPI clock definitions
enum
{
    kFlexSpiSerialClk_30MHz = 1,
    kFlexSpiSerialClk_50MHz = 2,
    kFlexSpiSerialClk_60MHz = 3,
    kFlexSpiSerialClk_80MHz = 4,
    kFlexSpiSerialClk_100MHz = 5,
    kFlexSpiSerialClk_120MHz = 6,
    kFlexSpiSerialClk_133MHz = 7,
    kFlexSpiSerialClk_166MHz = 8,
    kFlexSpiSerialClk_200MHz = 9,
};

//!@brief FLEXSPI instnaces
enum
{
    kFlexspiInstance_1 = 1,
    kFlexspiInstance_2 = 2,
};

//!@brief FLEXSPI Boot Clock Source
enum
{
    kFlexSpiBootClkcSrc = 4,
};

enum
{
    kProduct_USB_PID = 0x0073,
    kProduct_USB_VID = 0x15a2,
};

//!@brief Memory index definitions
enum
{
    kIndexM33CodeTCM = 0,
    kIndexM33CoreTCM_NS = 1,
    kIndexM33SysTCM = 2,
    kIndexM33SysTCM_NS = 3,
    kIndexM7TCM = 4,
    kIndexM7TCM_NS = 5,
    kIndexOCRAM = 6,
    kIndexOCRAM_NS = 7,
    kIndexFLEXSPI = 8,
    kIndexFLEXSPI_NS = 9,
    kIndexFLEXSPI_Alias = 10,
    kIndexFLEXSPI_Alias_NS = 11,
    kIndexSEMC_Code = 12,
    kIndexSEMC_Code_NS = 13,
    kIndexSEMC = 14
};

enum
{
    kIndexITCM = kIndexM33CodeTCM,
    kIndexDTCM = kIndexM33SysTCM,
};

#define SEMC_MAX_CLK_FREQ kSemcClkFreq_166MHz
#define SEMC_2ND_MAX_CLK_FREQ kSemcClkFreq_133MHz

//!@brief IMXRT SOC System ID
#define IMXRT_SOC_SYSTEM_ID (0x1189UL)

#define BUGFIX_INC_VER_RETRIEVER               \
    {                                          \
        0x68004801u, 0x47701e40u, 0x575184e8u, \
    }

#endif // __TARGET_CONFIG_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
