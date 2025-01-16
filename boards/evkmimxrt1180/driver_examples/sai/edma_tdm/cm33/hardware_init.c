/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_cs42448.h"
#include "fsl_trdc.h"
#include "fsl_edma.h"
#include "fsl_ele_base_api.h"
/*${header:end}*/

/*${macro:start}*/
#define ELE_TRDC_AON_ID    0x74
#define ELE_TRDC_WAKEUP_ID 0x78
#define ELE_CORE_CM33_ID   0x1
#define ELE_CORE_CM7_ID    0x2

/*
 * Set ELE_STICK_FAILED_STS to 0 when ELE status check is not required,
 * which is useful when debug reset, where the core has already get the
 * TRDC ownership at first time and ELE is not able to release TRDC
 * ownership again for the following TRDC ownership request.
 */
#define ELE_STICK_FAILED_STS 1

#if ELE_STICK_FAILED_STS
#define ELE_IS_FAILED(x) (x != kStatus_Success)
#else
#define ELE_IS_FAILED(x) false
#endif
/*${macro:end}*/

/*${function:start}*/
cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_CS42448_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .format       = {.mclk_HZ = 24576000, .sampleRate = 48000U, .bitWidth = 24U},
    .bus          = kCS42448_BusTDM,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 768/1000)  / 2
 *                              = 393.216MHZ
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,    /* Divider after the PLL, should only be 0, 1, 2, 3, 4, 5 */
    .numerator   = 768,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};

void SEI_EAR_TRDC_EDMA3_ResetPermissions()
{
    uint8_t i, j;
    /* Set the master domain access configuration for eDMA3 */
    trdc_non_processor_domain_assignment_t edma3Assignment;
    (void)memset(&edma3Assignment, 0, sizeof(edma3Assignment));
    edma3Assignment.domainId = 0x7U;
    /* Use the bus master's privileged/user attribute directly */
    edma3Assignment.privilegeAttr = kTRDC_MasterPrivilege;
    /* Use the bus master's secure/nonsecure attribute directly */
    edma3Assignment.secureAttr = kTRDC_MasterSecure;
    /* Use the DID input as the domain indentifier */
    edma3Assignment.bypassDomainId = true;
    edma3Assignment.lock           = false;
    TRDC_SetNonProcessorDomainAssignment(TRDC1, kTRDC1_MasterDMA3, &edma3Assignment);

    /* Enable all access modes for MBC and MRC. */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(TRDC1, &hwConfig);

    trdc_memory_access_control_config_t memAccessConfig;
    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));

    memAccessConfig.nonsecureUsrX  = 1U;
    memAccessConfig.nonsecureUsrW  = 1U;
    memAccessConfig.nonsecureUsrR  = 1U;
    memAccessConfig.nonsecurePrivX = 1U;
    memAccessConfig.nonsecurePrivW = 1U;
    memAccessConfig.nonsecurePrivR = 1U;
    memAccessConfig.secureUsrX     = 1U;
    memAccessConfig.secureUsrW     = 1U;
    memAccessConfig.secureUsrR     = 1U;
    memAccessConfig.securePrivX    = 1U;
    memAccessConfig.securePrivW    = 1U;
    memAccessConfig.securePrivR    = 1U;
    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }
    }
}

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL |= BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        BLK_CTRL_NS_AONMIX->SAI1_MCLK_CTRL &= ~BLK_CTRL_NS_AONMIX_SAI1_MCLK_CTRL_SAI1_MCLK_DIR_MASK;
    }
}

void BOARD_InitHardware(void)
{
    status_t sts;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Get ELE FW status */
    do
    {
        uint32_t ele_fw_sts;
        sts = ELE_BaseAPI_GetFwStatus(MU_RT_S3MUA, &ele_fw_sts);
    } while (sts != kStatus_Success);

    /* Release TRDC A to CM33 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_AON_ID, ELE_CORE_CM33_ID);
    } while (ELE_IS_FAILED(sts));

    /* Release TRDC W to CM33 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_WAKEUP_ID, ELE_CORE_CM33_ID);
    } while (ELE_IS_FAILED(sts));

    SEI_EAR_TRDC_EDMA3_ResetPermissions();

    /* Init Audio Pll. */
    CLOCK_InitAudioPll(&audioPllConfig);
    /*Clock setting for SAI1*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, kCLOCK_SAI1_ClockRoot_MuxAudioPllOut);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 16);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);
    
     /* enable codec power */
    RGPIO_PinWrite(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, 1U);
}

void BORAD_CodecReset(bool state)
{
    if (state)
    {
        RGPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 1U);
    }
    else
    {
        RGPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 0U);
    }
}
/*${function:end}*/
