/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "semc/fsl_semc.h"
#include "semc_nand_flash.h"
#include "crc/crc16.h"
#include "microseconds/microseconds.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define NAND_SPARE_AREA_MAX_SIZE (256U)
#define NAND_ECC_MAX_BLOCKS (32U)

#define NAND_READY_CHECK_INTERVAL_NORMAL (0U)
#define NAND_MAX_FEATURE_ACCESS_TIME_tFEAT_US (1000U)
#define NAND_MAX_READ_PARAMETER_PAGE_TIME_tR_US (1000U)
#define NAND_FIRST_CMD_CHANGE_COLUMN_SETUP_TIME_NS (500U)
#define NAND_MAX_RST_TIME0_tRST_US (5U)
#define NAND_MAX_RST_TIME1_tRST_US (10U)
#define NAND_MAX_RST_TIME2_tRST_US (500U)
#define NAND_MAX_RST_TIME3_tRST_US (1000U)

#define NAND_ONFI_SIGNATURE_MIN_MATCHED_BYTES (0x2U)

//!@brief Parallel NAND Flash information
enum _nand_manufactory_information
{
    kNandDeviceInfo_BadBlockMark_x8 = 0x00U,
    kNandDeviceInfo_BadBlockMark_x16 = 0x0000U,
    kNandDeviceInfo_NonBadBlockMark_x8 = 0xFFU,
    kNandDeviceInfo_NonBadBlockMark_x16 = 0xFFFFU,
};

//!@brief Parallel NAND ONFI status bit mask
enum _nand_onfi_status_bit
{
    kNandDeviceSR_ONFI_PassBitMask = 0x01U,
    kNandDeviceSR_ONFI_ReadyBitMask = 0x40U, // R/B#
};

//!@brief NAND command set selection
enum _nand_command_set_selection
{
    kNandDeviceCmdSet_OnfiCompliant = 0x00U, // As Micron MT29Fxx
    kNandDeviceCmdSet_OnfiLike = 0x01U,      // As Toshiba TC58xx
    kNandDeviceCmdSet_Specific = 0x02U,      // As Samsung K9F12xx
};

//!@brief Parallel NAND Flash commands
enum _nand_onfi_command_set
{
    // Must-have command
    kNandDeviceCmd_ONFI_Reset = 0xFFU,
    kNandDeviceCmd_ONFI_ReadMode = 0x00U,
    kNandDeviceCmd_ONFI_ReadParameterPage = 0xECU,
    kNandDeviceCmd_ONFI_ReadStatus = 0x70U,
    kNandDeviceCmd_ONFI_ReadPageSetup = 0x00U,
    kNandDeviceCmd_ONFI_ReadPageConfirm = 0x30U,
    kNandDeviceCmd_ONFI_ChangeReadColumnSetup = 0x05U,
    kNandDeviceCmd_ONFI_ChangeReadColumnConfirm = 0xE0U,
    kNandDeviceCmd_ONFI_EraseBlockSetup = 0x60U,
    kNandDeviceCmd_ONFI_EraseBlockConfirm = 0xD0U,
    kNandDeviceCmd_ONFI_ProgramPageSetup = 0x80U,
    kNandDeviceCmd_ONFI_ProgramPageConfirm = 0x10U,
    // Optional command
    kNandDeviceCmd_ONFI_ReadStatusEnhanced = 0x78U,
    kNandDeviceCmd_ONFI_SetFeatures = 0xEFU,
    kNandDeviceCmd_ONFI_GetFeatures = 0xEEU,
};

enum _nand_specific_command_set
{
    kNandDeviceCmd_SPEC_ReadStatusMultiPlane = 0x71U,
    kNandDeviceCmd_SPEC_ReadPage1stHalf = 0x00U,
    kNandDeviceCmd_SPEC_ReadPage2ndHalf = 0x01U,
    kNandDeviceCmd_SPEC_ReadPageSpare = 0x50U,
};

//!@brief Parallel NAND feature info
enum __nand_onfi_feature_info
{
    kNandDeviceFeature_ONFI_TimingModeAddress = 0x01U,

    kNandDeviceFeature_ONFI_TimingModeCode_Mode0 = 0x00U,
    kNandDeviceFeature_ONFI_TimingModeCode_Mode1 = 0x01U,
    kNandDeviceFeature_ONFI_TimingModeCode_Mode2 = 0x02U,
    kNandDeviceFeature_ONFI_TimingModeCode_Mode3 = 0x03U,
    kNandDeviceFeature_ONFI_TimingModeCode_Mode4 = 0x04U,
    kNandDeviceFeature_ONFI_TimingModeCode_Mode5 = 0x05U,
};

enum __nand_spec_feature_info
{
    kNandDeviceFeature_MT29F_ArrayOperationModeAddress = 0x90U,

    kNandDeviceFeature_MT29F_ArrayOperationModeCode_DisableECC = 0x00u,
    kNandDeviceFeature_MT29F_ArrayOperationModeCode_EnableECC = 0x08u,
};

//!@brief Parallel NAND ONFI feature config
typedef struct __nand_onfi_feature_config
{
    uint8_t command;
    uint8_t address;
    uint8_t parameter[4];
    uint8_t reserved[2];
} nand_onfi_feature_config_t;

//!@brief ONFI config block tag
enum __nand_onfi_tags
{
    //! @brief Tag value used to validate the ONFI parameter config block.
    kNandOnfiTag = FOUR_CHAR_CODE('O', 'N', 'F', 'I'),
};

//!@brief Parallel NAND ONFI parameter config
typedef struct __nand_onfi_parameter_config
{
    // Revision information and features block
    uint32_t signature;      //!< [0x000-0x003]
    uint16_t revisionNumber; //!< [0x004-0x005]
    struct
    {
        uint16_t x16bitDataBusWidth : 1;
        uint16_t multipleLUNoperations : 1;
        uint16_t reserved : 14;
    } supportedFeatures; //!< [0x006-0x007]
    struct
    {
        uint16_t reserved0 : 2;
        uint16_t setGetfeatures : 1;
        uint16_t readStatusEnhanced : 1;
        uint16_t reserved1 : 12;
    } optionalCommands;    //!< [0x008-0x009]
    uint8_t reserved0[22]; //!< [0x00a-0x01f]
    // Manufacturer information block
    uint8_t deviceManufacturer[12]; //!< [0x020-0x02b]
    uint8_t deviceModel[20];        //!< [0x02c-0x03f]
    uint8_t JEDECid;                //!< [0x040-0x040]
    uint8_t dataCode[2];            //!< [0x041-0x042]
    uint8_t reserved1[13];          //!< [0x043-0x04f]
    // Memory organization block
    uint32_t dataBytesPerPage;  //!< [0x050-0x053]
    uint16_t spareBytesPerPage; //!< [0x054-0x055]
    uint8_t reserved2[6];       //!< [0x056-0x05b]
    uint32_t pagesPerBlock;     //!< [0x05c-0x05f]
    uint32_t blocksPerLUN;      //!< [0x060-0x063]
    uint8_t LUNsPerDevice;      //!< [0x064-0x064]
    union
    {
        uint8_t addressCycles;
        struct
        {
            uint8_t rowAddressCycles : 4;
            uint8_t columnAddressCycles : 4;
        } addressCyclesStr;
    };                     //!< [0x065-0x065]
    uint8_t reserved3[26]; //!< [0x066-0x07f]
    // Electrical parameters block
    uint8_t reserved4; //!< [0x080-0x080]
    struct
    {
        uint8_t mode0 : 1;
        uint8_t mode1 : 1;
        uint8_t mode2 : 1;
        uint8_t mode3 : 1;
        uint8_t mode4 : 1;
        uint8_t mode5 : 1;
        uint8_t reserved : 2;
    } timingMode;                            //!< [0x081-0x081]
    uint8_t reserved5[3];                    //!< [0x082-0x084]
    uint8_t maxPageProgramTimeInUs[2];       //!< [0x085-0x086]
    uint8_t maxBlockEraseTimeInUs[2];        //!< [0x087-0x088]
    uint8_t maxPageReadTimeInUs[2];          //!< [0x089-0x08a]
    uint8_t minChangeColunmSetupTimeInNs[2]; //!< [0x08b-0x08c]
    uint8_t reserved6[23];                   //!< [0x08d-0x0a3]
    // Vendor block
    uint16_t vendorSpecificRevisionNumber; //!< [0x0a4-0x0a5]
    uint8_t reserved7[88];                 //!< [0x0a6-0x0fd]
    uint16_t integrityCRC;                 //!< [0x0fe-0x0ff]
                                           // Redundant Parameter Pages
    // uint8_t redundantParameterPage[2][256];       //!< [0x100-0x2ff]
} nand_onfi_parameter_config_t;

//!@brief Parallel NAND program process option
typedef enum _nand_program_process
{
    kNandDeviceProgramProcess_Pre = 0x00U,
    kNandDeviceProgramProcess_Post = 0x01U,
} nand_program_process_t;

//!@brief Parallel NAND read spare process option
typedef enum _nand_read_spare_process
{
    kNandDeviceReadSpareProcess_Pre = 0x00U,
    kNandDeviceReadSpareProcess_Post = 0x01U,
} nand_read_spare_process_t;

//!@brief Parallel NAND AC timing option
typedef enum _nand_ac_timing_set_option
{
    kNandAcTimingSetOption_Basic = 0x00U,
    kNandAcTimingSetOption_Advanced = 0x01U,
} nand_ac_timing_set_option_t;

//!@brief Parallel NAND Ready check use case
typedef enum _nand_ready_check_use_case
{
    kNandReadyCheckUseCase_DuringCommand = 0x00U,
    kNandReadyCheckUseCase_BeforeRead = 0x01U,
} nand_ready_check_use_case_t;

//!@brief Parallel NAND AC cycle option
typedef enum _nand_ac_cycle_set_option
{
    kNandAcCycleSetOption_Min = 0x00U,
    kNandAcCycleSetOption_Max = 0x01U,
} nand_ac_cycle_set_option_t;

//!@brief Parallel NAND spare uint (8-byte)
typedef struct __nand_8byte_spare_unit
{
    uint16_t badBlockMark;
    uint8_t userMetadata[2];
    uint32_t eccParityData;
} nand_8byte_spare_unit_t;

//!@brief Parallel NAND page spare area
typedef struct __nand_page_spare_area
{
    nand_8byte_spare_unit_t unit8Byte[NAND_ECC_MAX_BLOCKS];
} nand_page_spare_area_t;

//!@brief Common ECC parity data
typedef struct __nand_comm_ecc_parity_data
{
    uint32_t block[NAND_ECC_MAX_BLOCKS];
} nand_comm_ecc_parity_data_t;

