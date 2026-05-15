/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tickless_hal.h"
#include "fsl_rgpio.h"
#include "board.h"

/**
 * @file tickless_gpio_rgpio_hal.c
 * @brief RGPIO HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_gpio_hal_t interface for the RGPIO peripheral.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the example's app.h file:
 *
 * - BOARD_SW_GPIO: The RGPIO peripheral instance to use (e.g., RGPIO1)
 * - BOARD_SW_GPIO_PIN: The pin number for the button (e.g., 9)
 * - BOARD_SW_IRQ: The IRQ number for the button interrupt (e.g., GPIO1_0_IRQn)
 * - BOARD_SW_INT_OUTPUT: The interrupt output to use (e.g., kRGPIO_InterruptOutput0)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_SW_GPIO RGPIO1
 * #define BOARD_SW_GPIO_PIN 9
 * #define BOARD_SW_IRQ GPIO1_0_IRQn
 * #define BOARD_SW_INT_OUTPUT kRGPIO_InterruptOutput0
 * @endcode
 *
 */

#ifdef BOARD_SW_NAME

/* Initialize GPIO/button hardware */
static void rgpio_init(void)
{
    /* Define the init structure for the input switch pin */
    rgpio_pin_config_t sw_config = {
        kRGPIO_DigitalInput,
        0,
    };

    /* Init input switch GPIO. */
    RGPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, BOARD_SW_INT_OUTPUT, kRGPIO_InterruptFallingEdge);
    RGPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);
}

/* Get GPIO/button IRQ number */
static IRQn_Type rgpio_get_irqn(void)
{
    return BOARD_SW_IRQ;
}

/* GPIO/button ISR handler */
static void rgpio_isr_handler(void)
{
    /* Clear external interrupt flag. */
    RGPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, BOARD_SW_INT_OUTPUT, 1U << BOARD_SW_GPIO_PIN);
}

/* Check if button is pressed */
static bool rgpio_is_button_pressed(void)
{
    return (RGPIO_PinRead(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN) == 0);
}

/* RGPIO HAL implementation */
static const tickless_gpio_hal_t rgpio_hal = {.init              = rgpio_init,
                                              .get_irqn          = rgpio_get_irqn,
                                              .isr_handler       = rgpio_isr_handler,
                                              .is_button_pressed = rgpio_is_button_pressed};

#if defined(CONFIG_TICKLESS_GPIO_RGPIO)
const tickless_gpio_hal_t *tickless_get_gpio_hal(void)
{
    return &rgpio_hal;
}
#endif
#endif
