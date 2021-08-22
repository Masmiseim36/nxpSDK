/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_MODULE_GPIOINT_H_
#define _NT_MODULE_GPIOINT_H_

/**
 * \defgroup gpioint GPIO interrupt basic module
 * \ingroup modules
 * The GPIO module uses the General Purpose pins and Timer of the MCU.
 * Works on GPIO pins with interrupt.
 * \{
 */

#include "nt_modules.h"
#include "nt_types.h"
#include "nt_gpio.h"

#define NT_MODULE_GPIOINT_NAME "nt_module_gpioint_interface"

/* forward declaration */
struct nt_module;
struct nt_module_interface;

/** GPIO user's interface, which is used by the GPIO modules.
 *  All of these functions must be implemented in the application.
 */
struct nt_module_gpioint_user_interface
{
    void (*set_pin_output_ptr)(uint32_t port, uint32_t pin); /*!< Set the pin direction to output */
    void (*set_pin_input_ptr)(uint32_t port, uint32_t pin);  /*!< Set the pin direction to input */
    void (*set_pin_low_ptr)(uint32_t port, uint32_t pin);    /*!< Set the pin to logic low */
    void (*set_pin_high_ptr)(uint32_t port, uint32_t pin);   /*!< Set the pin to logic high */
    void (*init_pin_ptr)(uint32_t port, uint32_t pin);       /*!< Initialize the pin to a state ready for measurement */
    void (*set_pin_interrupt_ptr)(uint32_t port, uint32_t pin);   /*!< Enable the pin to generate an interrupt */
    void (*clear_pin_interrupt_ptr)(uint32_t port, uint32_t pin); /*!< Disable the pin to generate an interrupt */
    void (*init_timer_ptr)(void);                                 /*!< Init timer */
    void (*start_timer_ptr)(void);                                /*!< Start timer */
    void (*stop_timer_ptr)(void);                                 /*!< Stop timer */
    void (*timer_reset_counter_ptr)(void);                        /*!< Reset timer counter */
    uint32_t (*timer_get_counter_ptr)(void);                      /*!< Get timer counter */
};

/** GPIO interrupt module, which uses the port interrupts to sample a signal from the running timer counter.
 */
struct nt_module_gpioint_params
{
    const struct nt_module_gpioint_user_interface *user_interface;
};

extern const struct nt_module_interface nt_module_gpioint_interface; /*!< Can't be NULL. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief This interrupt handler must be invoked from the user's port interrupt ISR.
 *         There can be other pins on the same port which can invoke the interrupt; therefore,
 *         it is up to the application to decode which pin caused the interrupt. For example, if
 *         there's a button on the PTA3 and an electrode on the PTA4, the PORTA ISR handler must decode,
 *         whether the interrupt was caused by the PTA3 or PTA4. Invoke the nt_module_gpioint_isr() only if any of the
 * GPIO modules' electrodes caused an interrupt. \param module Pointer to the module that invokes the interrupt; it
 * depends on the user application to handle the right value. \return None.
 */
void nt_module_gpioint_isr(const struct nt_module *module);

/**
 *  \brief This interrupt handler should be invoked from the user's timer interrupt ISR.
 *         It is not mandatory to call this function, but it's designed to avoid,
 *         stuck to the NXP Touch GPIO Interrupt module. It should be called
 *         after the user-defined maximal timeout for one measurement.
 *  \param module Pointer to the module that invokes the interrupt; it depends on the user application to handle the
 * right value. \return None.
 */
void nt_module_gpioint_overflow_isr(const struct nt_module *module);
#ifdef __cplusplus
}
#endif

#endif

/** \} */ /* end of the gpioint group */