//!@brief Parallel NAND operation info
typedef struct __nand_operation_info
{
    uint8_t commandSet;
    uint8_t onfiAcTimingModeCode;
    uint8_t reserved[2];
    uint32_t rowAddressToGetSR;
    uint32_t pageReadTimeInUs_tR;
    uint32_t PageProgramTimeInUs_tPROG;
    uint32_t blockEraseTimeInUs_tBERS;
    uint32_t changeColumnSetupTimeInNs_tCCS;
    bool isFeatureCommandSupport;
    bool isBlankSpareArea;
} nand_operation_info_t;

////////////////////////////////////////////////////////////////////////////////
// Local prototypes
////////////////////////////////////////////////////////////////////////////////

static status_t semc_nand_issue_reset(semc_nand_config_t *config);
static status_t semc_nand_issue_read_mode(semc_nand_config_t *config);
static status_t semc_nand_issue_access_feature(semc_nand_config_t *config, uint8_t *featureConfig);
static status_t semc_nand_issue_read_parameter(semc_nand_config_t *config,
                                               nand_onfi_parameter_config_t *parameterConfig);
static status_t semc_nand_issue_erase_block(semc_nand_config_t *config, uint32_t ipgCmdAddr);
static status_t semc_nand_issue_program_page(semc_nand_config_t *config,
                                             uint32_t ipgCmdAddr,
                                             nand_program_process_t process);
static status_t semc_nand_issue_read_status(semc_nand_config_t *config, uint8_t *SR);
static status_t semc_nand_issue_read_page(semc_nand_config_t *config, uint32_t ipgCmdAddr);
static status_t semc_nand_issue_read_page_spare(semc_nand_config_t *config, uint32_t ipgCmdAddr);
static status_t semc_nand_issue_change_read_column(semc_nand_config_t *config, uint32_t ipgCmdAddr);

/* Calculate min AC timing cycle for SEMC according to AC time*/
static uint8_t semc_nand_get_specific_ac_cycles(semc_nand_config_t *config,
                                                nand_ac_cycle_set_option_t acSetOption,
                                                uint32_t requiredTime_ns,
                                                uint8_t bitWidth);
/* Set all AC timing cycle of SEMC according to list of AC time*/
static void semc_nand_set_ac_timing_parameter(semc_nand_config_t *config, nand_ac_timing_set_option_t acSetOption);
/* Adjust AC timing cycle tCCS/tWHR parameter if needed*/
static void semc_nand_adjust_timing_parameter_ccs(semc_nand_config_t *config);
/* Switch to use new timing mode for device*/
static status_t semc_nand_switch_device_timing_mode(semc_nand_config_t *config);
/* Read onfi parameter data from device and make use of them*/
static status_t semc_nand_make_use_of_onfi_parameter(semc_nand_config_t *config);
/* Check the device ready status*/
static status_t semc_nand_wait_for_ready(semc_nand_config_t *config,
                                         uint32_t readyCheckIntervalInUs,
                                         nand_ready_check_use_case_t useCaseOption);
/* Enable the device ECC via feature setting*/
static status_t semc_nand_set_device_ecc(semc_nand_config_t *config, semc_nand_device_ecc_status_t expectedStatus);
/* Validates data by using device ECC*/
static bool semc_nand_is_device_ecc_check_passed(semc_nand_config_t *config);
/* Check if given mem range are all blank data*/
static bool semc_nand_is_mem_blank(uint32_t address, uint32_t length);
/* Pre/Post process during reading spare page*/
static status_t semc_nand_read_spare_page_process(semc_nand_config_t *config,
                                                  uint32_t ipgCmdAddr,
                                                  nand_read_spare_process_t process);
/* Get all ECC codes stored in page spare area*/
static status_t semc_nand_read_software_ecc_code_from_spare_area(semc_nand_config_t *config, uint32_t baseAddress);
/* Validates data by using Software ECC*/
static bool semc_nand_is_software_ecc_check_passed(semc_nand_config_t *config, uint32_t baseAddress);
/* Writes calculated ecc data in device spare area*/
static status_t semc_nand_write_ecc_data_in_spare_area(semc_nand_config_t *config,
                                                       uint32_t baseAddress,
                                                       uint8_t *nandData,
                                                       uint32_t length);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! @brief Readback buffer for nand.
static uint8_t s_nandReadbackBlockBuffer[NAND_ECC_MAX_BLOCK_SIZE];
static nand_comm_ecc_parity_data_t s_nandCommEccParityData;
static nand_operation_info_t s_nandOperationInfo;
static nand_onfi_parameter_config_t s_nandOnfiParameterConfig;

const nand_ac_timing_parameter_t s_nandAcTimingParameterTable[] = {
    // User-Defined
    { 0 },
    // ONFI 1.0, mode 0, 10MHz, 100ns
    {.min_tCS_ns = 70,
     .min_tCH_ns = 20,
     .min_tCEITV_ns = 0,
     .min_tWP_ns = 50,
     .min_tWH_ns = 30,
     .min_tRP_ns = 50,
     .min_tREH_ns = 30,
     .min_tTA_ns = 0,
     .min_tWHR_ns = 120,
     .min_tRHW_ns = 200,
     .min_tADL_ns = 200,
     .min_tRR_ns = 40,
     .max_tWB_ns = 200 },
    // ONFI 1.0 mode 1, 20MHz, 50ns
    { 35, 10, 0, 25, 15, 25, 15, 0, 80, 100, 100, 20, 100 },
    // ONFI 1.0 mode 2, 28MHz, 35ns
    { 25, 10, 0, 17, 15, 17, 15, 0, 80, 100, 100, 20, 100 },
    // ONFI 1.0 mode 3, 33MHz, 30ns
    { 25, 5, 0, 15, 10, 15, 10, 0, 60, 100, 100, 20, 100 },

    // Note: From ONFI spec, The host shall use EDO data output cycle timings,
    //   when running with a tRC value less than 30 ns. (tRC = tRP + tREH)
    // ONFI 1.0 mode 4, 40MHz, 25ns
    { 20, 5, 0, 12, 10, 12, 10, 0, 60, 100, 70, 20, 100 },
    // ONFI 1.0 mode 5, 50MHz, 20ns
    { 15, 5, 0, 10, 7, 10, 7, 0, 60, 100, 70, 20, 100 },
    // Auto-Detection
    { 0 }
};

