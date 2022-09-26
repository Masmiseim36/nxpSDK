/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

////////////////////////////////////////////////////////////////////////////////
//! \addtogroup common_interface
//! @{
//
// Copyright (c) 2013-2014 Freescale, Inc.
//
//! \file mp3_extractmetadata.c
//! \brief Contains functionality that extracts metadata from the file
//!                and stores it in the file_meta_data_t structure.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdbool.h>
#include <error.h>
#include "codec_interface.h"
#include "cci_metadata.h"
#include "mp3_metadata.h"

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////
// For metadata display. Manually changing the global
// variable before compiling will make it effective.
//	Use:	NO_TAG_SELECTED	----display ID3V2, or ID3V1 if ID3V2 is not available.
//			ID3V1_TAG_SELECTED---display ID3V1 only
//			ID3V2_TAG_SELECTED---display ID3V2 only
// # bytes to look for sync before marking it bad
#define MP3_SYNC_THRESHOLD     100000 // 81920    //10000 (Stmp13504)
#define MAX_SYNCWORD_TO_VERIFY 5

////////////////////////////////////////////////////////////////////////////////
//    local function prototypes
///////////////////////////////////////////////////////////////////////////////
#ifndef FOR_INTERNAL_USE_ONLY
int MP3MetaDataReadVBRHeaders(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, uint32_t *pVBRTOCTable, MP3_DFFI_t *ptrThis);
int32_t Mp3MetaDataReadID3V2(CCI_Ctx *Ctx, int32_t iIgnored, file_meta_data_t *pMetaData, MP3_DFFI_t *ptrThis);
#endif /* FOR_INTERNAL_USE_ONLY */
int CheckID3Headers(CCI_Ctx *Ctx, file_meta_data_t *MetaData, MP3_DFFI_t *ptrThis);
int32_t ExtractMp3Frame(CCI_Ctx *Ctx, uint8_t *btFrameUnpack, int32_t *offset);
uint16_t *Mp3MetaDataGetGenreString(uint32_t uGenre);
// void StrToWStrN(uint8_t *pWStr, uint8_t *pStr, int32_t n);

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

extern uint8_t txt_ConvertDbcsToUtf16c(char *ptr, uint16_t *pUnicodeValue);
extern uint8_t txt_ConvertUtf8ToUtf16c(char *ptr, uint16_t *pUnicodeValue);
////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
const uint32_t u32BitRateIndex1[16] = {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 999};
const uint32_t u32BitRateIndex2[16] = {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 999};
const uint32_t u32BitRateIndex3[16] = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 999};
const uint32_t u32BitRateIndex4[16] = {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 999};
const uint32_t u32BitRateIndex5[16] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 999};
const uint32_t wMPEG1SampRate[4]    = {44100, 48000, 32000, 999};
const uint32_t wMPEG2SampRate[4]    = {22050, 24000, 16000, 999};
const uint32_t wMPEG2pt5SampRate[4] = {11025, 12000, 8000, 999};
uint32_t g_wVBRSignature; // Xing or VBR ("X"/"V"/"U"(unknown) or 0 for none)
int32_t Mp3DecoderStartFrameFilePosition;

uint8_t mp3_copy_one_byte(char *ptr, char *ptr1)
{
    *ptr1 = *ptr;
    return 1;
}
uint8_t mp3_copy_two_byte(char *ptr, uint16_t *ptr1)
{
    *ptr1 = *ptr;
    return 1;
}
void mp3_copy_16_to_8(uint8_t *ptr, uint16_t *ptr1, int32_t size)
{
    int i = 0;
    for (i = 0; i < size; i++)
    {
        *ptr = (uint8_t)*ptr1;
        ptr++;
        ptr1++;
    }
}

