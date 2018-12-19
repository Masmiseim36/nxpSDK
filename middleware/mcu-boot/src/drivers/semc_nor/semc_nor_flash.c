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
#include "semc_nor_flash.h"
#include "microseconds/microseconds.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define NOR_READY_CHECK_INTERVAL_NORMAL   (0U)
#define NOR_ERASE_TIMER_BIT_SET_INTERVAL_US   (200U)

//!@brief Parallel NOR data polling bit mask
// The data polling register can be read during PROGRAM, ERASE, or ERASE SUSPEND operations;
//  the READ operation outputs data on DQ[7:0].
enum _nor_data_polling_register_bit
{
    kNorDeviceDPR_DQ1_BufferedProgramAbortBitMask    = 0x02U,
    kNorDeviceDPR_DQ2_AlternativeToggleBitMask       = 0x04U,
    kNorDeviceDPR_DQ3_EraseTimerBitMask              = 0x08U,
    kNorDeviceDPR_DQ5_ErrorBitMask                   = 0x20U,
    kNorDeviceDPR_DQ6_ToggleBitMask                  = 0x40U,
    kNorDeviceDPR_DQ7_DataPollingBitMask             = 0x80U,
};

//!@brief Embedded Parallel NOR Standard Command Definitions
enum _nor_epscd_command_set
{
    // Must-have command
    kNorDeviceCmd_EPSCD_ResetToRead                   = 0xF0U,
    kNorDeviceCmd_EPSCD_AutoSelect                    = 0x90U,
    kNorDeviceCmd_EPSCD_ReadCfi                       = 0x98U,
    kNorDeviceCmd_EPSCD_ExitReadCfiAutoSlect          = 0xF0U,
    kNorDeviceCmd_EPSCD_UnlockBypass                  = 0x20U,
    kNorDeviceCmd_EPSCD_UnlockBypassResetSetup        = 0x90U,
    kNorDeviceCmd_EPSCD_UnlockBypassResetConfirm      = 0x00U,
    kNorDeviceCmd_EPSCD_EraseBlockSetup               = 0x80U,
    kNorDeviceCmd_EPSCD_EraseBlockConfirm             = 0x30U,
    kNorDeviceCmd_EPSCD_BufferProgramSetup            = 0x25U,
    kNorDeviceCmd_EPSCD_BufferProgramConfirm          = 0x29U,
};

//! @brief Constructs the three character code for tag
#if !defined(THREE_CHAR_CODE)
#define THREE_CHAR_CODE(a, b, c) (((c) << 16) | ((b) << 8) | ((a)))
#endif

//!@brief CFI config block tag
enum __nor_cfi_tags
{
    //! @brief Tag value used to validate the CFI parameter config block.
    kNorCfiTag = THREE_CHAR_CODE('Q', 'R', 'Y'),
};

#define NOR_CFI_PARAMETER_TABLE_START_ADDRESS   (0U)

//!@brief Parallel NOR CFI parameter config
typedef struct __nor_cfi_parameter_config
{
    struct
    {
        uint16_t reserved[16];                    //!< [0x000-0x01f]
        uint16_t queryUniqueString[3];            //!< [0x020-0x025]
        uint16_t primaryVendorId[2];              //!< [0x026-0x029]
        uint16_t primaryExtendedTableAddr[2];     //!< [0x02a-0x02d]
        uint16_t alternateVendorId[2];            //!< [0x02e-0x031]
        uint16_t alternateExtendedTableAddr[2];   //!< [0x032-0x035]
    } queryIdentificationString;
    struct
    {
        uint16_t reserved[4];                     //!< [0x036-0x03d]
        uint16_t typSingleProgramTimeInUs_2n;     //!< [0x03e-0x03f]
        uint16_t typBufferProgramTimeInUs_2n;     //!< [0x040-0x041]
        uint16_t typBlockEraseTimeInMs_2n;        //!< [0x042-0x043]
        uint16_t typChipEraseTimeInMs_2n;         //!< [0x044-0x045]
        uint16_t maxSingleProgramTimeOfTyp_2n;    //!< [0x046-0x047]
        uint16_t maxBufferProgramTimeOfTyp_2n;    //!< [0x048-0x049]
        uint16_t maxBlockEraseTimeOfTyp_2n;       //!< [0x04a-0x04b]
        uint16_t maxChipEraseTimeOfTyp_2n;        //!< [0x04c-0x04d]
    } querySystemInterfaceInformation;
    struct
    {
        uint16_t deviceSizeInBytes_2n;            //!< [0x04e-0x04f]
        uint16_t deviceInterfaceCode[2];          //!< [0x050-0x053]
        uint16_t maxProgramBufferInBytes_2n[2];   //!< [0x054-0x057]
        uint16_t eraseBlockRegionNumber;          //!< [0x058-0x059]
        uint16_t eraseBlockRegionInfo[4][4];      //!< [0x05a-0x079]
        uint16_t reserved[3];                     //!< [0x07a-0x07f]
    } deviceGeometryDefinition;
    // primaryExtendedTable;
    // alternateExtendedTable;
} nor_cfi_parameter_config_t;

