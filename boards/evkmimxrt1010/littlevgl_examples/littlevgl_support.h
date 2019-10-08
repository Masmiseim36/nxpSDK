/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LITTLEVGL_SUPPORT_H
#define LITTLEVGL_SUPPORT_H

#include <stdint.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_FB_BYTE_PER_PIXEL 2

#define BOARD_LCD_SPI Driver_SPI1
#define BOARD_LCD_SPI_BAUDRATE 80000000U /*! Transfer baudrate */

#define BOARD_LCD_DC_GPIO GPIO1   /*! LCD data/command port */
#define BOARD_LCD_DC_GPIO_PIN 03U /*! LCD data/command pin */

#define Display_Buffer_HEIGHT 25 /*The RAM of the MCU can't support the whole screen. Select a height of 25 here.*/

#define BOARD_TOUCH_I2C Driver_I2C1
#define BOARD_TOUCH_I2C_IRQ LPI2C1_IRQn

/* Select PLL2_MAIN_CLK (528 MHz) as master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (2U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (5U)
/* Select OSC_CLK (24 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (1U)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void lv_port_pre_init(void);
void lv_port_disp_init(void);
void lv_port_indev_init(void);

#if defined(__cplusplus)
}
#endif

#endif /*LITTLEVGL_SUPPORT_H */
