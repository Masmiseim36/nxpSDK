/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
* @file gpio_driver.c
* @brief The gpio_driver.c file contains Generic API Adaption to SDK 2.0 GPIO Driver.
*/

#include "gpio_driver.h"

/*******************************************************************************
* Definitions
******************************************************************************/
#define GPIO_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0) /* driver version */
// ISR handler array for each gpio pin in the system
#define GPIO_NUMBER_OF_PIN 0x20

/*******************************************************************************
* Variables
******************************************************************************/

/* Driver Version */
static const GENERIC_DRIVER_VERSION DriverVersion = {GPIO_API_VERSION, GPIO_DRV_VERSION};
static gpioIsrObj_t isrObj[TOTAL_NUMBER_PORT][GPIO_NUMBER_OF_PIN];
static gpioConfigKSDK_t gpioConfigDefault = {
    .pinConfig = {kGPIO_DigitalInput, 0}, .portPinConfig = {0}, .interruptMode = kPORT_InterruptRisingEdge};

/*******************************************************************************
 * Code
 ******************************************************************************/

/***********************************************************************
 *
 * Function Name : ksdk_gpio_get_version
 * Description   : get the driver version.
 *
 ***************************************************************************/
GENERIC_DRIVER_VERSION ksdk_gpio_get_version(void)
{
    return DriverVersion;
}

/***********************************************************************
 *
 * Function Name : ksdk_gpio_pin_init
 * Description   : Initialize particular GPIO pin used by board.
 *
 ***************************************************************************/
void ksdk_gpio_pin_init(
    pinID_t aPinId, gpio_direction_t dir, void *apPinConfig, gpio_isr_handler_t aIsrHandler, void *apUserData)
{
    gpioConfigKSDK_t *pGpioConfig = (gpioConfigKSDK_t *)apPinConfig;
    gpioHandleKSDK_t *pinHandle = (gpioHandleKSDK_t *)aPinId;
    if (NULL == apPinConfig)
    {
        pGpioConfig = &gpioConfigDefault;
        pGpioConfig->portPinConfig.pullSelect = kPORT_PullUp;
        pGpioConfig->portPinConfig.mux = kPORT_MuxAsGpio;
    }
    // Configure the clock
    CLOCK_EnableClock(pinHandle->clockName);

    // Set the port pin configuration value
    PORT_SetPinConfig(pinHandle->portBase, pinHandle->pinNumber, &pGpioConfig->portPinConfig);

    pGpioConfig->pinConfig.pinDirection = dir == GPIO_DIRECTION_IN ? kGPIO_DigitalInput : kGPIO_DigitalOutput;
    // Set the pin information
    GPIO_PinInit(pinHandle->base, pinHandle->pinNumber, &pGpioConfig->pinConfig);

    // Isr is installed
    if (aIsrHandler)
    {
        // Enable the IRQ
        EnableIRQ(pinHandle->irq);
        isrObj[pinHandle->portNumber][pinHandle->pinNumber].isrHandle = aIsrHandler;
        isrObj[pinHandle->portNumber][pinHandle->pinNumber].pUserData = apUserData;
        // Enable the interrupt on a pin.
        PORT_SetPinInterruptConfig(pinHandle->portBase, pinHandle->pinNumber, pGpioConfig->interruptMode);
    }
}

/***********************************************************************
 *
 * Function Name : ksdk_gpio_set_pin
 * Description   : Set output level of individual GPIO pin to logic 1.
 *
 ***************************************************************************/
void ksdk_gpio_set_pin(pinID_t aPinId)
{
    gpioHandleKSDK_t *pinHandle = (gpioHandleKSDK_t *)aPinId;
    GPIO_SetPinsOutput(pinHandle->base, pinHandle->mask);
}

/***********************************************************************
 *
 * Function Name : ksdk_gpio_clr_pin
 * Description   : Set output level of individual GPIO pin to logic 0..
 *
 ***************************************************************************/
void ksdk_gpio_clr_pin(pinID_t aPinId)
{
    gpioHandleKSDK_t *pinHandle = (gpioHandleKSDK_t *)aPinId;
    GPIO_ClearPinsOutput(pinHandle->base, pinHandle->mask);
}

/***********************************************************************
 *
 * Function Name : ksdk_gpio_toggle_pin
 * Description   : toggle the currrent output logic of individual GPIO pin.
 *
 ***************************************************************************/
void ksdk_gpio_toggle_pin(pinID_t aPinId)
{
    gpioHandleKSDK_t *pinHandle = (gpioHandleKSDK_t *)aPinId;
    GPIO_TogglePinsOutput(pinHandle->base, pinHandle->mask);
}

/***********************************************************************
 *
 * Function Name : ksdk_gpio_write_pin
 * Description   : Set output level of individual GPIO pin to desired value, ie 1 or 0.
 *
 ***************************************************************************/
void ksdk_gpio_write_pin(pinID_t aPinId, uint8_t aValue)
{
    gpioHandleKSDK_t *pinHandle = (gpioHandleKSDK_t *)aPinId;
    GPIO_WritePinOutput(pinHandle->base, pinHandle->pinNumber, aValue);
}

/***********************************************************************
 *
 * Function Name : ksdk_gpio_read_pin
 * Description   : Read current input value of individual GPIO pin.
 *
 ***************************************************************************/
uint32_t ksdk_gpio_read_pin(pinID_t aPinId)
{
    gpioHandleKSDK_t *pinHandle = (gpioHandleKSDK_t *)aPinId;
    return GPIO_ReadPinInput(pinHandle->base, pinHandle->pinNumber);
}

/***********************************************************************
 *
 * Function Name : ksdk_gpio_handle_interrupt
 * Description   : handle the gpio interrupt in a pin.
 *
 ***************************************************************************/
void ksdk_gpio_handle_interrupt(GPIO_Type *apBase, port_number_t aPortNumber)
{
    uint32_t isfr = GPIO_GetPinsInterruptFlags(apBase);

    // parse through all the pending interrupt for a PORT
    for (uint8_t i = 0; i < GPIO_NUMBER_OF_PIN; i++)
    {
        if (isfr & (1 << i))
        {
            gpio_isr_handler_t handle = isrObj[aPortNumber][i].isrHandle;
            if (handle == NULL)
            {
                continue;
            }
            // call user defined handler
            handle(isrObj[aPortNumber][i].pUserData);
            GPIO_ClearPinsInterruptFlags(apBase, (1 << i));
        }
    }
}

GENERIC_DRIVER_GPIO Driver_GPIO_KSDK = {
    ksdk_gpio_get_version, ksdk_gpio_pin_init,  ksdk_gpio_set_pin,  ksdk_gpio_clr_pin,
    ksdk_gpio_toggle_pin,  ksdk_gpio_write_pin, ksdk_gpio_read_pin,
};