//!@brief Parallel NOR command sequence address code
#define NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X08   (0xAAAU)
#define NOR_EPSCD_CMD_SEQ_ADDRESS_CODE2_X08   (0x555U)
#define NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X16   (0x555U)
#define NOR_EPSCD_CMD_SEQ_ADDRESS_CODE2_X16   (0x2AAU)

#define NOR_EPSCD_CMD_SEQ_DATA_CODE1_X08X16   (0xAAU)
#define NOR_EPSCD_CMD_SEQ_DATA_CODE2_X08X16   (0x55U)

//!@brief Parallel NOR command sequence cycle index
typedef enum _nor_cmd_seq_cycle_index
{
    kNorCmdSeqCycleIndex_1st          = 0x00U,
    kNorCmdSeqCycleIndex_2nd          = 0x01U,
    kNorCmdSeqCycleIndex_3rd          = 0x02U,
    kNorCmdSeqCycleIndex_4th          = 0x03U,
    kNorCmdSeqCycleIndex_5th          = 0x04U,
    kNorCmdSeqCycleIndex_6th          = 0x05U,
} nor_cmd_seq_cycle_index_t;

//!@brief Parallel NOR ipg cmd sequence cycle info
typedef struct __nor_ipg_cmd_seq_cycle_info
{
    struct
    {
        uint32_t address;
        uint32_t data;
    } addrDataCycle[6];
} nor_ipg_cmd_seq_cycle_info_t;

//!@brief Parallel NOR operation info
typedef struct __nor_operation_info
{
    uint32_t ipgCmdAddrToGetDPR;
    uint32_t bufferProgramTimeInUs;
    uint32_t blockEraseTimeInUs;
} nor_operation_info_t;


////////////////////////////////////////////////////////////////////////////////
// Local prototypes
////////////////////////////////////////////////////////////////////////////////

static void semc_nor_issue_reset_to_read(semc_nor_config_t *config);
static void semc_nor_issue_auto_select(semc_nor_config_t *config);
static void semc_nor_issue_read_cfi(semc_nor_config_t *config);
static void semc_nor_issue_exit_read_cfi_auto_select(semc_nor_config_t *config);
static void semc_nor_issue_unlock_bypass(semc_nor_config_t *config);
static void semc_nor_issue_unlock_bypass_reset(semc_nor_config_t *config);
static status_t semc_nor_issue_unlock_bypass_buffer_program(semc_nor_config_t *config, uint32_t ipgCmdAddr, uint8_t *data, uint32_t length);
static status_t semc_nor_issue_unlock_bypass_block_erase(semc_nor_config_t *config, uint32_t ipgCmdAddr);

/* Set AC timing cycle of SEMC according to list of AC time*/
static void semc_nor_set_ac_timing_parameter(semc_nor_config_t *config);
/* Init NOR command sequence cycle info*/
static void semc_nor_cmd_sequence_cycle_info_init(semc_nor_config_t *config);
/* Check the device toggle bit status*/
static status_t semc_nor_check_toggle_bit(semc_nor_config_t *config, uint32_t toggleCheckTimeoutInUs);
/* Check the device erase timer bit*/
static status_t semc_nor_poll_erase_timer_bit(semc_nor_config_t *config, uint32_t eraseCheckTimeoutInUs);
/* Read cfi parameter data from device and make use of them*/
static status_t semc_nor_make_use_of_cfi_parameter(semc_nor_config_t *config);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static nor_ipg_cmd_seq_cycle_info_t s_norIpgCmdSeqCycleInfo;
static nor_operation_info_t s_norOperationInfo;
static nor_cfi_parameter_config_t s_norCfiParameterConfig;

