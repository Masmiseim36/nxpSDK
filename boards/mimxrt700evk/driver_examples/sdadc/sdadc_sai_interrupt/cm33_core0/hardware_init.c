/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"
#include "fsl_wm8962.h"
#include "fsl_codec_adapter.h"
#include "fsl_clock.h"
#include "fsl_sai.h"
#include "pin_mux.h"
#include "clock_config.h"
/*${header:end}*/

/*${variable:start}*/
wm8962_config_t wm8962Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = DEMO_I2C_CLK_FREQ},
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
    .format =
        {
            .mclk_HZ    = 24576000U,
            .sampleRate = DEMO_AUDIO_SAMPLE_RATE,
            .bitWidth   = DEMO_AUDIO_BIT_WIDTH,
        },
    .masterSlave = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};

sai_master_clock_t mclkConfig;
/*${variable:end}*/

/*${function:start}*/
void BOARD_MasterClockConfig(void)
{
    mclkConfig.mclkOutputEnable = true;
    mclkConfig.mclkHz           = 24576000U;
    mclkConfig.mclkSourceClkHz  = 24576000U;
    SAI_SetMasterClockConfig(DEMO_SAI_BASE, &mclkConfig);
}

static void BOARD_ConfigureAudioPll(void)
{
    const clock_audio_pll_config_t audioPllConfig = {.audio_pll_src  = kCLOCK_AudioPllOscClk,
                                                     .numerator      = 66,
                                                     .denominator    = 125,
                                                     .audio_pll_mult = kCLOCK_AudioPllMult22,
                                                     .enableVcoOut   = true};

    /* Configure Audio PLL clock source. */
    CLOCK_InitAudioPll(&audioPllConfig);  /* 540.672MHz */
    CLOCK_InitAudioPfd(kCLOCK_Pfd3, 18U); /* 540.672MHZ */
    CLOCK_EnableAudioPllPfdClkForDomain(kCLOCK_Pfd3, kCLOCK_AllDomainEnable);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_ConfigureAudioPll();
    BOARD_InitDebugConsole();

    /* SDADC clock setting. */
    CLOCK_AttachClk(kAUDIO_PLL_to_SDADC);
    CLOCK_SetClkDiv(kCLOCK_DivSdadcClk, 22U);

    /* LPI2C clock setting. */
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM2);

    /* SAI clock setting. */
    CLOCK_AttachClk(kAUDIO_PLL_PFD3_to_AUDIO_VDD2);
    CLOCK_AttachClk(kAUDIO_VDD2_to_SAI012);
    CLOCK_SetClkDiv(kCLOCK_DivSai012Clk, 22U);

    SYSCON0->SAI0_MCLK_CTRL |= SYSCON0_SAI0_MCLK_CTRL_SAIMCLKDIR_MASK;
}
/*${function:end}*/
