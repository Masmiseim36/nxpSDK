/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DISPLAY_SUPPORT_H_
#define _DISPLAY_SUPPORT_H_

#include "fsl_dc_fb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PANEL_RK043FN02HC 0
#define DEMO_PANEL_RK055AHD091 1
#define DEMO_PANEL_RK055IQH091 2

#ifndef DEMO_PANEL
#if defined(CPU_MIMXRT1052DVL6B) || defined(CPU_MIMXRT1062DVL6A)

#define DEMO_PANEL DEMO_PANEL_RK043FN02HC

#define APP_PXP_OUT_FORMAT kPXP_OutputPixelFormatRGB565
#define APP_CSC1_MODE kPXP_Csc1YUV2RGB
#define APP_CSC1_MODE_ENABLE false
#define APP_ROTATE_DISPLAY kPXP_Rotate180
#define APP_FLIP_DISPLAY kPXP_FlipDisable
/* Frame buffer data alignment, for better performance, the LCDIF frame buffer should be 64B align. */
#define DEMO_FRAME_BUFFER_ALIGN 64
#define EIQ_DEFAULT_CAPTURE_RATE 50

#elif defined( CPU_MIMXRT1176DVMAA_cm7 )

#define DEMO_PANEL DEMO_PANEL_RK055AHD091
//#define DEMO_PANEL DEMO_PANEL_RK055IQH091
#define APP_PXP_OUT_FORMAT kPXP_OutputPixelFormatRGB565
#define APP_CSC1_MODE kPXP_Csc1YCbCr2RGB
#define APP_CSC1_MODE_ENABLE true
#define APP_ROTATE_DISPLAY kPXP_Rotate90
#define APP_FLIP_DISPLAY kPXP_FlipBoth
/* Frame buffer data alignment, for better performance, the LCD frame buffer should be 32B align. */
#define DEMO_FRAME_BUFFER_ALIGN 32
#define EIQ_DEFAULT_CAPTURE_RATE 25
#endif
#endif // CPU_MIMXRT1176DVMAA

#if (DEMO_PANEL == DEMO_PANEL_RK055AHD091)

#define DEMO_PANEL_WIDTH (720)
#define DEMO_PANEL_HEIGHT (1280)

#elif (DEMO_PANEL == DEMO_PANEL_RK055IQH091)

#define DEMO_PANEL_WIDTH (540)
#define DEMO_PANEL_HEIGHT (960)

#elif (DEMO_PANEL == DEMO_PANEL_RK043FN02HC)

#define DEMO_PANEL_WIDTH  (480)
#define DEMO_PANEL_HEIGHT (272)
#endif

#define DEMO_BUFFER_FIXED_ADDRESS 0

#if DEMO_BUFFER_FIXED_ADDRESS
//TODO check other devices
#define DEMO_BUFFER0_ADDR 0x80000000
#define DEMO_BUFFER1_ADDR 0x80200000
#endif
/* Definitions for the frame buffer. */
#define DEMO_LCD_BUFFER_COUNT 2
#define DEMO_BUFFER_PIXEL_FORMAT   kVIDEO_PixelFormatRGB565
#define DEMO_LCD_BUFFER_BPP 2

#define DEMO_BUFFER_WIDTH DEMO_PANEL_WIDTH
#define DEMO_BUFFER_HEIGHT DEMO_PANEL_HEIGHT

#define DEMO_BUFFER_STRIDE_BYTE (DEMO_BUFFER_WIDTH * DEMO_LCD_BUFFER_BPP)
/* There is not frame buffer aligned requirement, consider the 64-bit AXI data
 * bus width and 32-byte cache line size, the frame buffer alignment is set to
 * 32 byte.
 */




/* Where the frame buffer is shown in the screen. */
#define DEMO_BUFFER_START_X 0U
#define DEMO_BUFFER_START_Y 0U

extern const dc_fb_t g_dc;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

status_t BOARD_PrepareDisplayController(void);
void BOARD_ResetDisplayMix(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _DISPLAY_SUPPORT_H_ */
