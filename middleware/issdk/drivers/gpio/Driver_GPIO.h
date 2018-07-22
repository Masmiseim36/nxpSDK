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

#ifndef __DRIVER_GPIO_H
#define __DRIVER_GPIO_H

#include "Driver_Common.h"

#define GPIO_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0) /* API version */

/**
 * @file Driver_GPIO.h
* @brief  The Driver_GPIO.h file contains the definitions for GPIO Driver direction.
*/

typedef enum gpio_direction_en
{
    GPIO_DIRECTION_IN = 0U,  /*!< IN. */
    GPIO_DIRECTION_OUT = 1U, /*!< OUT. */
} gpio_direction_t;

/**
\brief GPIO Driver direction.
*/
typedef void *pinID_t;

typedef ARM_DRIVER_VERSION GENERIC_DRIVER_VERSION;

typedef void (*gpio_isr_handler_t)(void *apUserData);
/**
\brief Access structure of the GPIO Driver.
*/
typedef struct GENERIC_DRIVER_GPIO
{
    GENERIC_DRIVER_VERSION (*GetVersion)(void); /*!< Pointer to get driver version.*/
    void (*pin_init)(pinID_t aPinId,
                     gpio_direction_t dir,
                     void *apPinConfig,
                     gpio_isr_handler_t aIsrHandler,
                     void *apUserData);                /*!< Pointer to Initialize gpio Interface.*/
    void (*set_pin)(pinID_t aPinId);                   /*!< Pointer to set the pin.*/
    void (*clr_pin)(pinID_t aPinId);                   /*!< Pointer to clear the pin.*/
    void (*toggle_pin)(pinID_t aPinId);                /*!< Pointer to toggle the pin.*/
    void (*write_pin)(pinID_t aPinId, uint8_t aValue); /*!<< read the pin value.*/
    uint32_t (*read_pin)(pinID_t aPinId);              /*!<< read the pin value.*/
} const GENERIC_DRIVER_GPIO;

#endif /* __DRIVER_GPIO_H */
