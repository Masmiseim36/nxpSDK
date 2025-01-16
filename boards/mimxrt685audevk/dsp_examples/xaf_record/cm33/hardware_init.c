/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_debug_console.h"
#include "app.h"
#include "clock_config.h"
#include "fsl_cs42448.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_power.h"
#include "fsl_pca9420.h"
/*${header:end}*/

/*${variable:start}*/
codec_handle_t g_codecHandle;
cs42448_config_t g_cs42448Config = {
    .DACMode   = kCS42448_ModeSlave,
    .ADCMode   = kCS42448_ModeSlave,
    .reset     = NULL,
    .master    = false,
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
#ifdef XA_VIT_PRE_PROC
    .format = {.sampleRate = 16000U, .bitWidth = 16U},
    .bus    = kCS42448_BusI2S,
#else
    .format = {.sampleRate = 16000U, .bitWidth = BOARD_DMIC_NUM > 2 ? 24U : 16U},
    .bus    = BOARD_DMIC_NUM > 2 ? kCS42448_BusTDM : kCS42448_BusI2S,
#endif
    .slaveAddress = CS42448_I2C_ADDR,
};
codec_config_t g_boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &g_cs42448Config};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA_RST_SHIFT_RSTn);

    /* Clear MUA reset before run DSP core */
    RESET_PeripheralReset(kMU_RST_SHIFT_RSTn);

    /* I2C */
    CLOCK_AttachClk(kFFRO_to_FLEXCOMM2);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 6);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    g_cs42448Config.i2cConfig.codecI2CSourceClock = CLOCK_GetFlexCommClkFreq(2);
    g_cs42448Config.format.mclk_HZ                = CLOCK_GetMclkClkFreq();

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);
}

int BOARD_CODEC_Init(void)
{
    PRINTF("[CM33 Main] Configure cs42448 codec\r\n");

    if (CODEC_Init(&g_codecHandle, &g_boardCodecConfig) != kStatus_Success)
    {
        PRINTF("[CM33 Main] cs42448_Init failed!\r\n");
        return -1;
    }

    if (CODEC_SetVolume(&g_codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
        return -1;
    }

    return 0;
}

void BOARD_MuteRightChannel(bool mute)
{
    /* The CODEC_SetMute() funtion sets the volume to 100 after unmuting */
    CODEC_SetVolume(&g_codecHandle, kCODEC_PlayChannelHeadphoneRight, mute ? 0 : DEMO_CODEC_VOLUME);
}
/*${function:end}*/
