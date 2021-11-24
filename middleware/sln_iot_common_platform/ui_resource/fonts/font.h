/*
 * Copyright 2019, 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 *
 */

#ifndef _VIDEO_FONT_H
#define _VIDEO_FONT_H

/*******************************************************************************
 * Definitions
 *******************************************************************************/

typedef enum _font
{
    kFont_OpenSans8  = 1, // OpenSans8 font height=15 widthMax=10
    kFont_OpenSans16 = 2, // OpenSans16 font height=28 widthMax=20
    kFront_Cambo10   = 3,
    kFront_Cambo18   = 4,
    kFront_RaleWay10 = 5,
    kFront_RaleWay16 = 6
} font_t;

/*******************************************************************************
 * API
 *******************************************************************************/
int get_stringwidth(char *s, font_t type);
int get_fontheight(font_t type);
void put_string(int x, int y, char *s, int fontColor16b, int bgColor16b, font_t type, unsigned short *buf, int pitch);

#endif /* _VIDEO_FONT_H */
