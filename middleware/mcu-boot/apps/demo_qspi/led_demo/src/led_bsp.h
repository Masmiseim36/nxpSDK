/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __LED_BSP_H__
#define __LED_BSP_H__

#include "fsl_device_registers.h"

#define LED_ON_LOW_LEVEL 1
#define LED_ON_HIGH_LEVEL 2

#define LED0 (1)
#define LED1 (2)
#define LED2 (4)

#if defined(TWR_K80F150M)
#define MAX_LED_IDX (LED1)
#define LED0_OFFSET 14
#define LED1_OFFSET 15
#define LED0_PORT (PORTD)
#define LED0_GPIO (GPIOD)
#define LED1_PORT (PORTD)
#define LED1_GPIO (GPIOD)
#define LED_POLARITY LED_ON_HIGH_LEVEL
#elif defined(FRDM_K82F)
#define MAX_LED_IDX (LED2)
#define LED0_OFFSET 8
#define LED1_OFFSET 9
#define LED2_OFFSET 10
#define LED0_PORT (PORTC)
#define LED0_GPIO (GPIOC)
#define LED1_PORT (PORTC)
#define LED1_GPIO (GPIOC)
#define LED2_PORT (PORTC)
#define LED2_GPIO (GPIOC)
#define LED_POLARITY LED_ON_LOW_LEVEL
#elif defined(TWR_POS_K81)
#define MAX_LED_IDX (LED2)
#define LED0_OFFSET 7
#define LED1_OFFSET 8
#define LED2_OFFSET 13
#define LED0_PORT (PORTC)
#define LED0_GPIO (GPIOC)
#define LED1_PORT (PORTC)
#define LED1_GPIO (GPIOC)
#define LED2_PORT (PORTC)
#define LED2_GPIO (GPIOC)
#define LED_POLARITY LED_ON_LOW_LEVEL
#elif defined(TWR_KL82Z72M)
#define MAX_LED_IDX (LED1)
#define LED0_OFFSET 14
#define LED1_OFFSET 15
#define LED0_PORT (PORTD)
#define LED0_GPIO (GPIOD)
#define LED1_PORT (PORTD)
#define LED1_GPIO (GPIOD)
#define LED_POLARITY LED_ON_HIGH_LEVEL
#elif defined(FRDM_KL82Z)
#define MAX_LED_IDX (LED2)
#define LED0_OFFSET 0
#define LED1_OFFSET 1
#define LED2_OFFSET 2
#define LED0_PORT (PORTC)
#define LED0_GPIO (GPIOC)
#define LED1_PORT (PORTC)
#define LED1_GPIO (GPIOC)
#define LED2_PORT (PORTC)
#define LED2_GPIO (GPIOC)
#define LED_POLARITY LED_ON_LOW_LEVEL
#elif defined(TWR_POS_KL81)
#define MAX_LED_IDX (LED2)
#define LED0_OFFSET 7
#define LED1_OFFSET 8
#define LED2_OFFSET 13
#define LED0_PORT (PORTC)
#define LED0_GPIO (GPIOC)
#define LED1_PORT (PORTC)
#define LED1_GPIO (GPIOC)
#define LED2_PORT (PORTC)
#define LED2_GPIO (GPIOC)
#define LED_POLARITY LED_ON_LOW_LEVEL
#else
#error This board is not supported!
#endif

#endif
