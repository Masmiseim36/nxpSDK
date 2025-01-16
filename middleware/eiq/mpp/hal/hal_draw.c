/*
 * Copyright 2019-2024 NXP
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
 * this file is copied from sdk 2.11.0 folder:
 *  - middleware/eiq/common/gprintf/chgui.c
 * and modified to fit mpp needs
 */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "fsl_common.h"
#include "font.h"
#include "hal_draw.h"
#include "hal_debug.h"
#include "mpp_api_types.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Draws in X,Y configuration. */

#define INCREMENT(X, Y) ((X) + (Y))
#define DECREMENT(X, Y) ((X) - (Y))

#define GET_MSB(a, bits, ofs)  ((((a) >> (8 - (bits))) & ((1 << (bits)) - 1)) << (ofs))

#define BLACK_RGB565    (0x0000)
#define WHITE_RGB565    (0xFFFF)

/*!
 * @brief GUI font configuration structure
 */
typedef struct {
    char *name;
    uint8_t  x_size;
    uint8_t  y_size;
    const char *data;
} chgui_font_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Draws character to point in LCD buffer.
 *
 * @param c input character for drawing
 * @param x drawing position on X axe
 * @param y drawing position on Y axe
 */
static void GUI_DispChar(uint16_t *lcd_buf, char c, int x, int y, uint16_t bcolor, uint16_t fcolor,
        uint32_t width, int stripe_top, int stripe_bottom);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Allocated GUI font configuration structure. */
static chgui_font_t _gFontTbl[] =
{{ "Font", FONT_XSize, FONT_YSize, consolas_12ptBitmaps }, };


/*!
 * @brief Draws text stored in local text buffer to LCD buffer.
 *
 * This function copy content of data from local text buffer
 * to the LCD. This function should be called after
 * GUI_PrintfToBuffer.
 *
 * @param lcd_buf LCD buffer address destination for drawing text

 */
void hal_draw_text565(uint16_t *lcd_buf, uint16_t fcolor, uint16_t bcolor, uint32_t width,
                   int x, int y, const char *label, int stripe_top, int stripe_bottom)
{
    int idx = 0;

    while ((label[idx] != 0) && (idx < GUI_PRINTF_BUF_SIZE)) {
        GUI_DispChar(lcd_buf, label[idx],
                     INCREMENT(x, idx * _gFontTbl[0].x_size), y,
                     fcolor, bcolor, width, stripe_top, stripe_bottom);
        idx++;
    }
}

/*!
 * @brief Draw character to point in LCD buffer.
 *
 * @param c input character for drawing.
 * @param x drawing position on X axe.
 * @param y drawing position on Y axe.
 * @param pdata address to character bitmaps for console.
 * @param font_xsize size of font in X axe.
 * @param font_ysize size of font in Y axe.
 * @param fcolor foreground RGB565 color.
 * @param bcolor background RGB565 color.
 */
static inline void _GUI_DispChar(uint16_t *lcd_buf, char c, int x, int y, const char *pdata,
        int font_xsize, int font_ysize, uint16_t fcolor, uint16_t bcolor, uint16_t width, int stripe_top, int stripe_bottom)
{
    uint8_t j, t;
    int pos;
    uint8_t temp;
    uint8_t XNum;
    uint32_t base;
    XNum = (font_xsize / 8) + 1;
    if (font_ysize % 8 == 0)
    {
        XNum--;
    }
    if (c < ' ')
    {
        return;
    }
    c = c - ' ';
    base = (c * XNum * font_ysize);

    int ystart = MAX(stripe_top, y);
    int yend = MIN(stripe_bottom, y + font_ysize - 1);

    if ((ystart <= stripe_bottom) && (yend >= stripe_top))
    {   /* part of font is in stripe */
        for (j = 0; j < XNum; j++)
        {
            for (pos = ystart; pos < yend; pos++)
            {
                temp = (uint8_t) pdata[base + (pos - y) + j * font_ysize];
                for (t = 0; t < font_xsize; t++)
                {
                    if ((temp >> t) & 0x01) {
                        hal_draw_pixel565(lcd_buf, (x + font_xsize - t), (pos - stripe_top), fcolor, width);
                    } else {
                        hal_draw_pixel565(lcd_buf, (x + font_xsize - t), (pos - stripe_top), bcolor, width);
                    }
                }
            }
            x = x + 8;
        }
    }
}

static inline void GUI_DispChar(uint16_t *lcd_buf, char c, int x, int y, uint16_t fcolor, uint16_t bcolor,
        uint32_t width, int stripe_top, int stripe_bottom)
{
    _GUI_DispChar(lcd_buf, c, x, y, _gFontTbl[0].data, (int)_gFontTbl[0].x_size,
            (int)_gFontTbl[0].y_size, fcolor, bcolor, width, stripe_top, stripe_bottom);
}