const nor_ac_timing_parameter_t s_norAcTimingParameterTable[] = {
  // Default-Setting: Safe, 6MHz
  {
      .min_tCES_ns = 0,
      .min_tCEH_ns = 0,
      .min_tCEITV_ns = 14,
      .min_tAS_ns = 10,
      .min_tAH_ns = 0,
      .min_tTA_ns = 0,
      .min_tWEL_ns = 66,
      .min_tWEH_ns = 33,
      .min_tAWDH_ns = 10,
      .min_tREL_ns = 156,
      .max_tREH_ns = 10
  },
  // Default-Setting: Fast, 10MHz
  {
      .min_tCES_ns = 0,
      .min_tCEH_ns = 0,
      .min_tCEITV_ns = 7,
      .min_tAS_ns = 5,
      .min_tAH_ns = 0,
      .min_tTA_ns = 0,
      .min_tWEL_ns = 40,
      .min_tWEH_ns = 20,
      .min_tAWDH_ns = 5,
      .min_tREL_ns = 96,
      .max_tREH_ns = 7
  }
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

uint8_t semc_nor_get_specific_ac_cycles(semc_nor_config_t *config, nor_ac_cycle_set_option_t acSetOption, uint32_t requiredTime_ns)
{
    uint8_t cycles;
    uint32_t baseClkInMHz, baseTick_ps;
    uint32_t requiredTime_ps = requiredTime_ns * 1000;

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

    baseTick_ps = 1000000 / baseClkInMHz;

    if (acSetOption == kNorAcCycleSetOption_Min)
    {
        // Note: From Design team, we need to always allocate at least 1-2ns more time
        //  for each AC parameter, besides the axi/ipg clock from CCM is not always accurate,
        //  So just to be safe, we allocate one more cycle here.
        //while (((cycles + 1) * baseTick_ps) < requiredTime_ps)
        cycles = 0;
        while ((cycles * baseTick_ps) < requiredTime_ps)
        {
            cycles++;
        }

        if (cycles >= (1 << kSemcMiscProperty_NOR_AcParameterBitWidth4b))
        {
            cycles = (1 << kSemcMiscProperty_NOR_AcParameterBitWidth4b) - 1;
        }
    }
    else if (acSetOption == kNorAcCycleSetOption_Max)
    {
        cycles = (1 << kSemcMiscProperty_NOR_AcParameterBitWidth4b);
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

void semc_nor_set_ac_timing_parameter(semc_nor_config_t *config)
{
    nor_ac_timing_parameter_t acTimingArray;
    uint16_t *pAcTimingArray = &acTimingArray.min_tCES_ns;
    uint16_t *pAcTimingTable;
    uint8_t acTimingMode = config->acTimingMode;

    // Validate User-Defined AC timing cycle
    if (acTimingMode == kSemcNorAcTimingMode_UserDefined)
    {
        bool isUserDefinedTimingValid = false;
        uint8_t *pAcTimingParameter = &config->memConfig.norMemConfig.ceSetupTime;
        while (pAcTimingParameter <= &config->memConfig.norMemConfig.syncReadCycleTime)
        {
            if (*pAcTimingParameter != 0)
            {
                isUserDefinedTimingValid = true;
            }
            pAcTimingParameter++;
        }
        if (isUserDefinedTimingValid)
        {
            return;

        }
        else
        {
            acTimingMode = kSemcNorAcTimingMode_DefaultSafe;
            config->acTimingMode = acTimingMode;
        }
    }
    else if (acTimingMode >= kSemcNorAcTimingMode_Invalid)
    {
        acTimingMode = kSemcNorAcTimingMode_DefaultSafe;
        config->acTimingMode = acTimingMode;
    }
    pAcTimingTable = (uint16_t *)&s_norAcTimingParameterTable[acTimingMode].min_tCES_ns;

    // Calculate AC timing cycle for SEMC based on selected AC timing table
    memset(&acTimingArray, 0, sizeof(nor_ac_timing_parameter_t));
    for (;pAcTimingArray <= &acTimingArray.min_tREL_ns; pAcTimingArray++, pAcTimingTable++)
    {
        *pAcTimingArray = semc_nor_get_specific_ac_cycles(config, kNorAcCycleSetOption_Min, *pAcTimingTable);
    }
    pAcTimingArray = &acTimingArray.max_tREH_ns;
    pAcTimingTable = (uint16_t *)&s_norAcTimingParameterTable[acTimingMode].max_tREH_ns;
    *pAcTimingArray = semc_nor_get_specific_ac_cycles(config, kNorAcCycleSetOption_Max, *pAcTimingTable);

    // Set AC timing cycle
    // For Common sequence and Async mode READ
    config->memConfig.norMemConfig.ceSetupTime = acTimingArray.min_tCES_ns;
    config->memConfig.norMemConfig.ceMinHoldTime = acTimingArray.min_tCEH_ns;
    config->memConfig.norMemConfig.ceMinIntervalTime = acTimingArray.min_tCEITV_ns;
    config->memConfig.norMemConfig.addressSetupTime = acTimingArray.min_tAS_ns;
    config->memConfig.norMemConfig.addressHoldTime = acTimingArray.min_tAH_ns;
    config->memConfig.norMemConfig.asyncOeLowTime = acTimingArray.min_tREL_ns;
    config->memConfig.norMemConfig.asyncOeHighTime = acTimingArray.max_tREH_ns;
    config->memConfig.norMemConfig.asyncTurnaroundTime = acTimingArray.min_tTA_ns;

    // For Async mode WRITE
    config->memConfig.norMemConfig.asyncWeLowTime = acTimingArray.min_tWEL_ns;
    config->memConfig.norMemConfig.asyncWeHighTime = acTimingArray.min_tWEH_ns;
    config->memConfig.norMemConfig.asyncAddressToDataHoldTime = acTimingArray.min_tAWDH_ns;

    // For Sync mode sequnce only
    //config->memConfig.norMemConfig.syncDataSetupTime = 0;
    //config->memConfig.norMemConfig.syncDataHoldTime = 0;
    //config->memConfig.norMemConfig.syncLatencyCount = 0;
    //config->memConfig.norMemConfig.syncReadCycleTime = 0;
}

void semc_nor_cmd_sequence_cycle_info_init(semc_nor_config_t *config)
{
    // Cleanup config block
    memset((uint8_t *)&s_norIpgCmdSeqCycleInfo, 0, sizeof(s_norIpgCmdSeqCycleInfo));

    // Get sequence cycle address
    if (config->memConfig.norMemConfig.dataPortWidth == 8)
    {
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X08;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE2_X08;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_3rd].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X08;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_4th].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X08;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_5th].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE2_X08;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_6th].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X08;
    }
    else if (config->memConfig.norMemConfig.dataPortWidth == 16)
    {
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X16;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE2_X16;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_3rd].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X16;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_4th].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X16;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_5th].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE2_X16;
        s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_6th].address = NOR_EPSCD_CMD_SEQ_ADDRESS_CODE1_X16;
    }

    // Convert sequence cycle address
    for (uint32_t index = kNorCmdSeqCycleIndex_1st; index <= kNorCmdSeqCycleIndex_6th; index++)
    {
        s_norIpgCmdSeqCycleInfo.addrDataCycle[index].address = semc_ipg_command_convert_nor_address(&config->memConfig, s_norIpgCmdSeqCycleInfo.addrDataCycle[index].address);
    }

    // Get sequence cycle data
    s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].data = NOR_EPSCD_CMD_SEQ_DATA_CODE1_X08X16;
    s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].data = NOR_EPSCD_CMD_SEQ_DATA_CODE2_X08X16;
    s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_4th].data = NOR_EPSCD_CMD_SEQ_DATA_CODE1_X08X16;
    s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_5th].data = NOR_EPSCD_CMD_SEQ_DATA_CODE2_X08X16;
}

