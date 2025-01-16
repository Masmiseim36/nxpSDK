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
#include "fsl_cs42448.h"
#include "fsl_inputmux.h"
#include "app.h"
#include "fsl_power.h"

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
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    gpio_pin_config_t gpio_config = {
        kGPIO_DigitalOutput,
        1,
    };
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch0ToDmac0Ch16RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch1ToDmac0Ch17RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch2ToDmac0Ch18RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch3ToDmac0Ch19RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch4ToDmac0Ch20RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch5ToDmac0Ch21RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch6ToDmac0Ch22RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmic0Ch7ToDmac0Ch23RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm5TxToDmac0Ch11RequestEna, true);
    INPUTMUX_Deinit(INPUTMUX);

    /* attach main clock to I2C */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM2);

    /* attach AUDIO PLL clock to FLEXCOMM5 (I2S5) */
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM5);

    /* attach AUDIO PLL clock to MCLK */
    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 2);
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;
    /* DMIC source from audio pll, divider 8, 24.576M/8=3.072MHZ */
    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC);
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 8);

    cs42448Config.i2cConfig.codecI2CSourceClock = CLOCK_GetFlexcommClkFreq(2);
    cs42448Config.format.mclk_HZ                = CLOCK_GetMclkClkFreq();

    RESET_ClearPeripheralReset(kHSGPIO1_RST_SHIFT_RSTn);
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit(GPIO, 1, 6, &gpio_config);
}

/*${function:end}*/
