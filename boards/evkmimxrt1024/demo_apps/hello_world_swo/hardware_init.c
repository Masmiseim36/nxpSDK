/*
 * Copyright 2020-2022 NXP
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

void GPIO5_Combined_0_15_IRQHandler(void)
{
    /* clear the interrupt status */
    GPIO_PortClearInterruptFlags(APP_SW_PORT, 1UL << APP_SW_PIN);
    /* Change state of switch. */
    g_userPress = true;
    SDK_ISR_EXIT_BARRIER;
}

void BOARD_InitKey(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {kGPIO_DigitalInput, 0, kGPIO_IntFallingEdge};

    /* Init input switch GPIO. */
    GPIO_PinInit(APP_SW_PORT, APP_SW_PIN, &sw_config);

    /* Enable GPIO pin interrupt */
    GPIO_SetPinInterruptConfig(APP_SW_PORT, APP_SW_PIN, kGPIO_IntFallingEdge);
    GPIO_PortEnableInterrupts(APP_SW_PORT, 1UL << APP_SW_PIN);
    EnableIRQ(APP_SW_IRQ);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitKey();
}

void BOARD_InitDebugConsoleSWO(uint32_t port, uint32_t baudrate)
{
    SystemCoreClockUpdate();
    CLOCK_EnableClock(kCLOCK_Trace);

    /* For RT1024, ETB(embedded trace buffer) FFCR register needs
     * initialization for none-debug mode swo trace output.
     * Please check Serial_SwoInit in fsl_component_serial_port_swo.c
     * for additional coresight register initialization. */
#define ETB_FFCR (*(volatile unsigned int *)0xE0040304)
    ETB_FFCR = 0x100;

    uint32_t clkSrcFreq = CLOCK_GetClockRootFreq(kCLOCK_TraceClkRoot);
    DbgConsole_Init(port, baudrate, kSerialPort_Swo, clkSrcFreq);
}
/*${function:end}*/
