/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
/*${header:end}*/
extern volatile bool g_userPress;
/*${function:start}*/
void GPIO_INTA_DriverIRQHandler(void)
{
    /* clear the interrupt status */
    GPIO_PinClearInterruptFlag(GPIO, APP_SW_PORT, APP_SW_PIN, 0);
    /* Change state of switch. */
    g_userPress = true;
    SDK_ISR_EXIT_BARRIER;
}

void BOARD_InitKey(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config    = {kGPIO_DigitalInput, 0};
    gpio_interrupt_config_t config = {kGPIO_PinIntEnableEdge, kGPIO_PinIntEnableLowOrFall};

    /* Init input switch GPIO. */
    EnableIRQ(APP_SW_IRQ);
    GPIO_PortInit(GPIO, APP_SW_PORT);
    GPIO_PinInit(GPIO, APP_SW_PORT, APP_SW_PIN, &sw_config);

    /* Enable GPIO pin interrupt */
    GPIO_SetPinInterruptConfig(GPIO, APP_SW_PORT, APP_SW_PIN, &config);
    GPIO_PinEnableInterrupt(GPIO, APP_SW_PORT, APP_SW_PIN, 0);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitKey();
}

void BOARD_InitDebugConsoleSWO(uint32_t port, uint32_t baudrate)
{
    uint32_t clkSrcFreq = SystemCoreClock;

    DbgConsole_Init(port, baudrate, kSerialPort_Swo, clkSrcFreq);
}
/*${function:end}*/
