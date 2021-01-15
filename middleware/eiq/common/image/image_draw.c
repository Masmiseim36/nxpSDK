/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>

#include "image_utils.h"

void IMAGE_DrawPixel(uint16_t *pDst, uint32_t x, uint32_t y, uint32_t r,uint32_t g, uint32_t b, uint32_t lcd_w)
{
    uint16_t col = IMAGE_ConvRgb888ToRgb565(r, g, b);
    pDst[y * (lcd_w) + x] = col;
}

void IMAGE_DrawLine(uint16_t* pDst,uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t r, uint32_t g, uint32_t b, uint32_t lcd_w)
{
    uint32_t x;
    float dx, dy, y, k;
    dx = x1 - x0;
    dy = y1 - y0;
    if (dx != 0)
    {
        k = dy / dx;
        y = y0;
        for (x = x0; x < x1; x++)
        {
            IMAGE_DrawPixel(pDst, x, (uint32_t)(y + 0.5f), r, g, b, lcd_w);
            y = y + k;
        }
    }
    else
    {
        x = x0;
        for (y = y0; y < y1; y++)
        {
            IMAGE_DrawPixel(pDst, x, (uint32_t)y, r, g, b, lcd_w);
        }
    }
}

void IMAGE_DrawRect(uint16_t *pdst, uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint32_t r, uint32_t g, uint32_t b, uint32_t lcd_w)
{
    IMAGE_DrawLine(pdst, x0, y0, x0 + w, y0, r, g, b, lcd_w);
    IMAGE_DrawLine(pdst, x0, y0, x0, y0 + h, r, g, b, lcd_w);
    IMAGE_DrawLine(pdst, x0, y0 + h, x0 + w, y0 + h, r, g, b, lcd_w);
    IMAGE_DrawLine(pdst, x0 + w, y0, x0 + w, y0 + h, r, g, b, lcd_w);
}
