/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
/*${header:start}*/
#include "display_support.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_FB_ALIGN FRAME_BUFFER_ALIGN

#define APP_FB_USE_FIXED_ADDRESS 1
/* The PSRAM size is 8M, not enough for three 720*1280 XRGB8888 buffers. */
#define APP_FB_NUM   2
#define APP_FB0_ADDR 0x28000000
#define APP_FB1_ADDR 0x28400000

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL))
#define APP_USE_XRGB8888 1 /* LCDIF does not support RGB888 .*/
#else
#define APP_USE_XRGB8888 0
#endif

#define DEMO_SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST
#define DEMO_SDCARD_SWITCH_VOLTAGE_PORT 4U
#define DEMO_SDCARD_SWITCH_VOLTAGE_PIN  0U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_USDHC_Switch_VoltageTo1V8(void);
void BOARD_USDHC_Switch_VoltageTo3V3(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
