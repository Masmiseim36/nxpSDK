/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_tfa9xxx.h"
#include "tfa_config_TFA9894N2.h"
#include "fsl_i2s_bridge.h"
/*${header:end}*/

/*${variable:start}*/
tfa9xxx_config_t tfa9xxxConfigLeft = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .slaveAddress = TFA9XXX_I2C_ADDR_0,
    .protocol     = kTFA9XXX_BusI2S,
    .format       = {.sampleRate = kTFA9XXX_AudioSampleRate48KHz, .bitWidth = kTFA9XXX_AudioBitWidth16bit},
    .tfaContainer = tfa_container_bin,
    .deviceIndex  = 0,
};

tfa9xxx_config_t tfa9xxxConfigRight = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .slaveAddress = TFA9XXX_I2C_ADDR_1,
    .protocol     = kTFA9XXX_BusI2S,
    .format       = {.sampleRate = kTFA9XXX_AudioSampleRate48KHz, .bitWidth = kTFA9XXX_AudioBitWidth16bit},
    .tfaContainer = tfa_container_bin,
    .deviceIndex  = 1,
};

codec_config_t boardCodecConfigLeft  = {.codecDevType = kCODEC_TFA9XXX, .codecDevConfig = &tfa9xxxConfigLeft};
codec_config_t boardCodecConfigRight = {.codecDevType = kCODEC_TFA9XXX, .codecDevConfig = &tfa9xxxConfigRight};

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* attach main clock to I3C (500MHz / 20 = 25MHz). */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 20);

    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    /* attach AUDIO PLL clock to FLEXCOMM3 (I2S3) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    tfa9xxxConfigLeft.i2cConfig.codecI2CSourceClock  = CLOCK_GetI3cClkFreq();
    tfa9xxxConfigRight.i2cConfig.codecI2CSourceClock = tfa9xxxConfigLeft.i2cConfig.codecI2CSourceClock;

    /* Set shared signal set 0: SCK, WS from Flexcomm1 */
    I2S_BRIDGE_SetShareSignalSrc(kI2S_BRIDGE_ShareSet0, kI2S_BRIDGE_SignalSCK, kI2S_BRIDGE_Flexcomm1);
    I2S_BRIDGE_SetShareSignalSrc(kI2S_BRIDGE_ShareSet0, kI2S_BRIDGE_SignalWS, kI2S_BRIDGE_Flexcomm1);
    /* Set flexcomm3 SCK, WS from shared signal set 0 */
    I2S_BRIDGE_SetFlexcommSignalShareSet(kI2S_BRIDGE_Flexcomm3, kI2S_BRIDGE_SignalSCK, kI2S_BRIDGE_ShareSet0);
    I2S_BRIDGE_SetFlexcommSignalShareSet(kI2S_BRIDGE_Flexcomm3, kI2S_BRIDGE_SignalWS, kI2S_BRIDGE_ShareSet0);

    // System Tick Configuration, generate 1ms interrupt
    SysTick_Config(SystemCoreClock / 1000U);
}
/*${function:end}*/
