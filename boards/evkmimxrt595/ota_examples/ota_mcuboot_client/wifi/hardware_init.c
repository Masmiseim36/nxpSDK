/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_power.h"
/*${header:end}*/

#include "fsl_debug_console.h"

/*${macro:start}*/
#define APP_DEBUG_UART_TYPE     kSerialPort_Uart
#define APP_DEBUG_UART_INSTANCE 12U
#define APP_DEBUG_UART_CLK_FREQ CLOCK_GetFlexcommClkFreq(12)
#define APP_DEBUG_UART_FRG_CLK \
    (&(const clock_frg_clk_config_t){12U, kCLOCK_FrgPllDiv, 255U, 0U}) /*!< Select FRG0 mux as frg_pll */
#define APP_DEBUG_UART_CLK_ATTACH kFRG_to_FLEXCOMM12
#define APP_DEBUG_UART_BAUDRATE   115200
/*${macro:end}*/

/*${function:start}*/
/* Initialize debug console. */
void APP_InitAppDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    /* attach FRG0 clock to FLEXCOMM12 (debug console) */
    CLOCK_SetFRGClock(APP_DEBUG_UART_FRG_CLK);
    CLOCK_AttachClk(APP_DEBUG_UART_CLK_ATTACH);

    uartClkSrcFreq = APP_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(APP_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, APP_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    RESET_ClearPeripheralReset(kHSGPIO0_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kHSGPIO3_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kHSGPIO4_RST_SHIFT_RSTn);

    BOARD_InitBootPins();
    BOARD_InitPinsM2();
    BOARD_InitBootClocks();
    APP_InitAppDebugConsole();

    /* Make sure casper ram buffer has power up */
    POWER_DisablePD(kPDRUNCFG_PPD_CASPER_SRAM);
    POWER_ApplyPD();

    /* Configure 32K OSC clock. */
    CLOCK_EnableOsc32K(true);               /* Enable 32KHz Oscillator clock */
    CLOCK_EnableClock(kCLOCK_Rtc);          /* Enable the RTC peripheral clock */
    RTC->CTRL &= ~RTC_CTRL_SWRESET_MASK;    /* Make sure the reset bit is cleared */
    RTC->CTRL &= ~RTC_CTRL_RTC_OSC_PD_MASK; /* The RTC Oscillator is powered up */
}
/*${function:end}*/
