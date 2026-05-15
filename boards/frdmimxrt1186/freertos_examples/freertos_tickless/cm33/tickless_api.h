/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TICKLESS_API_H
#define TICKLESS_API_H

#include "FreeRTOS.h"
#include "task.h"
#include "tickless_hal.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if configUSE_TICKLESS_IDLE == 2

/**
 * @brief Timer ISR handler for tickless mode
 *
 * This function handles the timer interrupt for tickless mode.
 * It should be called from the platform-specific timer ISR.
 *
 * The function will:
 * 1. Set the internal flag indicating that a timer interrupt occurred
 * 2. Call the timer-specific ISR handler from the HAL implementation
 *
 * @note This function is implemented in tickless_impl.c
 */
void vPortTimerISRHandler(void);

/**
 * @brief Get the timer HAL implementation for the current platform
 *
 * This function returns a pointer to the timer HAL implementation
 * for the current platform. The implementation is selected at build time
 * based on the platform configuration.
 *
 * The returned HAL implementation provides functions for:
 * - Initializing the timer hardware
 * - Starting and stopping the timer
 * - Setting the timer period
 * - Handling timer interrupts
 * - Calculating elapsed ticks
 *
 * @return Pointer to the timer HAL implementation
 *
 * @note This function is implemented by the selected timer HAL module
 */
const tickless_timer_hal_t *tickless_get_timer_hal(void);

#endif /* configUSE_TICKLESS_IDLE == 2 */

/**
 * @brief Get the GPIO HAL implementation for the current platform
 *
 * This function returns a pointer to the GPIO HAL implementation
 * for the current platform. The implementation is selected at build time
 * based on the platform configuration.
 *
 * The returned HAL implementation provides functions for:
 * - Initializing the GPIO hardware for button input
 * - Getting the IRQ number for the button
 * - Handling button interrupts
 * - Checking if the button is pressed
 *
 * @return Pointer to the GPIO HAL implementation
 *
 * @note This function is implemented by the selected GPIO HAL module
 */
const tickless_gpio_hal_t *tickless_get_gpio_hal(void);

#if defined(__cplusplus)
}
#endif

#endif /* TICKLESS_API_H */
