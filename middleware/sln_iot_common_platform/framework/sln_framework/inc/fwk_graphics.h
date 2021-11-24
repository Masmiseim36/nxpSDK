/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief GPU device framework declaration.
 */

#ifndef _FWK_GXF_H_
#define _FWK_GXF_H_

#include "hal_graphics_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

int gfx_manager_init();
int gfx_manager_deinit();
int gfx_dev_register(gfx_dev_t *dev);
int gfx_blit(gfx_surface_t *pSrc, gfx_surface_t *pDst, gfx_rotate_config_t *pRotate, flip_mode_t flip);
int gfx_drawRect(gfx_surface_t *pOverlay, int x, int y, int w, int h, int color);
int gfx_drawPicture(gfx_surface_t *pOverlay, int x, int y, int w, int h, int alpha, const char *pIcon);
int gfx_drawText(gfx_surface_t *pOverlay, int x, int y, int textColor, int bgColor, int type, const char *pText);
int gfx_compose(
    gfx_surface_t *pSrc, gfx_surface_t *pOverlay, gfx_surface_t *pDst, gfx_rotate_config_t *pRotate, flip_mode_t flip);

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_GXF_H_*/
