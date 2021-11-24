/*
 * Copyright 2019, 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 *
 */

#include <fonts/cambo.h>
#include <fonts/font.h>
#include <fonts/open_sans.h>
#include <fonts/raleway.h>
#include "stdint.h"

/*******************************************************************************
 * Variables
 *******************************************************************************/
static D4D_FONT_DESCRIPTOR s_d4dFontDesc;

/*******************************************************************************
 * Code
 *******************************************************************************/

static void set_d4dfnt_desc(font_t type)
{
    if (type == kFont_OpenSans8)
    {
        s_d4dFontDesc = d4dfnt_OpenSans8_desc;
    }
    else if (type == kFont_OpenSans16)
    {
        s_d4dFontDesc = d4dfnt_OpenSans16_desc;
    }
    else if (type == kFront_Cambo10)
    {
        s_d4dFontDesc = d4dfnt_Cambo10_desc;
    }
    else if (type == kFront_Cambo18)
    {
        s_d4dFontDesc = d4dfnt_Cambo18_desc;
    }
    else if (type == kFront_RaleWay10)
    {
        s_d4dFontDesc = d4dfnt_Raleway10_desc;
    }
    else if (type == kFront_RaleWay16)
    {
        s_d4dFontDesc = d4dfnt_Raleway16_desc;
    }
}

static void put_char(int x, int y, int c, int fontColor16b, int bgColor16b, unsigned short *buf, int pitch)
{
    int i, j, k, bits, width, height, offset;
    k      = 0;
    height = s_d4dFontDesc.charFullSize.height;
    width  = s_d4dFontDesc.pSizeTable[c - s_d4dFontDesc.startChar];
    offset = s_d4dFontDesc.pOffTable[c - s_d4dFontDesc.startChar];
    bits   = s_d4dFontDesc.pFontData[offset++];

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++, bits <<= 1, k++)
        {
            if (k == 8 * sizeof(char))
            {
                bits = s_d4dFontDesc.pFontData[offset++];
                k    = 0;
            }

            if (bits & 0x80)
            {
                *(buf + (y + i) * pitch + x + j) = fontColor16b;
            }
            else if (bgColor16b != -1)
            {
                *(buf + (y + i) * pitch + x + j) = bgColor16b;
            }
        }
    }
}

void put_string(int x, int y, char *s, int fontColor16b, int bgColor16b, font_t type, unsigned short *buf, int pitch)
{
    int width;
    set_d4dfnt_desc(type);

    for (; *s; s++)
    {
        put_char(x, y, *s, fontColor16b, bgColor16b, buf, pitch);
        width = s_d4dFontDesc.pSizeTable[(int)*s - s_d4dFontDesc.startChar];
        x += width;
    }
}

int get_stringwidth(char *s, font_t type)
{
    int x = 0;
    set_d4dfnt_desc(type);

    for (; *s; s++)
    {
        x += s_d4dFontDesc.pSizeTable[(int)*s - s_d4dFontDesc.startChar];
    }

    return x;
}

int get_fontheight(font_t type)
{
    set_d4dfnt_desc(type);

    return s_d4dFontDesc.charFullSize.height;
}
