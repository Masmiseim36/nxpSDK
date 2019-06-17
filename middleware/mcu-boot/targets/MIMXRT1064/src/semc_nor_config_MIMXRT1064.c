/*
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "semc/fsl_semc.h"
#include "semc_nor/semc_nor_flash.h"
#include "memory/src/semc_nor_memory.h"
#include "fusemap.h"
#include "utilities/fsl_assert.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// 0 - RT1050 A0
// 1 - RT1050 A1
#define SEMC_NOR_FUSE_MAP_VERSION (0)

/* Fuse: Clock frequency */
enum
{
    kFuseSemcNor_ClkFreq_33MHz = 0U,
    kFuseSemcNor_ClkFreq_66MHz = 1U,
    kFuseSemcNor_ClkFreq_108MHz = 2U,
    kFuseSemcNor_ClkFreq_133MHz = 3U,
    // kFuseSemcNor_ClkFreq_Max  = 4-7U,
};

/* Fuse: AC Timing mode */
enum
{
    kFuseSemcNor_AcTiming_Default = 0U,
    kFuseSemcNor_AcTiming_Fuse = 1U,
};

/* Fuse: AC default Timing speed */
enum
{
    kFuseSemcNor_AcTimingSpeed_Safe = 0U,
    kFuseSemcNor_AcTimingSpeed_Fast = 1U,
};

/* Fuse: Data Port Size */
enum
{
    kFuseSemcNor_DataPortSize_16bit = 0U,
    kFuseSemcNor_DataPortSize_8bit = 1U,
};

/* Fuse: DQS mode */
enum
{
    kFuseSemcNor_DqsPadMode_Disabled = 0U,
    kFuseSemcNor_DqsPadMode_Enabled = 1U,
};

/* Fuse: Pcs selection */
enum
{
    kFuseSemcNor_PcsSelection_CSX0 = 0U,
    kFuseSemcNor_PcsSelection_CSX1 = 1U,
    kFuseSemcNor_PcsSelection_CSX2 = 2U,
    kFuseSemcNor_PcsSelection_CSX3 = 3U,
    // kFuseSemcNor_PcsSelection_A8 = 4-6U,
    kFuseSemcNor_PcsSelection_RDY = 7U,
};

/* Fuse: Address Port Size */
enum
{
    kFuseSemcNor_AddressPortSize_25bit = 4U,
    kFuseSemcNor_AddressPortSize_26bit = 5U,
    kFuseSemcNor_AddressPortSize_27bit = 6U,
    kFuseSemcNor_AddressPortSize_28bit = 7U,
    // kFuseSemcNor_AddressPortSize_24bit  = 0-3U,
};

/* Fuse: ADV# Polarity */
enum
{
    kFuseSemcNor_AdvPolarity_Low = 0U,
    kFuseSemcNor_AdvPolarity_High = 1U,
};

/* Fuse: RDY Polarity */
enum
{
    kFuseSemcNor_RdyPolarity_High = 0U,
    kFuseSemcNor_RdyPolarity_Low = 1U,
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void semc_nor_get_config_data_from_fuse(semc_nor_config_t *config)
{
    semc_nor_config_t *pNorConfig = config;
    uint32_t fuseValue;

    // Read fuse to get clk freq
    fuseValue = ROM_OCOTP_SEMC_NOR_CLK_FREQ_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_ClkFreq_33MHz:
            pNorConfig->memConfig.asyncClkFreq = kSemcClkFreq_33MHz;
            break;
        case kFuseSemcNor_ClkFreq_66MHz:
            pNorConfig->memConfig.asyncClkFreq = kSemcClkFreq_66MHz;
            break;
        case kFuseSemcNor_ClkFreq_108MHz:
            pNorConfig->memConfig.asyncClkFreq = kSemcClkFreq_108MHz;
            break;
        case kFuseSemcNor_ClkFreq_133MHz:
            pNorConfig->memConfig.asyncClkFreq = kSemcClkFreq_133MHz;
            break;
        default:
            pNorConfig->memConfig.asyncClkFreq = SEMC_MAX_CLK_FREQ;
            break;
    }

