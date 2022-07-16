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
// Copyright (c) 2002-2005 SigmaTel, Inc.
//
//! \file mp3_metadata.h
//! \brief Contains header data for MP3 metadata functions.

#ifndef __MP3METADATA_H
#define __MP3METADATA_H

////////////////////////////////////////////////////////////////////////////////
//  Definitions
////////////////////////////////////////////////////////////////////////////////
#define READ_BUFFER_SIZE               156
#define XING_BYTES_TO_READ             156
#define VBR_BYTES_TO_READ              400 // must be less than READ_BUFFER_SIZE * 3
#define MP3_BYTES_TO_READ              128
#define ID3_BYTES_TO_READ              128
#define MP3_FRAME_HEADER_BYTES_TO_READ 4
#define FRAME_HEADER_SIZE              4
#define FRAME_HEADER                   11

// *************** XING Header Equates ************************
// A Xing header may be present in the ancillary
// data field of the first frame of an mp3 bitstream
// The Xing header (optionally) contains
//      frames      total number of audio frames in the bitstream
//      bytes       total number of bytes in the bitstream
//      toc         table of contents

// toc (table of contents) gives seek points
// for random access
// the ith entry determines the seek point for
// i-percent duration
// seek point in bytes = (toc[i]/256.0) * total_bitstream_bytes
// e.g. half duration seek point = (toc[50]/256.0) * total_bitstream_bytes

#define FRAMES_FLAG      0x0001
#define BYTES_FLAG       0x0002
#define TOC_FLAG         0x0004
#define VBR_SCALE_FLAG   0x0008
#define FRAMES_AND_BYTES (FRAMES_FLAG | BYTES_FLAG)

// Frame MPEG Versions (matches MP3 frame format)
#define MPEGv2pt5  0x00
#define MPEGvERROR 0x01
#define MPEGv2     0x02
#define MPEGv1     0x03
// Frame Layer Info (matches MP3 frame format)
#define LayerI   3 // seems backwards, but matches mp3 frame format
#define LayerII  2
#define LayerIII 1

// Display selection on ID3V1 or ID3V2
#define NO_TAG_SELECTED    0
#define ID3V1_TAG_SELECTED 1
#define ID3V2_TAG_SELECTED 2

#define MP3L2_3FRAME_SIZE     1152 // Frame size for MPEG Layer 2 & 3 (# samples)
#define MP3L1FRAME_SIZE       384  // Frame size for MPEG Layer 1
#define MP3VBR_LOW_FRAME_SIZE 576  // Required constant for VBR - not sure where came from.

// VBR
#define MP3_FRAMES_TO_CHECK_VBR 100

/**
 * @brief Structure to receive extracted header for Xing or VBR
 *
 */
typedef struct
{
    uint32_t VBRVersion;      // Version 0=MPEG2, 1=MPEG1
    uint32_t VBRSampleRate;   // Sample Rate from VBR frame
    uint8_t VBRSignature;     // Xing or VBR
    uint32_t VBRStreamFrames; // total bit stream frames from Xing or VBR header
    uint32_t VBRStreamBytes;  // total bit stream bytes from Xing or VBR header
    uint32_t VBRTableScale;   // Scale of Table (only for VBRi)
    uint32_t VBREntryBytes;   // Entry Bytes??
    uint32_t VBREntryFrames;  // Entry Frames
} VBR_HEADER_DATA;

/**
 * @brief VBR global header data
 * VBRTable pointer may be NULL (but we're initializing it.)
 *
 */
typedef struct
{
    int32_t VBRTableSize;     // Size of Table (100 for Xing, 200 for VBRi)
    uint32_t VBRTotalSeconds; // Total Seconds - max out at 4660 hrs.
    uint32_t VBRDurPerBin;    // Duration per VBR Frame. x 128 for 2 digits of accuracy
    uint32_t *VBRTable;       // VBR Table
} VBR_GLOBAL_HEADER_DATA;

////////////////////////////////////////////////////////////////////////////////
//  Externs
////////////////////////////////////////////////////////////////////////////////
extern int32_t Mp3DecoderStartFrameFilePosition;

/**
 * @brief MP3 meta data information
 *
 */
typedef struct MP3_DFFI
{
    uint32_t g_wVBRSignature; // Xing or VBR ("X"/"V"/"U"(unknown) or 0 for none)
    int32_t Mp3DecoderStartFrameFilePosition;
    int32_t g_iTagSelected;
    VBR_GLOBAL_HEADER_DATA g_VBRGlobalData;
    VBR_HEADER_DATA g_VBRHeaderData; // struct for VBR Data (Xing or VBRI)
} MP3_DFFI_t;

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

int32_t SeekPoint(uint8_t TOC[100], int32_t file_bytes, float percent);
// return seekpoint in bytes (may be at eof if percent=100.0)
// TOC = table of contents from Xing header
// file_bytes = number of bytes in mp3 file
// percent = play time percentage of total playtime. May be
//           fractional (e.g. 87.245)

int32_t GetID3v1Tag(file_meta_data_t *MetaData, uint8_t *buf);
RtStatus_t GetMp3FrameInfo(file_meta_data_t *MetaData, CCI_Ctx *Ctx, MP3_DFFI_t *ptrThis);
int32_t ExtractMp3Frame(CCI_Ctx *Ctx, uint8_t *btFrameUnpack, int32_t *offset);
uint32_t GetFrameLength(uint8_t btMPEGVer, uint32_t bit_rate, uint32_t wSampRate, uint8_t btPaddingBit, int32_t iLayer);
int32_t GetMp3Rates(
    uint8_t btMPEGVer, uint8_t btSampRate, uint8_t btBitRate, int32_t iLayer, uint32_t *wSampRate, uint32_t *bit_rate);
int32_t ExtractI4(unsigned char *buf, int32_t i);
int GetMp3MetaData(int32_t PackedPathNameAddress, int32_t btCurrentDevice, file_meta_data_t *MetaData);
int Mp3MetaDataReadID3V1(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, MP3_DFFI_t *ptrThis);

RtStatus_t ScanForFF(CCI_Ctx *Ctx, uint32_t wThresholdMax, file_meta_data_t *pMetaData);

#endif
///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
