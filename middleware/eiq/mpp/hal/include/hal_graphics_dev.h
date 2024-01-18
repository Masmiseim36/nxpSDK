/*
 * Copyright 2021-2023 NXP.
 * NXP Confidential and Proprietary.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such
 * terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

/*! \addtogroup HAL_TYPES
 *  @{
 */

/**
 * @brief hal graphics (gfx) device declaration. Graphics processing devices can be used to perform conversion from one
 * image format to another, resize images, and compose images on top of one another.
 * Examples of graphics devices include the PXP (pixel pipeline) found on many i.MXRT series MCUs.
 */

#ifndef _HAL_GRAPHICS_DEV_H_
#define _HAL_GRAPHICS_DEV_H_

#include "mpp_api_types.h"
#include "hal_types.h"

/** Name of the graphic device using CPU operations **/
#define HAL_GFX_DEV_CPU_NAME "gfx_CPU"

/** Gfx surface parameters */
typedef struct _gfx_surface
{
    int height;                /*!< buffer height */
    int width;                 /*!< buffer width */
    int pitch;                 /*!< buffer pitch */
    int left;                  /*!< left position */
    int top;                   /*!< top position */
    int right;                 /*!< right position */
    int bottom;                /*!< bottom position */
    int swapByte;              /*!< swap byte per two bytes */
    mpp_pixel_format_t format; /*!< pixel format */
    void *buf;                 /*!< buffer */
    void *lock;                /*!< the structure is determined by hal and set to null if not use in hal*/
} gfx_surface_t;

/** gfx rotate target*/
typedef enum _gfx_rotate_target
{
    kGFXRotateTarget_None = 0,
    kGFXRotate_SRCSurface,
    kGFXRotate_DSTSurface,
} gfx_rotate_target_t;

/** gfx rotate configuration*/
typedef struct _gfx_rotate_config
{
    gfx_rotate_target_t target;
    mpp_rotate_degree_t degree;
} gfx_rotate_config_t;

typedef struct _gfx_dev gfx_dev_t;

/** @} */

/*! \addtogroup HAL_OPERATIONS
*  @{
*/

/** @brief Operation that needs to be implemented by gfx device */

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
        const gfx_dev_t *dev, const gfx_surface_t *pSrc, const gfx_surface_t *pDst, const gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip);
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


/*! @brief The mpp callback function prototype */
typedef int (*mpp_callback_t)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);

/** @} */

/*! \addtogroup HAL_TYPES
 *  @{
 */

struct _gfx_dev
{
    /* unique id */
    int id;
    /* operations */
    const gfx_dev_operator_t *ops;
    /* graphic source surface */
    gfx_surface_t src;
    /* graphic destination surface */
    gfx_surface_t dst;
    /* callback */
    mpp_callback_t callback;
    /* param for the callback */
    void *user_data;
};

/*!
 * @brief Register the graphic device with the CPU operations
 *
 * @param[in] dev graphic device to register
 * @return error code (0: success, otherwise: failure)
 *
 */
int HAL_GfxDev_CPU_Register(gfx_dev_t *dev);

/** @} */

#endif /* _HAL_GRAPHICS_DEV_H_ */