const uint8_t s_nandDeviceManufacturerList[][12] = { { 'M', 'I', 'C', 'R', 'O', 'N', ' ', ' ', ' ', ' ', ' ', ' ' },
                                                     { 'S', 'P', 'A', 'N', 'S', 'I', 'O', 'N', ' ', ' ', ' ', ' ' },
                                                     { 'S', 'A', 'M', 'S', 'U', 'N', 'G', ' ', ' ', ' ', ' ', ' ' },
                                                     { 'W', 'I', 'N', 'B', 'O', 'N', 'D', ' ', ' ', ' ', ' ', ' ' },
                                                     { 'H', 'Y', 'N', 'I', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
                                                     { 'T', 'O', 'S', 'H', 'I', 'B', 'A', ' ', ' ', ' ', ' ', ' ' },
                                                     { 'M', 'A', 'C', 'R', 'O', 'N', 'I', 'X', ' ', ' ', ' ', ' ' },
                                                     { 0 } };

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

static uint8_t semc_nand_get_specific_ac_cycles(semc_nand_config_t *config,
                                                nand_ac_cycle_set_option_t acSetOption,
                                                uint32_t requiredTime_ns,
                                                uint8_t bitWidth)
{
    uint8_t cycles;
    uint32_t baseClkInMHz;
    uint32_t baseTick_ps;
    uint32_t requiredTime_ps = requiredTime_ns * 1000;

#if defined(BL_TARGET_FPGA)
    semc_get_clock(kSemcClkType_AxiClock, &baseClkInMHz);
    baseClkInMHz /= 1000000;
#else
    semc_clk_freq_t clkFreq = (semc_clk_freq_t)config->memConfig.asyncClkFreq;
    switch (clkFreq)
    {
        case kSemcClkFreq_33MHz:
            baseClkInMHz = 33;
            break;
        case kSemcClkFreq_108MHz:
            baseClkInMHz = 108;
            break;
        case kSemcClkFreq_133MHz:
            baseClkInMHz = 133;
            break;
        case kSemcClkFreq_166MHz:
            baseClkInMHz = 166;
            break;
        default:
        case kSemcClkFreq_66MHz:
            baseClkInMHz = 66;
            break;
    }
#endif

    baseTick_ps = 1000000 / baseClkInMHz;

    if (acSetOption == kNandAcCycleSetOption_Min)
    {
        // Note: From Design team, we need to always allocate at least 1-2ns more time
        //  for each AC parameter, besides the axi/ipg clock from CCM is not always accurate,
        //  So just to be safe, we allocate one more cycle here.
        // while (((cycles + 1) * baseTick_ps) < requiredTime_ps)
        cycles = 0;
        while ((cycles * baseTick_ps) < requiredTime_ps)
        {
            cycles++;
        }

        if (cycles >= (1 << bitWidth))
        {
            cycles = (1 << bitWidth) - 1;
        }
    }
    else if (acSetOption == kNandAcCycleSetOption_Max)
    {
        cycles = (1 << bitWidth);
        while (cycles && ((cycles * baseTick_ps) > requiredTime_ps))
        {
            cycles--;
        }

        if (cycles >= 1)
        {
            cycles -= 1;
        }
    }

    return cycles;
}

/*  Set AC timing parameter*/
static void semc_nand_set_ac_timing_parameter(semc_nand_config_t *config, nand_ac_timing_set_option_t acSetOption)
{
    nand_ac_timing_parameter_t acTimingArray;
    uint16_t *pAcTimingArray = &acTimingArray.min_tCS_ns;
    uint16_t *pAcTimingTable;
    uint8_t acTimingTableIndex = config->acTimingTableIndex;

    // Validate AC timing table index
    if (acTimingTableIndex > kNandAcTimingTableIndex_ONFI_1p0_FastestMode)
    {
        acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz;
        config->acTimingTableIndex = acTimingTableIndex;
    }
    // Validate User-Defined AC timing cycle
    else if (acTimingTableIndex == kNandAcTimingTableIndex_UserDefined)
    {
        bool isUserDefinedTimingValid = false;
        uint8_t *pAcTimingParameter = &config->memConfig.nandMemConfig.ceSetupTime;
        while (pAcTimingParameter <= &config->memConfig.nandMemConfig.asyncTurnaroundTime)
        {
            if (*pAcTimingParameter != 0)
            {
                isUserDefinedTimingValid = true;
            }
            pAcTimingParameter++;
        }
        if (isUserDefinedTimingValid)
        {
            if (s_nandOperationInfo.commandSet != kNandDeviceCmdSet_OnfiCompliant)
            {
                return;
            }
            else
            {
                if (acSetOption == kNandAcTimingSetOption_Advanced)
                {
                    s_nandOperationInfo.onfiAcTimingModeCode = config->userOnfiAcTimingModeCode;
                    return;
                }
            }
        }
        else
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz;
            config->acTimingTableIndex = acTimingTableIndex;
        }
    }

    if (s_nandOperationInfo.commandSet == kNandDeviceCmdSet_OnfiCompliant)
    {
        if (acSetOption == kNandAcTimingSetOption_Basic)
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz;
        }
        else if (acSetOption == kNandAcTimingSetOption_Advanced)
        {
            switch (config->acTimingTableIndex)
            {
                case kNandAcTimingTableIndex_ONFI_1p0_Mode5_50MHz:
                    s_nandOperationInfo.onfiAcTimingModeCode = kNandDeviceFeature_ONFI_TimingModeCode_Mode5;
                    break;
                case kNandAcTimingTableIndex_ONFI_1p0_Mode4_40MHz:
                    s_nandOperationInfo.onfiAcTimingModeCode = kNandDeviceFeature_ONFI_TimingModeCode_Mode4;
                    break;
                case kNandAcTimingTableIndex_ONFI_1p0_Mode3_33MHz:
                    s_nandOperationInfo.onfiAcTimingModeCode = kNandDeviceFeature_ONFI_TimingModeCode_Mode3;
                    break;
                case kNandAcTimingTableIndex_ONFI_1p0_Mode2_28MHz:
                    s_nandOperationInfo.onfiAcTimingModeCode = kNandDeviceFeature_ONFI_TimingModeCode_Mode2;
                    break;
                case kNandAcTimingTableIndex_ONFI_1p0_Mode1_20MHz:
                    s_nandOperationInfo.onfiAcTimingModeCode = kNandDeviceFeature_ONFI_TimingModeCode_Mode1;
                    break;
                case kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz:
                default:
                    s_nandOperationInfo.onfiAcTimingModeCode = kNandDeviceFeature_ONFI_TimingModeCode_Mode0;
                    break;
            }
        }
    }
    pAcTimingTable = (uint16_t *)&s_nandAcTimingParameterTable[acTimingTableIndex].min_tCS_ns;

    // Calculate AC timing cycle for SEMC based on selected AC timing table
    memset(&acTimingArray, 0, sizeof(nand_ac_timing_parameter_t));
    for (; pAcTimingArray <= &acTimingArray.min_tTA_ns; pAcTimingArray++, pAcTimingTable++)
    {
        *pAcTimingArray = semc_nand_get_specific_ac_cycles(config, kNandAcCycleSetOption_Min, *pAcTimingTable,
                                                           kSemcMiscProperty_NAND_AcParameterBitWidth4b);
    }
    for (; pAcTimingArray < &acTimingArray.max_tWB_ns; pAcTimingArray++, pAcTimingTable++)
    {
        *pAcTimingArray = semc_nand_get_specific_ac_cycles(config, kNandAcCycleSetOption_Min, *pAcTimingTable,
                                                           kSemcMiscProperty_NAND_AcParameterBitWidth6b);
    }
    pAcTimingArray = &acTimingArray.max_tWB_ns;
    pAcTimingTable = (uint16_t *)&s_nandAcTimingParameterTable[acTimingTableIndex].max_tWB_ns;
    *pAcTimingArray = semc_nand_get_specific_ac_cycles(config, kNandAcCycleSetOption_Max, *pAcTimingTable,
                                                       kSemcMiscProperty_NAND_AcParameterBitWidth6b);

    // Set AC timing cycle
    config->memConfig.nandMemConfig.ceSetupTime = acTimingArray.min_tCS_ns;
    config->memConfig.nandMemConfig.ceMinHoldTime = acTimingArray.min_tCH_ns;
    config->memConfig.nandMemConfig.ceMinIntervalTime = acTimingArray.min_tCEITV_ns;
    config->memConfig.nandMemConfig.weLowTime = acTimingArray.min_tWP_ns;
    config->memConfig.nandMemConfig.weHighTime = acTimingArray.min_tWH_ns;
    config->memConfig.nandMemConfig.reLowTime = acTimingArray.min_tRP_ns;
    config->memConfig.nandMemConfig.reHighTime = acTimingArray.min_tREH_ns;
    config->memConfig.nandMemConfig.asyncTurnaroundTime = acTimingArray.min_tTA_ns;
    config->memConfig.nandMemConfig.weHighToReLowTime = acTimingArray.min_tWHR_ns;
    config->memConfig.nandMemConfig.reHighToWeLowTime = acTimingArray.min_tRHW_ns;
    config->memConfig.nandMemConfig.aleToDataStartTime = acTimingArray.min_tADL_ns;
    config->memConfig.nandMemConfig.readyToReLowTime = acTimingArray.min_tRR_ns;
    config->memConfig.nandMemConfig.weHighToBusyTime = acTimingArray.max_tWB_ns;
}

static void semc_nand_adjust_timing_parameter_ccs(semc_nand_config_t *config)
{
    uint32_t changeColumnSetupInNs;

    // Reset specific tWHR parameter if needed
    if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
    {
        // Note: For AXI memory access, tWHR is tCCS in fact, and tCCS is not defined in ONFI timing mode table, it can
        // only be read from device by ReadParameterPage cmd
        changeColumnSetupInNs = (s_nandOperationInfo.changeColumnSetupTimeInNs_tCCS != 0) ?
                                    s_nandOperationInfo.changeColumnSetupTimeInNs_tCCS :
                                    NAND_FIRST_CMD_CHANGE_COLUMN_SETUP_TIME_NS;
        config->memConfig.nandMemConfig.weHighToReLowTime = semc_nand_get_specific_ac_cycles(
            config, kNandAcCycleSetOption_Max, changeColumnSetupInNs, kSemcMiscProperty_NAND_AcParameterBitWidth6b);
    }
}

static status_t semc_nand_switch_device_timing_mode(semc_nand_config_t *config)
{
    status_t status = kStatus_Success;

#ifndef BL_TARGET_RTL

    if (s_nandOperationInfo.isFeatureCommandSupport)
    {
        // Set desired timing mode
        uint8_t deviceTimingMode;

        nand_onfi_feature_config_t featureConfig = {.command = kNandDeviceCmd_ONFI_SetFeatures,
                                                    .address = kNandDeviceFeature_ONFI_TimingModeAddress,
                                                    .parameter[0] = s_nandOperationInfo.onfiAcTimingModeCode,
                                                    .parameter[1] = 0,
                                                    .parameter[2] = 0,
                                                    .parameter[3] = 0 };
        status = semc_nand_issue_access_feature(config, (uint8_t *)&featureConfig);
        if (status != kStatus_Success)
        {
            return status;
        }

        // Get current timing mode to double check
        featureConfig.command = kNandDeviceCmd_ONFI_GetFeatures;
        featureConfig.parameter[0] = 0;
        status = semc_nand_issue_access_feature(config, (uint8_t *)&featureConfig);
        if (status != kStatus_Success)
        {
            return status;
        }
        deviceTimingMode = featureConfig.parameter[0];

        if (deviceTimingMode != s_nandOperationInfo.onfiAcTimingModeCode)
        {
            return kStatus_SemcNAND_SwitchTimingModeFailure;
        }
    }

#endif

    return status;
}

