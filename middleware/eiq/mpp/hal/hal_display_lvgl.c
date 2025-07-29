/*
 * Copyright 2024-2025 NXP.
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

/*
 * @brief LVGL display HAL implementation.
 */

#include "mpp_config.h"
#include "mpp_api_types.h"
#include "hal_debug.h"
#include "hal_display_dev.h"
#include "hal_utils.h"

#if (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_Lvgl == 1)
#include "fsl_common.h"
#include "display_support.h"
#include "lvgl.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#define DISPLAY_NAME "Lvgl"
#define DISPLAY_DEV_LVGLIMG_HEIGHT 1280
#define DISPLAY_DEV_LVGLIMG_WIDTH 720
#define DISPLAY_DEV_LVGLIMG_LEFT 0
#define DISPLAY_DEV_LVGLIMG_TOP 0
#define DISPLAY_DEV_LVGLIMG_RIGHT 1279
#define DISPLAY_DEV_LVGLIMG_BOTTOM 719
#define DISPLAY_DEV_LVGLIMG_ROTATE 0
#define DISPLAY_DEV_LVGLIMG_FORMAT MPP_PIXEL_RGB565
#define DISPLAY_DEV_LVGLIMG_BPP 2  /* Bytes per pixel, 2 for RGB565 */

/**** Local functions declaration section ****/
hal_display_status_t HAL_DisplayDev_LVGLIMG_Init(display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *params);
hal_display_status_t HAL_DisplayDev_LVGLIMG_Deinit(const display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_LVGLIMG_Start(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_LVGLIMG_Blit(const display_dev_t *dev, void *frame, int stripe);
hal_display_status_t HAL_DisplayDev_LVGLIMG_Getbufdesc(const display_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy);

/**** Static variables declaration section ****/

/* lvgl "image" widget descriptor */
static lv_img_dsc_t s_frame_image = 
{
        .header.magic       = LV_IMAGE_HEADER_MAGIC,
        .header.w           = DISPLAY_DEV_LVGLIMG_WIDTH,
        .header.h           = DISPLAY_DEV_LVGLIMG_HEIGHT,
        .data_size          = DISPLAY_DEV_LVGLIMG_WIDTH * DISPLAY_DEV_LVGLIMG_HEIGHT * LV_COLOR_DEPTH / 8,
        .header.cf          = LV_COLOR_FORMAT_NATIVE,          /*Set the color format*/
        .data               = NULL,
};

const static display_dev_operator_t s_DisplayDev_LVGLIMGOps =
{
        .init        = HAL_DisplayDev_LVGLIMG_Init,
        .deinit      = HAL_DisplayDev_LVGLIMG_Deinit,
        .start       = HAL_DisplayDev_LVGLIMG_Start,
        .blit        = HAL_DisplayDev_LVGLIMG_Blit,
        .get_buf_desc = HAL_DisplayDev_LVGLIMG_Getbufdesc,
};

static display_dev_t s_DisplayDev_LVGLIMG =
{
        .id   = 0,
        .name = DISPLAY_NAME,
        .ops  = &s_DisplayDev_LVGLIMGOps,
        .cap  = 
        {
                .width       = DISPLAY_DEV_LVGLIMG_WIDTH,
                .height      = DISPLAY_DEV_LVGLIMG_HEIGHT,
                .pitch       = DISPLAY_DEV_LVGLIMG_WIDTH * DISPLAY_DEV_LVGLIMG_BPP,
                .left        = DISPLAY_DEV_LVGLIMG_LEFT,
                .top         = DISPLAY_DEV_LVGLIMG_TOP,
                .right       = DISPLAY_DEV_LVGLIMG_RIGHT,
                .bottom      = DISPLAY_DEV_LVGLIMG_BOTTOM,
                .rotate      = DISPLAY_DEV_LVGLIMG_ROTATE,
                .format      = DISPLAY_DEV_LVGLIMG_FORMAT,
                .frameBuffers = NULL,
                .callback    = NULL,
                .handle      = NULL
        }
};

/* There must be only one lvgl display! */
static display_dev_private_capability_t *s_display_cap = NULL;

/* LVGL refresh event callback */
void lv_event_refr_ready(lv_event_t * e)
{
    if (lv_obj_is_valid(s_display_cap->handle))
    {
        /* update image pointer, invalidate image cache */
        lv_img_set_src(s_display_cap->handle, &s_frame_image);
        lv_image_cache_drop(lv_image_get_src(s_display_cap->handle));
        lv_obj_invalidate(s_display_cap->handle);
    }
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Init(display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *params)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_LVGLIMG_Init\n");
    s_display_cap = &dev->cap;
    /* set default config */
    memcpy(s_display_cap, &s_DisplayDev_LVGLIMG.cap, sizeof(display_dev_private_capability_t));
    /* copy mpp_display_params_t config into s_display_cap and lvgl image widget */
    s_frame_image.header.w = s_display_cap->width = config->width;
    s_frame_image.header.h = s_display_cap->height = config->height;
    s_frame_image.data_size = s_frame_image.header.w * s_frame_image.header.h * LV_COLOR_DEPTH / 8;
    
    if ( ((LV_COLOR_DEPTH == 16) && (config->format != MPP_PIXEL_RGB565))
         || ((LV_COLOR_DEPTH == 32) && (config->format != MPP_PIXEL_ARGB)) )
    {
        HAL_LOGE("Color format not matching LVGL color depth\n");
        return kStatus_HAL_DisplayError;
    }
    s_display_cap->handle = config->handle;
    HAL_LOGD("--HAL_DisplayDev_LVGLIMG_Init\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Start(display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_LVGLIMG_Start\n");

    HAL_LOGD("--HAL_DisplayDev_LVGLIMG_Start\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Blit(const display_dev_t *dev, void *frame, int stripe)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_LVGLIMG_Blit\n");

    if (frame == NULL) {
        HAL_LOGE("Invalid frame buffer\n");
        return kStatus_HAL_DisplayError;
    }
    /* Update the data pointer in image description,
	update of image widget is done async in callback lv_event_refr_ready() */
    s_frame_image.data = (const uint8_t *)frame;

    HAL_LOGD("LVGL handler buffer addr=0x%x\n", (unsigned int)dev->cap.handle);
    HAL_LOGD("--HAL_DisplayDev_LVGLIMG_Blit\n");

    return ret;
}

hal_display_status_t HAL_DisplayDev_LVGLIMG_Getbufdesc(const display_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    s_display_cap = (display_dev_private_capability_t *) &dev->cap;
    HAL_LOGD("++HAL_DisplayDev_LVGLIMG_Getbufdesc");

    do
    {
        if ((in_buf == NULL) || (policy == NULL))
        {
            HAL_LOGD("NULL pointer to buffer descriptor\n");
            ret = kStatus_HAL_DisplayError;
            break;
        }
        /* Set memory policy */
        *policy = HAL_MEM_ALLOC_NONE;
        in_buf->alignment = FRAME_BUFFER_ALIGN;
        in_buf->nb_lines = s_display_cap->height;  /* number of lines in image widget */
        in_buf->cacheable = true;
        in_buf->stride = s_display_cap->width * LV_COLOR_DEPTH / 8;
        in_buf->addr = NULL;
    } while (false);

    HAL_LOGD("--HAL_DisplayDev_LVGLIMG_Getbufdesc");
    return ret;
}

int HAL_DisplayDev_LVGLIMG_setup(display_dev_t *dev)
{
    dev->ops = &s_DisplayDev_LVGLIMGOps;
    return 0;
}

#else /* (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_Lvgl == 1) */
int HAL_DisplayDev_LVGLIMG_setup(display_dev_t *dev)
{
    HAL_LOGE("LVGL window not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_Lvgl == 1) */
