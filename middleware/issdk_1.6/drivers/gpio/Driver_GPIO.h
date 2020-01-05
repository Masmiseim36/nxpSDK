/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *
 * $Date:        2. Jan 2014
 * $Revision:    V2.00
 *
 * Project:      Common Driver definitions
 * -------------------------------------------------------------------------- */

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
