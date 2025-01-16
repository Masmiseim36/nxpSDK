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

#define BOARD_TOUCH_I2C           Driver_I2C4
#define BOARD_TOUCH_I2C_IRQ       LPI2C4_IRQn
#define BOARD_TOUCH_I2C_FREQ_FUNC LPI2C4_GetFreq
#define BOARD_TOUCH_I2C_FREQ      CLOCK_GetClockRootFreq(kCLOCK_Lpi2cClkRoot)

#define BOARD_LCD_SPI            Driver_SPI1
#define BOARD_LCD_SPI_BAUDRATE   10000000U
#define BOARD_LCD_SPI_IRQ        LPSPI1_IRQn
#define BOARD_LCD_SPI_FREQ_FUNC  LPSPI1_GetFreq
#define BOARD_LCD_SPI_FREQ       CLOCK_GetClockRootFreq(kCLOCK_LpspiClkRoot)
#define BOARD_LCD_SPI_DMA_TX_IRQ DMA11_DMA27_IRQn
#define BOARD_LCD_SPI_DMA_RX_IRQ DMA10_DMA26_IRQn

/* Select PLL2_MAIN_CLK (528 MHz) as master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (2U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (kCLOCK_LpspiDivBy6)
/* Select OSC_CLK (24 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (kCLOCK_MiscDivBy2)

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
