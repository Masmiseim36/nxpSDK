/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EMWIN_SUPPORT_H_
#define _EMWIN_SUPPORT_H_

#include "display_support.h"

#define LCD_WIDTH           DEMO_PANEL_WIDTH
#define LCD_HEIGHT          DEMO_PANEL_HEIGHT
#define LCD_BITS_PER_PIXEL  (DEMO_BUFFER_BYTE_PER_PIXEL * 8)
#define LCD_BYTES_PER_PIXEL (DEMO_BUFFER_BYTE_PER_PIXEL)

/* Color depth dependent definitions */
#define DISPLAY_DRIVER   GUIDRV_LIN_16
#define COLOR_CONVERSION GUICC_M565

/* Define scale factors */
#if (DEMO_PANEL_RM67162 == DEMO_PANEL || DEMO_PANEL_CO5300 == DEMO_PANEL)
#define GUI_SCALE_FACTOR   1
#define GUI_SCALE_FACTOR_X 1.2
#define GUI_SCALE_FACTOR_Y 1.5
#else
#define GUI_SCALE_FACTOR   2
#define GUI_SCALE_FACTOR_X 2.2
#define GUI_SCALE_FACTOR_Y 2
#endif

/* Use larger fonts */
#define GUI_NORMAL_FONT (&GUI_Font24_ASCII)
#define GUI_LARGE_FONT  (&GUI_Font32B_ASCII)

#define GUI_BUFFERS  2
#define GUI_NUMBYTES 20000U /*! Amount of memory assigned to the emWin library */

#define VRAM_SIZE (LCD_HEIGHT * LCD_WIDTH * LCD_BYTES_PER_PIXEL)

extern int BOARD_Touch_Poll(void);
extern void BOARD_TouchIntHandler(void);

#endif
