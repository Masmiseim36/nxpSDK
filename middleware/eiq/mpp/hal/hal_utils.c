/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* TODO - fix pixel format definition issue */
#include "mpp_api_types.h"
#include "hal_utils.h"

static const char* hal_camera_names[] = {
    "rgb_sim",
    "ir_sim",
    "yuv_sim",
    "MipiOv5640"
};

static const char* hal_display_names[] = {
    "opencv0",
    "opencv1",
    "Lcdifv2Rk055ah"
};


typedef int (*display_setup_func_t) (display_dev_t *);
int display_sim_setup(display_dev_t *);
static display_setup_func_t display_setup_funcs[] =
{
    display_sim_setup,
    display_sim_setup,
    display_sim_setup,  /* TODO find a more elegant way to setup display */
};

int setup_display_dev(const char *name, display_dev_t *dev)
{
    int i, found = 0;
    for (i = 0; i < sizeof(hal_display_names)/sizeof(hal_display_names[0]); i++)
        if (!strcmp(name, hal_display_names[i])) {
            found = 1;
            break;
        }
    if (!found)
        return MPP_ERROR;

    display_setup_func_t disp_setup_f;
    disp_setup_f = display_setup_funcs[i];
    if (disp_setup_f)
        return disp_setup_f(dev);

    return MPP_ERROR;

}


typedef int (*camera_setup_func_t)(const char *name, camera_dev_t *, _Bool);

/* simulated-camera setup function */
int camera_sim_setup(const char *name, camera_dev_t *dev, _Bool defconfig);
/* TODO find a more elegant way to setup camera
 * allows multiple cameras, and disabling some cameras at compile time */
static camera_setup_func_t camera_setup_funcs[] =
{
    camera_sim_setup,
    camera_sim_setup,
    camera_sim_setup,
    camera_sim_setup,
};

int setup_camera_dev(const char *name, camera_dev_t *dev, _Bool defconfig)
{
    int i, found = 0;
    /* search name */
    for (i = 0; i < sizeof(hal_camera_names)/sizeof(hal_camera_names[0]); i++)
        if (!strcmp(name, hal_camera_names[i])) {
            found = 1;
            break;
        }
    if (!found)
        return MPP_ERROR;

    /* call name-specific camera setup function*/
    camera_setup_func_t cam_setup_f;
    cam_setup_f = camera_setup_funcs[i];
    if (cam_setup_f)
        return cam_setup_f(name, dev, defconfig);

    return MPP_ERROR;
}

int setup_static_image(static_image_t *);

int setup_static_image_elt(static_image_t *elt)
{
    return setup_static_image(elt);
}

mpp_pixel_format_t hal_fsl_to_mpp_pixeltype(video_pixel_format_t fsl_typ)
{
    mpp_pixel_format_t mpp_typ;
    switch(fsl_typ)
    {
    case kVIDEO_PixelFormatXYUV:
        mpp_typ = MPP_PIXEL_YUV1P444;
        break;
    case kVIDEO_PixelFormatXRGB8888:
        mpp_typ = MPP_PIXEL_ARGB;
        break;
    case kVIDEO_PixelFormatRGB565:
        mpp_typ = MPP_PIXEL_RGB565;
        break;
    case kVIDEO_PixelFormatBGR888:
        mpp_typ = MPP_PIXEL_BGR;
        break;
    case kVIDEO_PixelFormatRGB888:
        mpp_typ = MPP_PIXEL_RGB;
        break;
    default:
        mpp_typ = MPP_PIXEL_INVALID;
        break;
    }
    return mpp_typ;
}
