/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_wm8904.h"
#include "fsl_inputmux.h"
#include "app.h"
#include "fsl_iopctl.h"
#include "fsl_i2s.h"
#include "fsl_i2s_bridge.h"
/*${header:end}*/

/*${variable:start}*/
wm8904_config_t wm8904Config = {
    .i2cConfig          = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .recordSource       = kWM8904_RecordSourceLineInput,
    .recordChannelLeft  = kWM8904_RecordChannelLeft2,
    .recordChannelRight = kWM8904_RecordChannelRight2,
    .playSource         = kWM8904_PlaySourceDAC,
    .slaveAddress       = WM8904_I2C_ADDRESS,
    .protocol           = kWM8904_ProtocolI2S,
    .format             = {.sampleRate = kWM8904_SampleRate48kHz, .bitWidth = kWM8904_BitWidth16},
    .master             = true,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8904, .codecDevConfig = &wm8904Config};
/*${variable:end}*/

/*${function:start}*/
static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < 100; i++)
    {
        __NOP();
    }
}

void BOARD_I3C_ReleaseBus(void)
{
    uint8_t i = 0;

    GPIO_PortInit(GPIO, 2);
    BOARD_InitI3CPinsAsGPIO();

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(GPIO, 2, 30, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(GPIO, 2, 29, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, 2, 30, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(GPIO, 2, 29, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(GPIO, 2, 29, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 2, 30, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 2, 29, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(GPIO, 2, 30, 1U);
    i2c_release_bus_delay();
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_I3C_ReleaseBus();
    BOARD_InitI3CPins();

    /* attach main clock to I3C */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 20);

    /* attach AUDIO PLL clock to FLEXCOMM0 (I2S0)*/
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM0);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;
    /* DMIC source from audio pll, divider 8, 24.576M/8=3.072MHZ */
    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC);
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 8);

    wm8904Config.i2cConfig.codecI2CSourceClock = CLOCK_GetI3cClkFreq();
    wm8904Config.mclk_HZ                       = CLOCK_GetMclkClkFreq();

    /* Set shared signal set 0: SCK, WS from Flexcomm1, enable Flexcomm0 and Flexcomm3 DATAOUT output. */
    I2S_BRIDGE_SetShareSetSrc(kI2S_BRIDGE_ShareSet0, kI2S_BRIDGE_Flexcomm1, kI2S_BRIDGE_Flexcomm1, 0U,
                              kI2S_BRIDGE_Flexcomm0DataOut | kI2S_BRIDGE_Flexcomm3DataOut);
    /* Set flexcomm0 SCK, WS from shared signal set 0 */
    I2S_BRIDGE_SetFlexcommShareSet(kI2S_BRIDGE_Flexcomm0, kI2S_BRIDGE_ShareSet0, kI2S_BRIDGE_ShareSet0,
                                   kI2S_BRIDGE_OriginalSignal, kI2S_BRIDGE_OriginalSignal);
    /* Set flexcomm3 data out from shared signal set 0 */
    I2S_BRIDGE_SetFlexcommShareSet(kI2S_BRIDGE_Flexcomm3, kI2S_BRIDGE_OriginalSignal, kI2S_BRIDGE_OriginalSignal,
                                   kI2S_BRIDGE_OriginalSignal, kI2S_BRIDGE_ShareSet0);
}
/*${function:end}*/
