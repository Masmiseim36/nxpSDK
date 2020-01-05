/*
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file gpio_driver.h
 * @brief The gpio_driver.h file containes the Generic Irq implmentation for the gpio.
 *        The type and variable names have been kept aligned to Kinetis family for compatibility of examples.
*/

#ifndef __DRIVER_GPIO_H__
#define __DRIVER_GPIO_H__

#include "Driver_Common.h"
#include "Driver_GPIO.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_gint.h"

typedef enum _gint_interrupt
{
    kGINT_InterruptLogic_0 = 0x0U, /*!< Interrupt on Logic 0 (Falling Edge or Level Low). */
    kGINT_InterruptLogic_1 = 0x1U, /*!< Interrupt on Logic 1 (Rising Edge or Level High). */
} gint_interrupt_t;

/*!
 * @brief The GPIO Configuration KSDK.
 */
typedef struct gpioConfigKSDK
{
    gpio_pin_config_t pinConfig;         /*!< General pin charactertics.*/
    gint_trig_t       interruptMode;     /*!< Interrupt mode for a pin.*/
    gint_interrupt_t  interruptPolarity; /*!< Interrupt Polarity 0/1 for a pin.*/
} gpioConfigKSDK_t;

/*!
 * @brief The GPIO pin handle for KSDK.
 */
typedef struct gpioHandleKSDK
{
    GPIO_Type *base;           /*!< Base address of the GPIO Port.*/
    uint32_t pinNumber;        /*!< pin number start from 0 -31.*/
    uint32_t mask;             /*!< mask value for a pin.*/
    clock_ip_name_t clockName; /*!< Clock Name for the port.*/
    gint_port_t portNumber;    /*!< Port Number for the port.*/
} gpioHandleKSDK_t;

/*!
 * @brief The gpio isr object.
 */
typedef struct gpioIsrObj
{
    void *pUserData;              /*!< Pointer to a UserData.*/
    gpio_isr_handler_t isrHandle; /*!< pointer to isrHandle.*/
} gpioIsrObj_t;

/*!
 * @brief Macro to create a Gpio handle
 */
#define MAKE_GPIO_HANDLE(PortName, Base, PinNumber, ClockName, PortNumber)   \
    static gpioHandleKSDK_t PortName##PinNumber = {.base = Base,             \
                                                   .pinNumber = PinNumber,   \
                                                   .mask = 1 << (PinNumber), \
                                                   .clockName = ClockName,   \
                                                   .portNumber = PortNumber};

#define GPIO_PIN_ID(PortName, PinNumber) &(PortName##PinNumber)
extern GENERIC_DRIVER_GPIO Driver_GPIO_KSDK;

#endif // __DRIVER_GPIO_H__
