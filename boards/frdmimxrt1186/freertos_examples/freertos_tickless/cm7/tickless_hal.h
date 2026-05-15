/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TICKLESS_HAL_H
#define TICKLESS_HAL_H

#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_device_registers.h"
#include "app.h"

/**
 * @brief Maximum 32-bit value
 *
 * This constant defines the maximum value that can be represented in a 32-bit unsigned integer.
 * It is used for calculating the maximum number of ticks that can be suppressed in tickless mode.
 */
#define portMAX_32_BIT_NUMBER (0xffffffffUL)

#if configUSE_TICKLESS_IDLE == 2

/**
 * @brief Hardware Abstraction Layer (HAL) interface for timer implementations
 *
 * This structure defines the common interface that all timer HAL implementations must provide.
 * It contains function pointers for all operations needed by the tickless implementation.
 */
typedef struct
{
    /**
     * @brief Initialize the timer hardware
     *
     * This function initializes the timer hardware for use in tickless mode.
     * It should configure the timer with appropriate settings for low power operation.
     */
    void (*init)(void);

    /**
     * @brief Get timer base address
     *
     * This function returns the base address of the timer peripheral.
     *
     * @return Base address of the timer peripheral
     */
    void *(*get_timer_base)(void);

    /**
     * @brief Get timer IRQ number
     *
     * This function returns the IRQ number for the timer peripheral.
     *
     * @return IRQ number for the timer peripheral
     */
    IRQn_Type (*get_timer_irqn)(void);

    /**
     * @brief Start the timer with given value
     *
     * This function starts the timer with the specified value.
     *
     * @param base Base address of the timer peripheral
     */
    void (*start_timer)(void *base);

    /**
     * @brief Stop the timer
     *
     * This function stops the timer.
     *
     * @param base Base address of the timer peripheral
     */
    void (*stop_timer)(void *base);

    /**
     * @brief Get current timer count
     *
     * This function returns the current count value of the timer.
     *
     * @param base Base address of the timer peripheral
     * @return Current count value of the timer
     */
    uint32_t (*get_current_count)(void *base);

    /**
     * @brief Set timer period/match value
     *
     * This function sets the period or match value for the timer.
     *
     * @param base Base address of the timer peripheral
     * @param value Period or match value to set
     */
    void (*set_timer_period)(void *base, uint32_t value);

    /**
     * @brief Clear timer status flags
     *
     * This function clears the status flags of the timer.
     *
     * @param base Base address of the timer peripheral
     */
    void (*clear_status_flags)(void *base);

    /**
     * @brief Timer-specific ISR handler
     *
     * This function handles timer-specific operations in the ISR.
     * It is called by the common timer ISR handler.
     */
    void (*isr_handler)(void);

    /**
     * @brief Get timer clock frequency in Hz
     *
     * This function returns the clock frequency of the timer in Hz.
     *
     * @return Timer clock frequency in Hz
     */
    uint32_t (*get_timer_frequency)(void);

    /**
     * @brief Calculate elapsed ticks based on current timer state
     *
     * This function calculates how many tick periods have elapsed based on the current timer state.
     *
     * @param base Base address of the timer peripheral
     * @param reload_value The reload value that was set for the timer
     * @param expected_idle_time The expected idle time in ticks
     * @return Number of tick periods that have elapsed
     */
    uint32_t (*calculate_elapsed_ticks)(void *base, uint32_t reload_value, uint32_t expected_idle_time);
} tickless_timer_hal_t;

#endif /* configUSE_TICKLESS_IDLE == 2 */

/**
 * @brief Hardware Abstraction Layer (HAL) interface for GPIO/button operations
 *
 * This structure defines the common interface that all GPIO HAL implementations must provide.
 * It contains function pointers for all operations needed for button input in tickless mode.
 */
typedef struct
{
    /**
     * @brief Initialize GPIO/button hardware
     *
     * This function initializes the GPIO hardware for button input.
     * It should configure the pin as an input with appropriate settings (pull-up/down, etc.).
     */
    void (*init)(void);

    /**
     * @brief Get GPIO/button IRQ number
     *
     * This function returns the IRQ number for the GPIO/button.
     *
     * @return IRQ number for the GPIO/button
     */
    IRQn_Type (*get_irqn)(void);

    /**
     * @brief GPIO/button ISR handler
     *
     * This function handles GPIO/button-specific operations in the ISR.
     * It is called by the platform-specific GPIO/button ISR.
     */
    void (*isr_handler)(void);

    /**
     * @brief Check if button is pressed
     *
     * This function checks if the button is currently pressed.
     *
     * @return true if the button is pressed, false otherwise
     */
    bool (*is_button_pressed)(void);
} tickless_gpio_hal_t;

#endif /* TICKLESS_HAL_H */
