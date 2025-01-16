/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*${header:start}*/
#include <stdbool.h>
#include "app.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_cs42448.h"
#include "fsl_sx1502.h"
#include "fsl_debug_console.h"

/*${header:end}*/

/*${variable:start}*/
cs42448_config_t cs42448Config = {
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = NULL,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = DEMO_CODEC_I2C_INSTANCE},
    .format       = {.sampleRate = 48000U, .bitWidth = 24U},
    .bus          = kCS42448_BusTDM,
    .slaveAddress = CS42448_I2C_ADDR,
};

codec_config_t boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &cs42448Config};

sx1502_config_t sx1502Config = {
    .initRegDataValue  = 0xFFU,
    .initRegDirValue   = 0,
    .sx1502I2CInstance = DEMO_SX1502_I2C_INSTANCE,
};

sx1502_handle_t sx1502Handle;
codec_handle_t s_codecHandle;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* attach AUDIO PLL clock to FLEXCOMM3 (I2S3) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);
    /* I2C */
    CLOCK_AttachClk(kFFRO_to_FLEXCOMM2);
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM1);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 2);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 8);

    cs42448Config.i2cConfig.codecI2CSourceClock = CLOCK_GetFlexCommClkFreq(2);
    cs42448Config.format.mclk_HZ                = CLOCK_GetMclkClkFreq();

    sx1502Config.sx1502I2CSourceClock = CLOCK_GetFlexCommClkFreq(1U);
}

void BOARD_InitCodec(void)
{
    if (CODEC_Init(&s_codecHandle, &boardCodecConfig) != kStatus_Success)
    {
        PRINTF("CODEC_Init failed!\r\n");
        assert(false);
    }
}

void BOARD_InitSX1502(void)
{
    if (SX1502_Init(&sx1502Handle, &sx1502Config) != kStatus_Success)
    {
        PRINTF("SX1502_Init failed!\r\n");
        assert(false);
    }
}
/*${function:end}*/
