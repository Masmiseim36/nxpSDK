/**************************************************************************
 * Copyright 2014 by Petr Gargulak. eGUI Community.
 * Copyright 2009-2013 by Petr Gargulak. Freescale Semiconductor, Inc.
 *****************************************************************************
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 ****************************************************************************/

#ifndef D4D_FONT_H_
#define D4D_FONT_H_

#include "stdint.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/

#define D4D_FONT_PACK_BITORDER           0x01
#define D4D_FONT_PACK_BITORDER_BIGEND    0
#define D4D_FONT_PACK_BITORDER_LITTLEEND 0x01

#define D4D_FONT_PACK_SCANBASED        0x02
#define D4D_FONT_PACK_SCANBASED_ROW    0
#define D4D_FONT_PACK_SCANBASED_COLUMN 0x02

#define D4D_FONT_PACK_SCANPREFERRED        0x04
#define D4D_FONT_PACK_SCANPREFERRED_ROW    0
#define D4D_FONT_PACK_SCANPREFERRED_COLUMN 0x04

#define D4D_FONT_PACK_COMPRESSED     0x08
#define D4D_FONT_PACK_COMPRESSED_OFF 0
#define D4D_FONT_PACK_COMPRESSED_ON  0x08

#define D4D_FONT_PACK_DATA_LEN     0x030
#define D4D_FONT_PACK_DATA_LEN_8B  0x00
#define D4D_FONT_PACK_DATA_LEN_16B 0x10
#define D4D_FONT_PACK_DATA_LEN_32B 0x20

#define D4D_FONT_FLAGS_IX_STYLE           0x01
#define D4D_FONT_FLAGS_IX_STYLE_LINEAR    0x00
#define D4D_FONT_FLAGS_IX_STYLE_NONLINEAR 0x01

#define D4D_FONT_FLAGS_IX_TYPE        0x02
#define D4D_FONT_FLAGS_IX_TYPE_LOOKUP 0x00
#define D4D_FONT_FLAGS_IX_TYPE_MAP    0x02

#define D4D_FONT_FLAGS_FNT_WIDTH              0x04
#define D4D_FONT_FLAGS_FNT_WIDTH_MONOSPACE    0x00
#define D4D_FONT_FLAGS_FNT_WIDTH_PROPORTIONAL 0x04

typedef uint8_t D4D_FONT_PACK;
typedef uint8_t D4D_FONT_FLAGS;

typedef uint8_t D4D_FONT_IX;
typedef uint8_t D4D_FONT_REV;

typedef uint32_t D4D_FONT_OFFSET;
typedef uint8_t D4D_FONT_DATA;
typedef uint8_t D4D_FONT;

typedef uint8_t D4D_FONT_SIZE;
typedef uint32_t D4D_FONT_DSIZE;

typedef struct {
    D4D_FONT_SIZE width;
    D4D_FONT_SIZE height;
} D4D_FONT_SIZES;

typedef struct D4D_FONT_DESCRIPTOR_S {
    D4D_FONT_REV revision;       // 1 Font descriptor version number
    D4D_FONT_FLAGS flags;        // 1 linear / non linear , proporcional or not,
    D4D_FONT_IX startChar;       // 2/1 start char of used table
    D4D_FONT_IX charCnt;         // 2/1 end char of used table
    D4D_FONT_IX charDefault;     // 2/1 index of char that will be printed instead of
    D4D_FONT_SIZE charSize;      // 1 Font size (size of font loaded in PC)
    D4D_FONT_DSIZE charBmpSize;  // 2 Size of font bitmpap for non proportional fonts
    D4D_FONT_SIZE charBaseLine;  // 1 offset from Y0 coordinate to baseline
    D4D_FONT_SIZES charFullSize; // 2 size of biggest char x/y
    D4D_FONT_PACK pack;          // 1 packing condition of individual bitmaps 15
    const D4D_FONT_IX
    * pIxTable; // Index table - is used when nonlinearIX is set in flags, flags also determine the type of IxTable
    const D4D_FONT_OFFSET* pOffTable;          // Offset table - used when proporcial font is set in flags
    const D4D_FONT_SIZE* pSizeTable;           // Size table - used when proporcial font is set in flags
    const D4D_FONT_DATA* pFontData;            // bitmap/font data array
    const struct D4D_FONT_DESCRIPTOR_S* pNext; // pointer on next page for this font (used in unicode)
} D4D_FONT_DESCRIPTOR;

#endif /* D4D_FONT_H_ */