/*  Check the device ready status*/
status_t semc_nor_check_toggle_bit(semc_nor_config_t *config, uint32_t toggleCheckTimeoutInUs)
{
    status_t status = kStatus_Success;
    uint32_t DPR_r1 = 0;
    uint32_t DPR_r2 = 0;
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();

    uint64_t deltaTicks;
    uint64_t baseTicks = microseconds_get_ticks();
    uint64_t timeoutTicks = microseconds_convert_to_ticks(toggleCheckTimeoutInUs);

    // Step 1: Read DQ6 (into a word)
    uint32_t anyAddress = semc_ipg_command_convert_nor_address(&config->memConfig, 0);
    semc_ipg_command_device_read(anyAddress, SEMC_IPCMD_NOR_IPG_READ_ARRAY, (uint8_t *)&DPR_r1, ipgCmdDataSize);

    while(1)
    {
        // TimeOut: If, for some reason, the hardware fails then this
        //  loop exit and the function return flash_OperationTimeOut.
        deltaTicks = microseconds_get_ticks() - baseTicks;
        if (deltaTicks > timeoutTicks)
        {
            return kStatus_SemcNOR_DeviceTimeout;
        }

        // Step 2: Read DA1,DQ5 and DQ6 (into another word)
        semc_ipg_command_device_read(anyAddress, SEMC_IPCMD_NOR_IPG_READ_ARRAY, (uint8_t *)&DPR_r2, ipgCmdDataSize);

        // Step 3: If DQ6 did not toggle between the two reads , operation is complete.
        if ((DPR_r1 & kNorDeviceDPR_DQ6_ToggleBitMask) == (DPR_r2 & kNorDeviceDPR_DQ6_ToggleBitMask))
        {
            return kStatus_Success;
        }

        // DQ6 is toggling.
        if (!(DPR_r2 & kNorDeviceDPR_DQ5_ErrorBitMask))
        {
            if (!(DPR_r2 & kNorDeviceDPR_DQ1_BufferedProgramAbortBitMask))
            {
                // DQ1 and DQ5 are zero.
                DPR_r1 = DPR_r2;

                // Step 4: Else if DQ5 is zero  and DQ!=1  then operation is not yet complete, goto 1
                continue;
            }
        }
        // DQ1 or DQ5 is 1.

        // Step 5: Else (DQ5 != 0) and DQ1==0, read DQ6 again
        semc_ipg_command_device_read(anyAddress, SEMC_IPCMD_NOR_IPG_READ_ARRAY, (uint8_t *)&DPR_r1, ipgCmdDataSize);
        semc_ipg_command_device_read(anyAddress, SEMC_IPCMD_NOR_IPG_READ_ARRAY, (uint8_t *)&DPR_r2, ipgCmdDataSize);

        // Step 6: If DQ6 did not toggle between the last two reads then return
        //         Flash_Success
        if ((DPR_r1 & kNorDeviceDPR_DQ6_ToggleBitMask) != (DPR_r2 & kNorDeviceDPR_DQ6_ToggleBitMask))
        {
            // DQ6 is toggling, an error has occurred. We don't know whether a write or an erase
            // operation is/was running, so we can't return anything useful as an error code.

            // Step 7: Else return error
            if (DPR_r2 & kNorDeviceDPR_DQ5_ErrorBitMask)
            {
                status = kStatus_SemcNOR_CommandErrorNoOpToSuspend;
            }
            else
            {
                status = kStatus_SemcNOR_CommandErrorNoInfoAvailable;
            }
        }
       break;
    }

    return status;
}

