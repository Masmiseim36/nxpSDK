/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "MIMXRT1062.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "MIMXRT1060-EVK"

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
#define BOARD_FLASH_SIZE    (0x4000000U)
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
