/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EMWIN_SUPPORT_H_
#define _EMWIN_SUPPORT_H_

#define BOARD_LCD_SPI Driver_SPI1
#define BOARD_LCD_SPI_BAUDRATE 8000000U /*! Transfer baudrate */

#define BOARD_LCD_DC_GPIO GPIO2   /*! LCD data/command port */
#define BOARD_LCD_DC_GPIO_PIN 22U /*! LCD data/command pin */

#define BOARD_LCD_READABLE 0

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define BOARD_TOUCH_I2C Driver_I2C1
#define BOARD_TOUCH_I2C_IRQ LPI2C1_IRQn

#define GUI_NUMBYTES 0x8000 /*! Amount of memory assigned to the emWin library */

extern int BOARD_Touch_Poll(void);

#endif
