/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
#include "MIMXRT1052.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "MIMXRT1050-EVK"  

/*! @brief The USER_LED used for board */
#define BOARD_USER_LED_GPIO GPIO1
#define BOARD_USER_LED_GPIO_PIN (9U)

#define USER_LED_ON()  BOARD_USER_LED_GPIO->DR &= ~(1U << BOARD_USER_LED_GPIO_PIN); /*!< Turn off */
#define USER_LED_OFF() BOARD_USER_LED_GPIO->DR |= (1U << BOARD_USER_LED_GPIO_PIN); /*!<Turn on */
#define USER_LED_TOGGLE() 0x1^(((BOARD_USER_LED_GPIO->DR) >> BOARD_USER_LED_GPIO_PIN) & 0x1U) ? \
                          (BOARD_USER_LED_GPIO->DR |= (1U << BOARD_USER_LED_GPIO_PIN)) : \
                          (BOARD_USER_LED_GPIO->DR &= ~(1U << BOARD_USER_LED_GPIO_PIN)); /*!< Toggle */
                          
/*! @brief Define the port interrupt number for the board switches */
#define BOARD_USER_BUTTON_GPIO GPIO5
#define BOARD_USER_BUTTON_GPIO_PIN (0U)
#define BOARD_USER_BUTTON_IRQ GPIO5_Combined_0_15_IRQn
#define BOARD_USER_BUTTON_IRQ_HANDLER GPIO5_Combined_0_15_IRQHandler
#define BOARD_USER_BUTTON_NAME "SW8"
#define BOARD_USER_BUTTON_PRIORITY 4
   
/*! @brief The hyper flash size */
#define BOARD_FLASH_SIZE	(0x4000000U)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
  
uint32_t BOARD_UARTSrcFreq(void);
void BOARD_InitUART(uint32_t u32BaudRate);
void BOARD_InitSysTick(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
