/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_rgpio.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SW_GPIO        BOARD_USER_BUTTON_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_USER_BUTTON_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_USER_BUTTON_NAME

#define BOARD_SW_INT_OUTPUT kRGPIO_InterruptOutput0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 */
void BOARD_SW_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    RGPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, BOARD_SW_INT_OUTPUT, 1U << BOARD_SW_GPIO_PIN);
    /* Change state of button. */
    g_ButtonPress = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the input switch pin */
    rgpio_pin_config_t sw_config = {
        kRGPIO_DigitalInput,
        0,
    };

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Workaround: Disable interrupt which might be enabled by ROM. */
    RGPIO_SetPinInterruptConfig(RGPIO1, 9U, kRGPIO_InterruptOutput0, kRGPIO_InterruptOrDMADisabled);
    NVIC_ClearPendingIRQ(GPIO1_0_IRQn);

    /* Print a note to terminal. */
    PRINTF("\r\n RGPIO Driver example\r\n");
    PRINTF("\r\n Press %s\r\n", BOARD_SW_NAME);

    /* Init input switch GPIO. */
    RGPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, BOARD_SW_INT_OUTPUT, kRGPIO_InterruptFallingEdge);

    EnableIRQ(BOARD_SW_IRQ);
    RGPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

    while (1)
    {
        if (g_ButtonPress)
        {
            PRINTF(" %s is pressed \r\n", BOARD_SW_NAME);
            /* Reset state of button. */
            g_ButtonPress = false;
        }
    }
}
