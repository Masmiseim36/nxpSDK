/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
#include "fsl_dmamux.h"
/*${header:end}*/

/*${function:start}*/
wm8960_config_t wm8960Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route     = kWM8960_RoutePlaybackandRecord,
    .leftInputSource  = kWM8960_InputDifferentialMicInput3,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .format = {.mclk_HZ = 12000000, .sampleRate = kWM8960_AudioSampleRate16KHz, .bitWidth = kWM8960_AudioBitWidth16bit},
    .master_slave = true,
    .masterClock =
        {
            .sysclkSource = kWM8960_SysClkSourceInternalPLL,
            .sysclkFreq   = DEMO_AUDIO_MASTER_CLOCK,
        },
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (30 + 200/100)
 *                              = 768 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 30,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 200, /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI3_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI3_MCLK_DIR_MASK);
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    CLOCK_InitAudioPll(&audioPllConfig);
    BOARD_InitDebugConsole();

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI3*/
    CLOCK_SetMux(kCLOCK_Sai3Mux, DEMO_SAI3_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, DEMO_SAI3_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai3Div, DEMO_SAI3_CLOCK_SOURCE_DIVIDER);

    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);

    /* Init DMAMUX */
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, DEMO_EDMA_CHANNEL, DEMO_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, DEMO_EDMA_CHANNEL);
}
/*${function:end}*/
