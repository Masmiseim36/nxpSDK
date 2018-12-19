/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file gpio_aml.h
 *
 * This driver creates abstraction for GPIO for S32 SDK and MCUXpresso SDK.
 */

#ifndef SOURCE_MIDDLEWARE_GPIO_H_
#define SOURCE_MIDDLEWARE_GPIO_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "common_aml.h"

#if (SDK_VERSION == SDK_S32)
#include "Cpu.h"
#include "pin_mux.h"
#elif (SDK_VERSION == SDK_KSDK)
#include "fsl_common.h"
#include "fsl_gpio.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup enum_group
 * @{
 */
 /*! @brief Instance of GPIO peripheral (GPIO A = instanceA). */ 
typedef enum 
{
    instanceA = 0U,                 /*!< GPIO A. */
    instanceB,                      /*!< GPIO B. */
    instanceC,                      /*!< GPIO C. */
    instanceD,                      /*!< GPIO D. */
    instanceE,                      /*!< GPIO E. */
    instanceF,                      /*!< GPIO F. */
    instanceG,                      /*!< GPIO G. */
    instanceH,                      /*!< GPIO H. */
    instanceI                       /*!< GPIO I. */
} gpio_aml_instance_t;

/*! @brief GPIO peripheral direction. */
typedef enum 
{
    gpioDirDigitalInput = 0U,  /*!< Set current pin as digital input. */
    gpioDirDigitalOutput = 1U  /*!< Set current pin as digital output. */
} gpio_aml_pin_direction_t;
 /*! @} */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to GPIO bases for each instance. */
static GPIO_Type *const g_gpioBases[] = GPIO_BASE_PTRS;
/*! @brief Pointers to port bases for each instance. */
static PORT_Type *const g_portBases[] = PORT_BASE_PTRS;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup function_group
 * @{
 */  
/*!
 * @brief   Sets output pin to '1' or '0'.
 *
 * @param   instance    Instance of GPIO peripheral, e.g. 0 for GPIOA, 1 for GPIOB, ....
 *                      Options are available in enumeration gpio_aml_instance_t.
 * @param   pinIndex    Number of GPIO pin to be set.
 * @param   outputValue Value to be written to GPIO output pin, '1' or '0'.
 */
static inline void GPIO_AML_WriteOutput(aml_instance_t instance, uint8_t pinIndex, uint8_t outputValue)
{
    (outputValue == 0U) ? (g_gpioBases[instance]->PCOR = 1U << pinIndex) : 
            (g_gpioBases[instance]->PSOR = 1U << pinIndex);
}

/*!
 * @brief   Sets output pin to '1'.
 *
 * @param   instance    Instance of GPIO peripheral, e.g. 0 for GPIOA, 1 for GPIOB, ....
 *                      Options are available in enumeration gpio_aml_instance_t.
 * @param   pinIndex    Number of GPIO pin to be set.
 */
static inline void GPIO_AML_SetOutput(aml_instance_t instance, uint8_t pinIndex)
{
    g_gpioBases[instance]->PSOR = (uint32_t)(1U << pinIndex);
}

/*!
 * @brief   Clears output pin. Sets it to '0'.
 *
 * @param   instance    Instance of GPIO peripheral, e.g. 0 for GPIOA, 1 for GPIOB, ....
 *                      Options are available in enumeration gpio_aml_instance_t.
 * @param   pinIndex    Number of GPIO pin to be cleared.
 */
static inline void GPIO_AML_ClearOutput(aml_instance_t instance, uint8_t pinIndex)
{
    g_gpioBases[instance]->PCOR = (uint32_t)(1U << pinIndex);
}

/*!
 * @brief   Toggles output pin.
 *
 * @param   instance    Instance of GPIO peripheral, e.g. 0 for GPIOA, 1 for GPIOB, ....
 *                      Options are available in enumeration gpio_aml_instance_t.
 * @param   pinIndex    Number of GPIO pin to be toggled.
 */
static inline void GPIO_AML_ToggleOutput(aml_instance_t instance, uint8_t pinIndex)
{
    g_gpioBases[instance]->PTOR = (uint32_t)(1U << pinIndex);
}

/*!
 * @brief   Returns value of input pin.
 *
 * @param   instance    Instance of GPIO peripheral, e.g. 0 for GPIOA, 1 for GPIOB, ....
 *                      Options are available in enumeration gpio_aml_instance_t.
 * @param   pinIndex    Number of GPIO pin to be read.
 *
 * @return  Value of input pin (0 - low, 1 - high).
 */
static inline uint32_t GPIO_AML_ReadInput(aml_instance_t instance, uint8_t pinIndex)
{
    return (((g_gpioBases[instance]->PDIR) >> pinIndex) & 1U);
}

/*!
 * @brief   Returns interrupt flags of whole port, e.g. PORTA. Returned value
 *          needs to be masked in order to find out the interrupt source.
 *          If configured for a level sensitive interrupt that remains asserted,
 *          the flag is set again immediately.
 *
 * @param   instance    Instance of PORT, e.g. 0 for PORTA, 1 for PORTB, ....
 *                      Options are available in enumeration gpio_aml_instance_t.
 *
 * @return  Interrupt flags of desired port.
 */
static inline uint32_t GPIO_AML_GetInterruptFlags(aml_instance_t instance)
{
    return g_portBases[instance]->ISFR;
}

/*!
 * @brief   Clears interrupt.
 *
 * @param   instance    Instance of PORT, e.g. 0 for PORTA, 1 for PORTB, ....
 *                      Options are available in enumeration gpio_aml_instance_t.
 * @param   pinIndex    Number of GPIO input pin which asserted interrupt.
 */
static inline void GPIO_AML_ClearInterruptFlags(aml_instance_t instance, uint8_t pinIndex)
{
    g_portBases[instance]->ISFR = (uint32_t)(1U << pinIndex);
}

/*!
 * @brief   Changes pin direction.
 *
 * @param   instance    Instance of PORT, e.g. 0 for PORTA, 1 for PORTB,... 
 *                      Options are available in enumeration gpio_aml_instance_t.
 * @param   pinIndex    Pin index in context of selected port.
 * @param   pinDir      Pin direction.
 */
static inline void GPIO_AML_SetDirection(aml_instance_t instance, uint8_t pinIndex, 
        gpio_aml_pin_direction_t pinDir)
{
#if (SDK_VERSION == SDK_KSDK)
    if (pinDir == gpioDirDigitalInput)
    {
        g_gpioBases[instance]->PDDR &= ~(1U << pinIndex);
    }
    else
    {
        g_gpioBases[instance]->PDDR |= (1U << pinIndex);
 
    }
#elif (SDK_VERSION == SDK_S32)
    PINS_DRV_SetPinDirection(g_gpioBases[instance], pinIndex, pinDir);
#endif
}
/*! @} */

#endif /* SOURCE_MIDDLEWARE_GPIO_H_ */

 /*******************************************************************************
 * EOF
 ******************************************************************************/