#ifndef AUDIO_DECODE_STREAM
static uint64_t Divide_64_by_32(uint64_t n, uint32_t base)
{
    uint64_t rem = n;
    uint64_t b   = base;
    uint64_t res, d = 1;
    uint32_t high = rem >> 32;

    /* Reduce the thing a bit first */
    res = 0;
    if (high >= base)
    {
        high /= base;
        res = (uint64_t)high << 32;
        rem -= (uint64_t)(high * base) << 32;
    }

    while ((int64_t)b > 0 && b < rem)
    {
        b = b + b;
        d = d + d;
    }

    do
    {
        if (rem >= b)
        {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);

    // n = res;
    return res;
}
#endif

////////////////////////////////////////////////////////////////////////////////
//! \brief get file size Callback API for all metadata extraction.
//!
//!
//! \param[in] Ctx         Pointer to CCI Context structure
//!
//! \retval File Size.
////////////////////////////////////////////////////////////////////////////////
int32_t mp3_GetFileSize(CCI_Ctx *Ctx)
{
    int32_t offset;
    int32_t size;

    offset = Ctx->cci_dec_tell(0, Ctx->user_data);
    Ctx->cci_dec_seek(0, 0, SEEK_END, Ctx->user_data);
    size = Ctx->cci_dec_tell(0, Ctx->user_data);
    Ctx->cci_dec_seek(0, offset, SEEK_SET, Ctx->user_data);

    return size;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Extract the meta data from the file.
//!
//!
//! Extract the meta data from the file.
//!
//! \param[in]      *Ctx        Pointer to CCI Context structure.
//! \param[out]  *MetaData Metadata extracted from the file.
//!
//! \return Status of the call.
//! \retval SUCCESS (kCodecSuccess)
//! \retval CODEC_METADATA_FILETYPE_FAILED
//!
////////////////////////////////////////////////////////////////////////////////
int32_t codec_extract_metadata_mp3(file_meta_data_t *MetaData, CCI_Ctx *Ctx)
{
    RtStatus_t Retcode = SUCCESS;

    uint8_t mp3_buf[140];
#ifndef AUDIO_DECODE_STREAM
    uint64_t u64TotalBits;
#endif

    MP3_DFFI_t *mp3_dffi;

    /* Set some defaults */
    MetaData->num_bytes        = 0;
    MetaData->start_pos        = 0;
    MetaData->total_time       = 0;
    MetaData->valid_vbr_header = 0;
    MetaData->avg_bit_rate     = 0;

    if (Ctx->user_data != NULL)
    {
        Ctx->cci_dec_seek(0, 0, SEEK_SET, Ctx->user_data);

        memset(mp3_buf, 0, 140);
        // read 10 bytes for "NETSYNCDRM" tag identifier
        if (Ctx->cci_dec_read(0, (uint8_t *)&mp3_buf, 10, 0, Ctx->user_data) == 10)
        {
            if (!memcmp(mp3_buf, "NETSYNCDRM", 10))
                return CODEC_METADATA_UNSUPPORTED_FILE;
        }
        else
            return CODEC_METADATA_FILETYPE_FAILED;

#ifdef AUDIO_DECODE_STREAM
        mp3_dffi = (MP3_DFFI_t *)&mp3_buf[0];
#else
        Ctx->cci_dec_seek(0, 0, SEEK_SET, Ctx->user_data);
        memset(mp3_buf, 0, 10);

        // Get the number of bytes in song (updated as all the headers are found)
        MetaData->num_bytes = (uint32_t)mp3_GetFileSize(Ctx);
        MetaData->start_pos = 0;

        if (MetaData->num_bytes == 0)
            return CODEC_METADATA_FILETYPE_FAILED;

        mp3_dffi = (MP3_DFFI_t *)&mp3_buf[0]; // startOCRAMAdd;

        Retcode = CheckID3Headers(Ctx, MetaData, mp3_dffi);

        char *genre, *album, *artist;
        int j;

        for (j = 0; j < CASCFG_TRACK_GENRE_LEN; j++)
        {
            genre = (char *)(&MetaData->song_genre);
            if (j == 0)
            {
                if ((genre[j] >= 'a') && (genre[j] <= 'z'))
                {
                    genre[j] -= ('a' - 'A');
                }
            }
            else
            {
                if ((genre[j] >= 'A') && (genre[j] <= 'Z'))
                {
                    genre[j] += ('a' - 'A');
                }
            }
        }
        for (j = 0; j < CASCFG_TRACK_ALBUM_LEN; j++)
        {
            album = (char *)(&MetaData->song_album);
            if (j == 0)
            {
                if ((album[j] >= 'a') && (album[j] <= 'z'))
                {
                    album[j] -= ('a' - 'A');
                }
            }
            else
            {
                if ((album[j] >= 'A') && (album[j] <= 'Z'))
                {
                    album[j] += ('a' - 'A');
                }
            }
        }
        for (j = 0; j < CASCFG_TRACK_ARTIST_LEN; j++)
        {
            artist = (char *)(&MetaData->song_artist);
            if (j == 0)
            {
                if ((artist[j] >= 'a') && (artist[j] <= 'z'))
                {
                    artist[j] -= ('a' - 'A');
                }
            }
            else
            {
                if ((artist[j] >= 'A') && (artist[j] <= 'Z'))
                {
                    artist[j] += ('a' - 'A');
                }
            }
        }
        if (Retcode != SUCCESS)
            return CODEC_METADATA_FILETYPE_FAILED;
#endif

        /* The XING header should be just right after ID3V2 tag
        this will leave the file position at where the function entered if no
        XING header, or just after the XING header */
        MP3MetaDataReadVBRHeaders(Ctx, (void *)MetaData, 0, mp3_dffi);

        /* Do a fast scan, looking for "FF" in the file.
        ScanForFF returns at the first FF byte, or with an Error Code */
        Retcode = ScanForFF(Ctx, (uint32_t)MP3_SYNC_THRESHOLD, MetaData);
        if (SUCCESS == Retcode)
        {
            /* we know this is an MP3 file, so fill in file type and resource */
            MetaData->stream_type = STREAM_TYPE_MP3;

            // search for frame header to get sample rate and bitrate
            Retcode = GetMp3FrameInfo(MetaData, Ctx, mp3_dffi);
            if (SUCCESS != Retcode)
            {
                /* Error */
                Retcode = CODEC_METADATA_FILETYPE_FAILED;
                return (RtStatus_t)(Retcode);
            }

            mp3_dffi->Mp3DecoderStartFrameFilePosition = MetaData->start_pos;
            MetaData->bits_per_sample                  = 16;
        } /* if(SUCCESS == Retcode) */

        // calculate time for files with no Xing header
        if (MetaData->total_time == 0)
        {
            if (MetaData->bit_rate)
            {
#ifdef AUDIO_DECODE_STREAM
                /* Cannot calculate with audio stream.  Set to non-zero to avoid
                 * streamer pipeline error about invalid duration. */
                MetaData->total_time = 1;
#else
                u64TotalBits         = (uint64_t)MetaData->num_bytes * 8;
                MetaData->total_time = (uint32_t)Divide_64_by_32(
                    (uint64_t)u64TotalBits,
                    MetaData->bit_rate); //((uint64_t)u64TotalBits / MetaData->bit_rate); // convert to mSec
#endif
            }
        }

        MetaData->bit_rate = MetaData->bit_rate * 1000; // Convert bitrate in bps.
        if (MetaData->avg_bit_rate == 0)
            MetaData->avg_bit_rate = MetaData->bit_rate;
    }
    else /* if(ERROR_GENERIC == iHandle) */
    {
        Retcode = CODEC_METADATA_FILETYPE_FAILED;
    } /* if(ERROR_GENERIC != iHandle) */

    return (RtStatus_t)(Retcode);
}
#ifdef EXTRACT_TOC
////////////////////////////////////////////////////////////////////////////////
//! \brief Extract the meta data Table of Contents (TOC) from the file.
//!
//!
//! Extract the meta data's Table of Contents (TOC) from the file.
//!
//! \param[in]  file_handle Fast open key; this key allows the file to be quickly found and opened
//! \param[in]  pVBRTOCTable --
//!                 pointer to the TOC table (NULL if it doesn't exist, in which case it will NOT be filled)
//! \param[out]  *MetaData Metadata extracted from the file.
//!
//! \return Status of the call.
//! \retval SUCCESS
//! \retval ERROR_ASI_DFFI_EXTRACTMETADATA
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t asi_dffi_ExtractMetaDataMp3TOC(int64_t file_handle, file_meta_data_t *MetaData, uint32_t *pVBRTOCTable)
{
    RtStatus_t Retcode = SUCCESS;
    int32_t iHandle;
    file_meta_data_t md;
    int32_t *startOCRAMAdd;
    MP3_DFFI_t *mp3_dffi;

    /* Set some defaults */
    md.start_pos        = 0;
    md.total_time       = 0;
    md.valid_vbr_header = 0;
    md.avg_bit_rate     = 0;

    /*/ Open the file. */
    iHandle = FastOpen(file_handle, (char *)"r");
    if (ERROR_GENERIC != iHandle)
    {
        os_dmi_MemAlloc((void **)&startOCRAMAdd, sizeof(MP3_DFFI_t), 1, DMI_MEM_SOURCE_FASTMEM);

        mp3_dffi = (MP3_DFFI_t *)startOCRAMAdd;

        // CheckID3Headers(iHandle, &md);

        /* The XING header should be just right after ID3V2 tag
        this will leave the file position at where the function entered if no
        XING header, or just after the XING header */
        MP3MetaDataReadVBRHeaders(Ctx, (void *)&md, pVBRTOCTable, mp3_dffi);

        os_dmi_MemFree(startOCRAMAdd);
    }
    else /* if(ERROR_GENERIC == iHandle) */
    {
        Retcode = CODEC_MEDIA_READ_ERROR;
    } /* if(ERROR_GENERIC != iHandle) */

    return (RtStatus_t)(Retcode);
}
#endif
////////////////////////////////////////////////////////////////////////////////
//! \brief Checks the ID3 headers for MP3 file.
//!
//!
//! Checks the ID3 headers for MP3 file.
//!
//! \param[in]  *Ctx        Pointer to CCI Context structure.
//! \param[in]  *MetaData   This structure will be filled in by the function.
//! \retval SUCCESS
//!
////////////////////////////////////////////////////////////////////////////////
int CheckID3Headers(CCI_Ctx *Ctx, file_meta_data_t *MetaData, MP3_DFFI_t *ptrThis)
{
    int32_t ret_value;
    ptrThis->g_iTagSelected = NO_TAG_SELECTED;

    // This leaves the file position the same.
    ret_value = Mp3MetaDataReadID3V2(Ctx, 0, (void *)MetaData, ptrThis);
    if (ret_value != SUCCESS)
        return ret_value;

    // Save ID3V2 data in MetaDataTemp
    if (ptrThis->g_iTagSelected != ID3V2_TAG_SELECTED)
    {
        // Attempt to read the ID3V2 tag
        // this leaves the file position at the first uint8_t after the Id3V1 tag, or where it entered the function
        Mp3MetaDataReadID3V1(Ctx, MetaData, ptrThis);
    }

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Checks for ID3v1 header at end of MP3 file.
//!
//!
//! Checks for ID3v1 header at end of MP3 file. (the last 128 bytes of the file)
//!
//! \param[in]      *Ctx            Pointer to CCI Context structure.
//! \param[out]  *pMetaData Metadata extracted from the file.
//! \retval SUCCESS or !SUCCESS
//!
//  Notes:       If TAG is found, fills in the following in the meta-data struct:
//                 Title, Artist, Album, Genre.
//               Always returns the file position to the same as it was prior to entering this function.
////////////////////////////////////////////////////////////////////////////////
int Mp3MetaDataReadID3V1(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, MP3_DFFI_t *ptrThis)
{
    uint32_t uLength;
    uint32_t uTemp = 0;
    uint8_t Bufstring[31];
    uint8_t *pStringbuf    = &Bufstring[0];
    uint8_t *pTempString   = pMetaData->song_title;
    uint8_t *pStringTitle  = pMetaData->song_title;
    uint8_t *pStringArtist = pMetaData->song_artist;
    uint8_t *pStringAlbum  = pMetaData->song_album;
    uint8_t *pStringYear   = pMetaData->song_year;

    int32_t uPosition = Ctx->cci_dec_tell(0, Ctx->user_data);

    if (Ctx->cci_dec_seek(0, -128, SEEK_END, Ctx->user_data) != ERROR_GENERIC)
        Ctx->cci_dec_read(0, (uint8_t *)&uTemp, 3, 0, Ctx->user_data);

    if (uTemp == 0x474154)
    {
        // if we get this far, we have found an ID3v1 tag
        // subtract off 128 bytes from the length of the song.
        pMetaData->num_bytes -= 128;
        memset((void *)Bufstring, 0, 31);

        // Read in the song title.
        Ctx->cci_dec_read(0, (uint8_t *)Bufstring, 30, 0, Ctx->user_data);
        // changed from if(pMetaData->xxx) to if(pMetaData). It is struct, so pMetaData->xxx is always nonzero
        if (pMetaData)
        {
            uint8_t bytesConsumed = 0;
            uLength               = 31;
            if (TITLE_SIZE < uLength)
                uLength = TITLE_SIZE;

            while (uLength)
            {
                bytesConsumed = mp3_copy_one_byte((char *)pStringbuf, (char *)pStringTitle);
                // assert(pTempString == pStringTitle);
                pStringbuf += bytesConsumed;
                pStringTitle++;
                pTempString++;
                uLength -= bytesConsumed;
            }
        }

        memset((void *)Bufstring, 0, 31);
        pStringbuf = &Bufstring[0];

        // Read in the song artist.
        Ctx->cci_dec_read(0, (uint8_t *)Bufstring, 30, 0, Ctx->user_data);
        // changed from if(pMetaData->xxx) to if(pMetaData). It is struct, so pMetaData->xxx is always nonzero
        if (pMetaData)
        {
            uLength = 31;
            if (ARTIST_SIZE < uLength)
                uLength = ARTIST_SIZE;

            while (uLength)
            {
                pStringbuf += mp3_copy_one_byte((char *)pStringbuf, (char *)pStringArtist);
                pStringArtist++;
                uLength--;
            }
        }

        memset((void *)Bufstring, 0, 31);
        pStringbuf = &Bufstring[0];

        // Read in the song album.
        Ctx->cci_dec_read(0, (uint8_t *)Bufstring, 30, 0, Ctx->user_data);
        // changed from if(pMetaData->xxx) to if(pMetaData). It is struct, so pMetaData->xxx is always nonzero
        if (pMetaData)
        {
            uLength = 31;
            if (ALBUM_SIZE < uLength)
                uLength = ALBUM_SIZE;

            while (uLength)
            {
                pStringbuf += mp3_copy_one_byte((char *)pStringbuf, (char *)pStringAlbum);
                pStringAlbum++;
                uLength--;
            }
        }

        memset((void *)Bufstring, 0, 31);
        pStringbuf = &Bufstring[0];

        // Read in the year.
        Ctx->cci_dec_read(0, (uint8_t *)Bufstring, 4, 0, Ctx->user_data);
        // changed from if(pMetaData->xxx) to if(pMetaData). It is struct, so pMetaData->xxx is always nonzero
        if (pMetaData)
        {
            uLength = 5;
            if (YEAR_SIZE < uLength)
                uLength = YEAR_SIZE;

            while (uLength)
            {
                pStringbuf += mp3_copy_one_byte((char *)pStringbuf, (char *)pStringYear);
                pStringYear++;
                uLength--;
            }
        }

        memset((void *)Bufstring, 0, 31);
        pStringbuf = &Bufstring[0];

        /* As the comments field is too short to write anything useful, in the
        ID3v1.1 standard, declares that this field should be 28 characters,
        that the next byte always should be zero and that the last byte before
        the genre byte should contain the track number. */

        // seek past the comment
        Ctx->cci_dec_seek(0, 29, SEEK_CUR, Ctx->user_data);

        // Read in the track number.
        uTemp = 0;
        Ctx->cci_dec_read(0, (uint8_t *)&uTemp, 1, 0, Ctx->user_data);
        pMetaData->track_number = uTemp;

        // Read in the genre.
        uTemp = 0;
        Ctx->cci_dec_read(0, (uint8_t *)&uTemp, 1, 0, Ctx->user_data);
        pMetaData->genre = uTemp;
        // changed from if(pMetaData->xxx) to if(pMetaData). It is struct, so pMetaData->xxx is always nonzero
        if (pMetaData)
        {
            uint16_t *song_genre = Mp3MetaDataGetGenreString(uTemp);
            if (song_genre)
            {
                mp3_copy_16_to_8(pMetaData->song_genre, song_genre, 17);
            }
        }

        ptrThis->g_iTagSelected = ID3V1_TAG_SELECTED;
    }
    // return to where we were when we entered the function.
    Ctx->cci_dec_seek(0, uPosition, SEEK_SET, Ctx->user_data);
    return (SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Used to find the Mp3 frame (searches for sync bytes -- 11 bits set to 1).
//!
//!
//! Used to find the Mp3 frame (searches for sync bytes -- 11 bits set to 1)
//! Also looks through consecutive frames for changing bitrates and sets VBR to be true
//! bitrates are indeed changing.
//!
//! \param[in]      *Ctx            Pointer to CCI Context structure.
//! \param[out]  *MetaData Metadata extracted from the file.
//! \retval SUCCESS or !SUCCESS
//!
//  Notes:       If frame is found, fills in the following in the meta-data struct:
//                 sample_rate, bit_rate, total_time (if no Xing header), bitrate_type
//               Currently: A frame is considered to have been found if the following criteria
//               are met:
//                   1. The 11 sync bits are found (11 ones in a row)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   2. The bitrate is NOT equal to 4 ones in a row (not allowed)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   3. Two frames in a row are found and critical values match between them
////////////////////////////////////////////////////////////////////////////////
RtStatus_t GetMp3FrameInfo(file_meta_data_t *MetaData, CCI_Ctx *Ctx, MP3_DFFI_t *ptrThis)
{
    uint8_t btFrameUnpack[MP3_BYTES_TO_READ];
    int32_t iExtract = SUCCESS, i, offset;
    bool bFrameFound = 0, bFirstFrameFound = 0;
    codec_bitrate_type_t bitrate_type = CONSTANT_BITRATE;
    uint32_t bit_rate = 0, sample_rate = 0;
    uint32_t u32BitRateTemp = 0, u32SampleRateTemp = 0;
    uint32_t wFrameLengthInBytes;
    uint8_t btBitRateTemp, btPaddingBitTemp;
    uint32_t u32StartPosOffset     = 0;
    uint32_t wByteCountWithoutSync = 0;
    volatile uint8_t btMPEGVer, btBitRate, btSampRate, btPaddingBit;
    volatile int32_t iLayer = 0;
    uint32_t u32StartPosFirstSync[MAX_SYNCWORD_TO_VERIFY + 2];
    int32_t i32SyncCount;

    uint32_t numchannels;

    // Search through file until sync is found
    u32StartPosFirstSync[0] = MetaData->start_pos;
    u32StartPosFirstSync[1] = 0;
    u32StartPosFirstSync[2] = 0;
    u32StartPosFirstSync[3] = 0;
    u32StartPosFirstSync[4] = 0;
    u32StartPosFirstSync[5] = 0;
    u32StartPosFirstSync[6] = 0;

    i32SyncCount = 0;
    while (!bFrameFound)
    {
        while (!bFirstFrameFound)
        {
            offset = MP3_BYTES_TO_READ; // so that Read can happen after seeking
            // Reset to beginning of file or start position offset by the current offset
            if ((Ctx->cci_dec_seek(0, u32StartPosFirstSync[i32SyncCount] + u32StartPosOffset, SEEK_SET,
                                   Ctx->user_data)) < 0)
                return (CODEC_FSEEK_FAILED);

            // Extract frame header from file
            iExtract = ExtractMp3Frame(Ctx, btFrameUnpack, &offset);

            if (iExtract == ERROR_GENERIC) // ERROR_GENERIC reading file
                return (CODEC_MEDIA_READ_ERROR);

            if (iExtract == SUCCESS) // SYNC bits found (11 ones)
            {
                bFirstFrameFound = 1;
                // Update start position
                u32StartPosFirstSync[i32SyncCount + 1] = u32StartPosFirstSync[i32SyncCount] + u32StartPosOffset;
                i32SyncCount++;
            }
            else
            {
                // else (iExtract == seek increment value)
                // Update Counters
                u32StartPosOffset += (uint32_t)iExtract;
                wByteCountWithoutSync += (uint32_t)iExtract;

                i32SyncCount = 0;
                // Return error if Sync not found within MP3_SYNC_THRESHOLD (project.inc) bytes
                // This is to prevent searching a large file that is perhaps bad.
                if (wByteCountWithoutSync > MP3_SYNC_THRESHOLD)
                    return (CODEC_MEDIA_READ_ERROR);
            }
        }

        // At this point, we have found what looks like the first frame
        // We are pointing to the first byte of the frame header. (Sync byte).
        u32StartPosOffset = 0; // reset the offset

        // We have found the first frame header, extract the data from it
        btMPEGVer    = (((btFrameUnpack[offset + 1]) >> 3) & (0x03));
        iLayer       = (((btFrameUnpack[offset + 1]) >> 1) & (0x03));
        btBitRate    = ((btFrameUnpack[offset + 2]) >> 4);
        btSampRate   = (((btFrameUnpack[offset + 2]) >> 2) & (0x03));
        btPaddingBit = ((btFrameUnpack[offset + 2] >> 1) & (0x01));

        numchannels = ((btFrameUnpack[offset + 3] >> 6) & (0x03));

        if (numchannels == 0x03)
            MetaData->num_channels = 1;
        else
            MetaData->num_channels = 2;

        if ((btMPEGVer != 1) && (btSampRate != 3) && (btBitRate != 0) && (iLayer != 0))
        {
            // get the sampling rate and bitrate
            GetMp3Rates(btMPEGVer, btSampRate, btBitRate, iLayer, &sample_rate, &bit_rate);
            // calculate frame length
            wFrameLengthInBytes = GetFrameLength(btMPEGVer, bit_rate, sample_rate, btPaddingBit, iLayer);

            offset = MP3_BYTES_TO_READ; // so that Read can happen after seeking

            // Move forward to next frame and read it.
            if ((Ctx->cci_dec_seek(0, (u32StartPosFirstSync[i32SyncCount] + (uint32_t)wFrameLengthInBytes), SEEK_SET,
                                   Ctx->user_data)) < 0)
                return CODEC_FSEEK_FAILED;

            // Extract frame header from file
            iExtract = ExtractMp3Frame(Ctx, btFrameUnpack, &offset);

            if (iExtract == SUCCESS)
            {
                // Check that sampling rate and MPEG version and layer match
                if (((((btFrameUnpack[offset + 1]) >> 3) & (0x03)) == (btMPEGVer)) &&
                    (((btFrameUnpack[offset + 2]) >> 4) != 0) && (((btFrameUnpack[offset + 2]) >> 4) != 15) &&
                    ((((btFrameUnpack[offset + 1]) >> 1) & (0x03)) == iLayer) &&
                    ((((btFrameUnpack[offset + 2]) >> 2) & (0x03)) == btSampRate))
                {
                    u32StartPosFirstSync[i32SyncCount + 1] = u32StartPosFirstSync[i32SyncCount] + wFrameLengthInBytes;
                    i32SyncCount++;
                    // Fill in rates
                    MetaData->sample_rate = sample_rate;
                    MetaData->bit_rate    = bit_rate;
                    btBitRateTemp         = ((btFrameUnpack[offset + 2]) >> 4);
                    btPaddingBitTemp      = ((btFrameUnpack[offset + 2] >> 1) & (0x01));
                    // get the sampling rate and bitrate
                    GetMp3Rates(btMPEGVer, btSampRate, btBitRateTemp, iLayer, &u32SampleRateTemp, &u32BitRateTemp);
                    // calculate frame length
                    wFrameLengthInBytes =
                        GetFrameLength(btMPEGVer, u32BitRateTemp, u32SampleRateTemp, btPaddingBitTemp, iLayer);

                    // Move forward to next frame and read it.
                    // Question : Is this seek operation necessary ?
                    offset = MP3_BYTES_TO_READ; // so that Read can happen after seeking

                    if (i32SyncCount < MAX_SYNCWORD_TO_VERIFY)
                    {
                        u32StartPosOffset = 0;
                        // u32StartPosFirstSync[i32SyncCount] =
                        // u32StartPosFirstSync[i32SyncCount-1];//+wFrameLengthInBytes;
                        bFirstFrameFound = 0; // begin looking for first frame again
                        continue;
                    }
                    else
                    {
                        if ((Ctx->cci_dec_seek(0, (wFrameLengthInBytes - (uint32_t)MP3_FRAME_HEADER_BYTES_TO_READ),
                                               SEEK_CUR, Ctx->user_data)) < 0)
                            return CODEC_FSEEK_FAILED;

                        if ((bit_rate != u32BitRateTemp) || (ptrThis->g_wVBRSignature == 'X') ||
                            (ptrThis->g_wVBRSignature == 'V'))
                        {
                            if (MetaData->total_time)
                                MetaData->bit_rate = (MetaData->num_bytes / MetaData->total_time * 8) /
                                                     1000; // for stmp6282 vbr w/o TOC uses avg time
                            bitrate_type           = VARIABLE_BITRATE;
                            MetaData->bitrate_type = VARIABLE_BITRATE;
                        }
                        MetaData->mpeg_layer = 4 - iLayer;
                        bFrameFound          = 1; // second frame matches first, so take it to be right
                        break;
                    }
                }
                else
                {
                    i32SyncCount                       = 0;
                    u32StartPosFirstSync[i32SyncCount] = u32StartPosFirstSync[i32SyncCount + 1];
                }
            }
            else
            {
                i32SyncCount                       = 0;
                u32StartPosFirstSync[i32SyncCount] = u32StartPosFirstSync[i32SyncCount + 1];
            }
        }
        else
        {
            i32SyncCount                       = 0;
            u32StartPosFirstSync[i32SyncCount] = u32StartPosFirstSync[i32SyncCount + 1];
        }
        // ELSE -- the frame did not match, so start over
        u32StartPosOffset++;  // increment start position to prevent looping on bad data
        bFirstFrameFound = 0; // begin looking for first frame again
    }                         // while (!bFrameFound)

    MetaData->start_pos = u32StartPosFirstSync[0];
    if (bitrate_type != VARIABLE_BITRATE)
    {
        // check for variable bit rates -- this is not 100% right since need to search entire file
        for (i = 0; i < (MP3_FRAMES_TO_CHECK_VBR - 2); i++)
        {
            iExtract = ExtractMp3Frame(Ctx, btFrameUnpack, &offset);
            // safety check offset+2 is not >= 128
            if ((offset + 2) >= 128)
            {
                return CODEC_METADATA_UNSUPPORTED_FILE;
            }
            btBitRateTemp    = ((btFrameUnpack[offset + 2]) >> 4);
            btPaddingBitTemp = ((btFrameUnpack[offset + 2] >> 1) & (0x01));
            if (iExtract != SUCCESS)
                break;

            // get the sampling rate and bitrate
            if (GetMp3Rates(btMPEGVer, btSampRate, btBitRateTemp, iLayer, &u32SampleRateTemp, &u32BitRateTemp) ==
                ERROR_GENERIC)
                break;
            // calculate frame length
            wFrameLengthInBytes =
                GetFrameLength(btMPEGVer, u32BitRateTemp, u32SampleRateTemp, btPaddingBitTemp, iLayer);

            offset = MP3_BYTES_TO_READ;

            // Move forward to next frame and read it.
            if ((Ctx->cci_dec_seek(0, (wFrameLengthInBytes - (uint32_t)MP3_FRAME_HEADER_BYTES_TO_READ), SEEK_CUR,
                                   Ctx->user_data)) < 0)
                return CODEC_FSEEK_FAILED;

            if (bit_rate != u32BitRateTemp)
            {
                bitrate_type           = VARIABLE_BITRATE;
                MetaData->bitrate_type = VARIABLE_BITRATE;
                if (ptrThis->g_wVBRSignature == 0)
                {
                    ptrThis->g_wVBRSignature = 'U'; // This is probably a VBR file, but not enough information.
                }
                break;
            }
        }
    }

    if (bitrate_type != VARIABLE_BITRATE)
        MetaData->bitrate_type = CONSTANT_BITRATE;

    if (MetaData->mpeg_layer != 3)
        return (CODEC_METADATA_NOT_FOUND);

    if (iExtract == ERROR_GENERIC)
        return (CODEC_METADATA_NOT_FOUND);

    return (SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Searches for mp3 frame headers.
//!
//!
//! Searches for mp3 frame headers.
//!
//! \param[in]  *Ctx            Pointer to CCI Context structure.
//! \param[in]  btFrameUnpack TBD
//! \param[in]  offset TBD
//! \retval 0 if SUCCESS
//! \retval -1 if Failure
//! \retval 1-4 Byte 1, 2, 3, 4 (the byte number which is FF)
//!
//! \todo [PUBS] Define TBD parameter(s)
//  Outputs:     0 if Success, -1 if Failure, otherwise 1, 2, 3, or 4.
//                    A '1' is returned if byte number 1 (count starts with byte 0) is FF.
//                    A '2' is returned if byte number 2 is FF.
//                    A '3' is returned if byte number 3 is FF.
//                    A '4' is returned if none are FF.
//                    This allows calling routine to FileSeek forward this many bytes and call again.
//
//               Currently: A frame is considered to have been found if the following criteria
//               are met:
//                   1. The 11 sync bits are found (11 ones in a row)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   2. The bitrate is NOT equal to 4 ones in a row (not allowed)  -- (ExtractMp3Frame does this)
//                             ****AND****
//                   3. Two frames in a row are found and critical values match between them -- (GetMp3FrameInfo does
//                   this)
////////////////////////////////////////////////////////////////////////////////
int32_t ExtractMp3Frame(CCI_Ctx *Ctx, uint8_t *btFrameUnpack, int32_t *offset)
{
    int32_t i;

    // Read from File
    if (*offset >= MP3_BYTES_TO_READ)
    {
        if (Ctx->cci_dec_read(0, (uint8_t *)btFrameUnpack, MP3_BYTES_TO_READ, 0, Ctx->user_data) == 0)
            return (ERROR_GENERIC);
        *offset = 0;
    }

    if ((btFrameUnpack[*offset] == 0xFF) && ((btFrameUnpack[*offset + 1] >> 5) == 0x07))
    // IF sync bits are all set (11 of them)
    {
        // If the bitrate index == 0x0F, then this is not a valid frame
        if (((btFrameUnpack[*offset + 2] >> 4) != 0x0F))
            return SUCCESS;
    }

    for (i = 1; i <= 3; i++)
        if (btFrameUnpack[*offset + i] == 0xFF)
            break;

    *offset += i;
    // IF none of the 4 bytes are FF, return a '4' to seek forward to 5th byte
    return i;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Quickly scans through file looking for the first instance of "FF"
//!
//!
//! Quickly scans through file looking for the first instance of "FF"
//!
//! \param[in]  *Ctx                Pointer to CCI Context structure.
//! \param[in]  wThresholdMax Maximum threshold before declaring file "bad"
//! \param[in]  *pMetaData          This structure will be filled in by the function
//! \retval 0 if SUCCESS & points to byte number of first FF. Error if not success.
//!
////////////////////////////////////////////////////////////////////////////////
RtStatus_t ScanForFF(CCI_Ctx *Ctx, uint32_t wThresholdMax, file_meta_data_t *pMetaData)
{
    RtStatus_t rtn = CODEC_METADATA_NOT_FOUND;
    int32_t i;
    uint8_t wTempBuffer[150];
    uint32_t wCount = 0;
    bool bLoopDone  = 0;

    while (!bLoopDone)
    {
        // Read from File
        if (Ctx->cci_dec_read(0, (uint8_t *)wTempBuffer, 150, 0, Ctx->user_data) != ERROR_GENERIC)
        {
            // Scan buffer for "FF" in any of the words.
            for (i = 0; i < 150; i++)
            {
                if ((wTempBuffer[i] == 0xFF))
                {
                    rtn = SUCCESS;
                    // update the start position of the MP3 data
                    pMetaData->start_pos += (uint32_t)wCount;
                    Ctx->cci_dec_seek(0, pMetaData->start_pos, SEEK_SET, Ctx->user_data);
                    bLoopDone = 1;
                    break;
                }
                wCount += 1;
                if (wCount > wThresholdMax)
                {
                    bLoopDone = 1;
                    break;
                }
            }
        }
        else
        {
            rtn = CODEC_MEDIA_READ_ERROR;
            break;
        }
    } // while(1)
    return rtn;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Computes the frame length for mp3 file.
//!
//!
//! Computes the frame length for mp3 file.
//!
//! \param[in]  btMPEGVer      TBD
//! \param[in]  bit_rate     TBD
//! \param[in]  sample_rate  TBD
//! \param[in]  btPaddingBit   TBD
//! \param[in]  iLayer         TBD
//! \returns Frame length.
//!
//! \todo [PUBS] Define TBD parameter(s)
////////////////////////////////////////////////////////////////////////////////
uint32_t GetFrameLength(
    uint8_t btMPEGVer, uint32_t bit_rate, uint32_t sample_rate, uint8_t btPaddingBit, int32_t iLayer)
{
    uint32_t wFrameLengthInBytes = 0;

    // For Layer I files us this formula:
    // FrameLengthInBytes = (12 * BitRate / SampleRate + Padding) * 4
    if (iLayer == LayerI)
        wFrameLengthInBytes = ((((uint32_t)12000 * bit_rate / sample_rate) + (uint32_t)btPaddingBit) * (uint32_t)4);

    // For Layer II & III files use this formula:
    // FrameLengthInBytes = (FrameSizeInBits/8) * BitRate / SampleRate + Padding
    // This works out to:
    //    MPEG Version 1 :         FrameLengthInBytes = 144 * Bitrate / SampleRate + Padding
    //    MPEG Version 2 and 2.5 : FrameLengthInBytes = 72 * Bitrate / SampleRate + Padding
    else
    {
        // Layer II uses the same formula irrespective of MPEG versions.
        if ((btMPEGVer == MPEGv1) || (iLayer == LayerII))
            wFrameLengthInBytes =
                (((((int32_t)bit_rate * (int32_t)1000 * (int32_t)144) / (int32_t)sample_rate)) + (int32_t)btPaddingBit);
        else
            wFrameLengthInBytes =
                (((((int32_t)bit_rate * (int32_t)1000 * (int32_t)72) / (int32_t)sample_rate)) + (int32_t)btPaddingBit);
    }
    return (wFrameLengthInBytes);
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Finds the sampling rate and bitrate for the current frame.
//!
//!
//! Finds the sampling rate and bitrate for the current frame.
//!
//! \param[in] btMPEGVer     TBD
//! \param[in] btSampRate    TBD
//! \param[in] btBitRate     TBD
//! \param[in] iLayer        TBD
//! \param[in] sample_rate TBD
//! \param[in] bit_rate    TBD
//! \returns SUCCESS or failure
//! \todo [PUBS] Define TBD parameter(s)
//!
////////////////////////////////////////////////////////////////////////////////
int32_t GetMp3Rates(
    uint8_t btMPEGVer, uint8_t btSampRate, uint8_t btBitRate, int32_t iLayer, uint32_t *sample_rate, uint32_t *bit_rate)
{
    // Find BitRate & Sampling Rate
    if (btMPEGVer == MPEGv1)
    {
        *sample_rate = wMPEG1SampRate[btSampRate];
        if (iLayer == LayerI)
            *bit_rate = u32BitRateIndex1[btBitRate];
        else if (iLayer == LayerII)
            *bit_rate = u32BitRateIndex2[btBitRate];
        else
            *bit_rate = u32BitRateIndex3[btBitRate];
    }
    else
    {
        if (btMPEGVer == MPEGv2)
            *sample_rate = wMPEG2SampRate[btSampRate];
        if (btMPEGVer == MPEGv2pt5)
            *sample_rate = wMPEG2pt5SampRate[btSampRate];
        if (iLayer == LayerI)
            *bit_rate = u32BitRateIndex4[btBitRate];
        else
            *bit_rate = u32BitRateIndex5[btBitRate];
    }
    return (SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Copy  uint8_t string to  unicode(UTF16)
//!
//!
//! Copy  uint8_t string to  unicode(UTF16)
//!
//! \param[in]  *pWStr TBD
//! \param[in]  *pStr  TBD
//! \param[in]  n      TBD
//!
//! \todo [PUBS] Define TBD parameter(s)
////////////////////////////////////////////////////////////////////////////////
#if 0
void StrToWStrN(uint8_t   *pWStr, uint8_t   *pStr, int32_t   n)
{
    if(pStr == NULL || pWStr == NULL)
    {
        return;
    }

    while(n-- > 0 && *pStr != '\0')
    {
        *pWStr = *pStr++;
        *(pWStr +1) = 0;
        pWStr += 2;
    }
    *pWStr = 0;
    *(pWStr +1) = 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////
// End of file
////////////////////////////////////////////////////////////////////////////////
//! @}
