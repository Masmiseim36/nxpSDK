/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <FreeRTOS.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "mpp_api_types.h"
#include "hal_camera_dev.h"
#include "hal_valgo_dev.h"
#include "hal_draw.h"

#define BLACK_RGB565    (0x0000)
#define WHITE_RGB565    (0xFFFF)

int hal_label_rectangle(uint8_t *frame, int width, int height, mpp_pixel_format_t format,
                        mpp_labeled_rect_t *lr)
{
    uint32_t xsize, ysize, lw;

    xsize = lr->right - lr->left;
    ysize = lr->bottom - lr->top;

    for (lw = 0; lw < lr -> line_width; lw++) {
      hal_draw_rect((uint16_t *)frame, lr->left + lw, lr->top + lw,
                       xsize, ysize, lr->line_color.rgb.R,
                       lr->line_color.rgb.G, lr->line_color.rgb.B, width);
    }
    hal_draw_text((uint16_t *)frame, WHITE_RGB565, BLACK_RGB565, width,
                 lr->left, lr->top, (char *)lr->label);
    return 0;
}

int hal_pxp_setup(gfx_dev_t *dev);

int hal_img_convert_setup(gfx_dev_t *dev)
{
    return hal_pxp_setup(dev);
}

int hal_tflite_setup(vision_algo_dev_t *dev);
int hal_inference_tflite_setup(vision_algo_dev_t *dev)
{
    return hal_tflite_setup(dev);
}
