/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

//! \addtogroup common_interface
//! @{
//
// Copyright (c) 2013-2014 Freescale, Inc.
//
//! \file mp3_vbr.c
//! \brief Contains functionality for extracting MP3 metadata.

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "cci_metadata.h"
#include "mp3_metadata.h"

////////////////////////////////////////////////////////////////////////////////
//  Defines
////////////////////////////////////////////////////////////////////////////////
#define MP3_SYNC_THRESHOLD 10000
////////////////////////////////////////////////////////////////////////////////
//  Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Externs
////////////////////////////////////////////////////////////////////////////////
#ifdef EXTRACT_TOC
extern uint32_t g_wVBRTOCTable[];
#endif

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
int32_t ExtractI4(uint8_t *buf, int32_t i);
uint32_t ExtractIVar(uint8_t *buf, int32_t i, uint8_t iByteCount);
int32_t GetXingHeader(
    VBR_HEADER_DATA *VbrHeader, uint8_t *buf, uint32_t *pOffset, uint32_t *pVBRTOCTable, MP3_DFFI_t *ptrThis);
int32_t GetVBRiHeader(
    VBR_HEADER_DATA *VbrHeader, uint8_t *buf, uint32_t *pOffset, uint32_t *pVBRTOCTable, MP3_DFFI_t *ptrThis);

