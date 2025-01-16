/*
 * Copyright 2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "app.h"
#include "fsl_codec_common.h"
#include "fsl_cs42448.h"
#include "fsl_codec_adapter.h"
#include "fsl_clock.h"
#include "fsl_sai.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_edma_soc.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${variable:start}*/
cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = BORAD_CodecReset,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_CS42448_I2C_INSTANCE, .codecI2CSourceClock = DEMO_I2C_CLK_FREQ},
    .format       = {.mclk_HZ = 24576000, .sampleRate = DEMO_AUDIO_SAMPLE_RATE, .bitWidth = 24U},
    .bus          = kCS42448_BusTDM,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};

sai_master_clock_t mclkConfig;

/*${variable:end}*/

/*${function:start}*/
void BOARD_MasterClockConfig(void)
{
    mclkConfig.mclkOutputEnable = true;
    mclkConfig.mclkHz           = 24576000U;
    mclkConfig.mclkSourceClkHz  = 24576000U;
    SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kAUDIO_PLL_PFD3_to_MICFIL0);
    CLOCK_SetClkDiv(kCLOCK_DivMicfil0Clk, 15U);

    RESET_ClearPeripheralReset(kSAI0_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kPDM_RST_SHIFT_RSTn);

    /*Clock setting for LPI2C */
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM2);

    /* SAI clock 368.64 / 15 = 24.576MHz */
    CLOCK_AttachClk(kAUDIO_PLL_PFD3_to_AUDIO_VDD2);
    CLOCK_AttachClk(kAUDIO_VDD2_to_SAI012);
    CLOCK_SetClkDiv(kCLOCK_DivSai012Clk, 15U);

    RESET_ClearPeripheralReset(kSAI0_RST_SHIFT_RSTn);

    SYSCON0->SAI0_MCLK_CTRL |= SYSCON0_SAI0_MCLK_CTRL_SAIMCLKDIR_MASK;

    POWER_DisablePD(kPDRUNCFG_APD_DMA0_1_PKC_ETF);
    POWER_DisablePD(kPDRUNCFG_PPD_DMA0_1_PKC_ETF);
    POWER_ApplyPD();

    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    EDMA_EnableRequest(DEMO_EDMA, DEMO_PDM_EDMA_SOURCE);
    EDMA_EnableRequest(DEMO_EDMA, DEMO_SAI_EDMA_SOURCE);

    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio0);

    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PinInit(DEMO_CODEC_POWER_GPIO, DEMO_CODEC_POWER_GPIO_PIN, &led_config);
    GPIO_PinInit(DEMO_CODEC_RESET_GPIO, DEMO_CODEC_RESET_GPIO_PIN, &led_config);

    /* Enable codec power. */
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
