/* Copyright 2018-2019 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SE_RESET_CONFIG_H
#define _SE_RESET_CONFIG_H

#if defined(CPU_MPC5748G)
#else
#include "board.h"
#endif

/*
 * Platform based definitions for Enable pin
 * Define GPIO port for enable pin
 * Define Pin number on GPIO port for enable pin
 */
#if defined(FRDM_K64F)
#define SE05X_ENA_HOST_PORT GPIOA
#define SE05X_ENA_HOST_PIN 2U
#elif defined(MIMXRT1062_SERIES) || defined (MIMXRT1061_SERIES)
#define SE05X_ENA_HOST_PORT BOARD_USER_LED_GPIO
#define SE05X_ENA_HOST_PIN 10U
#elif defined(MIMXRT1176_cm7_SERIES) || defined(MIMXRT1175_cm7_SERIES) || defined (MIMXRT1173_cm7_SERIES) || defined (MIMXRT1172_SERIES) || defined (MIMXRT1171_SERIES)
#define SE05X_ENA_HOST_PORT GPIO9
#define SE05X_ENA_HOST_PIN 4U
#elif defined(LPC_55x)
#define SE05X_ENA_HOST_PORT BOARD_SW2_GPIO_PORT
#define SE05X_ENA_HOST_PIN 4U
#elif defined(NORDIC_MCU)
/* To Do se05x ena pin*/
#define SE05X_ENA_HOST_PIN NRF_GPIO_PIN_MAP(0,20)
#endif

#endif // _SE_RESET_CONFIG_H
