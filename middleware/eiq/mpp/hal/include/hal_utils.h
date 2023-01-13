/*
 * Copyright 2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef HAL_UTILS_H_
#define HAL_UTILS_H_

#include "hal_camera_dev.h"
#include "hal_display_dev.h"
#include "hal_static_image.h"

/* display setup */
typedef int (*display_setup_func_t) (display_dev_t *);
typedef struct
{
    const char* display_name;
    display_setup_func_t display_setup_func;
} hal_display_setup_t;

/* camera setup */
typedef int (*camera_setup_func_t)(const char *, camera_dev_t *, _Bool);
typedef struct
{
    const char* camera_name;
    camera_setup_func_t camera_setup_func;
} hal_camera_setup_t;

int setup_static_image_elt(static_image_t *elt);
mpp_pixel_format_t hal_fsl_to_mpp_pixeltype(video_pixel_format_t fsl_typ);
uint32_t calc_checksum(int size_b, void *pbuf);

/* returns the number of bits per pixel per format, unknown format return 0 */
static inline int get_bitpp(mpp_pixel_format_t type)
{
    int ret;

    switch(type) {
    case MPP_PIXEL_ARGB:
    case MPP_PIXEL_YUYV:
    case MPP_PIXEL_GRAY888X:
    case MPP_PIXEL_YUV1P444:
        ret = 32;
        break;
    case MPP_PIXEL_RGB:
    case MPP_PIXEL_BGR:
    case MPP_PIXEL_GRAY888:
        ret = 24;
        break;
    case MPP_PIXEL_RGB565:
    case MPP_PIXEL_DEPTH16:
    case MPP_PIXEL_GRAY16:
    case MPP_PIXEL_UYVY1P422:
    case MPP_PIXEL_VYUY1P422:
        ret = 16;
        break;
    case MPP_PIXEL_GRAY:
    case MPP_PIXEL_DEPTH8:
        ret = 8;
        break;
    case MPP_PIXEL_YUV420P:
        ret = 12;
        break;
    case MPP_PIXEL_INVALID:
    default:
        ret = 0;
        break;
    }
    return ret;
}

#endif /* HAL_UTILS_H_ */
