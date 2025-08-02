/*
 * Copyright 2022-2025 NXP.
 * All rights reserved.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <FreeRTOS.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "fsl_cache.h"

#include "hal_graphics_dev.h"
#include "hal_utils.h"

/* Graphics setup */
int HAL_GfxDev_PXP_Register(gfx_dev_t *dev);

hal_graphics_setup_t gfx_setup[] =
{
    {"gfx_PXP", HAL_GfxDev_PXP_Register},
    {"gfx_CPU", HAL_GfxDev_CPU_Register},
    {"gfx_GPU", HAL_GfxDev_GPU_Register},
};

int setup_graphic_dev(hal_graphics_setup_t gfx_setup[], int graphic_nb,
                      const char *name, gfx_dev_t *dev);
int hal_gfx_setup(const char *name, gfx_dev_t *dev)
{
    return setup_graphic_dev(gfx_setup, ARRAY_SIZE(gfx_setup), name, dev);
}

/* Display setup */
int HAL_DisplayDev_Lcdifv2Rk055_setup(display_dev_t *dev);
int HAL_DisplayDev_LVGLIMG_setup(display_dev_t *dev);

hal_display_setup_t display_setup[] =
{
    {"Lcdifv2Rk055", HAL_DisplayDev_Lcdifv2Rk055_setup},
    {"Lvgl", HAL_DisplayDev_LVGLIMG_setup},
};

int setup_display_dev(hal_display_setup_t display_setup[], int display_nb,
		      const char *name, display_dev_t *dev);
int hal_display_setup(const char *name, display_dev_t *dev)
{
    return setup_display_dev(display_setup, ARRAY_SIZE(display_setup), name, dev);
}

/* Camera setup */
int HAL_CameraDev_MipiOv5640_setup(const char *name, camera_dev_t *dev);

hal_camera_setup_t camera_setup[] =
{
    {"MipiOv5640", HAL_CameraDev_MipiOv5640_setup},
};

int setup_camera_dev(hal_camera_setup_t camera_setup[], int camera_nb,
		      const char *name, camera_dev_t *dev);
int hal_camera_setup(const char *name, camera_dev_t *dev)
{
  return setup_camera_dev(camera_setup, ARRAY_SIZE(camera_setup), name, dev);
}

void HAL_DCACHE_CleanInvalidateByRange(uint32_t addr, uint32_t size)
{
    DCACHE_CleanInvalidateByRange(addr, size);
    return;
}