    // Get AC Timing
    fuseValue = ROM_OCOTP_SEMC_NOR_AC_TIMING_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_AcTiming_Fuse:
            pNorConfig->acTimingMode = kSemcNorAcTimingMode_UserDefined;
            break;
        case kFuseSemcNor_AcTiming_Default:
        default:
            pNorConfig->acTimingMode = kSemcNorAcTimingMode_DefaultSafe;
            break;
    }
    // Get User defined AC timing
    if (pNorConfig->acTimingMode == kSemcNorAcTimingMode_UserDefined)
    {
        pNorConfig->memConfig.norMemConfig.ceSetupTime = ROM_OCOTP_SEMC_NOR_AC_CES_VALUE() * 5;
        pNorConfig->memConfig.norMemConfig.addressSetupTime = ROM_OCOTP_SEMC_NOR_AC_AS_VALUE();
        pNorConfig->memConfig.norMemConfig.addressHoldTime = ROM_OCOTP_SEMC_NOR_AC_AH_VALUE();
        pNorConfig->memConfig.norMemConfig.asyncTurnaroundTime = ROM_OCOTP_SEMC_NOR_AC_TA_VALUE() * 5;
        pNorConfig->memConfig.norMemConfig.asyncOeLowTime = ROM_OCOTP_SEMC_NOR_AC_REL_VALUE();
        pNorConfig->memConfig.norMemConfig.asyncOeHighTime = ROM_OCOTP_SEMC_NOR_AC_REH_VALUE();
        pNorConfig->memConfig.norMemConfig.ceMinHoldTime = ROM_OCOTP_SEMC_NOR_AC_CEH_VALUE() * 5;
        pNorConfig->memConfig.norMemConfig.ceMinIntervalTime = ROM_OCOTP_SEMC_NOR_AC_CEITV_VALUE() * 5;
    }
#if (SEMC_NOR_FUSE_MAP_VERSION == 1)
    else
    {
        // Get Default AC Timing Speed
        fuseValue = ROM_OCOTP_SEMC_NOR_AC_TIMING_SPEED_VALUE();
        switch (fuseValue)
        {
            case kFuseSemcNor_AcTimingSpeed_Fast:
                pNorConfig->acTimingMode = kSemcNorAcTimingMode_DefaultFast;
                break;
            case kFuseSemcNor_AcTimingSpeed_Safe:
            default:
                pNorConfig->acTimingMode = kSemcNorAcTimingMode_DefaultSafe;
                break;
        }
    }
#endif

    // Get Data Port Size
    fuseValue = ROM_OCOTP_SEMC_NOR_DATA_PORT_SIZE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_DataPortSize_16bit:
            pNorConfig->memConfig.norMemConfig.dataPortWidth = 16;
            break;
        case kFuseSemcNor_DataPortSize_8bit:
        default:
            pNorConfig->memConfig.norMemConfig.dataPortWidth = 8;
            break;
    }

#if (SEMC_NOR_FUSE_MAP_VERSION == 0)
    // Get DQS pad mode
    fuseValue = ROM_OCOTP_SEMC_NOR_DQS_PAD_MODE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_DqsPadMode_Enabled:
            pNorConfig->memConfig.readStrobeMode = kSemcDqsMode_LoopbackFromDqsPad;
            break;
        case kFuseSemcNor_DqsPadMode_Disabled:
        default:
            pNorConfig->memConfig.readStrobeMode = kSemcDqsMode_LoopbackInternally;
            break;
    }

    // Get PCS selection
    fuseValue = ROM_OCOTP_SEMC_NOR_PCS_SELECTION_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_PcsSelection_CSX0:
            pNorConfig->memConfig.norMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX0;
            break;
        case kFuseSemcNor_PcsSelection_CSX1:
            pNorConfig->memConfig.norMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX1;
            break;
        case kFuseSemcNor_PcsSelection_CSX2:
            pNorConfig->memConfig.norMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX2;
            break;
        case kFuseSemcNor_PcsSelection_CSX3:
            pNorConfig->memConfig.norMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX3;
            break;
        case kFuseSemcNor_PcsSelection_RDY:
            pNorConfig->memConfig.norMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_RDY;
            break;
        // case kFuseSemcNor_PcsSelection_A8:
        default:
            pNorConfig->memConfig.norMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_A8;
            break;
    }

    // Get Address Port Size
    fuseValue = ROM_OCOTP_SEMC_NOR_ADDRESS_PORT_SIZE_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_AddressPortSize_25bit:
            pNorConfig->memConfig.norMemConfig.addressPortWidth = 25;
            break;
        case kFuseSemcNor_AddressPortSize_26bit:
            pNorConfig->memConfig.norMemConfig.addressPortWidth = 26;
            break;
        case kFuseSemcNor_AddressPortSize_27bit:
            pNorConfig->memConfig.norMemConfig.addressPortWidth = 27;
            break;
        case kFuseSemcNor_AddressPortSize_28bit:
            pNorConfig->memConfig.norMemConfig.addressPortWidth = 28;
            break;
        // case kFuseSemcNor_AddressPortSize_24bit:
        default:
            pNorConfig->memConfig.norMemConfig.addressPortWidth = kSemcMiscProperty_NOR_BaseFlashAddressPortWidth;
            break;
    }
