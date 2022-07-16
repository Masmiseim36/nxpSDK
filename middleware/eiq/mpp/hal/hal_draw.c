/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * this file is copied from sdk 2.11.0 folder:
 *  - middleware/eiq/common/gprintf/chgui.c
 * and modified to fit mpp needs
 */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "font.h"
#include "hal_draw.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Draws in X,Y configuration. */

#define INCREMENT(X, Y) ((X) + (Y))
#define DECREMENT(X, Y) ((X) - (Y))

#define GET_MSB(a, bits, ofs)  ((((a) >> (8 - (bits))) & ((1 << (bits)) - 1)) << (ofs))

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
static void GUI_DispChar(uint16_t *lcd_buf, char c, int x, int y, uint16_t bcolor, uint16_t fcolor, uint32_t width);

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
void hal_draw_text(uint16_t *lcd_buf, uint16_t fcolor, uint16_t bcolor, uint32_t width,
                   int x, int y, const char *label)
{
    int idx = 0;

    while ((label[idx] != 0) && (idx < GUI_PRINTF_BUF_SIZE)) {
        GUI_DispChar(lcd_buf, label[idx],
                     INCREMENT(x, idx * _gFontTbl[0].x_size), y,
                     fcolor, bcolor, width);
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
static void _GUI_DispChar(uint16_t *lcd_buf, char c, int x, int y, const char *pdata,
        int font_xsize, int font_ysize, uint16_t fcolor, uint16_t bcolor, uint16_t width)
{
    uint8_t j, pos, t;
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

    for (j = 0; j < XNum; j++)
    {
        for (pos = 0; pos < font_ysize; pos++)
        {
            temp = (uint8_t) pdata[base + pos + j * font_ysize];
            for (t = 0; t < 8; t++)
            {
                if ((temp >> t) & 0x01) {
                    hal_draw_pixel(lcd_buf, DECREMENT(x + font_xsize, t), INCREMENT(y, pos), fcolor, width);
                } else {
                    hal_draw_pixel(lcd_buf, DECREMENT(x + font_xsize, t), INCREMENT(y, pos), bcolor, width);
                }
            }
        }
        x = INCREMENT(x, 8);
    }
}

static void GUI_DispChar(uint16_t *lcd_buf, char c, int x, int y, uint16_t fcolor, uint16_t bcolor, uint32_t width)
{
    _GUI_DispChar(lcd_buf, c, x, y, _gFontTbl[0].data, (int)_gFontTbl[0].x_size,
            (int)_gFontTbl[0].y_size, fcolor, bcolor, width);
}

/*!
 * @brief Converts image from RGB888 to RGB565.
 *
 * @param r 0-255 red color value
 * @param g 0-255 green color value
 * @param b 0-255 blue color value
 * @return color in RGB565
 */
static uint16_t ConvRgb888Rgb565(uint32_t r, uint32_t g, uint32_t b)
{
    b = GET_MSB(b, 5, 0);
    g = GET_MSB(g, 6, 5);
    r = GET_MSB(r, 5, 11);

    return (uint16_t)(r | g | b);
}

void hal_draw_rect(uint16_t *lcd_buf, uint32_t x, uint32_t y, uint32_t xsize, uint32_t ysize,
                  uint32_t r, uint32_t g, uint32_t b, uint32_t width)
{
    uint16_t color16 = ConvRgb888Rgb565(r,g,b);

    /* verticals */
    for (int i = x; i < x + xsize; i++) {
        hal_draw_pixel(lcd_buf, i, y, color16, width);
        hal_draw_pixel(lcd_buf, i, y + ysize, color16, width);
    }
    /* horizontals */
    for (int i = y; i < y + ysize; i++) {
        hal_draw_pixel(lcd_buf, x, i, color16, width);
        hal_draw_pixel(lcd_buf, x + xsize, i, color16, width);
    }
}

void hal_draw_pixel(uint16_t *pDst, uint32_t x, uint32_t y, uint16_t color, uint32_t lcd_w)
{
    pDst[y * (lcd_w) + x] = color;
}
