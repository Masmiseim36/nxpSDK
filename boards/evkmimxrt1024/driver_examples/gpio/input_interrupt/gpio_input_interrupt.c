/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "app.h"
#include "pin_mux.h"

#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
static void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW is turned on */
volatile bool g_InputSignal = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 */
void EXAMPLE_GPIO_IRQHandler(void)
{
    /* clear the interrupt status */
    GPIO_PortClearInterruptFlags(EXAMPLE_SW_GPIO, 1U << EXAMPLE_SW_GPIO_PIN);
    /* Change state of switch. */
    g_InputSignal = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
        kGPIO_IntRisingEdge,
    };

    /* hardware initialiize, include IOMUX, Uart debug initialize */
    BOARD_InitHardware();
    PRINTF("GPIO Driver example.\r\n");

    /* Init input switch GPIO. */
    EnableIRQ(EXAMPLE_SW_IRQ);
    GPIO_PinInit(EXAMPLE_SW_GPIO, EXAMPLE_SW_GPIO_PIN, &sw_config);

    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(EXAMPLE_SW_GPIO, 1U << EXAMPLE_SW_GPIO_PIN);

    while (1)
    {
        if (g_InputSignal)
        {
            delay();
            if (1 == GPIO_PinRead(EXAMPLE_SW_GPIO, EXAMPLE_SW_GPIO_PIN))
            {
                PRINTF("%s is turned on.\r\n", EXAMPLE_SW_NAME);
            }
            /* Reset state of switch. */
            g_InputSignal = false;
        }
    }
}

static void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __NOP(); /* delay */
    }
}
