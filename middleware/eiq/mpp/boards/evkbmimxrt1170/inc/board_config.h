/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

/*
 * This is the evkmimxrt1170 board configuration for:
 * - the HAL
 * - the application parameters
 */

/*******************************************************************************
 * HAL configuration
 ******************************************************************************/
#define HAL_ENABLE_CAMERA_DEV_MipiOv5640 1
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055ah 1
#define HAL_ENABLE_GFX_DEV_Pxp 1

/* Cache is optionally used by DeepViewRT to optimize certain internal loops.
 * If  HAL_DVRT_USE_CACHE is set to 0 then it will be allocated on the heap using the provided HAL_DVRT_CACHE_SIZE_KB.
 * If  HAL_DVRT_USE_CACHE is set to 1 it will be placed in the fastest available memory defined by HAL_DVRT_CACHE_MEMORY
 * for maximum performance. */
#define HAL_DVRT_USE_CACHE     1
#define HAL_DVRT_CACHE_SIZE_KB 256
#define BOARD_SRAM_DTC_CM7     0x20000000           /* Board SRAM_DTC address */
#define HAL_DVRT_CACHE_MEMORY  BOARD_SRAM_DTC_CM7  /* DVRT cache will be allocated in the SRAM_DTC_CM7 */

/* Workaround for the PXP bug where BGR888 is output instead of RGB888 [MPP-97] */
#define PXP_WORKAROUND_OUT_RGB 1

/*******************************************************************************
 * Application parameters
 ******************************************************************************/
/*
 * This section provides a template for the application parameters.
 * These application parameters can be configured to other supported values
*/
/* camera params */
#define APP_CAMERA_NAME "MipiOv5640"
#define APP_CAMERA_WIDTH  1280
#define APP_CAMERA_HEIGHT 720
#define APP_CAMERA_FORMAT MPP_PIXEL_YUV1P444

/* display params */
#define APP_DISPLAY_NAME "Lcdifv2Rk055ah"
#define APP_DISPLAY_WIDTH  720
#define APP_DISPLAY_HEIGHT 1280
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

/* other params */
/* rotation is needed because camera is landscape, display RK055 is portrait */
#define APP_CAMERA_DISPLAY_ROTATE ROTATE_90

#endif /* _BOARD_CONFIG_H */
