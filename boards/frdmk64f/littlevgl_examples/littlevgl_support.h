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
/* The virtual buffer for DBI panel, it should be ~1/10 screen size. */
#define LCD_VIRTUAL_BUF_SIZE (LCD_WIDTH * LCD_HEIGHT / 10)

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