static status_t semc_nand_make_use_of_onfi_parameter(semc_nand_config_t *config)
{
    status_t status = kStatus_Success;

#ifndef BL_TARGET_RTL

    // Cleanup ONFI config block
    memset((uint8_t *)&s_nandOnfiParameterConfig, 0, sizeof(s_nandOnfiParameterConfig));

    // Read First ONFI parameter data from device
    status = semc_nand_issue_read_parameter(config, &s_nandOnfiParameterConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Validate ONFI parameter
    // From Device Spec, To insure data integrity, device contain more than one copies of the parameter page,
    //  The Integrity CRC (Cyclic Redundancy Check) field is used to verify that the contents of the parameters page
    //  were transferred correctly to the host.
    while (1)
    {
        // Validate signature
        if (s_nandOnfiParameterConfig.signature == kNandOnfiTag)
        {
            // Validate the interity CRC
            // From ONFI spec:
            //  1. The CRC calculation covers all of data between byte 0 and byte 253 of the parameter page inclusive.
            //  2. The CRC shall be calculated on word (16-bit) quantities starting with bytes 1:0 in the parameter
            //  page.
            //  3. The CRC shall be calculated using the following 16-bit generator polynomial: G(X) = X16 + X15 + X2 +
            //  1. This polynomial in hex may be represented as 8005h.
            //  4. The CRC value shall be initialized with a value of 4F4Eh before the calculation begins.
            //  5. There is no XOR applied to the final CRC value after it is calculated.
            //  6. There is no reversal of the data bytes or the CRC calculated value.
            uint16_t calculatedCrc = 0;
            uint8_t *calculatedStart = (uint8_t *)&s_nandOnfiParameterConfig;
            uint32_t calculatedSize = sizeof(s_nandOnfiParameterConfig) - 2;

            crc16_data_t crc16Info;
            crc16_onfi_init(&crc16Info);
            crc16_onfi_update(&crc16Info, calculatedStart, calculatedSize);
            crc16_finalize(&crc16Info, &calculatedCrc);

            if (calculatedCrc == s_nandOnfiParameterConfig.integrityCRC)
            {
                break;
            }
        }

        // Read the Redundant Parameter Page data from device
        status = semc_ipg_memory_read(&config->memConfig, (uint8_t *)&s_nandOnfiParameterConfig,
                                      sizeof(nand_onfi_parameter_config_t));
        if (status != kStatus_Success)
        {
            return status;
        }
        // From ONFI Spec: The host should determine whether an additional parameter page is present by checking the
        // first
        // Dword. If at least two out of four bytes match the parameter page signature, then an additional
        // parameter page is present.
        {
            uint32_t matchedBytes = 0;
            uint32_t nandOnfiTag = kNandOnfiTag;
            for (uint32_t i = 0; i < sizeof(s_nandOnfiParameterConfig.signature); i++)
            {
                if ((*(((uint8_t *)&nandOnfiTag) + i)) == (*(((uint8_t *)&s_nandOnfiParameterConfig.signature) + i)))
                {
                    matchedBytes++;
                }
            }
            if (matchedBytes < NAND_ONFI_SIGNATURE_MIN_MATCHED_BYTES)
            {
                return kStatus_SemcNAND_InvalidOnfiParameter;
            }
        }
    }

    // Get device vendor
    config->vendorType = kSemcNandVendorType_Unknown;
    for (uint32_t vendorIndex = kSemcNandVendorType_Micron; vendorIndex < kSemcNandVendorType_Unknown; vendorIndex++)
    {
        uint32_t ch;
        for (ch = 0; ch < sizeof(s_nandDeviceManufacturerList[vendorIndex]); ch++)
        {
            if (s_nandDeviceManufacturerList[vendorIndex][ch] != s_nandOnfiParameterConfig.deviceManufacturer[ch])
            {
                break;
            }
        }
        if (ch == sizeof(s_nandDeviceManufacturerList[vendorIndex]))
        {
            config->vendorType = vendorIndex;
            break;
        }
    }

    // Set NAND feature/command info
    s_nandOperationInfo.isFeatureCommandSupport = s_nandOnfiParameterConfig.optionalCommands.setGetfeatures;
    if (s_nandOnfiParameterConfig.optionalCommands.readStatusEnhanced)
    {
        config->statusCommandType = kSemcNandStatusCommandType_Enhanced;
    }
    else
    {
        config->statusCommandType = kSemcNandStatusCommandType_Common;
    }

    // Set new NAND memory info
    config->bytesInPageDataArea = s_nandOnfiParameterConfig.dataBytesPerPage;
    {
        uint32_t dataBytes, columnWidth;
        columnWidth = 0;
        dataBytes = config->bytesInPageDataArea;
        dataBytes >>= 1;
        while (dataBytes)
        {
            columnWidth++;
            dataBytes >>= 1;
        }
        // See Note NA1.x for reason
        if (config->memConfig.nandMemConfig.ioPortWidth == 8)
        {
            config->memConfig.nandMemConfig.columnAddressWidth = columnWidth;
        }
        else if (config->memConfig.nandMemConfig.ioPortWidth == 16)
        {
            config->memConfig.nandMemConfig.columnAddressWidth = columnWidth - 1;
        }
    }
    config->bytesInPageSpareArea = s_nandOnfiParameterConfig.spareBytesPerPage;
    config->pagesInBlock = s_nandOnfiParameterConfig.pagesPerBlock;
    config->blocksInPlane = s_nandOnfiParameterConfig.blocksPerLUN;
    config->planesInDevice = s_nandOnfiParameterConfig.LUNsPerDevice;

    // Set new SEMC NAND info
    // Note: We should never set io port according to ONFI parameter table, x8 is supported 
    //   by all device, and some device support x16 as well. so we cannot set port mode to x16 
    //   if device support it. port mode is always assigned by FUSE setting.
    // config->memConfig.nandMemConfig.ioPortWidth = (s_nandOnfiParameterConfig.supportedFeatures.x16bitDataBusWidth ==
    // 1) ? 16: 8;
    switch (s_nandOnfiParameterConfig.addressCycles)
    {
        case 0x22u:
            config->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_4byte_CA2RA2;
            break;
        case 0x21u:
            config->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_3byte_CA2RA1;
            break;
        case 0x13u:
            config->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_4byte_CA1RA3;
            break;
        case 0x12u:
            config->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_3byte_CA1RA2;
            break;
        case 0x11u:
            config->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_2byte_CA1RA1;
            break;
        case 0x23u:
        default:
            config->memConfig.nandMemConfig.arrayAddressOption = kSemcNandAddressOption_5byte_CA2RA3;
            break;
    }

    // Set new timing mode
    if (config->acTimingTableIndex == kNandAcTimingTableIndex_ONFI_1p0_FastestMode)
    {
        if (s_nandOnfiParameterConfig.timingMode.mode5)
        {
            config->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode5_50MHz;
        }
        else if (s_nandOnfiParameterConfig.timingMode.mode4)
        {
            config->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode4_40MHz;
        }
        else if (s_nandOnfiParameterConfig.timingMode.mode3)
        {
            config->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode3_33MHz;
        }
        else if (s_nandOnfiParameterConfig.timingMode.mode2)
        {
            config->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode2_28MHz;
        }
        else if (s_nandOnfiParameterConfig.timingMode.mode1)
        {
            config->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode1_20MHz;
        }
        else
        {
            config->acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz;
        }
    }

    // Set Ready check interval time
    s_nandOperationInfo.pageReadTimeInUs_tR = ((uint32_t)s_nandOnfiParameterConfig.maxPageReadTimeInUs[1] << 8) +
                                              s_nandOnfiParameterConfig.maxPageReadTimeInUs[0];
    s_nandOperationInfo.blockEraseTimeInUs_tBERS = ((uint32_t)s_nandOnfiParameterConfig.maxBlockEraseTimeInUs[1] << 8) +
                                                   s_nandOnfiParameterConfig.maxBlockEraseTimeInUs[0];
    s_nandOperationInfo.PageProgramTimeInUs_tPROG =
        ((uint32_t)s_nandOnfiParameterConfig.maxPageProgramTimeInUs[1] << 8) +
        s_nandOnfiParameterConfig.maxPageProgramTimeInUs[0];

#ifndef BL_TARGET_RTL
    // Actual ready check interval time = max operation time / 4;
    s_nandOperationInfo.pageReadTimeInUs_tR >>= 2;
    s_nandOperationInfo.blockEraseTimeInUs_tBERS >>= 2;
    s_nandOperationInfo.PageProgramTimeInUs_tPROG >>= 2;
#endif

    // Set change cloumn setup time for AXI access
    s_nandOperationInfo.changeColumnSetupTimeInNs_tCCS =
        ((uint32_t)s_nandOnfiParameterConfig.minChangeColunmSetupTimeInNs[1] << 8) +
        s_nandOnfiParameterConfig.minChangeColunmSetupTimeInNs[0];

#endif

    return status;
}

/*  Check the device ready status*/
status_t semc_nand_wait_for_ready(semc_nand_config_t *config,
                                  uint32_t readyCheckIntervalInUs,
                                  nand_ready_check_use_case_t useCaseOption)
{
    status_t status = kStatus_Success;
    bool isDeviceReady = false;
    uint64_t deltaTicks;
    uint64_t baseTicks = microseconds_get_ticks();
    uint64_t timeoutTicks = microseconds_convert_to_ticks(config->readyCheckTimeoutInMs * 1000);

    // Get SR value from Device by issuing READ STATUS commmand
    while (!isDeviceReady)
    {
        deltaTicks = microseconds_get_ticks() - baseTicks;
        if (deltaTicks > timeoutTicks)
        {
            return kStatus_SemcNAND_DeviceTimeout;
        }

        if (config->readyCheckOption == kSemcNandReadyCheckOption_SR)
        {
            uint8_t SR = 0;

            // As the AC cycle time unit of all NAND operations is us, so we don't
            //  need to check status continuously.
            microseconds_delay(readyCheckIntervalInUs);

            status = semc_nand_issue_read_status(config, &SR);
            if (status != kStatus_Success)
            {
                return status;
            }

            // SR[RDY] = 0, Busy
            // SR[RDY] = 1, Ready
            isDeviceReady = (SR & kNandDeviceSR_ONFI_ReadyBitMask);
        }
        // Monitors the target's R/B# signal to determine the progress.
        else if (config->readyCheckOption == kSemcNandReadyCheckOption_RB)
        {
            isDeviceReady = semc_is_nand_ready();
        }
    }

    if (useCaseOption == kNandReadyCheckUseCase_BeforeRead)
    {
        // Note: If the ReadStatus command is used to monitor for command completion, the
        //  ReadMode command must be used to re-enable data output mode.
        if (config->readyCheckOption == kSemcNandReadyCheckOption_SR)
        {
            // READ MODE command must be issued before starting folowing read cycles.
            status = semc_nand_issue_read_mode(config);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
    }

    return kStatus_Success;
}

status_t semc_nand_issue_reset(semc_nand_config_t *config)
{
    status_t status = kStatus_Success;
    uint32_t dummyData = 0;

    // The RESET command may be executed with the target in any state.
    uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_Reset,
                                                          SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Don't care
                                                          SEMC_IPCMD_NAND_CM_IPG_CMD_ONLY); // Command Only
    status =
        semc_ipg_command_nand_access(config->memConfig.nandMemConfig.ipgMemBaseAddress, commandCode, 0, &dummyData);

    // For ONFI 1.0 Timing mode 0, the max tRST = 1000us
    // For ONFI 1.O other timing modes, the max tRST = 5/10/500us
    //  The target is allowed a longer maximum reset time when a program or erase operation is in progress. The maximums
    //  correspond to:
    //    1. The target is not performing an erase or program operation.
    //    2. The target is performing a program operation.
    //    3. The target is performing an erase operation.
    status = semc_nand_wait_for_ready(config, NAND_MAX_RST_TIME3_tRST_US, kNandReadyCheckUseCase_DuringCommand);

    return status;
}

status_t semc_nand_issue_read_mode(semc_nand_config_t *config)
{
    status_t status = kStatus_Success;
    uint32_t dummyData = 0;

    // READ MODE command is accepted by device when it is ready (RDY = 1, ARDY = 1).
    status = semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL, kNandReadyCheckUseCase_DuringCommand);
    if (status != kStatus_Success)
    {
        return status;
    }

    // The READ MODE command disables status output and enables data output
    uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ReadMode,
                                                          SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Don't care
                                                          SEMC_IPCMD_NAND_CM_IPG_CMD_ONLY); // Command Only
    status =
        semc_ipg_command_nand_access(config->memConfig.nandMemConfig.ipgMemBaseAddress, commandCode, 0, &dummyData);

    return status;
}

status_t semc_nand_issue_access_feature(semc_nand_config_t *config, uint8_t *featureConfig)
{
    status_t status = kStatus_Success;
    uint32_t dummyData = 0;

    // SET/GET FEATURES command is accepted by the target only when all die
    //  (LUNs) on the target are idle.
    nand_onfi_feature_config_t *nandFeatureConfig = (nand_onfi_feature_config_t *)featureConfig;
    uint16_t commandCode = semc_ipg_command_get_nand_code(nandFeatureConfig->command,
                                                          SEMC_IPCMD_NAND_AM_COL_ADDR_1BYTE, // CA1
                                                          SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR);  // Commmand Address
    status = semc_ipg_command_nand_access(nandFeatureConfig->address, commandCode, 0, &dummyData);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Only ipg command is supported here
    if (nandFeatureConfig->command == kNandDeviceCmd_ONFI_SetFeatures)
    {
        status = semc_ipg_memory_write(&config->memConfig, nandFeatureConfig->parameter,
                                       sizeof(nandFeatureConfig->parameter));
        if (status != kStatus_Success)
        {
            return status;
        }
        // Note: From Spec, both R/B and SR can be used to determine the progress,
        //    but actually only when we choose R/B, then it works well on the EVB and FPGA.
        if (config->readyCheckOption == kSemcNandReadyCheckOption_RB)
        {
            // Monitors the target's R/B# signal to determine the progress.
            status = semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL,
                                              kNandReadyCheckUseCase_DuringCommand);
        }
        else
        {
            // Just delay some time to workaround issue
            microseconds_delay(NAND_MAX_FEATURE_ACCESS_TIME_tFEAT_US);
        }
    }
    else if (nandFeatureConfig->command == kNandDeviceCmd_ONFI_GetFeatures)
    {
        // Note: From Spec, both R/B and SR can be used to determine the progress,
        //    but actually only when we choose R/B, then it works well on the EVB and FPGA.
        if (config->readyCheckOption == kSemcNandReadyCheckOption_RB)
        {
            // Monitors the target's R/B# signal to determine the progress.
            status =
                semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL, kNandReadyCheckUseCase_BeforeRead);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
        else
        {
            // Just delay some time to workaround issue
            microseconds_delay(NAND_MAX_FEATURE_ACCESS_TIME_tFEAT_US);
        }

        status = semc_ipg_memory_read(&config->memConfig, nandFeatureConfig->parameter,
                                      sizeof(nandFeatureConfig->parameter));
    }

    return status;
}