/*!
 * @brief Converts image from RGB888 to RGB565.
 *
 * @param r 0-255 red color value
 * @param g 0-255 green color value
 * @param b 0-255 blue color value
 * @return color in RGB565
 */
static inline uint16_t ConvRgb888Rgb565(mpp_color_t col)
{
    uint16_t b = GET_MSB(col.rgb.B, 5, 0);
    uint16_t g = GET_MSB(col.rgb.G, 6, 5);
    uint16_t r = GET_MSB(col.rgb.B, 5, 11);

    return (uint16_t)(r | g | b);
}

static inline void hal_draw_rect565(uint16_t *lcd_buf, hal_rect_t rect,
                  mpp_color_t rgb, uint32_t width,
                  int stripe_top, int stripe_bottom)
{
    uint16_t color16 = ConvRgb888Rgb565(rgb);

    /* horizontal top bar */
    if ((rect.top >= stripe_top) && (rect.top <= stripe_bottom))
    {
        for (int i = rect.left; i < rect.right; i++) {
            hal_draw_pixel565(lcd_buf, i, rect.top - stripe_top, color16, width);
        }
    }
    /* horizontal bottom bar */
    if ((rect.bottom >= stripe_top) && (rect.bottom <= stripe_bottom))
    {
        for (int i = rect.left; i < rect.right; i++)
        {
            hal_draw_pixel565(lcd_buf, i, rect.bottom - stripe_top, color16, width);
        }
    }

    /* verticals */
    int ystart = MAX(stripe_top, rect.top);
    int yend = MIN(stripe_bottom, rect.bottom);
    if ((ystart <= stripe_bottom) && (yend >= stripe_top))
    {
        for (int i = ystart; i <= yend; i++)
        {
            hal_draw_pixel565(lcd_buf, rect.left, i - stripe_top, color16, width);
            hal_draw_pixel565(lcd_buf, rect.right, i - stripe_top, color16, width);
        }
    }
}

static inline void hal_draw_pixel565(uint16_t *pDst, uint32_t x, uint32_t y, uint16_t color, uint32_t lcd_w)
{
    pDst[y * (lcd_w) + x] = color;
}

int hal_label_rectangle(uint8_t *frame, int width, int height, mpp_pixel_format_t format,
                        mpp_labeled_rect_t *lr, int stripe, int stripe_max)
{
    uint32_t xsize, ysize, lw;
    hal_rect_t rect;
    int stripe_top, stripe_bottom;

    /* for now only support RGB565 format */
    if (format != MPP_PIXEL_RGB565) return MPP_INVALID_PARAM;

    xsize = lr->right - lr->left;
    ysize = lr->bottom - lr->top;

    /* check rectangle fits in frame */
    if (    (lr->left < 0) || (lr->top < 0)
            || (lr->right > width) || (lr->bottom > height)
            || (lr->left > lr->right) || (lr->top > lr->bottom)
    ) return MPP_INVALID_PARAM;

    /* check rectangle is large enough to draw lines inside */
    /* if not, skip the drawing to avoid crash */
    if (   ( xsize < (2 * lr->line_width) )
        || ( ysize < (2 * lr->line_width) ) )
    {
        HAL_LOGD("rectangle %dx%d too small to draw %d line width\n", xsize, ysize, lr->line_width);
        return MPP_SUCCESS;
    }

    if (stripe)
    {
        int stripe_h = height / stripe_max;
        stripe_top = (stripe - 1) * stripe_h;
        stripe_bottom = stripe_top + stripe_h - 1;
    }
    else
    {
        stripe_top = 0;
        stripe_bottom = height - 1;
    }

    for (lw = 0; lw < lr->line_width; lw++) {
        rect.top = lr->top + lw;
        rect.left = lr->left + lw;
        rect.right = lr->right - lw;
        rect.bottom = lr->bottom - lw;
        hal_draw_rect565((uint16_t *)frame, rect, lr->line_color, width, stripe_top, stripe_bottom);
    }

    /* check label fits in frame */
    int strxsize = strlen((const char *)lr->label) * FONT_XSize;
    if (    (lr->left + strxsize > width)
            || (lr->top + FONT_YSize > height)
    )   return MPP_INVALID_PARAM;

    hal_draw_text565((uint16_t *)frame, WHITE_RGB565, BLACK_RGB565, width,
                 lr->left, lr->top, (char *)lr->label, stripe_top, stripe_bottom);
    return MPP_SUCCESS;
}