///////////////////////////////////////////////////////////////////////////////
//> Name:        int FileReadWithSync (int32_t BytesToRead, CCI_Ctx *Ctx, uint8_t *ReadBuffer)
//
//  Type:        Function
//
//  Description: Finds sync and reads bytes from there.
//
//  Inputs:
//  Outputs:     Number of bytes read or -1 if Failure.
//
//<
/////////////////////////////////////////////////////////////////////////////////
int FileReadWithSync(int32_t BytesToRead, CCI_Ctx *Ctx, uint8_t *ReadBuffer)
{
    uint32_t wCount = 0;
    int32_t i;
    while (Ctx->cci_dec_read(0, (uint8_t *)ReadBuffer, 150, 0, Ctx->user_data) != ERROR_GENERIC)
    {
        // Scan buffer for "FF" in any of the words.
        for (i = 0; i < 150; i++)
        {
            if (ReadBuffer[i] == 0xFF)
            {
                Ctx->cci_dec_seek(0, -150 + i, SEEK_CUR, Ctx->user_data);
                return Ctx->cci_dec_read(0, (uint8_t *)ReadBuffer, BytesToRead, 0, Ctx->user_data);
            }

            wCount++;
            if (wCount > MP3_SYNC_THRESHOLD)
                return ERROR_GENERIC;
        }
    }
    return ERROR_GENERIC;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        int MP3MetaDataReadVBRHeaders(CCI_Ctx *Ctx, file_meta_data_t *pMetaData)
//
//  Type:        Function
//
//  Description: Reads in the XING header, and fills in the related parts of the meta data structure.
//
//  Inputs:
//  Outputs:     0 if Success, -1 if Failure.
//
//  Notes:       Currently we're checking for VBRi (Fraunhofer) and Xing headers.
//<
/////////////////////////////////////////////////////////////////////////////////
int MP3MetaDataReadVBRHeaders(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, uint32_t *pVBRTOCTable, MP3_DFFI_t *ptrThis)
{
    uint8_t ReadBuffer[VBR_BYTES_TO_READ];
    int32_t uPosition = Ctx->cci_dec_tell(0, Ctx->user_data);
    uint32_t wOffset  = 0;

    ptrThis->g_wVBRSignature = 0; // Initialize to zero (No VBR).

    if (pVBRTOCTable)
    {
        ptrThis->g_VBRGlobalData.VBRTable = pVBRTOCTable; // Initialize VBR Table pointer (Global)
        // Clear the lookup table.
        memset(pVBRTOCTable, 0, VBR_TOC_SIZE_IN_BYTES);
    }

    ptrThis->g_VBRGlobalData.VBRTotalSeconds = 0;
    ptrThis->g_VBRGlobalData.VBRDurPerBin    = 0;

    if (FileReadWithSync(VBR_BYTES_TO_READ, Ctx, ReadBuffer) != ERROR_GENERIC)
    {
        // Check if Xing Header exists -- if so, use it
        if ((GetXingHeader(&ptrThis->g_VBRHeaderData, (uint8_t *)ReadBuffer, &wOffset, pVBRTOCTable, ptrThis) ==
             SUCCESS) ||
            (GetVBRiHeader(&ptrThis->g_VBRHeaderData, (uint8_t *)ReadBuffer, &wOffset, pVBRTOCTable, ptrThis) ==
             SUCCESS))
        { // fill in Xing Meta-Data if Xing header found
            // fill in the sampling rate
            pMetaData->sample_rate = ptrThis->g_VBRHeaderData.VBRSampleRate;
            // fill in the number of bytes
            pMetaData->num_bytes = ptrThis->g_VBRHeaderData.VBRStreamBytes;
            // Fill in the time
            pMetaData->total_time = ptrThis->g_VBRGlobalData.VBRTotalSeconds * 1000; // convert to mSec ;
            if (ptrThis->g_VBRGlobalData.VBRTotalSeconds == 0)
            {
                pMetaData->avg_bit_rate = 0;
            }
            else
            {
                pMetaData->avg_bit_rate =
                    (ptrThis->g_VBRHeaderData.VBRStreamBytes / ptrThis->g_VBRGlobalData.VBRTotalSeconds) * 8;
            }
            pMetaData->valid_vbr_header = 1;
        }
        // increment the StartPosition so we're positioned after the Xing or VBR Headers.
        pMetaData->start_pos += wOffset;
        Ctx->cci_dec_seek(0, (uPosition + wOffset), SEEK_SET, Ctx->user_data);
    }
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        GetXingHeader
//
//  Type:        Function
//
//  Description: Used to check for Xing header at beginning of file and extract
//               meta-data if found.
//
//  Inputs:      XHEADDATA *X  Pointer to Xing header data structure to be filled in
//               *buf          Pointer to packed buffer containing file read contents (to search)
//               *pOffset      Number of bytes from beginning of file to end of Xing header.
//
//  Outputs:     Returns SUCCESS if Xing header found, ERROR_GENERIC if not.
//
//  Notes: Bytes - description.
// 4   Xing
// 4   flags
// 4   frames
// 4   bytes
// 100 toc
//
//<
/////////////////////////////////////////////////////////////////////////////////
int32_t GetXingHeader(
    VBR_HEADER_DATA *VbrHeader, uint8_t *btVBRUnpack, uint32_t *pOffset, uint32_t *pVBRTOCTable, MP3_DFFI_t *ptrThis)
{
    uint8_t i;
    int32_t iXingPtr = 0;
    int32_t head_flags;
    int8_t h_id, h_id_mp2_5, h_mode, h_sr_index;
    uint32_t sr_table[4]        = {44100, 48000, 32000, 99999};
    uint32_t wPreviousByteCount = 0, wCurrentByteCount;
    uint32_t wLookup256, wTemp;

    // get selected MPEG header data
    h_id_mp2_5 = (btVBRUnpack[1] >> 4) & 1;
    h_id       = (btVBRUnpack[1] >> 3) & 1;
    h_sr_index = (btVBRUnpack[2] >> 2) & 3;
    h_mode     = (btVBRUnpack[3] >> 6) & 3;

    // determine offset of header
    if (h_id)
    { // mpeg1
        if (h_mode != 3)
            iXingPtr = (32 + 4);
        else
            iXingPtr = (17 + 4);
    }
    else
    { // mpeg2
        if (h_mode != 3)
            iXingPtr = (17 + 4);
        else
            iXingPtr = (9 + 4);
    }

    // look for header title "Xing"
    if (btVBRUnpack[iXingPtr] != 'X')
        return ERROR_GENERIC; // fail
    if (btVBRUnpack[iXingPtr + 1] != 'i')
        return ERROR_GENERIC; // header not found
    if (btVBRUnpack[iXingPtr + 2] != 'n')
        return ERROR_GENERIC;
    if (btVBRUnpack[iXingPtr + 3] != 'g')
        return ERROR_GENERIC;

    // if we get this far, we have a Xing header
    // extract the info from it
    VbrHeader->VBRSampleRate = sr_table[h_sr_index];
    if (h_id == 0)
        VbrHeader->VBRSampleRate >>= 1;
    if (h_id_mp2_5 == 0)
        VbrHeader->VBRSampleRate >>= 1; // MPEG 2.5 SR

    // unpack next 12 bytes (4 flags bytes, 4 frames bytes (if exist), 4 bytes bytes (if exist))
    iXingPtr += 4;
    head_flags = ExtractI4(btVBRUnpack, iXingPtr); // get flags
    iXingPtr += 4;

    if (head_flags & FRAMES_FLAG)
    {
        VbrHeader->VBRStreamFrames = ExtractI4(btVBRUnpack, iXingPtr);
        iXingPtr += 4;
    }
    else
        VbrHeader->VBRStreamFrames = 0;

    if (head_flags & BYTES_FLAG)
    {
        VbrHeader->VBRStreamBytes = ExtractI4(btVBRUnpack, iXingPtr);
        iXingPtr += 4;
    }

    ptrThis->g_VBRGlobalData.VBRTableSize = XING_TOC_SIZE;

    // Calculate the total seconds
    wTemp = VbrHeader->VBRSampleRate >= 32000 ? MP3L2_3FRAME_SIZE : MP3VBR_LOW_FRAME_SIZE;
    ptrThis->g_VBRGlobalData.VBRTotalSeconds =
        (uint32_t)(((uint32_t)VbrHeader->VBRStreamFrames * wTemp) / VbrHeader->VBRSampleRate);

    // Note - multiply by 128 so we can preserve two digits of accuracy.
    ptrThis->g_VBRGlobalData.VBRDurPerBin =
        ((ptrThis->g_VBRGlobalData.VBRTotalSeconds * 128) / (ptrThis->g_VBRGlobalData.VBRTableSize + 1));

    // If we made it down here, we're probably OK - but need to validate table (see below).
    ptrThis->g_wVBRSignature = 'X';
    if (head_flags & TOC_FLAG)
    {
        for (i = 0; i <= XING_TOC_SIZE; i++)
        {
            if (i == (XING_TOC_SIZE - 1))
                wLookup256 = 256;
            else
                wLookup256 = btVBRUnpack[iXingPtr + i + 1];
            // 4/7/04 - Change integrity check of table.  Instead of marking a
            // VBR file as invalid if the last entry is zero, mark as invalid if
            // a zero is found in the first 90.  Zeros in the last 10 entries will
            // will be filled with 0xFF.  We'll try to use as much of the table as
            // possible.
            if ((wLookup256 == 0) && (i > 0)) // Check integrity of table.
            {
                if (i < 90)
                {
                    ptrThis->g_wVBRSignature = 'U'; // Error in table (invalid) - Don't use Xing header
                }
                else
                {
                    // We'll just fill the array with 255 if a zero is found.
                    wLookup256 = 255;
                }
            }

            // In order to emulate the VBRi template, we only need to store the difference
            // between samples.
            wCurrentByteCount = ((((uint32_t)VbrHeader->VBRStreamBytes * (uint32_t)wLookup256) + 128) / 256);
            if (pVBRTOCTable)
            {
                *pVBRTOCTable = (uint32_t)((uint32_t)wCurrentByteCount - (uint32_t)wPreviousByteCount);
                pVBRTOCTable++;
            }
            wPreviousByteCount = wCurrentByteCount;
        }
    }

    *pOffset = iXingPtr;

    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        GetVBRiHeader
//
//  Type:        Function
//
//  Description: Used to check for VBRi header at beginning of file and extract
//               meta-data if found.
//
//  Inputs:      VBR_HEADER_DATA *V   Pointer to Xing header data structure to be filled in
//               *buf                 Pointer to packed buffer containing file read contents
//                                    (to search)
//
//  Outputs:     Returns SUCCESS if VBR header found, ERROR_GENERIC if not.
//
//  Notes:
// 4    VBRI          // Recognition string.
// 2    Version       // MPEG Version.
// 2    Delay         // Not filled in - I'm not sure what it's purpose is.
// 2    Quality       // Not filled in
// 4    Stream Bytes  // Number of bytes in media stream.
// 4    Stream Frames // Number of frames in media stream.
// 2    Table Size    // Size of Table
// 2    Table Scale   // ???
// 2    Entry Bytes   // # bytes in each entry of table
// 2    Entry Frames  // ???
// 200  Table         // Table correlating linear space (consumed bytes) to non-linear
//                    // space (Time) since this is variable bit rate.
/////////////////////////////////////////////////////////////////////////////////
int32_t GetVBRiHeader(
    VBR_HEADER_DATA *VbrHeader, uint8_t *btVBRUnpack, uint32_t *pOffset, uint32_t *pVBRTOCTable, MP3_DFFI_t *ptrThis)
{
    int32_t i;
    int32_t iVBRiPtr = 0;
    int32_t h_id, h_mode, h_sr_index;
    uint32_t sr_table[4] = {44100, 48000, 32000, 99999};
    uint32_t wTemp1; //, wTemp2;

    // get selected MPEG header data
    h_id       = (btVBRUnpack[1] >> 3) & 1;
    h_sr_index = (btVBRUnpack[2] >> 2) & 3;
    h_mode     = (btVBRUnpack[3] >> 6) & 3;

    // determine offset of header
    if (h_id)
    { // mpeg1
        if (h_mode != 3)
            iVBRiPtr = (32 + 4);
        else
            iVBRiPtr = (17 + 4);
    }
    else
    { // mpeg2
        if (h_mode != 3)
            iVBRiPtr = (17 + 4);
        else
            iVBRiPtr = (9 + 4);
    }

    // look for header title "VBRI"
    if (btVBRUnpack[iVBRiPtr] != 'V')
        return ERROR_GENERIC; // fail
    if (btVBRUnpack[iVBRiPtr + 1] != 'B')
        return ERROR_GENERIC; // header not found
    if (btVBRUnpack[iVBRiPtr + 2] != 'R')
        return ERROR_GENERIC;
    if (btVBRUnpack[iVBRiPtr + 3] != 'I')
        return ERROR_GENERIC;

    // if we get this far, we have a VBR header
    // extract the info from it
    VbrHeader->VBRVersion    = h_id;
    VbrHeader->VBRSampleRate = sr_table[h_sr_index];
    if (h_id == 0)
        VbrHeader->VBRSampleRate >>= 1;

    // Now extract the tag information.
    iVBRiPtr += 4;
    VbrHeader->VBRVersion = ExtractIVar(btVBRUnpack, iVBRiPtr, 2);     // get Version
    iVBRiPtr += 6;                                                     // Skip Delay & Quality.
    VbrHeader->VBRStreamBytes = ExtractIVar(btVBRUnpack, iVBRiPtr, 3); // get Hi 3 Bytes - Stream Bytes
    VbrHeader->VBRStreamBytes <<= 8;
    iVBRiPtr += 3;                                                      // Point to Low word.
    VbrHeader->VBRStreamBytes |= ExtractIVar(btVBRUnpack, iVBRiPtr, 1); // get Low Byte - Stream bytes
    iVBRiPtr += 1;                                                      // Point to Stream Frames.
    VbrHeader->VBRStreamFrames = ExtractIVar(btVBRUnpack, iVBRiPtr, 3); // get Hi Word - Stream Frames
    VbrHeader->VBRStreamFrames <<= 8;
    iVBRiPtr += 3;                                                                 // Point to Low word.
    VbrHeader->VBRStreamFrames |= ExtractIVar(btVBRUnpack, iVBRiPtr, 1);           // get Low Word - Stream Frames
    iVBRiPtr += 1;                                                                 // Point to Table Size.
    ptrThis->g_VBRGlobalData.VBRTableSize = ExtractIVar(btVBRUnpack, iVBRiPtr, 2); // get Table Size
    iVBRiPtr += 2;                                                                 // Point to Table Scale.
    VbrHeader->VBRTableScale = ExtractIVar(btVBRUnpack, iVBRiPtr, 2);              // get Table Scale
    iVBRiPtr += 2;                                                                 // Point to Entry Bytes.
    VbrHeader->VBREntryBytes = ExtractIVar(btVBRUnpack, iVBRiPtr, 2);              // get Entry Bytes
    iVBRiPtr += 2;                                                                 // Point to Entry Frames.
    VbrHeader->VBREntryFrames = ExtractIVar(btVBRUnpack, iVBRiPtr, 2);             // get Entry Frames
    iVBRiPtr += 2;                                                                 // Point to Entry Bytes.

    ptrThis->g_VBRGlobalData.VBRTableSize =
        (ptrThis->g_VBRGlobalData.VBRTableSize < VBR_TOC_SIZE) ? ptrThis->g_VBRGlobalData.VBRTableSize : VBR_TOC_SIZE;

    // Calculate the total seconds
    wTemp1 = VbrHeader->VBRSampleRate >= 32000 ? MP3L2_3FRAME_SIZE : MP3VBR_LOW_FRAME_SIZE;
    ptrThis->g_VBRGlobalData.VBRTotalSeconds =
        (uint32_t)(((uint32_t)VbrHeader->VBRStreamFrames * wTemp1) / VbrHeader->VBRSampleRate);

    ptrThis->g_VBRGlobalData.VBRDurPerBin =
        ((ptrThis->g_VBRGlobalData.VBRTotalSeconds * 128) / (ptrThis->g_VBRGlobalData.VBRTableSize + 1));

    // Retrieve the table values from the files.
    for (i = 0; i < ptrThis->g_VBRGlobalData.VBRTableSize && i < VBR_TOC_SIZE; i++)
    {
        // TBD - we should set some parameters that will alert us if the table is corrupt.
        //       I have seen zero entries in a VBRI table, so that is not invalid.
        if (pVBRTOCTable)
        {
            *pVBRTOCTable = ExtractIVar(btVBRUnpack, iVBRiPtr, VbrHeader->VBREntryBytes); // get Data Frames.
            pVBRTOCTable++;
        }
        else
        {
            ExtractIVar(btVBRUnpack, iVBRiPtr, VbrHeader->VBREntryBytes); // get Data Frames.
        }
        iVBRiPtr += VbrHeader->VBREntryBytes; // Increment Pointer.
    }

    ptrThis->g_wVBRSignature = 'V';
    *pOffset                 = iVBRiPtr;
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ExtractI4
//
//   Type:          Function
//
//   Description:   Big endian extract for Xing finder.
//
//   Inputs:        Ptr to buffer containing unpacked header, offset in buffer
//
//   Outputs:       int integer value
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
int32_t ExtractI4(uint8_t *buf, int32_t i)
{
    int32_t x;
    // big endian extract
    x = buf[i];
    x <<= 8;
    x |= buf[i + 1];
    x <<= 8;
    x |= buf[i + 2];
    x <<= 8;
    x |= buf[i + 3];

    return x;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ExtractIVar
//
//   Type:          Function
//
//   Description:   Big endian extract for Buffer Grab.
//
//   Inputs:        Ptr to buffer containing unpacked header, offset in buffer,
//                  Number of Bytes.
//
//   Outputs:       uint32_t value - Result of fetch.
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
uint32_t ExtractIVar(uint8_t *buf, int32_t i, uint8_t iByteCount)
{
    uint32_t x = 0;
    uint8_t iCount;

    //    if (iByteCount > 3)
    //    {
    //      SystemHalt();     // Can't return more than 3 bytes.
    //    }

    // big endian extract
    for (iCount = 0; iCount < iByteCount; iCount++)
    {
        x <<= 8;
        x |= buf[i + iCount];
    }

    return x;
}
///////////////////////////////////////////////////////////////////////////////
//> Name:        CalculateCurrentTimeVBR
//
//  Type:        Function
//
//  Description: Given a consumed byte count, interpolate where we are in
//               time using the VBR lookup table - for use when playing VBR files.
//
//  Inputs:      NONE
//
//  Outputs:     NONE
//
//  Notes:
//              1)  We can have up to 200 entries.
//              2)  Although not used here, compiling here then copying to
//                  decoder_overlay.asm eases changes.
/////////////////////////////////////////////////////////////////////////////////
#if (0)
void CalculateCurrentTimeVBR(void)
{
    uint8_t i = 0;
    uint32_t wRoundError;
    uint32_t wCurrentSecondsx128 = 0;
    uint32_t wTemp;
    uint32_t wCurrentBytePointer = ((uint32_t)g_wSongByteTotalConsumedHigh << 24) + g_wSongByteTotalConsumedLow;
    uint32_t wAccumulatedBytes   = 0;
    uint32_t Subtraction;

    // loop while accumulating bytes until we exceed CurrentPosition in file or at end of table.
    while (((uint32_t)wCurrentBytePointer >= (uint32_t)wAccumulatedBytes) && (i < g_VBRGlobalData.VBRTableSize))
    {
        wAccumulatedBytes += g_wVBRTOCTable[i++];
        wCurrentSecondsx128 += g_VBRGlobalData.VBRDurPerBin;
    }

    // Get the previous bin's value which is differential bytes between bin[i-1] and bin[i];
    wTemp = g_wVBRTOCTable[i - 1];

    // Calculate the time to subtract off by interpolating between bins.  Multiply by duration per
    // bin to estimate where we are.
    wRoundError = (wTemp >> 1);
    Subtraction = (uint32_t)(wAccumulatedBytes - wCurrentBytePointer);
    Subtraction = ((g_VBRGlobalData.VBRDurPerBin * Subtraction) + wRoundError) / wTemp;

    // Subtract off the interpolated difference.
    wCurrentSecondsx128 -= Subtraction;

    g_wAccumulatedSecs = wCurrentSecondsx128 / 128;
}
#endif
///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
