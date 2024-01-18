/*
 * Copyright 2020-2023 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <hal_static_image.h>
#include <string.h>
#include "hal.h"
#include "hal_utils.h"

hal_image_status_t HAL_Image_Init(static_image_t *elt, mpp_img_params_t *config, void *param)
{
    hal_image_status_t ret = MPP_kStatus_HAL_ImageSuccess;
    HAL_LOGD("++HAL_STATIC_Image_Init\n");

    elt->config.width = config->width;
    elt->config.height = config->height;
    elt->config.format = config->format;
    elt->buffer = param ;
    HAL_LOGD("--HAL_STATIC_Image_Init\n");
    return ret;
}

hal_image_status_t HAL_Image_Dequeue(const static_image_t *elt, hw_buf_desc_t *out_buf, mpp_pixel_format_t *format)
{
    hal_image_status_t ret = MPP_kStatus_HAL_ImageSuccess;
    static_image_static_config_t config = elt->config;
    int image_stride = config.width * get_bitpp(config.format) / 8;
    int dest_stride = out_buf->stride;
    HAL_LOGI("++HAL_IMAGE_Dequeue\n");

    *format = elt->config.format;

    for (int y = 0; y < config.height; y++) {
        memcpy(out_buf->addr + dest_stride*y,
               elt->buffer + image_stride*y,
               image_stride);
    }

    HAL_LOGI("--HAL_IMAGE_Dequeue\n");
    return ret;
}

const static static_image_operator_t static_image_ops = {
    .init        = HAL_Image_Init,
    .dequeue     = HAL_Image_Dequeue,
};

int setup_static_image(static_image_t *elt)
{
    elt->ops = &static_image_ops;
    return 0;
}