status_t semc_nor_poll_erase_timer_bit(semc_nor_config_t *config, uint32_t eraseCheckTimeoutInUs)
{
    uint32_t DPR;
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();

    uint64_t deltaTicks;
    uint64_t baseTicks = microseconds_get_ticks();
    uint64_t timeoutTicks = microseconds_convert_to_ticks(eraseCheckTimeoutInUs);

    while(1)
    {
        // TimeOut: If, for some reason, the hardware fails then this
        //  loop exit and the function return flash_OperationTimeOut.
        deltaTicks = microseconds_get_ticks() - baseTicks;
        if (deltaTicks > timeoutTicks)
        {
            return kStatus_SemcNOR_DeviceTimeout;
        }

        // Step 2: Wait for the Erase Timer Bit (DQ3) to be set
        semc_ipg_command_device_read(s_norOperationInfo.ipgCmdAddrToGetDPR, SEMC_IPCMD_NOR_IPG_READ_ARRAY, (uint8_t *)&DPR, ipgCmdDataSize);
        if (DPR & kNorDeviceDPR_DQ3_EraseTimerBitMask)
        {
            break;
        }
    }

    return kStatus_Success;
}

status_t semc_nor_make_use_of_cfi_parameter(semc_nor_config_t *config)
{
    status_t status = kStatus_Success;
    uint32_t cfiStartAddress = NOR_CFI_PARAMETER_TABLE_START_ADDRESS;

    // Cleanup CFI config block
    memset((uint8_t *)&s_norCfiParameterConfig, 0, sizeof(s_norCfiParameterConfig));

    // Read CFI parameter data from device
    semc_nor_issue_auto_select(config);
    semc_nor_issue_read_cfi(config);
    cfiStartAddress = semc_ipg_command_convert_nor_address(&config->memConfig, cfiStartAddress);
    status = semc_axi_memory_read(cfiStartAddress, (uint8_t *)&s_norCfiParameterConfig, sizeof(s_norCfiParameterConfig));
    // Exit CFI Read mode
    semc_nor_issue_exit_read_cfi_auto_select(config);
    // Exit Auto select mode
    semc_nor_issue_exit_read_cfi_auto_select(config);
    semc_nor_issue_reset_to_read(config);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Validate CFI signature
    uint32_t cfiSignature = ((s_norCfiParameterConfig.queryIdentificationString.queryUniqueString[0] & 0xFFu) + \
                            ((s_norCfiParameterConfig.queryIdentificationString.queryUniqueString[1] & 0xFFu) << 8) + \
                            ((s_norCfiParameterConfig.queryIdentificationString.queryUniqueString[2] & 0xFFu) << 16));
    if (cfiSignature != kNorCfiTag)
    {
        return kStatus_SemcNOR_InvalidCfiSignature;
    }

    // Set device timeout info
    s_norOperationInfo.bufferProgramTimeInUs = (1 << (s_norCfiParameterConfig.querySystemInterfaceInformation.typBufferProgramTimeInUs_2n + \
                                                      s_norCfiParameterConfig.querySystemInterfaceInformation.maxBufferProgramTimeOfTyp_2n));
    s_norOperationInfo.blockEraseTimeInUs = 1000 * (1 << (s_norCfiParameterConfig.querySystemInterfaceInformation.typBlockEraseTimeInMs_2n + \
                                                          s_norCfiParameterConfig.querySystemInterfaceInformation.maxBlockEraseTimeOfTyp_2n));

    // Set NOR memory info
    if (s_norCfiParameterConfig.deviceGeometryDefinition.eraseBlockRegionNumber == 1)
    {
        config->blockSizeInBytes = (s_norCfiParameterConfig.deviceGeometryDefinition.eraseBlockRegionInfo[0][2] + \
                                   (s_norCfiParameterConfig.deviceGeometryDefinition.eraseBlockRegionInfo[0][3] << 16));
        config->blockCount = (s_norCfiParameterConfig.deviceGeometryDefinition.eraseBlockRegionInfo[0][0] + \
                             (s_norCfiParameterConfig.deviceGeometryDefinition.eraseBlockRegionInfo[0][1] << 16) + 1);
    }
    config->pageSizeInBytes = (1 << (s_norCfiParameterConfig.deviceGeometryDefinition.maxProgramBufferInBytes_2n[0] + \
                                    (s_norCfiParameterConfig.deviceGeometryDefinition.maxProgramBufferInBytes_2n[1] << 16)));

    return status;
}

