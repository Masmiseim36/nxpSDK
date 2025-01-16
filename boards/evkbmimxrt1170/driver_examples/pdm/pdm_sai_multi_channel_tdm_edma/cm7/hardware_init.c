/*
 * Copyright 2022 NXP
 * All rights reserved.
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
#include "fsl_dmamux.h"
#include "fsl_pdm.h"
#include "fsl_cs42448.h"
/*${header:end}*/

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
static uint32_t s_pdmGain           = DEMO_PDM_CHANNEL_GAIN;
static volatile bool s_increaseGain = true;

void GPIO13_Combined_0_31_IRQHandler(void)
{
    GPIO_PortClearInterruptFlags(GPIO13, 1U << 0U);

    if (s_increaseGain)
    {
        s_pdmGain++;
    }
    else
    {
        s_pdmGain--;
    }

    if (s_pdmGain == kPDM_DfOutputGain15)
    {
        s_increaseGain = false;
    }

    if (s_pdmGain == kPDM_DfOutputGain0)
    {
        s_increaseGain = true;
    }

    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_0, (pdm_df_output_gain_t)s_pdmGain);
    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_1, (pdm_df_output_gain_t)s_pdmGain);
    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_2, (pdm_df_output_gain_t)s_pdmGain);
    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_3, (pdm_df_output_gain_t)s_pdmGain);
    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_4, (pdm_df_output_gain_t)s_pdmGain);
    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_5, (pdm_df_output_gain_t)s_pdmGain);
    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_6, (pdm_df_output_gain_t)s_pdmGain);
    PDM_SetChannelGain(DEMO_PDM, DEMO_PDM_ENABLE_CHANNEL_7, (pdm_df_output_gain_t)s_pdmGain);
}

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR0 |= 1 << 8U;
    }
    else
    {
        IOMUXC_GPR->GPR0 &= ~(1 << 8U);
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    EnableIRQ(GPIO13_Combined_0_31_IRQn);

    CLOCK_InitAudioPll(&audioPllConfig);

    CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c6, 1);
    /* audio pll  */
    CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, 4);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, 16);
    /* 0SC400M */
    /* 24.576m mic root clock */
    CLOCK_SetRootClockMux(kCLOCK_Root_Mic, 6);
    CLOCK_SetRootClockDiv(kCLOCK_Root_Mic, 8);

    BOARD_EnableSaiMclkOutput(true);

    DMAMUX_Init(DEMO_DMAMUX);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_PDM_EDMA_CHANNEL_0, DEMO_PDM_REQUEST_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_PDM_EDMA_CHANNEL_0);
    DMAMUX_SetSource(DEMO_DMAMUX, DEMO_SAI_EDMA_CHANNEL, DEMO_SAI_REQUEST_SOURCE);
    DMAMUX_EnableChannel(DEMO_DMAMUX, DEMO_SAI_EDMA_CHANNEL);

    /* enable codec power */
    GPIO_PinWrite(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, 1U);
}

void BORAD_CodecReset(bool state)
{
    if (state)
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 1U);
    }
    else
    {
        GPIO_PinWrite(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, 0U);
    }
}
/*${function:end}*/
