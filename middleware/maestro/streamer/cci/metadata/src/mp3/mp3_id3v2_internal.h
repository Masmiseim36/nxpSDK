/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

//! \addtogroup asi_dffi
//! @{
//
// Copyright (c) 2004-2005 SigmaTel, Inc.
//
//! \file mp3_id3v2_internal.h
//! \brief Contains functionality to read MP3 metadata.

#ifndef __ID3V2_INTERNAL_H
#define __ID3V2_INTERNAL_H

#include <stdbool.h>

#ifndef MAX_STRING
#define MAX_STRING 100
#endif

#define ID3V2_HEADER_FLAG_FOOTER      0x10
#define ID3V2_HEADER_EXPERIMENTAL     0x20
#define ID3V2_HEADER_EXTENDED_HEADER  0x40
#define ID3V2_HEADER_UNSYNCRONIZATION 0x80

// We don't support these
#define ID3V2_FRAME_HEADER_TAG_ALTER_PREVERVE  0x0040
#define ID3V2_FRAME_HEADER_FILE_ALTER_PRESERVE 0x0020
#define ID3V2_FRAME_HEADER_FRAME_READ_ONLY     0x0010

// We don't support these
#define ID3V2_FRAME_HEADER_GROUPING_IDENTITY     0x4000
#define ID3V2_FRAME_HEADER_DATA_LENGTH_INDICATOR 0x0100

// We only use these to determine that we cannot read a frame with these bits present.
#define ID3V2_FRAME_HEADER_UNSYNCRONIZATION 0x0200
#define ID3V2_FRAME_HEADER_COMPRESSION      0x0800
#define ID3V2_FRAME_HEADER_ENCRYPTION       0x0400

// These represent the last 3 letters of the frame identifier.
// The function Id3V2ReadFrame() will read the first byte (the T, for example) in one word,
// and then the last 3 letters as another word.  This helps to easily determine what the identifier for
// the tag is easier.  Remember, these 3 bytes are read from the file in reversed order. (So TALB is actually
// read into 2 words:   'T' and 'BLA')
#define TALB 0x424c41
#define TIT1 0x315449
#define TIT2 0x325449
#define TPE1 0x314550
#define TRCK 0x4b4352
#define TCON 0x4e4f43
#define TYER 0x524559
#define TDRC 0x435244

// These represent the last 3 letters of the frame identifier.
// The function Id3V2ReadFramev2() will read the first byte (the T, for example) in one word,
// and then the last 2 letters as another word.  This helps to easily determine what the identifier for
// the tag is easier. Remember, these 2 bytes are read from the file in reversed order. (So TAL is actually
// read into 2 words:   'T' and 'LA')
#define TAL 0x4c41
#define TT1 0x3154
#define TT2 0x3254
#define TP1 0x3150
#define TRK 0x4b52
#define TCO 0x4f43
#define TYE 0x4559

#define ENCODING_ISO_8559     0
#define ENCODING_UTF_16       1
#define ENCODING_UTF_16_NOBOM 2
#define ENCODING_UTF_8        3

#define BOM_BIG_ENDIAN    0xFFFE
#define BOM_LITTLE_ENDIAN 0xFEFF

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
bool Id3V2ReadFrame(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, bool bSyncSafe);
bool Id3V2ReadFramev2(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, bool bSyncSafe);
int Id3V2ReadVariedStringToUnicodeBuffer(
    CCI_Ctx *Ctx, uint32_t uFlags, uint32_t uFrameSize, uint16_t *pString, int32_t iBufferLength);
int Id3V2ReadUTF16String(
    CCI_Ctx *Ctx, uint32_t uEndianNess, uint32_t uFlags, uint32_t uFrameSize, uint16_t *pString, int32_t iBufferLength);
int Id3V2ReadUTF8String(CCI_Ctx *Ctx, uint32_t uFrameSize, uint16_t *pString, int32_t iBufferLength);

uint32_t Id3V2ReadInt(CCI_Ctx *Ctx, bool bSyncSafe);
uint32_t Id3V2ReadIntv2(CCI_Ctx *Ctx, bool bSyncSafe);

#endif
///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
