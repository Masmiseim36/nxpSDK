/* Copyright 2018-2019 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
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
#elif defined(CPU_MIMXRT1052DVL6B)
#define SE05X_ENA_HOST_PORT BOARD_USER_LED_GPIO
#define SE05X_ENA_HOST_PIN 11U
#elif defined(LPC_55x)
#define SE05X_ENA_HOST_PORT BOARD_SW2_GPIO_PORT
#define SE05X_ENA_HOST_PIN BOARD_SW1_GPIO_PIN
#endif

#endif // _SE_RESET_CONFIG_H
