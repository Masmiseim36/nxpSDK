/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief GPU manager framework implementation.
 */

#include "fwk_sln_platform.h"

#include "fwk_log.h"
#include "fwk_message.h"

#include "fwk_graphics.h"

static gfx_dev_t *gGfxDev = NULL;

int gfx_manager_init()
{
    return 0;
}

int gfx_manager_deinit()
{
    return 0;
}

/*
 * @brief blit the source surface to the destination surface.
 * @param *pSrc [in]  Pointer to source surface.
 * @param *pDst [out]    Pointer to destination surface.
 * @param *pRotate [in]    Pointer to the rotation config.
 * @param flip [in]    Flip mode.
 * @returns 0 for the success.
 */
int gfx_blit(gfx_surface_t *pSrc, gfx_surface_t *pDst, gfx_rotate_config_t *pRotate, flip_mode_t flip)
{
    int ret = -1;

    if (gGfxDev != NULL && gGfxDev->ops->blit != NULL)
    {
        ret = gGfxDev->ops->blit(gGfxDev, pSrc, pDst, pRotate, flip);
    }

    return ret;
}

int gfx_drawRect(gfx_surface_t *pOverlay, int x, int y, int w, int h, int color)
{
    int ret = -1;

    if (gGfxDev != NULL && gGfxDev->ops->drawRect != NULL)
    {
        ret = gGfxDev->ops->drawRect(gGfxDev, pOverlay, x, y, w, h, color);
    }

    return ret;
}

int gfx_drawPicture(gfx_surface_t *pOverlay, int x, int y, int w, int h, int alpha, const char *pIcon)
{
    int ret = -1;

    if (gGfxDev != NULL && gGfxDev->ops->drawPicture != NULL)
    {
        ret = gGfxDev->ops->drawPicture(gGfxDev, pOverlay, x, y, w, h, alpha, pIcon);
    }

    return ret;
}

int gfx_drawText(gfx_surface_t *pOverlay, int x, int y, int text_color, int bg_color, int type, const char *pText)
{
    int ret = -1;

    if (gGfxDev != NULL && gGfxDev->ops->drawText != NULL)
    {
        ret = gGfxDev->ops->drawText(gGfxDev, pOverlay, x, y, text_color, bg_color, type, pText);
    }

    return ret;
}

int gfx_compose(
    gfx_surface_t *pSrc, gfx_surface_t *pOverlay, gfx_surface_t *pDst, gfx_rotate_config_t *pRotate, flip_mode_t flip)
{
    int ret = -1;

    if (gGfxDev != NULL && gGfxDev->ops->compose != NULL)
    {
        ret = gGfxDev->ops->compose(gGfxDev, pSrc, pOverlay, pDst, pRotate, flip);
    }

    return ret;
}

int gfx_dev_register(gfx_dev_t *dev)
{
    gGfxDev = dev;
    return 0;
}
