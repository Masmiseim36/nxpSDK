/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_tfa9896.h"
#include "fsl_inputmux.h"
#include "app.h"
#include "fsl_i2s_bridge.h"
/*${header:end}*/

/*${variable:start}*/
tfa9896_config_t tfa9896ConfigL = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .slaveAddress = TFA9896_I2C_ADDRESS_LEFT,
    .protocol     = kTFA9896_ProtocolI2S,
    .format       = {.sampleRate = ktfa9896_SampleRate48kHz, .bitWidth = ktfa9896_BitWidth16},
    .calibrate    = false,
};
tfa9896_config_t tfa9896ConfigR = {
    .i2cConfig    = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE},
    .slaveAddress = TFA9896_I2C_ADDRESS_RIGHT,
    .protocol     = kTFA9896_ProtocolI2S,
    .format       = {.sampleRate = ktfa9896_SampleRate48kHz, .bitWidth = ktfa9896_BitWidth16},
    .calibrate    = false,
};

codec_config_t boardCodecConfigL = {.codecDevType = kCODEC_TFA9896, .codecDevConfig = &tfa9896ConfigL};
codec_config_t boardCodecConfigR = {.codecDevType = kCODEC_TFA9896, .codecDevConfig = &tfa9896ConfigR};
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

    /* Configure DMAMUX. */
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX);
    /* Enable DMA request */
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm1TxToDmac0Ch3RequestEna, true);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    /* Attach main clock to I3C, 396MHz / 16 = 24.75MHz. */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 16);

    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    /* attach AUDIO PLL clock to FLEXCOMM3 (I2S3) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    tfa9896ConfigL.i2cConfig.codecI2CSourceClock = CLOCK_GetI3cClkFreq();
    tfa9896ConfigR.i2cConfig.codecI2CSourceClock = tfa9896ConfigL.i2cConfig.codecI2CSourceClock;

    /* Set shared signal set 0: SCK, WS from Flexcomm1 */
    I2S_BRIDGE_SetShareSignalSrc(kI2S_BRIDGE_ShareSet0, kI2S_BRIDGE_SignalSCK, kI2S_BRIDGE_Flexcomm1);
    I2S_BRIDGE_SetShareSignalSrc(kI2S_BRIDGE_ShareSet0, kI2S_BRIDGE_SignalWS, kI2S_BRIDGE_Flexcomm1);
    /* Set flexcomm3 SCK, WS from shared signal set 0 */
    I2S_BRIDGE_SetFlexcommSignalShareSet(kI2S_BRIDGE_Flexcomm3, kI2S_BRIDGE_SignalSCK, kI2S_BRIDGE_ShareSet0);
    I2S_BRIDGE_SetFlexcommSignalShareSet(kI2S_BRIDGE_Flexcomm3, kI2S_BRIDGE_SignalWS, kI2S_BRIDGE_ShareSet0);
}
/*${function:end}*/
