/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_trdc.h"
#include "fsl_ele_base_api.h"
#include "app.h"
#include "fsl_codec_common.h"
#if defined DEMO_CODEC_WM8962
#include "fsl_wm8962.h"
#else
#include "fsl_cs42448.h"
#endif
#include "fsl_codec_adapter.h"
/*${header:end}*/

/*${macro:start}*/
/* When CM33 set TRDC, CM7 must NOT require TRDC ownership from ELE */
#define CM33_SET_TRDC 0U

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

/*${variable:start}*/
#if DEMO_CODEC_WM8962
wm8962_config_t wm8962Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route =
        {
            .enableLoopBack            = false,
            .leftInputPGASource        = kWM8962_InputPGASourceInput1,
            .leftInputMixerSource      = kWM8962_InputMixerSourceInputPGA,
            .rightInputPGASource       = kWM8962_InputPGASourceInput3,
            .rightInputMixerSource     = kWM8962_InputMixerSourceInputPGA,
            .leftHeadphoneMixerSource  = kWM8962_OutputMixerDisabled,
            .leftHeadphonePGASource    = kWM8962_OutputPGASourceDAC,
            .rightHeadphoneMixerSource = kWM8962_OutputMixerDisabled,
            .rightHeadphonePGASource   = kWM8962_OutputPGASourceDAC,
        },
    .slaveAddress = WM8962_I2C_ADDR,
    .bus          = kWM8962_BusI2S,
    .format       = {.mclk_HZ    = 24576000U / 2,
                     .sampleRate = kWM8962_AudioSampleRate16KHz,
                     .bitWidth   = kWM8962_AudioBitWidth16bit},
    .masterSlave  = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};
#elif DEMO_CODEC_CS42448
cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_CS42448_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .format       = {.mclk_HZ = 24576000, .sampleRate = 48000U, .bitWidth = 16U},
    .bus          = kCS42448_BusI2S,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};
#else
#error "no codec enabled, please check."
#endif

/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM) / (2^POST)
 *                              = 24 * (32 + 768/1000) / 2
 *                              = 393.216 MHZ
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,   /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,    /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 768,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};
/*${variable:end}*/

/*${function:start}*/
#if !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U))
static void TRDC_EDMA3_EDMA4_ResetPermissions(void)
{
    uint8_t i, j;

    /* Set the master domain access configuration for eDMA4 */
    trdc_non_processor_domain_assignment_t edmaAssignment;

    (void)memset(&edmaAssignment, 0, sizeof(edmaAssignment));
    edmaAssignment.domainId       = 0x7U;
    edmaAssignment.privilegeAttr  = kTRDC_MasterPrivilege;
    edmaAssignment.secureAttr     = kTRDC_ForceSecure;
    edmaAssignment.bypassDomainId = true;
    edmaAssignment.lock           = false;

    TRDC_SetNonProcessorDomainAssignment(TRDC1, kTRDC1_MasterDMA3, &edmaAssignment);
    TRDC_SetNonProcessorDomainAssignment(TRDC2, kTRDC2_MasterDMA4, &edmaAssignment);

    /* Enable all access modes for MBC and MRC of TRDCA and TRDCW */
    trdc_hardware_config_t hwConfig;
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

    TRDC_GetHardwareConfig(TRDC1, &hwConfig);
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

    /* Set TRDC1(A) secure access for Domain 7(eDMA domain ID), MBC 0, MEM0 (AIPS1->SAI1) */
    TRDC_MbcNseClearAll(TRDC1, 0U, 1UL << 7U, 0x1 << 0);

    /* Set TRDC1(A) secure access for Domain 7(eDMA domain ID), MBC 1, MEM1 (CM33 System TCM) */
    TRDC_MbcNseClearAll(TRDC1, 1U, 1UL << 7U, 0x1 << 1);

    TRDC_GetHardwareConfig(TRDC2, &hwConfig);
    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC2, &memAccessConfig, i, j);
        }
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        for (j = 0U; j < 8; j++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC2, &memAccessConfig, i, j);
        }
    }

    /* Set TRDC2(W) secure access for Domain 7(eDMA domain ID), MBC 1, MEM0(AIPS3->AIPS3_MEGA->PDM) */
    TRDC_MbcNseClearAll(TRDC2, 1U, 1UL << 7U, 0x1 << 0);

    /* Set TRDC2(W) secure access for Domain 7(eDMA domain ID), MRC 5, all region(SEMC->SDRAM) */
    TRDC_MrcEnableDomainNseUpdate(TRDC2, 5U, 1UL << 7U, true);
    TRDC_MrcRegionNseClear(TRDC2, 5U, 0xFFFFU);

    /* Set TRDC2(W) secure access for Domain 7(eDMA domain ID), MRC 2, all region(CM7 I/D TCM) */
    TRDC_MrcEnableDomainNseUpdate(TRDC2, 2U, 1UL << 7U, true);
    TRDC_MrcRegionNseClear(TRDC2, 2U, 0xFFFFU);
}
#endif /* !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U)) */

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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    
#if DEMO_CODEC_WM8962
    BOARD_InitWM8962Pins();
#else
    BOARD_InitCS42448Pins();
#endif

    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

#if !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U))

    status_t sts;

    /* Get ELE FW status */
    do
    {
        uint32_t ele_fw_sts;
        sts = ELE_BaseAPI_GetFwStatus(MU_RT_S3MUA, &ele_fw_sts);
    } while (sts != kStatus_Success);

    /* Release TRDC A to CM7 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_AON_ID, ELE_CORE_CM7_ID);
    } while (ELE_IS_FAILED(sts));

    /* Release TRDC W to CM7 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_WAKEUP_ID, ELE_CORE_CM7_ID);
    } while (ELE_IS_FAILED(sts));

    TRDC_EDMA3_EDMA4_ResetPermissions();

#endif /* !(defined(CM33_SET_TRDC) && (CM33_SET_TRDC > 0U)) */

    /* Init Audio Pll. */
    CLOCK_InitAudioPll(&audioPllConfig);
    /*Clock setting for SAI1*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, kCLOCK_SAI1_ClockRoot_MuxAudioPllOut);
#if DEMO_CODEC_WM8962
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 32);
    /*Clock setting for LPI2C*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c0102, kCLOCK_LPI2C0102_ClockRoot_MuxOscRc24M);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Lpi2c0102, 1);
#else
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 16);
    /*Clock setting for LPI2C*/
    CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c0304, kCLOCK_LPI2C0304_ClockRoot_MuxOscRc24M);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Lpi2c0304, 1);
#endif
    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);

#if DEMO_CODEC_CS42448
    /* enable codec power */
    RGPIO_PinWrite(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, 1U);
#endif
}

#if DEMO_CODEC_CS42448
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
#endif
/*${function:end}*/
