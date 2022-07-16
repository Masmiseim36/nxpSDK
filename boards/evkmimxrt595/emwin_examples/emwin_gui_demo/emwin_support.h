/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EMWIN_SUPPORT_H_
#define _EMWIN_SUPPORT_H_

#include "display_support.h"

/* Macros for the LCD controller. */
#define APP_ELCDIF LCDIF

#define APP_IMG_HEIGHT 272
#define APP_IMG_WIDTH  480
#define APP_HSW        41
#define APP_HFP        4
#define APP_HBP        8
#define APP_VSW        10
#define APP_VFP        4
#define APP_VBP        2
#define APP_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/* Display. */
#define LCD_DISP_GPIO     GPIO1
#define LCD_DISP_GPIO_PIN 2
/* Back light. */
#define LCD_BL_GPIO     GPIO2
#define LCD_BL_GPIO_PIN 31

/* Macros for the touch touch controller. */
#define BOARD_TOUCH_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)

#define BOARD_TOUCH_I2C_CLOCK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define BOARD_TOUCH_I2C_BAUDRATE   100000U

#define LPI2C_DATA_LENGTH 32U

#define LPI2C_MASTER_IRQ        LPI2C1_IRQn
#define LPI2C_MASTER_IRQHandler LPI2C1_IRQHandler

#define BOARD_LCD_READABLE 1

#define LCD_WIDTH           DEMO_BUFFER_WIDTH
#define LCD_HEIGHT          DEMO_BUFFER_HEIGHT
#define LCD_BITS_PER_PIXEL  (DEMO_BUFFER_BYTE_PER_PIXEL * 8)
#define LCD_BYTES_PER_PIXEL (DEMO_BUFFER_BYTE_PER_PIXEL)

/* Color depth dependent definitions */
#define DISPLAY_DRIVER      GUIDRV_LIN_16
#define COLOR_CONVERSION    GUICC_M565
#define ELCDIF_PIXEL_FORMAT kELCDIF_PixelFormatRGB565
#define APP_LCDIF_DATA_BUS  kELCDIF_DataBus16Bit

/* Define scale factors */
#define GUI_SCALE_FACTOR   0.8
#define GUI_SCALE_FACTOR_X 1.5
#define GUI_SCALE_FACTOR_Y 1.1

/* Use larger fonts */
#define GUI_NORMAL_FONT (&GUI_Font24_ASCII)
#define GUI_LARGE_FONT  (&GUI_Font32B_ASCII)

#define GUI_BUFFERS  2
#define GUI_NUMBYTES 20000U /*! Amount of memory assigned to the emWin library */

#define VRAM_SIZE (LCD_HEIGHT * LCD_WIDTH * LCD_BYTES_PER_PIXEL)

extern int BOARD_Touch_Poll(void);
extern void BOARD_TouchIntHandler(void);

#endif