status_t semc_nand_issue_read_parameter(semc_nand_config_t *config, nand_onfi_parameter_config_t *parameterConfig)
{
    status_t status = kStatus_Success;
    uint32_t readyCheckIntervalInUs;
    uint32_t dummyData = 0;

    uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ReadParameterPage,
                                                          SEMC_IPCMD_NAND_AM_COL_ADDR_1BYTE, // 1 byte address
                                                          SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR);  // Command Address
    status = semc_ipg_command_nand_access(0, commandCode, 0, &dummyData);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Note2: ReadStatus may be used to check the status of Read Parameter Page during execution.
    // Note3: Use of the ReadStatusEnhanced command is prohibited during the power-on
    //  Reset command and when OTP mode is enabled. It is also prohibited following
    //  some of the other reset, identification, and configuration operations.
    readyCheckIntervalInUs = (NAND_MAX_READ_PARAMETER_PAGE_TIME_tR_US > s_nandOperationInfo.pageReadTimeInUs_tR) ?
                                 NAND_MAX_READ_PARAMETER_PAGE_TIME_tR_US :
                                 s_nandOperationInfo.pageReadTimeInUs_tR;
    if (config->readyCheckOption == kSemcNandReadyCheckOption_RB)
    {
        status = semc_nand_wait_for_ready(config, readyCheckIntervalInUs, kNandReadyCheckUseCase_BeforeRead);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    else
    {
        microseconds_delay(readyCheckIntervalInUs);
    }

    // Only ipg command is supported here
    status = semc_ipg_memory_read(&config->memConfig, (uint8_t *)parameterConfig, sizeof(nand_onfi_parameter_config_t));

    return status;
}

status_t semc_nand_issue_read_status(semc_nand_config_t *config, uint8_t *SR)
{
    status_t status = kStatus_Success;
    uint32_t readoutData;
    uint16_t commandCode;
    uint32_t slaveAddress;

    // Note: If there is only one plane per target, the READ STATUS (70h) command can be used
    //   to return status following any NAND command.
    // Note: In devices that have more than one plane per target, during and following interleaved
    //  die (multi-plane) operations, the READ STATUS ENHANCED command must be used to select the
    //  die (LUN) that should report status.
    if (config->statusCommandType == kSemcNandStatusCommandType_Enhanced)
    {
        if (s_nandOperationInfo.commandSet == kNandDeviceCmdSet_Specific)
        {
            // READ MULTI-PLANE STATUS command is accepted by device even when it is busy (RDY = 0).
            commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_SPEC_ReadStatusMultiPlane,
                                                         SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Don't care
                                                         SEMC_IPCMD_NAND_CM_IPG_CMD_READ); // Commmand Read
            slaveAddress = config->memConfig.nandMemConfig.ipgMemBaseAddress;
        }
        else
        {
            // READ STATUS ENHANCED command is accepted by all planes in device even when they are busy (RDY = 0).
            commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ReadStatusEnhanced,
                                                         SEMC_IPCMD_NAND_AM_ROW_ADDR_3BYTE,     // 3 Byte-RA0/RA1/RA2
                                                         SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR_READ); // Commmand Address Read
            slaveAddress = s_nandOperationInfo.rowAddressToGetSR;
        }
    }
    else
    {
        // READ STATUS command is accepted by device even when it is busy (RDY = 0).
        commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ReadStatus,
                                                     SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Don't care
                                                     SEMC_IPCMD_NAND_CM_IPG_CMD_READ); // Commmand Read

        // Note: For those command without address, the address should be valid as well,
        //  it shouldn't be out of IPG memory space, or SEMC IP will ignore this command.
        slaveAddress = config->memConfig.nandMemConfig.ipgMemBaseAddress;
    }
    status = semc_ipg_command_nand_access(slaveAddress, commandCode, 0, &readoutData);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Set SR value according to readout data from Device
    *SR = (uint8_t)readoutData;

    return kStatus_Success;
}

status_t semc_nand_issue_read_page(semc_nand_config_t *config, uint32_t ipgCmdAddr)
{
    status_t status = kStatus_Success;
    uint32_t readyCheckIntervalInUs;
    uint32_t dummyData = 0;

    // READ PAGE command is accepted by the device when it is ready (RDY = 1, ARDY = 1).
    status = semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL, kNandReadyCheckUseCase_DuringCommand);
    if (status != kStatus_Success)
    {
        return status;
    }

    // SA = pageIndex * pageDataSize;
    // CA = SA[COL-1:0], RA = SA[31:COL]
    // For Specific device with specific addressing and command set
    if (s_nandOperationInfo.commandSet == kNandDeviceCmdSet_Specific)
    {
        uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_SPEC_ReadPage1stHalf,
                                                              SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Address value
                                                              SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR); // Command Address
        status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    // For other ONFI devices (ONFI Compliant, ONFI-like)
    else
    {
        uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ReadPageSetup,
                                                              SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Address value
                                                              SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR); // Command Address
        status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
        if (status != kStatus_Success)
        {
            return status;
        }

        commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ReadPageConfirm,
                                                     SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Don't care
                                                     SEMC_IPCMD_NAND_CM_IPG_CMD_HOLD); // Commmand Hold
        status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // Monitors the target's R/B# signal or Reads the status register
    //  to determine the progress of the page data transfer.
    readyCheckIntervalInUs = (config->readyCheckIntervalInUs > s_nandOperationInfo.pageReadTimeInUs_tR) ?
                                 config->readyCheckIntervalInUs :
                                 s_nandOperationInfo.pageReadTimeInUs_tR;
    status = semc_nand_wait_for_ready(config, readyCheckIntervalInUs, kNandReadyCheckUseCase_BeforeRead);

    return status;
}

status_t semc_nand_issue_read_page_spare(semc_nand_config_t *config, uint32_t ipgCmdAddr)
{
    status_t status = kStatus_Success;
    uint32_t readyCheckIntervalInUs;
    uint32_t dummyData = 0;

    // READ PAGE SPARE command is accepted by the device when it is ready (RDY = 1, ARDY = 1).
    status = semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL, kNandReadyCheckUseCase_DuringCommand);
    if (status != kStatus_Success)
    {
        return status;
    }

    uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_SPEC_ReadPageSpare,
                                                          SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Address value
                                                          SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR); // Command Address
    status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Monitors the target's R/B# signal or Reads the status register
    //  to determine the progress of the page data transfer.
    readyCheckIntervalInUs = (config->readyCheckIntervalInUs > s_nandOperationInfo.pageReadTimeInUs_tR) ?
                                 config->readyCheckIntervalInUs :
                                 s_nandOperationInfo.pageReadTimeInUs_tR;
    status = semc_nand_wait_for_ready(config, readyCheckIntervalInUs, kNandReadyCheckUseCase_BeforeRead);

    return status;
}

status_t semc_nand_issue_change_read_column(semc_nand_config_t *config, uint32_t ipgCmdAddr)
{
    status_t status = kStatus_Success;
    uint32_t readyCheckIntervalInUs;
    uint32_t changeColumnSetupTimeInNs;
    uint32_t dummyData = 0;

    // CHANGE READ COLUMN command is accepted by the device when it is ready (RDY = 1, ARDY = 1).
    status = semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL, kNandReadyCheckUseCase_DuringCommand);
    if (status != kStatus_Success)
    {
        return status;
    }

    uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ChangeReadColumnSetup,
                                                          SEMC_IPCMD_NAND_AM_COL_ADDR_2BYTE, // 2 Byte-CA0/CA1
                                                          SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR);  // Command Address
    status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
    if (status != kStatus_Success)
    {
        return status;
    }

    commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ChangeReadColumnConfirm,
                                                 SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Don't care
                                                 SEMC_IPCMD_NAND_CM_IPG_CMD_HOLD); // Commmand Hold
    status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Note: The host shall not read data from the device until tCCS ns after the E0h(ChangeReadColumnConfirm) command
    // is written to the device.
    changeColumnSetupTimeInNs = (s_nandOperationInfo.changeColumnSetupTimeInNs_tCCS != 0) ?
                                    s_nandOperationInfo.changeColumnSetupTimeInNs_tCCS :
                                    NAND_FIRST_CMD_CHANGE_COLUMN_SETUP_TIME_NS;
    readyCheckIntervalInUs = (changeColumnSetupTimeInNs / 1000) + 1;
    microseconds_delay(readyCheckIntervalInUs);

    return status;
}

status_t semc_nand_issue_erase_block(semc_nand_config_t *config, uint32_t ipgCmdAddr)
{
    status_t status = kStatus_Success;
    uint32_t readyCheckIntervalInUs;
    uint32_t dummyData = 0;

    // ERASE BLOCK command is accepted by the device when it is ready (RDY = 1, ARDY = 1).
    status = semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL, kNandReadyCheckUseCase_DuringCommand);
    if (status != kStatus_Success)
    {
        return status;
    }

    // SA = blockIndex * pagesInBlock * pageDataSize;
    // CA = SA[COL-1:0], RA = SA[31:COL]
    uint16_t commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_EraseBlockSetup,
                                                          SEMC_IPCMD_NAND_AM_ROW_ADDR_3BYTE, // Address value
                                                          SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR);  // Command Address
    status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
    if (status != kStatus_Success)
    {
        return status;
    }

    commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_EraseBlockConfirm,
                                                 SEMC_IPCMD_NAND_AM_ROW_ADDR_3BYTE, // Don't care
                                                 SEMC_IPCMD_NAND_CM_IPG_CMD_HOLD);  // Commmand Hold
    status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Monitors the target's R/B# signal or Reads the status register
    //  to determine the progress.
    readyCheckIntervalInUs = (config->readyCheckIntervalInUs > s_nandOperationInfo.blockEraseTimeInUs_tBERS) ?
                                 config->readyCheckIntervalInUs :
                                 s_nandOperationInfo.blockEraseTimeInUs_tBERS;
    status = semc_nand_wait_for_ready(config, readyCheckIntervalInUs, kNandReadyCheckUseCase_DuringCommand);

    return status;
}

