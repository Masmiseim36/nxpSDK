/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_inputmux.h"
#include "fsl_power.h"
#include "fsl_rtc.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void EXAMPLE_EnterDeepSleep(void)
{
    /* Enable deep sleep IRQ. */
    EnableDeepSleepIRQ(EXAMPLE_UART_DMA_IRQn);
    /* Enter deep sleep mode by using power API. */
    POWER_EnterDeepSleep(EXAMPLE_EXCLUDE_FROM_DEEPSLEEP);
}

void EXAMPLE_EnableDmaWakeup(void)
{
    /* Enable DMA wake up by RX FIFO. */
    SYSCTL0->HWWAKE |= SYSCTL0_HWWAKE_FCWAKE_MASK;
    EXAMPLE_USART->FIFOCFG |= USART_FIFOCFG_WAKERX_MASK;
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();

    /* Enable OSC32K clock for FLEXCOMM0 */
    CLOCK_EnableOsc32K(true);
    RTC_Init(RTC);

    /* Configure DMAMUX. */
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX);
    /* Enable DMA request */
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm0RxToDmac0Ch0RequestEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Flexcomm0TxToDmac0Ch1RequestEna, true);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);
}
/*${function:end}*/
