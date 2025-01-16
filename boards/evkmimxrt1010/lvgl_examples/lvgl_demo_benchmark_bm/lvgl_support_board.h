/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_BOARD_H
#define LVGL_SUPPORT_BOARD_H

#include <stdint.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BOARD_LCD_SPI          Driver_SPI1
#define BOARD_LCD_SPI_BAUDRATE 10000000U /*! Transfer baudrate */

#define BOARD_LCD_DC_GPIO     GPIO2      /*! LCD data/command port */
#define BOARD_LCD_DC_GPIO_PIN 22U        /*! LCD data/command pin */

#define BOARD_TOUCH_I2C     Driver_I2C1
#define BOARD_TOUCH_I2C_IRQ LPI2C1_IRQn

#define LPSPI_FREQ CLOCK_GetClockRootFreq(kCLOCK_LpspiClkRoot)
#define LPI2C_FREQ CLOCK_GetClockRootFreq(kCLOCK_Lpi2cClkRoot)

#define LPSPI_GetFreq LPSPI1_GetFreq
#define LPI2C_GetFreq LPI2C1_GetFreq

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /*LVGL_SUPPORT_BOARD_H */
