/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MODULE_GPIO_H_
#define _NT_MODULE_GPIO_H_

/**
 * \defgroup gpio GPIO module
 * \ingroup modules
 * GPIO module uses the MCU's General Purpose pins and Timer.
 * \{
 */

#include "nt_modules.h"
#include "nt_types.h"
#include "nt_gpio.h"
#include "nt_safety.h"

#define NT_MODULE_GPIO_NAME "nt_module_gpio_interface"

/* forward declaration */
struct nt_module;
struct nt_module_interface;
struct nt_module_data;

/** Gpio user's interface, which is used by the GPIO modules.
 *  All of these functions must be implemented in the application.
 */
struct nt_module_gpio_user_interface
{
    void (*set_pin_output_ptr)(uint32_t port, uint32_t pin);    /*!< Set pin direction to output */
    void (*set_pin_input_ptr)(uint32_t port, uint32_t pin);     /*!< Set pin direction to input */
    void (*set_pin_low_ptr)(uint32_t port, uint32_t pin);       /*!< Set pin to logic low */
    void (*set_pin_high_ptr)(uint32_t port, uint32_t pin);      /*!< Set pin to logic high */
    uint32_t (*get_pin_value_ptr)(uint32_t port, uint32_t pin); /*!< Get pin value */
    void (*set_pin_default_state_ptr)(uint32_t port,
                                      uint32_t pin); /*!< Set pin to default state when it's not being measured */
    void (*init_timer_ptr)(void);                    /*!< Init timer */
    void (*start_timer_ptr)(void);                   /*!< Start timer */
    void (*stop_timer_ptr)(void);                    /*!< Stop timer */
    void (*timer_reset_counter_ptr)(void);           /*!< Reset timer counter */
    uint32_t (*timer_get_counter_ptr)(void);         /*!< Get timer counter */
    uint32_t (*timer_get_overrun_ptr)(void);         /*!< Get timer overrun */
    void (*set_pin_pull_up_ptr)(uint32_t port, uint32_t pin);   /*!< Set pin to GPIO with pull-up */
    void (*set_pin_pull_down_ptr)(uint32_t port, uint32_t pin); /*!< Set pin to GPIO with pull-down */
    void (*set_pin_tsi_mode_ptr)(uint32_t port, uint32_t pin);  /*!< Set pin to TSI mode */
    void (*set_pin_gpio_mode_ptr)(uint32_t port, uint32_t pin); /*!< Set pin to GPIO mode */
};

/** GPIO module, which uses the \b ??interrupts? port to sample a signal from the running timer counter.
 */
struct nt_module_gpio_params
{
    const struct nt_module_gpio_user_interface *user_interface;
};

extern const struct nt_module_interface nt_module_gpio_interface; /*!< Can't be NULL. */

int32_t nt_gpio_data_check(const struct nt_module_data *module);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif

/** \} */ /*/ end of gpio group */
