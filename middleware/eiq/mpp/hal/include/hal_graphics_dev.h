/*
 * Copyright 2021-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal graphics (gfx) device declaration. Graphics processing devices can be used to perform conversion from one
 * image format to another, resize images, and compose images on top of one another.
 * Examples of graphics devices include the PXP (pixel pipeline) found on many i.MXRT series MCUs.
 */

#ifndef _HAL_GRAPHICS_DEV_H_
#define _HAL_GRAPHICS_DEV_H_

#include "mpp_api_types.h"
#include "hal_types.h"

/**
 * @brief declare the graphic dev ##name
 */
#define HAL_GFX_DEV_DECLARE(name) int HAL_GfxDev_##name##_Register();

/**
 * @brief register the graphic dev ##name
 */
#define HAL_GFX_DEV_REGISTER(name, ret)                             \
    {                                                               \
        ret = HAL_GfxDev_##name##_Register();                       \
        if (ret != 0)                                               \
        {                                                           \
            HAL_LOGE("HAL_GfxDev_%s_Register error %d\n", "##name", ret); \
            return ret;                                             \
        }                                                           \
    }

typedef struct _gfx_surface
{
    int height;
    int width;
    int pitch;
    /* target rect */
    int left;
    int top;
    int right;
    int bottom;
    int swapByte;
    mpp_pixel_format_t format;
    void *buf;
    void *lock; /*the structure is determined by hal and set to null if not use in hal*/
} gfx_surface_t;

typedef enum _gfx_rotate_target
{
    kGFXRotateTarget_None = 0,
    kGFXRotate_SRCSurface,
    kGFXRotate_DSTSurface,
} gfx_rotate_target_t;

typedef struct _gfx_rotate_config
{
    gfx_rotate_target_t target;
    mpp_rotate_degree_t degree;
} gfx_rotate_config_t;

typedef struct _gfx_dev gfx_dev_t;

typedef struct
{
    /* initialize the dev */
    int (*init)(const gfx_dev_t *dev, void *param);
    /* deinitialize the dev */
    int (*deinit)(const gfx_dev_t *dev);
    /* get buffer descriptors and policy */
    int (*get_buf_desc)(const gfx_dev_t *dev,
            hw_buf_desc_t *in_buf, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy);
    /* blit the source surface to the destination surface */
    int (*blit)(
        const gfx_dev_t *dev, gfx_surface_t *pSrc, gfx_surface_t *pDst, gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip);
    /* draw rect in overlay surface */
    int (*drawRect)(const gfx_dev_t *dev, gfx_surface_t *pOverlay, int x, int y, int w, int h, int color);
    /* draw picture in overlay surface */
    int (*drawPicture)(
        const gfx_dev_t *dev, gfx_surface_t *pOverlay, int x, int y, int w, int h, int alpha, const char *pIcon);
    /* draw Text in overlay surface */
    int (*drawText)(const gfx_dev_t *dev,
                    gfx_surface_t *pOverlay,
                    int x,
                    int y,
                    int textColor,
                    int bgColor,
                    int type,
                    const char *pText);
    /* blit the source surface with overlay surface to the destination surface */
    int (*compose)(const gfx_dev_t *dev,
                   gfx_surface_t *pSrc,
                   gfx_surface_t *pOverlay,
                   gfx_surface_t *pDst,
                   gfx_rotate_config_t *pRotate,
                   mpp_flip_mode_t flip);
} gfx_dev_operator_t;

struct _gfx_dev
{
    /* unique id */
    int id;
    /* operations */
    const gfx_dev_operator_t *ops;
};

#endif /* _HAL_GRAPHICS_DEV_H_ */
