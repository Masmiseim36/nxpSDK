/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H

/*
 * This is the evkbimxrt1050 board configuration for:
 * - the HAL
 * - the application parameters
 */

/*******************************************************************************
 * HAL configuration
 ******************************************************************************/
#define HAL_ENABLE_CAMERA_DEV_CsiMt9m114 1
#define HAL_ENABLE_DISPLAY_DEV_LcdifRk043fn 1
#define HAL_ENABLE_GFX_DEV_Pxp 1

/* Cache is optionally used by DeepViewRT to optimize certain internal loops.
 * If  HAL_DVRT_USE_CACHE is set to 0 then it will be allocated on the heap using the provided HAL_DVRT_CACHE_SIZE_KB.
 * If  HAL_DVRT_USE_CACHE is set to 1 it will be placed in the fastest available memory defined by HAL_DVRT_CACHE_MEMORY
 * for maximum performance. */
#define HAL_DVRT_USE_CACHE     1
#define HAL_DVRT_CACHE_SIZE_KB 128
#define BOARD_SRAM_DTC         0x20000000       /* Board SRAM_DTC address */
#define HAL_DVRT_CACHE_MEMORY  BOARD_SRAM_DTC  /* DVRT cache will be allocated in the SRAM_DTC */

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
#define APP_CAMERA_NAME "CsiMt9m114"
#define APP_CAMERA_WIDTH  480
#define APP_CAMERA_HEIGHT 272
#define APP_CAMERA_FORMAT MPP_PIXEL_RGB565

/* display params */
#define APP_DISPLAY_NAME "LcdifRk043fn"
#define APP_DISPLAY_WIDTH  480
#define APP_DISPLAY_HEIGHT 272
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

/* other params */
#define APP_CAMERA_DISPLAY_ROTATE ROTATE_0

#endif /* _BOARD_CONFIG_H */
