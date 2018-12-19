/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "usb_io.h"

void USB_GpioInterruptInit(uint8_t instance, uint8_t port, uint32_t pin, uint8_t trrigger, usb_gpio_callback_t callback)
{
    uint8_t trig;
    PORT_Type *portList[] = PORT_BASE_PTRS;
    GPIO_Type *gpioList[] = GPIO_BASE_PTRS;

    switch (trrigger)
    {
        case kUSB_GpioInterruptLogicZero:
            trig = kPORT_InterruptLogicZero;
            break;
        case kUSB_GpioInterruptLogicOne:
            trig = kPORT_InterruptLogicOne;
            break;

        case kUSB_GpioInterruptRisingEdge:
            trig = kPORT_InterruptRisingEdge;
            break;

        case kUSB_GpioInterruptFallingEdge:
            trig = kPORT_InterruptFallingEdge;
            break;

        default:
            trig = kPORT_InterruptEitherEdge;
            break;
    }
    /* initialize port interrupt */
    PORT_SetPinInterruptConfig(portList[instance], pin, (port_interrupt_t)trig);
    gpioList[instance]->PDDR &= ~(1U << pin); /* gpio set as input */
}

void USB_GpioInterruptEnable(uint8_t instance, uint8_t port, uint32_t pin, uint8_t enable)
{
}

void USB_GpioOutputInit(uint8_t instance, uint32_t port, uint32_t pin, uint8_t defaultValue)
{
    GPIO_Type *instanceList[] = GPIO_BASE_PTRS;
    gpio_pin_config_t pin_config = {
        kGPIO_DigitalOutput, defaultValue,
    };

    GPIO_PinInit(instanceList[instance], pin, &pin_config);
}

void USB_GpioOutputWritePin(uint8_t instance, uint32_t port, uint32_t pin, uint8_t output)
{
    GPIO_Type *instanceList[] = GPIO_BASE_PTRS;

    GPIO_PinWrite(instanceList[instance], pin, output);
}

void USB_GpioInputInit(uint8_t instance, uint32_t port, uint32_t pin)
{
    GPIO_Type *instanceList[] = GPIO_BASE_PTRS;
    gpio_pin_config_t pin_config = {
        kGPIO_DigitalInput, 0,
    };

    GPIO_PinInit(instanceList[instance], pin, &pin_config);
}

uint8_t USB_GpioInputReadPin(uint8_t instance, uint32_t port, uint32_t pin)
{
    GPIO_Type *instanceList[] = GPIO_BASE_PTRS;
    return GPIO_PinRead(instanceList[instance], pin);
}