#endif

    // Get ADV# Polarity
    fuseValue = ROM_OCOTP_SEMC_NOR_ADV_POLARITY_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_AdvPolarity_High:
            pNorConfig->memConfig.norMemConfig.advPortPolarity = kSemcPortPloarity_HighActive;
            break;
        case kFuseSemcNor_AdvPolarity_Low:
        default:
            pNorConfig->memConfig.norMemConfig.advPortPolarity = kSemcPortPloarity_LowActive;
            break;
    }

    // Get RDY Polarity
    fuseValue = ROM_OCOTP_SEMC_NOR_RDY_POLARITY_VALUE();
    switch (fuseValue)
    {
        case kFuseSemcNor_RdyPolarity_Low:
            pNorConfig->memConfig.norMemConfig.rdyPortPolarity = kSemcPortPloarity_LowActive;
            break;
        case kFuseSemcNor_RdyPolarity_High:
        default:
            pNorConfig->memConfig.norMemConfig.rdyPortPolarity = kSemcPortPloarity_HighActive;
            break;
    }
}

status_t semc_nor_get_default_config_block(semc_nor_config_t *config)
{
    semc_nor_config_t *pNorConfig = config;

    // Cleanup config block
    memset(pNorConfig, 0, sizeof(semc_nor_config_t));

    // Set Fuse related config parameters
    pNorConfig->acTimingMode = kSemcNorAcTimingMode_DefaultSafe;
    pNorConfig->memConfig.asyncClkFreq = SEMC_2ND_MAX_CLK_FREQ;
    pNorConfig->memConfig.norMemConfig.dataPortWidth = 16;
    pNorConfig->memConfig.norMemConfig.advPortPolarity = kSemcPortPloarity_LowActive;
    pNorConfig->memConfig.norMemConfig.rdyPortPolarity = kSemcPortPloarity_HighActive;

    // Get config data from Fuse
    semc_nor_get_config_data_from_fuse(config);

    pNorConfig->deviceCommandSet = kSemcNorDeviceCommandSet_EPSCD;

    // Set other config parameters
    pNorConfig->vendorType = kSemcNorVendorType_Micron;
    pNorConfig->memConfig.tag = kSemcConfigBlockTag;
    pNorConfig->memConfig.version = SEMC_DRIVER_VERSION;

    pNorConfig->memConfig.deviceMemType = kSemcDeviceMemType_NOR;
    pNorConfig->memConfig.busTimeoutCycles = 255;
    pNorConfig->memConfig.commandExecutionTimeoutCycles = 255;

    pNorConfig->memConfig.norMemConfig.comMemBaseAddress = SEMC_MEM_NOR_AXI_BASE;
    pNorConfig->memConfig.norMemConfig.comMemSizeInByte = SEMC_NOR_MAX_SIZE;
    pNorConfig->memConfig.norMemConfig.addressMode = kSemcNorAddressMode_ADM;
#if (SEMC_NOR_FUSE_MAP_VERSION == 1)
    pNorConfig->memConfig.readStrobeMode = kSemcDqsMode_LoopbackInternally;
    pNorConfig->memConfig.norMemConfig.addressPortWidth = kSemcMiscProperty_NOR_BaseFlashAddressPortWidth;
    pNorConfig->memConfig.norMemConfig.cePortOutputSelection = kSemcCeOutputSelection_MUX_CSX0;
#endif
    pNorConfig->memConfig.norMemConfig.columnAddressWidth = 12;
    pNorConfig->memConfig.norMemConfig.burstLengthInBytes = kSemcMiscProperty_NORNAND_MaxBurstLengthInBytes;

    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
