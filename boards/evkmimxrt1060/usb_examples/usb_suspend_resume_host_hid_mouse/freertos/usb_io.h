/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_IO_H_
#define _USB_IO_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef void (*usb_gpio_callback_t)(void);

typedef enum _usb_gpio_interrupt_trrigger
{
    kUSB_GpioInterruptLogicZero = 0x1U,   /*!< Interrupt when logic zero. */
    kUSB_GpioInterruptRisingEdge = 0x2U,  /*!< Interrupt on rising edge. */
    kUSB_GpioInterruptFallingEdge = 0x3U, /*!< Interrupt on falling edge. */
    kUSB_GpioInterruptEitherEdge = 0x4U,  /*!< Interrupt on either edge. */
    kUSB_GpioInterruptLogicOne = 0x5U,    /*!< Interrupt when logic one. */
} usb_gpio_interrupt_trigger_t;

/*******************************************************************************
 * API
 ******************************************************************************/
void USB_GpioInit(uint8_t instance, uint8_t port, uint32_t pinMask, usb_gpio_callback_t callback);
void USB_GpioInt(uint8_t instance, uint8_t enable);
void USB_GpioInterruptInit(
    uint8_t instance, uint8_t port, uint32_t pin, uint8_t trrigger, usb_gpio_callback_t callback);
void USB_GpioInterruptEnable(uint8_t instance, uint8_t port, uint32_t pin, uint8_t enable);
void USB_GpioOutputInit(uint8_t instance, uint32_t port, uint32_t pin, uint8_t defaultValue);
void USB_GpioOutputWritePin(uint8_t instance, uint32_t port, uint32_t pin, uint8_t output);
void USB_GpioInputInit(uint8_t instance, uint32_t port, uint32_t pin);
uint8_t USB_GpioInputReadPin(uint8_t instance, uint32_t port, uint32_t pin);

#endif /* _USB_IO_H_ */
