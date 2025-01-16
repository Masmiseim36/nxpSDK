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
    .DACMode      = kCS42448_ModeSlave,
    .ADCMode      = kCS42448_ModeSlave,
    .reset        = NULL,
    .master       = false,
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .format       = {.sampleRate = 48000U, .bitWidth = 16U},
    .bus          = kCS42448_BusI2S,
    .slaveAddress = CS42448_I2C_ADDR,
};
codec_config_t g_boardCodecConfig = {.codecDevType = kCODEC_CS42448, .codecDevConfig = &g_cs42448Config};
/*${variable:end}*/

/*${function:start}*/

/*!
 * @brief Function for changing codec settings according to selected parameters.
 *
 * @param[in] nchannel Number of chnnels.
 */
int BOARD_CodecChangeSettings(uint8_t nchannel, uint32_t sample_rate, uint32_t bit_width)
{
    cs42448_handle_t *devHandle = (cs42448_handle_t *)((uint32_t)(((codec_handle_t *)&g_codecHandle)->codecDevHandle));
    CS42448_ConfigDataFormat(devHandle, g_cs42448Config.format.mclk_HZ, sample_rate, bit_width);

    /* set protocol */
    switch (nchannel)
    {
        case 2:
            return CS42448_SetProtocol(devHandle, kCS42448_BusI2S, bit_width);
        case 8:
            /* Intentional fall */
        default:
            return CS42448_SetProtocol(devHandle, kCS42448_BusTDM, bit_width);
    }
}

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
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
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