status_t semc_nand_issue_program_page(semc_nand_config_t *config, uint32_t ipgCmdAddr, nand_program_process_t process)
{
    status_t status = kStatus_Success;
    uint16_t commandCode;
    uint32_t readyCheckIntervalInUs;
    uint32_t dummyData = 0;

    // SA = pageIndex * pageDataSize;
    // CA = SA[COL-1:0], RA = SA[31:COL]
    if (process == kNandDeviceProgramProcess_Pre)
    {
        // PROGRAM PAGE command is accepted by the device when it is ready (RDY = 1, ARDY = 1).
        status =
            semc_nand_wait_for_ready(config, NAND_READY_CHECK_INTERVAL_NORMAL, kNandReadyCheckUseCase_DuringCommand);
        if (status != kStatus_Success)
        {
            return status;
        }
        commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ProgramPageSetup,
                                                     SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Address value
                                                     SEMC_IPCMD_NAND_CM_IPG_CMD_ADDR); // Command Address
        status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    else if (process == kNandDeviceProgramProcess_Post)
    {
        commandCode = semc_ipg_command_get_nand_code(kNandDeviceCmd_ONFI_ProgramPageConfirm,
                                                     SEMC_IPCMD_NAND_AM_COL_ROW_ADDR,  // Don't care
                                                     SEMC_IPCMD_NAND_CM_IPG_CMD_ONLY); // Commmand Only
        status = semc_ipg_command_nand_access(ipgCmdAddr, commandCode, 0, &dummyData);
        if (status != kStatus_Success)
        {
            return status;
        }
        // Monitors the target's R/B# signal or Reads the status register
        //  to determine the progress of the page data transfer.
        readyCheckIntervalInUs = (config->readyCheckIntervalInUs > s_nandOperationInfo.PageProgramTimeInUs_tPROG) ?
                                     config->readyCheckIntervalInUs :
                                     s_nandOperationInfo.PageProgramTimeInUs_tPROG;
        status = semc_nand_wait_for_ready(config, readyCheckIntervalInUs, kNandReadyCheckUseCase_DuringCommand);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return kStatus_Success;
}

status_t semc_nand_set_device_ecc(semc_nand_config_t *config, semc_nand_device_ecc_status_t expectedStatus)
{
    status_t status;
    bool isEccSetAsExpected = false;

    if (config->deviceEccStatus == expectedStatus)
    {
        return kStatus_Success;
    }

    // Note: ECC is enabled by default in most ONFI compliant device,
    //  But for some Micron device, it is disabled by default
    if (config->vendorType == kSemcNandVendorType_Micron)
    {
        if (s_nandOperationInfo.isFeatureCommandSupport)
        {
            nand_onfi_feature_config_t featureConfig = {.command = kNandDeviceCmd_ONFI_SetFeatures,
                                                        .address = kNandDeviceFeature_MT29F_ArrayOperationModeAddress,
                                                        .parameter[1] = 0,
                                                        .parameter[2] = 0,
                                                        .parameter[3] = 0 };
            // Note:  Device ECC is disabled on power cycle.
            if (expectedStatus == kSemcNandDeviceEccStatus_Disabled)
            {
                featureConfig.parameter[0] = kNandDeviceFeature_MT29F_ArrayOperationModeCode_DisableECC;
            }
            else if (expectedStatus == kSemcNandDeviceEccStatus_Enabled)
            {
                featureConfig.parameter[0] = kNandDeviceFeature_MT29F_ArrayOperationModeCode_EnableECC;
            }

            status = semc_nand_issue_access_feature(config, (uint8_t *)&featureConfig);
            if (status != kStatus_Success)
            {
                return status;
            }

            isEccSetAsExpected = true;
            config->deviceEccStatus = expectedStatus;
        }
    }

    if (!isEccSetAsExpected)
    {
        if (expectedStatus == kSemcNandDeviceEccStatus_Enabled)
        {
            return kStatus_SemcNAND_CannotEnableDeviceEcc;
        }
        else if (expectedStatus == kSemcNandDeviceEccStatus_Disabled)
        {
            return kStatus_SemcNAND_CannotDisableDeviceEcc;
        }
    }

    return kStatus_Success;
}

bool semc_nand_is_device_ecc_check_passed(semc_nand_config_t *config)
{
    bool isEccPassed = true;

    // Check NAND Device ECC status
    if ((config->enableEccCheck == kSemcNandEccCheckOption_Enabled) &&
        (config->eccCheckType == kSemcNandEccCheckType_DeviceECC))
    {
        // During READ operations the device executes the internal ECC engine (n-bit detection
        //  and (n-1)-bit error correction). When the READ operaton is complete, read status bit 0 must
        //  be checked to determine whether errors larger than n bits have occurred.
        status_t status = kStatus_Success;
        uint8_t SR = 0;
        ;

        // Note1: For MT29 series device: We just need to check SR[PASS] to see the ECC result for
        //  all types of operation(PROGRAM/ERASE/READ)
        // Note2: For S34 series device: Error Detection Code check is a feature that can be used during the
        //  copy back program operation. For common program/erase, the Status Bit SR[PASS] may be checked. The
        //  internal write/erase verify detects only errors for 1’s/0's that are not successfully programmed to 0’s/1's.
        status = semc_nand_issue_read_status(config, &SR);
        if (status == kStatus_Success)
        {
            // SR[PASS] = 0, Successful PROGRAM/ERASE/READ
            // SR[PASS] = 1, Error in PROGRAM/ERASE/READ
            isEccPassed = !(SR & kNandDeviceSR_ONFI_PassBitMask);
        }
        else
        {
            isEccPassed = false;
        }

        // READ MODE command should be issued in case read cycle is folowing.
        status = semc_nand_issue_read_mode(config);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return isEccPassed;
}

bool semc_nand_is_mem_blank(uint32_t address, uint32_t length)
{
    const uint8_t *start = (const uint8_t *)address;
    bool isMemoryErased = true;

    while (length)
    {
        if (*start != NAND_MEM_BLANK_VALUE)
        {
            isMemoryErased = false;
            break;
        }
        else
        {
            length--;
            start++;
        }
    }

    return isMemoryErased;
}

status_t semc_nand_read_spare_page_process(semc_nand_config_t *config,
                                           uint32_t ipgCmdAddr,
                                           nand_read_spare_process_t process)
{
    status_t status = kStatus_Success;

    if (s_nandOperationInfo.commandSet == kNandDeviceCmdSet_Specific)
    {
        // For Pre process, issue read_page_spare command
        // For Post process, issue read_page command
        if (process == kNandDeviceReadSpareProcess_Pre)
        {
            status = semc_nand_issue_read_page_spare(config, ipgCmdAddr);
        }
        else if (process == kNandDeviceReadSpareProcess_Post)
        {
            status = semc_nand_issue_read_page(config, ipgCmdAddr);
        }
    }
    else
    {
        // For Pre process, set column address to spare area start
        // For Post process, set column address to page area start
        if (process == kNandDeviceReadSpareProcess_Pre)
        {
            ipgCmdAddr += config->bytesInPageDataArea;
        }
        status = semc_nand_issue_change_read_column(config, ipgCmdAddr);
    }

    return status;
}

status_t semc_nand_read_software_ecc_code_from_spare_area(semc_nand_config_t *config, uint32_t baseAddress)
{
    status_t status = kStatus_Success;

    if ((config->enableEccCheck == kSemcNandEccCheckOption_Enabled) &&
        (config->eccCheckType == kSemcNandEccCheckType_SoftwareECC))
    {
        uint32_t eccBlocks = config->bytesInPageDataArea / config->swEccBlockBytes;
        uint32_t spareAreaStart;
        uint32_t spareAreaArray[NAND_SPARE_AREA_MAX_SIZE / 4];

        assert(config->bytesInPageSpareArea < sizeof(spareAreaArray));
        // Cleanup spare area buffer
        memset(&spareAreaArray, 0xFFu, sizeof(spareAreaArray));

        // Read ECC parity data stored in device spare area
        if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
        {
            uint32_t pageAreaStart = baseAddress / config->memConfig.nandMemConfig.axiMemBaseAddress;
            pageAreaStart *= config->memConfig.nandMemConfig.axiMemBaseAddress;
            spareAreaStart = pageAreaStart + config->bytesInPageDataArea;
            status = semc_axi_memory_read(spareAreaStart, (uint8_t *)spareAreaArray, config->bytesInPageSpareArea);
        }
        else if (config->memConfig.accessCommandType == kSemcAccessCommandType_IPBUSCMD)
        {
            uint32_t pageAreaStart = baseAddress;
            // Set read column address to spare area
            status = semc_nand_read_spare_page_process(config, pageAreaStart, kNandDeviceReadSpareProcess_Pre);
            if (status != kStatus_Success)
            {
                return status;
            }
            status = semc_ipg_memory_read(&config->memConfig, (uint8_t *)spareAreaArray, config->bytesInPageSpareArea);
            if (status != kStatus_Success)
            {
                return status;
            }
            // Set back read column address to page area
            status = semc_nand_read_spare_page_process(config, pageAreaStart, kNandDeviceReadSpareProcess_Post);
        }
        if (status != kStatus_Success)
        {
            return status;
        }

        // Check if spare area are all blank data
        if (semc_nand_is_mem_blank((uint32_t)&spareAreaArray, sizeof(spareAreaArray)))
        {
            s_nandOperationInfo.isBlankSpareArea = true;
        }
        else
        {
            s_nandOperationInfo.isBlankSpareArea = false;
        }

        // Fill in ECC block with ECC parity data
        memset(&s_nandCommEccParityData, 0, sizeof(s_nandCommEccParityData));
        {
            assert(eccBlocks <= NAND_ECC_MAX_BLOCKS);
            nand_page_spare_area_t *spareArea = (nand_page_spare_area_t *)spareAreaArray;
            uint32_t eccIndex = 0;
            while (eccIndex < eccBlocks)
            {
                s_nandCommEccParityData.block[eccIndex] = spareArea->unit8Byte[eccIndex].eccParityData;
                eccIndex++;
            }
        }
    }

    return status;
}

bool semc_nand_is_software_ecc_check_passed(semc_nand_config_t *config, uint32_t baseAddress)
{
    bool isEccPassed = true;

    if ((config->enableEccCheck == kSemcNandEccCheckOption_Enabled) &&
        (config->eccCheckType == kSemcNandEccCheckType_SoftwareECC))
    {
        // Validate data using ECC parity data
        if (config->swEccAlgorithm == kSemcNandSwEccAlgorithm_SEC_HammingCode)
        {
            uint32_t eccIndex = 0;
            uint32_t eccBlocks = config->bytesInPageDataArea / config->swEccBlockBytes;
            uint8_t *nandData = s_nandReadbackBlockBuffer;
            uint32_t eccBlockStartAddress = baseAddress;
            if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
            {
                eccBlockStartAddress = baseAddress - config->memConfig.nandMemConfig.axiMemBaseAddress;
            }
            while (eccIndex < eccBlocks)
            {
                uint32_t dstEccIndex = (eccBlockStartAddress % config->bytesInPageDataArea) / config->swEccBlockBytes;
                if (dstEccIndex == eccIndex)
                {
                    // Only if it is not a blank block (either main or spare block), then we will check SW ECC.
                    // So the behavior could be aligned with HW ECC
                    if ((!semc_nand_is_mem_blank((uint32_t)nandData, config->swEccBlockBytes)) ||
                        (!s_nandOperationInfo.isBlankSpareArea))
                    {
                        status_t status =
                            nand_ecc_sec_verify_correct((nand_ecc_block_size_t)config->swEccBlockBytes, nandData,
                                                        (uint8_t *)&s_nandCommEccParityData.block[eccIndex]);
                        if ((status != kStatus_NandECC_NoBitErrorDetected) &&
                            (status != kStatus_NandECC_SingleBitErrorCorrected))
                        {
                            isEccPassed = false;
                            break;
                        }
                    }
                    nandData += config->swEccBlockBytes;
                    if (nandData >= (s_nandReadbackBlockBuffer + sizeof(s_nandReadbackBlockBuffer)))
                    {
                        break;
                    }
                    else
                    {
                        eccBlockStartAddress += config->swEccBlockBytes;
                    }
                }
                eccIndex++;
            }
        }
    }

    return isEccPassed;
}

status_t semc_nand_write_ecc_data_in_spare_area(semc_nand_config_t *config,
                                                uint32_t baseAddress,
                                                uint8_t *nandData,
                                                uint32_t length)
{
    status_t status = kStatus_Success;

    if ((config->enableEccCheck == kSemcNandEccCheckOption_Enabled) &&
        (config->eccCheckType == kSemcNandEccCheckType_SoftwareECC))
    {
        nand_comm_ecc_parity_data_t eccParityData;
        uint32_t eccBlocks = config->bytesInPageDataArea / config->swEccBlockBytes;
        uint32_t spareAreaStart;
        uint32_t spareAreaArray[NAND_SPARE_AREA_MAX_SIZE / 4];

        assert(config->bytesInPageSpareArea < sizeof(spareAreaArray));

        // Cleanup eccParityData
        memset((uint8_t *)&eccParityData, 0, sizeof(nand_comm_ecc_parity_data_t));

        // Calculate ECC parity data
        if (config->swEccAlgorithm == kSemcNandSwEccAlgorithm_SEC_HammingCode)
        {
            uint32_t eccIndex = 0;
            uint32_t requiredBytes = length;
            uint32_t payloadedBytes = 0;
            uint8_t *srcNandData = nandData;
            bool isAllBlankPayload = false;

            assert(!(sizeof(s_nandReadbackBlockBuffer) / config->swEccBlockBytes));
            while (eccIndex < eccBlocks)
            {
                uint32_t eccCode = 0;
                if (requiredBytes)
                {
                    srcNandData = nandData + payloadedBytes;
                    // Calculate data from src buffer (all src bytes)
                    if (requiredBytes >= config->swEccBlockBytes)
                    {
                        requiredBytes -= config->swEccBlockBytes;
                    }
                    else
                    {
                        // Calculate data from ReadbackBlockBuffer (src bytes + 0xFFu bytes)
                        memcpy(s_nandReadbackBlockBuffer, srcNandData, requiredBytes);
                        memset(&s_nandReadbackBlockBuffer[requiredBytes], NAND_MEM_BLANK_VALUE,
                               config->swEccBlockBytes - requiredBytes);
                        srcNandData = &s_nandReadbackBlockBuffer[0];
                        requiredBytes = 0;
                    }
                }
                else if (!isAllBlankPayload)
                {
                    // Calculate data from ReadbackBlockBuffer (all 0xFFu bytes)
                    memset(s_nandReadbackBlockBuffer, NAND_MEM_BLANK_VALUE, config->swEccBlockBytes);
                    srcNandData = &s_nandReadbackBlockBuffer[0];
                    isAllBlankPayload = true;
                }

                nand_ecc_sec_calculate_encode((nand_ecc_block_size_t)config->swEccBlockBytes, srcNandData,
                                              (uint8_t *)&eccCode);
                eccParityData.block[eccIndex] = eccCode;
                eccIndex++;
                payloadedBytes += config->swEccBlockBytes;
            }
        }
        // Fill in spare area array with ECC parity data
        {
            assert(eccBlocks <= NAND_ECC_MAX_BLOCKS);
            nand_page_spare_area_t *spareArea = (nand_page_spare_area_t *)spareAreaArray;
            uint32_t eccIndex = 0;
            memset(spareArea, kNandDeviceInfo_NonBadBlockMark_x8, sizeof(spareAreaArray));
            while (eccIndex < eccBlocks)
            {
                spareArea->unit8Byte[eccIndex].eccParityData = eccParityData.block[eccIndex];
                eccIndex++;
            }
        }

        // Write ECC parity data to device spare area
        if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
        {
            spareAreaStart = baseAddress + config->bytesInPageDataArea;
            status = semc_axi_memory_write(spareAreaStart, (uint8_t *)spareAreaArray, config->bytesInPageSpareArea);
        }
        else if (config->memConfig.accessCommandType == kSemcAccessCommandType_IPBUSCMD)
        {
            status = semc_ipg_memory_write(&config->memConfig, (uint8_t *)spareAreaArray, config->bytesInPageSpareArea);
        }
    }

    return status;
}

#if BL_FEATURE_SEMC_NAND_MODULE
// Initialize Parallel NAND Flash device
status_t semc_nand_flash_init(semc_nand_config_t *config)
{
    status_t status;

    // Cleanup operation info
    memset((uint8_t *)&s_nandOperationInfo, 0, sizeof(s_nandOperationInfo));

    // Set Command set
    if (config->onfiVersion != kNandOnfiVersion_None)
    {
        s_nandOperationInfo.commandSet = kNandDeviceCmdSet_OnfiCompliant;
    }
    else
    {
        switch (config->memConfig.nandMemConfig.arrayAddressOption)
        {
            // Note: Some device use ONFI-like addressing and command set, but it is not ONFI
            //  compliant device, such as Toshiba TC58xx.
            case kSemcNandAddressOption_5byte_CA2RA3:
            case kSemcNandAddressOption_4byte_CA2RA2:
            case kSemcNandAddressOption_3byte_CA2RA1:
                s_nandOperationInfo.commandSet = kNandDeviceCmdSet_OnfiLike;
                break;
            // Note: Some device use specific addressing and command set, and of course it is
            //    not ONFI compliant device, such as Samsung K9F12xx.
            case kSemcNandAddressOption_4byte_CA1RA3:
            case kSemcNandAddressOption_3byte_CA1RA2:
            case kSemcNandAddressOption_2byte_CA1RA1:
            default:
                s_nandOperationInfo.commandSet = kNandDeviceCmdSet_Specific;
                break;
        }
    }

    // Reset columnAddressOption parameter if needed
    if ((config->enableEccCheck == kSemcNandEccCheckOption_Enabled) &&
        (config->eccCheckType == kSemcNandEccCheckType_SoftwareECC))
    {
        config->memConfig.nandMemConfig.columnAddressOption = kSemcNandColumnAddressOption_SpareAreaAccess;
    }
    else
    {
        config->memConfig.nandMemConfig.columnAddressOption = kSemcNandColumnAddressOption_PageAreaAccess;
    }

    // Set AC timing parameter
    semc_nand_set_ac_timing_parameter(config, kNandAcTimingSetOption_Basic);

    // Init SEMC module
    status = semc_init((semc_mem_config_t *)&config->memConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Try to read parameter page from device if it is ONFI compliant
    if (s_nandOperationInfo.commandSet == kNandDeviceCmdSet_OnfiCompliant)
    {
        // Issues the RESET command to device, make sure that we have clean NAND device status
        status = semc_nand_issue_reset(config);
        if (status != kStatus_Success)
        {
            return status;
        }

        // Just try to read ONFI parameter once
        status = semc_nand_make_use_of_onfi_parameter(config);
        if (status != kStatus_Success)
        {
            return status;
        }

        // Set AC timing parameter again
        semc_nand_set_ac_timing_parameter(config, kNandAcTimingSetOption_Advanced);
        // Reset specific tWHR parameter if needed
        semc_nand_adjust_timing_parameter_ccs(config);

        if ((s_nandOperationInfo.onfiAcTimingModeCode > kNandDeviceFeature_ONFI_TimingModeCode_Mode0) &&
            (s_nandOperationInfo.onfiAcTimingModeCode <= kNandDeviceFeature_ONFI_TimingModeCode_Mode5))
        {
            // Switch to specific timing mode
            status = semc_nand_switch_device_timing_mode(config);
            if (status != kStatus_Success)
            {
                return status;
            }
        }

        // Re-Init SEMC module using new parameter
        status = semc_init((semc_mem_config_t *)&config->memConfig);
        if (status != kStatus_Success)
        {
            return status;
        }
    }

    // Enable/Disable device ECC if necessary
    if (config->enableEccCheck == kSemcNandEccCheckOption_Enabled)
    {
        if (config->eccCheckType == kSemcNandEccCheckType_DeviceECC)
        {
            status = semc_nand_set_device_ecc(config, kSemcNandDeviceEccStatus_Enabled);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
        else if (config->eccCheckType == kSemcNandEccCheckType_SoftwareECC)
        {
            status = semc_nand_set_device_ecc(config, kSemcNandDeviceEccStatus_Disabled);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
    }

    return status;
}
#endif //BL_FEATURE_SEMC_NAND_MODULE

//!@brief Read page data from Parallel NAND via SEMC
status_t semc_nand_flash_read_page(semc_nand_config_t *config, uint32_t pageIndex, uint8_t *buffer, uint32_t length)
{
    status_t status = kStatus_Success;
    uint32_t ipgCmdAddr, memoryAccessAddr;
    uint32_t eccCheckedByte;

    ipgCmdAddr = pageIndex * config->bytesInPageDataArea;
    // Validate given page address
    if (ipgCmdAddr >= config->memConfig.nandMemConfig.ipgMemSizeInByte)
    {
        return kStatus_SemcNAND_InvalidMemoryAddress;
    }

    // Validate given length
    if (length > config->bytesInPageDataArea)
    {
        return kStatus_SemcNAND_MoreThanOnePageSize;
    }

    // Calculate the slave address for ipg command
    // Note NA2.0: the below ipgCmdAddr must be byte address no matter it is x8 
    //  or x16 device, see Note NA1.1 for reason
    ipgCmdAddr = semc_ipg_command_convert_nand_address(&config->memConfig, ipgCmdAddr);
    s_nandOperationInfo.rowAddressToGetSR = ipgCmdAddr;

    memoryAccessAddr = ipgCmdAddr;
    if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
    {
        memoryAccessAddr = config->memConfig.nandMemConfig.axiMemBaseAddress;
        // Issues the page read command to device
        status = semc_nand_issue_read_page(config, ipgCmdAddr);
        if (status != kStatus_Success)
        {
            return status;
        }
        // Check NAND Device ECC status if applicable
        if (!semc_nand_is_device_ecc_check_passed(config))
        {
            return kStatus_SemcNAND_EccCheckFail;
        }
    }
    else if (config->memConfig.accessCommandType == kSemcAccessCommandType_IPBUSCMD)
    {
        memoryAccessAddr = ipgCmdAddr;
        // Issues the page read command to device
        status = semc_nand_issue_read_page(config, ipgCmdAddr);
        if (status != kStatus_Success)
        {
            return status;
        }
        // Check NAND Device ECC status if applicable
        if (!semc_nand_is_device_ecc_check_passed(config))
        {
            return kStatus_SemcNAND_EccCheckFail;
        }
    }

    // Read all software ECC codes from spare area if applicable
    status = semc_nand_read_software_ecc_code_from_spare_area(config, memoryAccessAddr);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Read Page data from device
    while (length)
    {
        // Note: the memory address(row, column address) is always assigned by ipg commmand,
        //    the given address for axi command is just byte offset of available page array.
        if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
        {
            status =
                semc_axi_memory_read(memoryAccessAddr, s_nandReadbackBlockBuffer, sizeof(s_nandReadbackBlockBuffer));
            if (status != kStatus_Success)
            {
                return status;
            }
        }
        else if (config->memConfig.accessCommandType == kSemcAccessCommandType_IPBUSCMD)
        {
            status = semc_ipg_memory_read(&config->memConfig, (uint8_t *)s_nandReadbackBlockBuffer,
                                          sizeof(s_nandReadbackBlockBuffer));
            if (status != kStatus_Success)
            {
                return status;
            }
        }

        // Check NAND Software ECC status if applicable
        if (!semc_nand_is_software_ecc_check_passed(config, memoryAccessAddr))
        {
            return kStatus_SemcNAND_EccCheckFail;
        }

        // Copy temp readback data to user buffer
        if (length >= sizeof(s_nandReadbackBlockBuffer))
        {
            eccCheckedByte = sizeof(s_nandReadbackBlockBuffer);
        }
        else
        {
            eccCheckedByte = length;
        }
        memcpy(buffer, s_nandReadbackBlockBuffer, eccCheckedByte);
        memoryAccessAddr += eccCheckedByte;
        buffer += eccCheckedByte;
        length -= eccCheckedByte;
    }

    return status;
}

//!@brief Program page data to Parallel NAND via SEMC
status_t semc_nand_flash_page_program(semc_nand_config_t *config, uint32_t pageIndex, uint8_t *src, uint32_t length)
{
    status_t status = kStatus_Success;
    uint32_t memoryAccessAddr = 0;
    uint32_t ipgCmdAddr = pageIndex * config->bytesInPageDataArea;

    // Validate given length
    if (length < 1)
    {
        return kStatus_InvalidArgument;
    }
    // Validate given page address
    if (ipgCmdAddr >= config->memConfig.nandMemConfig.ipgMemSizeInByte)
    {
        return kStatus_SemcNAND_InvalidMemoryAddress;
    }

    // Validate given length
    if (length > config->bytesInPageDataArea)
    {
        return kStatus_SemcNAND_MoreThanOnePageSize;
    }

    // Calculate the slave address for ipg command
    // Note NA2.1: the below ipgCmdAddr must be byte address no matter it is x8 
    //  or x16 device, see Note NA1.1 for reason		
    ipgCmdAddr = semc_ipg_command_convert_nand_address(&config->memConfig, ipgCmdAddr);
    s_nandOperationInfo.rowAddressToGetSR = ipgCmdAddr;
    if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
    {
        memoryAccessAddr = config->memConfig.nandMemConfig.axiMemBaseAddress;
    }
    else if (config->memConfig.accessCommandType == kSemcAccessCommandType_IPBUSCMD)
    {
        memoryAccessAddr = ipgCmdAddr;
    }

    // Issues the page program command to device
    status = semc_nand_issue_program_page(config, ipgCmdAddr, kNandDeviceProgramProcess_Pre);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Write Page data to device
    // Note: the memory address(row, column address) is always assigned by ipg commmand,
    //    the given address for axi command is just byte offset of available page array.
    {
        uint32_t requiredBytes = length;
        uint32_t payloadedBytes = 0;
        uint8_t *writeSrc;
        bool isAllBlankPayload = false;

        assert(!(config->bytesInPageDataArea / sizeof(s_nandReadbackBlockBuffer)));
        while (payloadedBytes < config->bytesInPageDataArea)
        {
            uint32_t writeBytes = 0;
            if (requiredBytes)
            {
                if (requiredBytes >= sizeof(s_nandReadbackBlockBuffer))
                {
                    // Cache data from src buffer (all src bytes)
                    writeBytes =
                        (requiredBytes / sizeof(s_nandReadbackBlockBuffer)) * sizeof(s_nandReadbackBlockBuffer);
                    writeSrc = src;
                    requiredBytes -= writeBytes;
                }
                else
                {
                    // Cache data from ReadbackBlockBuffer (src bytes + 0xFFu bytes)
                    writeBytes = sizeof(s_nandReadbackBlockBuffer);
                    writeSrc = src + payloadedBytes;
                    memcpy(s_nandReadbackBlockBuffer, writeSrc, requiredBytes);
                    memset(&s_nandReadbackBlockBuffer[requiredBytes], NAND_MEM_BLANK_VALUE, writeBytes - requiredBytes);
                    writeSrc = &s_nandReadbackBlockBuffer[0];
                    requiredBytes = 0;
                }
            }
            else if (!isAllBlankPayload)
            {
                // Cache data from ReadbackBlockBuffer (all 0xFFu bytes)
                writeBytes = sizeof(s_nandReadbackBlockBuffer);
                memset(s_nandReadbackBlockBuffer, NAND_MEM_BLANK_VALUE, writeBytes);
                writeSrc = &s_nandReadbackBlockBuffer[0];
                isAllBlankPayload = true;
            }

            // Write cached data to NAND device
            if (config->memConfig.accessCommandType == kSemcAccessCommandType_AXI32CMD)
            {
                status = semc_axi_memory_write(memoryAccessAddr + payloadedBytes, writeSrc, writeBytes);
            }
            else if (config->memConfig.accessCommandType == kSemcAccessCommandType_IPBUSCMD)
            {
                status = semc_ipg_memory_write(&config->memConfig, writeSrc, writeBytes);
            }
            if (status != kStatus_Success)
            {
                return status;
            }

            payloadedBytes += writeBytes;
        }
    }

    // Prepare NAND Software ECC data if applicable
    status = semc_nand_write_ecc_data_in_spare_area(config, memoryAccessAddr, src, length);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Issues the page program command to device
    status = semc_nand_issue_program_page(config, ipgCmdAddr, kNandDeviceProgramProcess_Post);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Check NAND Device ECC status if applicable
    if (!semc_nand_is_device_ecc_check_passed(config))
    {
        return kStatus_SemcNAND_EccCheckFail;
    }

    if (config->enableReadbackVerify == kSemcNandReadbackVerifyOption_Enabled)
    {
        // Readback data to verify program
        status = semc_nand_flash_verify_page_program(config, pageIndex, src, length);
    }

    return status;
}

//!@brief Erase blocks of the Parallel NAND devices connected on SEMC
status_t semc_nand_flash_erase_block(semc_nand_config_t *config, uint32_t blockIndex)
{
    status_t status = kStatus_Success;
    uint32_t ipgCmdAddr;

    ipgCmdAddr = blockIndex * config->pagesInBlock * config->bytesInPageDataArea;
    // Validate given blockIndex
    if (ipgCmdAddr >= config->memConfig.nandMemConfig.ipgMemSizeInByte)
    {
        return kStatus_SemcNAND_InvalidMemoryAddress;
    }

    // Calculate the slave address for ipg command
    // Note NA2.2: the below ipgCmdAddr must be byte address no matter it is x8 
    //  or x16 device, see Note NA1.1 for reason
    ipgCmdAddr = semc_ipg_command_convert_nand_address(&config->memConfig, ipgCmdAddr);
    s_nandOperationInfo.rowAddressToGetSR = ipgCmdAddr;

    // Issues the block erase command to device
    status = semc_nand_issue_erase_block(config, ipgCmdAddr);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Check NAND Device ECC status if applicable
    if (!semc_nand_is_device_ecc_check_passed(config))
    {
        return kStatus_SemcNAND_EccCheckFail;
    }

    if (config->enableReadbackVerify == kSemcNandReadbackVerifyOption_Enabled)
    {
        // Readback data to verify erase
        uint32_t pageIndex = blockIndex * config->pagesInBlock;
        status = semc_nand_flash_verify_erase(config, pageIndex, config->pagesInBlock);
    }

    return kStatus_Success;
}

// Verify erase on Parallel NAND Flash device
status_t semc_nand_flash_verify_erase(semc_nand_config_t *config, uint32_t pageIndex, uint32_t pageCount)
{
    status_t status;
    uint8_t *dest = (uint8_t *)config->readbackPageBufferAddress;

    if (dest == NULL)
    {
        return kStatus_SemcNAND_InvalidReadbackBuffer;
    }

    while (pageCount--)
    {
        status = semc_nand_flash_read_page(config, pageIndex, dest, config->bytesInPageDataArea);
        if (status != kStatus_Success)
        {
            return status;
        }

        if (!semc_nand_is_mem_blank((uint32_t)dest, config->bytesInPageDataArea))
        {
            return kStatus_SemcNAND_EraseVerifyFailure;
        }
        pageIndex++;
    }

    return kStatus_Success;
}

// Verify program on Parallel NAND Flash device
status_t semc_nand_flash_verify_page_program(semc_nand_config_t *config,
                                             uint32_t pageIndex,
                                             const uint8_t *src,
                                             uint32_t lengthInBytes)
{
    status_t status;
    uint8_t *dest = (uint8_t *)config->readbackPageBufferAddress;

    if (dest == NULL)
    {
        return kStatus_SemcNAND_InvalidReadbackBuffer;
    }

    status = semc_nand_flash_read_page(config, pageIndex, dest, lengthInBytes);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (memcmp(src, dest, lengthInBytes))
    {
        return kStatus_SemcNAND_ProgramVerifyFailure;
    }

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
