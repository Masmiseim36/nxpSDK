/* Copyright 2018-2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SE_RESET_CONFIG_H
#define _SE_RESET_CONFIG_H

#include "board.h"

/*
 * Platform based definitions for Enable pin
 * Define GPIO port for enable pin
 * Define Pin number on GPIO port for enable pin
 */
#if defined(FRDM_K64F)
#define SE05X_ENA_HOST_PORT GPIOA
#define SE05X_ENA_HOST_PIN 2U
#elif defined(FRDM_KW41Z)
#define SE05X_ENA_HOST_PORT BOARD_LED_RED_GPIO
#define SE05X_ENA_HOST_PIN 19U
#elif defined(FRDM_K82F)
#define SE05X_ENA_HOST_PORT BOARD_LED_RED_GPIO
#define SE05X_ENA_HOST_PIN 12U
#elif defined(CPU_MIMXRT1062DVL6A)
#define SE05X_ENA_HOST_PORT BOARD_USER_LED_GPIO
#define SE05X_ENA_HOST_PIN 11U
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
#define SE05X_ENA_HOST_PORT GPIO9
#define SE05X_ENA_HOST_PIN 4U
#elif defined(LPC_55x)
#define SE05X_ENA_HOST_PORT BOARD_SW2_GPIO_PORT
#define SE05X_ENA_HOST_PIN BOARD_SW1_GPIO_PIN
#endif

#endif // _SE_RESET_CONFIG_H