void semc_nor_issue_reset_to_read(semc_nor_config_t *config)
{
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();
    uint32_t anyAddress = semc_ipg_command_convert_nor_address(&config->memConfig, 0);

    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].data,
                                  ipgCmdDataSize);

    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].data,
                                  ipgCmdDataSize);

    semc_ipg_command_device_write(anyAddress, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_ResetToRead, ipgCmdDataSize);
}

void semc_nor_issue_auto_select(semc_nor_config_t *config)
{
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();
    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].data,
                                  ipgCmdDataSize);

    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].data,
                                  ipgCmdDataSize);

    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_3rd].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  kNorDeviceCmd_EPSCD_AutoSelect,
                                  ipgCmdDataSize);
}

void semc_nor_issue_read_cfi(semc_nor_config_t *config)
{
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();
    uint32_t ipgCmdAddr = s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].address;
    semc_ipg_command_device_write(ipgCmdAddr, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_ReadCfi, ipgCmdDataSize);
}

void semc_nor_issue_exit_read_cfi_auto_select(semc_nor_config_t *config)
{
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();
    uint32_t anyAddress = semc_ipg_command_convert_nor_address(&config->memConfig, 0);

    semc_ipg_command_device_write(anyAddress, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_ExitReadCfiAutoSlect, ipgCmdDataSize);
}

void semc_nor_issue_unlock_bypass(semc_nor_config_t *config)
{
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();
    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_1st].data,
                                  ipgCmdDataSize);

    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_2nd].data,
                                  ipgCmdDataSize);

    semc_ipg_command_device_write(s_norIpgCmdSeqCycleInfo.addrDataCycle[kNorCmdSeqCycleIndex_3rd].address,
                                  SEMC_IPCMD_NOR_IPG_WRITE_ARRAY,
                                  kNorDeviceCmd_EPSCD_UnlockBypass,
                                  ipgCmdDataSize);
}

void semc_nor_issue_unlock_bypass_reset(semc_nor_config_t *config)
{
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();
    uint32_t anyAddress = semc_ipg_command_convert_nor_address(&config->memConfig, 0);
    semc_ipg_command_device_write(anyAddress, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_UnlockBypassResetSetup, ipgCmdDataSize);
    semc_ipg_command_device_write(anyAddress, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_UnlockBypassResetConfirm, ipgCmdDataSize);
}

