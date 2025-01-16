/* Copyright 2023 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SE_BOARD_CONFIG_H
#define _SE_BOARD_CONFIG_H

#include "board.h"
#include "fsl_iomuxc.h"
#include "fsl_common.h"
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && FSL_FEATURE_SOC_LPI2C_COUNT > 0
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#endif /* FSL_FEATURE_SOC_LPI2C_COUNT */

#if FSL_FEATURE_SOC_PIT_COUNT
#include "fsl_pit.h"
#endif /* FSL_FEATURE_SOC_PIT_COUNT */

/*
 * Platform based definitions for Enable pin
 * Define GPIO port for enable pin
 * Define Pin number on GPIO port for enable pin
 */

#define SE05X_ENA_HOST_PORT GPIO9
#define SE05X_ENA_HOST_PIN  4U

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && FSL_FEATURE_SOC_LPI2C_COUNT > 0

#if defined(__GNUC__)
#pragma GCC push_options
#pragma GCC optimize("O0")
#endif

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY (CLOCK_GetFreq(kCLOCK_OscRc48MDiv2))

#define AX_I2CM          (LPI2C_Type *)LPI2C1_BASE
#define AX_LPI2C_CLK_SRC LPI2C_CLOCK_FREQUENCY
#define AX_I2CM_IRQN     LPI2C1_IRQn
#define USE_LIP2C        1

#if defined(__GNUC__)
#pragma GCC pop_options
#endif

#endif /* FSL_FEATURE_SOC_LPI2C_COUNT */

#if FSL_FEATURE_SOC_PIT_COUNT
#define SE_PIT_RESET_HANDLER PIT0_IRQHandler
#define PIT_BASE_ADDR        PIT1
#define PIT_IRQ_ID           PIT1_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#endif /* FSL_FEATURE_SOC_PIT_COUNT */

/*
 * Where applicable, Configure the PINs on the Host
 *
 */
void se05x_host_configure(void);

/*
 * Where applicable, put SE in low power/standby mode
 *
 * Pre-Requistie: @ref se05x_host_configure has been called
 */
void se05x_host_powerdown(void);

/*
 * Where applicable, put SE in powered/active mode
 *
 * Pre-Requistie: @ref se05x_host_configure has been called
 */
void se05x_host_powerup(void);

#endif // _SE_BOARD_CONFIG_H
