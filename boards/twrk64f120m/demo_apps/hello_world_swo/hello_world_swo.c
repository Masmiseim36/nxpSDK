/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_port.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DEBUG_CONSOLE_SWO_PORT (0U)
#define DEMO_DEBUG_CONSOLE_SWO_BAUDRATE (100000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitDebugConsoleSWO(uint32_t port, uint32_t baudrate);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_userPress = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitKey(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Init input switch GPIO. */
    PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptFallingEdge);
    EnableIRQ(BOARD_SW3_IRQ);
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw_config);
}


void BOARD_InitDebugConsoleSWO(uint32_t port, uint32_t baudrate)
{
    uint32_t clkSrcFreq = BOARD_BOOTCLOCKRUN_CORE_CLOCK;

    DbgConsole_Init(port, baudrate, kSerialPort_Swo, clkSrcFreq);
}

extern volatile bool g_userPress;
void PORTA_IRQHandler(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN);
    /* Change state of button. */
    g_userPress = true;
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitKey();
    BOARD_InitDebugConsoleSWO(DEMO_DEBUG_CONSOLE_SWO_PORT, DEMO_DEBUG_CONSOLE_SWO_BAUDRATE);

    while (1)
    {
        if (g_userPress)
        {
            PRINTF("SWO: hello_world\r\n");
            g_userPress = false;
        }
    }
}