status_t semc_nor_issue_unlock_bypass_buffer_program(semc_nor_config_t *config, uint32_t ipgCmdAddr, uint8_t *data, uint32_t length)
{
    status_t status = kStatus_Success;
    uint32_t bytesPerCycle = (config->memConfig.norMemConfig.dataPortWidth / 8);
    uint32_t remainCycles = ALIGN_UP(length, bytesPerCycle) / bytesPerCycle;
    bool isPaddingByteNeeded = length % bytesPerCycle;
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();

    assert(config->memConfig.norMemConfig.dataPortWidth <= 16);

    do
    {
        uint32_t currentCycles = config->pageSizeInBytes / bytesPerCycle;
        uint32_t firstOffsetInBuffer = ipgCmdAddr;

        if (currentCycles > remainCycles)
        {
            currentCycles = remainCycles;
        }

        // Issue unlock bypass buffer program command
        semc_ipg_command_device_write(firstOffsetInBuffer, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_BufferProgramSetup, ipgCmdDataSize);
        semc_ipg_command_device_write(firstOffsetInBuffer, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, currentCycles - 1, ipgCmdDataSize);

        // From Device RM: the fourth bus WRITE cycle sets up the number of words/bytes to be programmed.
        //  So we need to make sure that the IP CMD data size is equal to byte per device cycle.
        if (bytesPerCycle != ipgCmdDataSize)
        {
            semc_ipg_command_set_data_size(bytesPerCycle);
        }
        for (uint32_t i = 0; i < currentCycles; i++)
        {
            uint32_t programData = (uint32_t)*data;
            if (bytesPerCycle == 2)
            {
                uint8_t payloadByte;
                if ((i == (currentCycles - 1)) && isPaddingByteNeeded)
                {
                    payloadByte = NOR_MEM_BLANK_VALUE;
                }
                else
                {
                    payloadByte = *(data+1);
                }
                programData += (uint32_t)payloadByte << 8;
            }
            semc_ipg_command_device_write(ipgCmdAddr, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, programData, bytesPerCycle);
            data += bytesPerCycle;
            ipgCmdAddr += bytesPerCycle;
        }
        // Re-cover IP CMD data size if necessary
        if (bytesPerCycle != ipgCmdDataSize)
        {
            semc_ipg_command_set_data_size(ipgCmdDataSize);
        }

        // Issue unlock bypass buffer program confirm command
        semc_ipg_command_device_write(firstOffsetInBuffer, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_BufferProgramConfirm, ipgCmdDataSize);

        // Follow Bit Toggle Flow Chart until Program Controller completes
        status = semc_nor_check_toggle_bit(config, s_norOperationInfo.bufferProgramTimeInUs);
        if (status != kStatus_Success)
        {
            semc_nor_issue_reset_to_read(config);
            return kStatus_SemcNOR_BufferProgramCommandFailure;
        }

        remainCycles -= currentCycles;
    }
    while (remainCycles);

    return status;
}

status_t semc_nor_issue_unlock_bypass_block_erase(semc_nor_config_t *config, uint32_t ipgCmdAddr)
{
    status_t status = kStatus_Success;
    uint8_t ipgCmdDataSize = semc_ipg_command_get_data_size();
    uint32_t anyAddress = semc_ipg_command_convert_nor_address(&config->memConfig, 0);

    // Issue unlock bypass block erase command
    semc_ipg_command_device_write(anyAddress, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_EraseBlockSetup, ipgCmdDataSize);
    semc_ipg_command_device_write(ipgCmdAddr, SEMC_IPCMD_NOR_IPG_WRITE_ARRAY, kNorDeviceCmd_EPSCD_EraseBlockConfirm, ipgCmdDataSize);

    // Wait for the Erase Timer Bit (DQ3) to be set
    status = semc_nor_poll_erase_timer_bit(config, NOR_ERASE_TIMER_BIT_SET_INTERVAL_US);
    if (status != kStatus_Success)
    {
        semc_nor_issue_reset_to_read(config);
        return status;
    }

    // Follow Bit Toggle Flow Chart until Erase Controller completes
    status = semc_nor_check_toggle_bit(config, s_norOperationInfo.blockEraseTimeInUs);
    if (status != kStatus_Success)
    {
        semc_nor_issue_reset_to_read(config);
        return status;
    }

    return status;
}

// Verify erase on Parallel NOR Flash device
status_t semc_nor_flash_verify_erase(uint32_t startAddr, uint32_t lengthInBytes)
{
    uint8_t *dstByte = (uint8_t *)startAddr;

    while (lengthInBytes)
    {
        if ((*dstByte) != NOR_MEM_BLANK_VALUE)
        {
            return kStatus_SemcNOR_EraseVerifyFailure;
        }
        dstByte++;
        lengthInBytes--;
    }

    return kStatus_Success;
}

