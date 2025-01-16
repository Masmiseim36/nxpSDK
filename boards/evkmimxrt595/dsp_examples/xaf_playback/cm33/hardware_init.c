/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_debug_console.h"
#include "app.h"
#include "clock_config.h"
#include "fsl_wm8904.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
/*${header:end}*/

/*${variable:start}*/
codec_handle_t g_codecHandle;
wm8904_config_t g_wm8904Config = {
    .i2cConfig          = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .recordSource       = kWM8904_RecordSourceLineInput,
    .recordChannelLeft  = kWM8904_RecordChannelLeft2,
    .recordChannelRight = kWM8904_RecordChannelRight2,
    .playSource         = kWM8904_PlaySourceDAC,
    .slaveAddress       = WM8904_I2C_ADDRESS,
    .protocol           = kWM8904_ProtocolI2S,
    .format             = {.sampleRate = kWM8904_SampleRate48kHz, .bitWidth = kWM8904_BitWidth16},
    .master             = false,
};
codec_config_t g_boardCodecConfig = {.codecDevType = kCODEC_WM8904, .codecDevConfig = &g_wm8904Config};
/*${variable:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA_RST_SHIFT_RSTn);

    /* Clear MUA reset before run DSP core */
    RESET_PeripheralReset(kMU_RST_SHIFT_RSTn);

    /* Attach main clock to I3C */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 4);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    g_wm8904Config.i2cConfig.codecI2CSourceClock = CLOCK_GetI3cClkFreq();
    g_wm8904Config.mclk_HZ                       = CLOCK_GetMclkClkFreq();

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);
}

int BOARD_CODEC_Init(void)
{
    PRINTF("[CM33 Main] Configure WM8904 codec\r\n");

    if (CODEC_Init(&g_codecHandle, &g_boardCodecConfig) != kStatus_Success)
    {
        PRINTF("[CM33 Main] WM8904_Init failed!\r\n");
        return -1;
    }

    /* Invert the DAC data in order to output signal with correct polarity - set DACL_DATINV and DACR_DATINV = 1 */
    if (WM8904_WriteRegister((wm8904_handle_t *)g_codecHandle.codecDevHandle, WM8904_AUDIO_IF_0, 0x1850) !=
        kStatus_WM8904_Success)
    {
        PRINTF("[CM33 Main] WM8904 configuration failed!\r\n");
        return -1;
    }

    /* Initial volume kept at 75% for hearing safety. */
    /* Adjust it to your needs between 0 - 100*/
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
