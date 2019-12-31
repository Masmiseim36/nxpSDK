/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "USB-KW24D512"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_BLUE1_GPIO GPIOD
#define BOARD_LED_BLUE1_GPIO_PORT PORTD
#define BOARD_LED_BLUE1_GPIO_PIN 4U
#define BOARD_LED_BLUE2_GPIO GPIOD
#define BOARD_LED_BLUE2_GPIO_PORT PORTD
#define BOARD_LED_BLUE2_GPIO_PIN 5U

#define LED_BLUE1_INIT(output)                                   \
    GPIO_WritePinOutput(BOARD_LED_BLUE1_GPIO, BOARD_LED_BLUE1_GPIO_PIN, output);\
    BOARD_LED_BLUE1_GPIO->PDDR |= (1U << BOARD_LED_BLUE1_GPIO_PIN)  /*!< Enable target LED_BLUE1 */
#define LED_BLUE1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_BLUE1_GPIO, 1U << BOARD_LED_BLUE1_GPIO_PIN) /*!< Turn on target LED_BLUE1 */
#define LED_BLUE1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_BLUE1_GPIO, 1U << BOARD_LED_BLUE1_GPIO_PIN) /*!< Turn off target LED_BLUE1 */
#define LED_BLUE1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_BLUE1_GPIO, 1U << BOARD_LED_BLUE1_GPIO_PIN) /*!< Toggle on target LED_BLUE1 */

#define LED_BLUE2_INIT(output)                                   \
    GPIO_WritePinOutput(BOARD_LED_BLUE2_GPIO, BOARD_LED_BLUE2_GPIO_PIN, output);\
    BOARD_LED_BLUE2_GPIO->PDDR |= (1U << BOARD_LED_BLUE2_GPIO_PIN)  /*!< Enable target LED_BLUE2 */
#define LED_BLUE2_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_BLUE2_GPIO, 1U << BOARD_LED_BLUE2_GPIO_PIN) /*!< Turn on target LED_BLUE2 */
#define LED_BLUE2_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_BLUE2_GPIO, 1U << BOARD_LED_BLUE2_GPIO_PIN) /*!< Turn off target LED_BLUE2 */
#define LED_BLUE2_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_BLUE2_GPIO, 1U << BOARD_LED_BLUE2_GPIO_PIN) /*!< Toggle on target LED_BLUE2 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);
void setModemClock(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