// Verify program on Parallel NOR Flash device
status_t semc_nor_flash_verify_program(uint32_t startAddr, const uint8_t * src, uint32_t lengthInBytes)
{
    uint8_t *dstByte = (uint8_t *)startAddr;
    uint8_t *srcByte = (uint8_t *)src;

    while (lengthInBytes)
    {
        if ((*dstByte) != (*srcByte))
        {
            return kStatus_SemcNOR_ProgramVerifyFailure;
        }
        dstByte++;
        srcByte++;
        lengthInBytes--;
    }

    return kStatus_Success;
}

#if BL_FEATURE_SEMC_NOR_MODULE
// Initialize Parallel NOR Flash device
status_t semc_nor_flash_init(semc_nor_config_t *config)
{
    status_t status;

    semc_nor_set_ac_timing_parameter(config);

    status = semc_init((semc_mem_config_t *)&config->memConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (config->deviceCommandSet != kSemcNorDeviceCommandSet_EPSCD)
    {
        return kStatus_SemcNOR_unmatchedCommandSet;
    }

    // Cleanup operation info
    memset((uint8_t *)&s_norOperationInfo, 0, sizeof(s_norOperationInfo));

    semc_nor_cmd_sequence_cycle_info_init(config);

    // Issues the RESET command to device, make sure that we have clean NOR device status
    // From Spec:
    //   Reset command may cause a return to read mode from undefined states that might result from invalid command sequences.
    //   A hardware reset may be required to return to normal operation from some undefined states.
    semc_nor_issue_reset_to_read(config);

    status = semc_nor_make_use_of_cfi_parameter(config);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Reset Device to Read mode after reading CFI parameter
    semc_nor_issue_reset_to_read(config);

    return kStatus_Success;
}
#endif //BL_FEATURE_SEMC_NOR_MODULE

// Program buffer data to Parallel NOR Flash device
status_t semc_nor_flash_page_program(semc_nor_config_t *config,
                                     uint32_t dstAddr,
                                     const uint8_t *src,
                                     uint32_t length)
{
    status_t status;
    uint32_t ipgCmdAddr = dstAddr;
    uint32_t axiCmdAddr = dstAddr;
    uint32_t alignmentBaseByte = config->memConfig.norMemConfig.dataPortWidth / 8;
    s_norOperationInfo.ipgCmdAddrToGetDPR = ipgCmdAddr;

    // Validate given memory range
    if ((axiCmdAddr < config->memConfig.norMemConfig.comMemBaseAddress) ||
        ((axiCmdAddr + length) > (config->memConfig.norMemConfig.comMemBaseAddress + config->memConfig.norMemConfig.comMemSizeInByte)))
    {
        return kStatus_SemcNOR_InvalidMemoryAddress;
    }

    // Validate given length
    if (axiCmdAddr % alignmentBaseByte)
    {
        return kStatus_SemcNOR_AddressAlignmentError;
    }

    // Issues the buffer program command to device
    semc_nor_issue_unlock_bypass(config);
    status = semc_nor_issue_unlock_bypass_buffer_program(config, ipgCmdAddr, (uint8_t *)src, length);
    semc_nor_issue_unlock_bypass_reset(config);

    // Only return to read mode then we can see the result
    semc_nor_issue_reset_to_read(config);

    if (config->enableReadbackVerify == kSemcNorReadbackVerifyOption_Enabled)
    {
        // Readback data to verify program
        status = semc_nor_flash_verify_program(axiCmdAddr, src, length);
    }

    return status;
}

// Erase sectors on Parallel NOR Flash device
status_t semc_nor_flash_erase_block(semc_nor_config_t *config, uint32_t blockIndex)
{
    status_t status;
    uint32_t axiCmdAddr = blockIndex * config->blockSizeInBytes + config->memConfig.norMemConfig.comMemBaseAddress;
    uint32_t ipgCmdAddr = axiCmdAddr;
    s_norOperationInfo.ipgCmdAddrToGetDPR = ipgCmdAddr;

    // Validate given blockIndex
    if ((blockIndex >= config->blockCount) || (axiCmdAddr < config->memConfig.norMemConfig.comMemBaseAddress))
    {
        return kStatus_SemcNOR_InvalidMemoryAddress;
    }

    // Issues the block erase command to device
    semc_nor_issue_unlock_bypass(config);
    status = semc_nor_issue_unlock_bypass_block_erase(config, ipgCmdAddr);
    semc_nor_issue_unlock_bypass_reset(config);

    // Only return to read mode then we can see the result
    semc_nor_issue_reset_to_read(config);

    if (config->enableReadbackVerify == kSemcNorReadbackVerifyOption_Enabled)
    {
        // Readback data to verify erase
        status = semc_nor_flash_verify_erase(axiCmdAddr, config->blockSizeInBytes);
    }

    return status;
}


////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
