/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

///////////////////////////////////////////////////////////////////////////////
//! \addtogroup asi_dffi
//! @{
//
// Copyright (c) 2004-2005 SigmaTel, Inc.
//
//! \file wav_common.c
//! \brief Contains functionality that extracts the meta data from the file
//!                and stores it in the file_meta_data_t structure.
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include <error.h>
#include <stdlib.h>
#include "cci_metadata.h"
#include "wavedefs.h"
#include "wavedata.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////
#define INTERNAL_BUF 30

#define PACKED

#ifndef GUID_DEFINED
#define GUID_DEFINED
#pragma pack(4)
typedef PACKED struct _tGUID
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];

} GUID;
#pragma pack()
#endif

GUID PCMGUID = {0x0000001, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71}};

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

const unsigned int ckidlist_dffi[TOTAL_CKID] = {
    0x46464952, // RIFF_CKID
    0x45564157, // WAVE_CKID
    0x20746d66, // FMT_CKID
    0x74636166, // FACT_CKID
    0x61746164, // DATA_CKID
    0x5453494C, // LIST_CKID
    0x4F464E49, // INFO_CKID
    0x54465349, // ISFT_CKID
    0x54524149, // IART_CKID
    0x50534944, // DISP_CKID
    0x524e4749, // IGNR_CKID
    0x44524349, // ICRD_CKID = creation date
    0x4D414E49  // INAM_CKTD
};

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
int32_t DffiProcessChunk(int32_t i32ckIDval, int32_t *i32ckSize, CCI_Ctx *Ctx, file_meta_data_t *MetaData, WAVDEC *wav);
int32_t DffiGetckID(WAVDEC *ptrThis, CCI_Ctx *Ctx);
void DffiProcess_Common_Fmt_Header(WAVDEC *ptrThis, CCI_Ctx *Ctx, int32_t *i32ckSize);
void store_file_offset(int);
RtStatus_t DffiCompareGUID(GUID *pLeft, GUID *pRight);
void StrToWStrN(uint8_t *pWStr, uint8_t *pStr, int32_t n);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
uint64_t BigNum_Divide_64_by_32_wav(uint64_t n, uint32_t base)
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

void StrToWStrN(uint8_t *pWStr, uint8_t *pStr, int32_t n)
{
    if (pStr == NULL || pWStr == NULL)
    {
        return;
    }

    while (n-- > 0 && *pStr != '\0')
    {
        *pWStr       = *pStr++;
        *(pWStr + 1) = 0;
        pWStr += 2;
    }
    *pWStr       = 0;
    *(pWStr + 1) = 0;
}

/////////////////////////////////////////////////////////////////////////////////
//!  \brief         Extract the meta data.
//!
//!
//!  \param[in]     file_handle -- this key allows the file to be quickly found and opened
//!  \param[in]     *MetaData -- this structure will be filled in by the function
//!
//!  \retval         SUCCESS
//!  \retval         CODEC_METADATA_FILETYPE_FAILED -- metadata failed
//!  \retval         CODEC_FILE_OPEN_ERROR -- the file failed to open
//!
//!  \note          This should be accessed via the generic function DffiExtractMetaData.
//!      This function parses file up to Data chunk and gives start position.
//!
//!
/////////////////////////////////////////////////////////////////////////////////

RtStatus_t CodecExtractMetaDataAdpcm(file_meta_data_t *MetaData, CCI_Ctx *Ctx)
{
    // Insert Code Here.
    RtStatus_t Retcode = SUCCESS;
    int32_t i32ret_val;
    int32_t i32ckIDval, i32ckSize;
    int32_t i32chunklevel = 0;
    int32_t filecheck     = 1;
    WAVDEC wav;
    int32_t i32DataSize[5] = {0, 0, 0, 0, 0};

    /* Set some defaults */
    MetaData->start_pos  = 0;
    MetaData->total_time = 0;

    memset(&wav, 0, sizeof(WAVDEC));

    wav.wnNumCoef = 0;

    // if(ERROR_GENERIC != (int32_t)i32Handle)
    {
        while ((i32ckIDval = DffiGetckID(&wav, Ctx)) >= 0) // Returns -1 on error
        {
            if ((i32ckIDval != UNKNOWN_CKID) && (i32ckIDval != RIFF_CKID))
            {
                filecheck = 0;
            }

            Ctx->cci_dec_read(0, (uint8_t *)&i32ckSize, 4, 0, Ctx->user_data);

            if (((i32chunklevel > 0) && (i32ckSize > i32DataSize[i32chunklevel])) || (i32ckSize < 0))
            {
                i32ckSize = i32DataSize[i32chunklevel] - 8;
            }

            // chunk lenght, even padding
            if (i32ckSize % 2)
            {
                i32ckSize += 1;
            }

            // Subtract this chunk size from the current level
            i32DataSize[i32chunklevel] -= (i32ckSize + 8);
            // Skip unknown chunks
            if (i32ckIDval == UNKNOWN_CKID)
            {
                Ctx->cci_dec_seek(0, i32ckSize, SEEK_CUR, Ctx->user_data);
            }
            else
            {
                i32ret_val = DffiProcessChunk(i32ckIDval, &i32ckSize, Ctx, MetaData, &wav);
                if (i32ret_val == 1)
                {
                    // Contains nested chunks. Go one level up.
                    i32chunklevel++;
                    i32DataSize[i32chunklevel] = i32ckSize;
                }
                else if (i32ret_val == 2)
                {
                    if (i32ckIDval != DATA_CKID)
                        Ctx->cci_dec_seek(0, i32ckSize, SEEK_CUR, Ctx->user_data);
                    else
                        break;
                    // SKV :: Can we break from here ?
                }
                else if (i32ret_val < 0)
                {
                    Retcode = CODEC_METADATA_FILETYPE_FAILED;
                    break;
                }
            }

            // Check whether all the data at this level is exhausted
            // i32chunklevel 0 will always contain negative value.

            if (i32DataSize[i32chunklevel] <= 0)
            {
                // Go down one level
                i32chunklevel--;
                if (i32chunklevel <= 0)
                    break; // Done!
            }
        }

        if ((i32ckIDval == UNKNOWN_CKID) && (filecheck == 1))
        {
            Retcode = CODEC_METADATA_FILETYPE_FAILED;
            return Retcode;
        }

        // For IMA Adpcm decoding we support only 4 bit samples
        // if ((wav.gwFormatTag == WAVE_FORMAT_IMA_ADPCM || wav.gwFormatTag == WAVE_FORMAT_ADPCM)
        //  && wav.wBitsPerSample != 4)
        if (wav.gwFormatTag != WAVE_FORMAT_PCM)
        {
            return CODEC_METADATA_FILETYPE_FAILED;
        }
        /*if(wav.gwChannels > 2) {
            return CODEC_METADATA_FILETYPE_FAILED;
        }*/
        if (wav.datachunksize == 0)
        {
            return CODEC_METADATA_FILETYPE_FAILED;
        }
        // Update remaining Metadata fields
        if (Retcode == SUCCESS)
        {
            MetaData->sample_rate  = wav.gwSamplesPerSec;
            MetaData->num_channels = wav.gwChannels;
            MetaData->bit_rate     = wav.wBitsPerSample * wav.gwSamplesPerSec * wav.gwChannels;
            MetaData->total_time   = (uint32_t)BigNum_Divide_64_by_32_wav(
                ((uint64_t)MetaData->num_bytes * 1000),
                (wav.gwAvgBytesPerSec)); //(((uint64_t)MetaData->num_bytes*1000)/(wav.gwAvgBytesPerSec));
                                           //// convert to mSec
            MetaData->avg_bit_rate    = (wav.gwAvgBytesPerSec * 8);
            MetaData->bits_per_sample = wav.wBitsPerSample;
        }
    }
    // else  /* if(ERROR_GENERIC == i32Handle) */
    //{
    //   Retcode = CODEC_FILE_OPEN_ERROR;
    //} /* if(ERROR_GENERIC != i32Handle) */

    return Retcode;
}

/////////////////////////////////////////////////////////////////////////////////
//!  \brief  Processes MS ADPCM header
//!
//!
//!   \param[in]  ptrThis    Pointer to WAVE
//!   \param[in]  i32Handle  File Handle
//!
//!   \note       Can be overlaid
////////////////////////////////////////////////////////////////////////////////
//

void Decode_MS_ADPCM_header(WAVDEC *ptrThis, CCI_Ctx *Ctx, int32_t *i32ckSize)
{
    int32_t i;
    int16_t data[4];

    Ctx->cci_dec_read(0, (uint8_t *)data, 2 * 4, 0, Ctx->user_data);

    ptrThis->wBitsPerSample    = data[0];
    ptrThis->wcbSize           = data[1];
    ptrThis->wnSamplesPerBlock = data[2];
    ptrThis->wnNumCoef         = data[3];
    *i32ckSize -= 8;
    for (i = 0; i < ptrThis->wnNumCoef; i++)
    {
        Ctx->cci_dec_read(0, (uint8_t *)data, 2 * 2, 0, Ctx->user_data);
        ptrThis->u.ms.wCoef1[i] = data[0];
        ptrThis->u.ms.wCoef2[i] = data[1];
        *i32ckSize -= 4;
    }
}

/////////////////////////////////////////////////////////////////////////////////
//!   \brief      Processes IMA ADPCM header
//!
//!
//!   \param[in]  ptrThis    Pointer to WAVE
//!   \param[in]  i32Handle  File Handle
//!
//!   \note        Can be overlaid
////////////////////////////////////////////////////////////////////////////////

void Decode_IMA_ADPCM_header(WAVDEC *ptrThis, CCI_Ctx *Ctx, int32_t *i32ckSize)
{
    int16_t data[3];
    Ctx->cci_dec_read(0, (uint8_t *)data, 2 * 3, 0, Ctx->user_data);
    ptrThis->wBitsPerSample    = data[0];
    ptrThis->wcbSize           = data[1];
    ptrThis->wnSamplesPerBlock = data[2];
    *i32ckSize -= 6;
}

/////////////////////////////////////////////////////////////////////////////////
//!   \brief      Processes WAVEFORMATEXTENSIBLE header
//!
//!
//!   \param[in]  ptrThis    Pointer to WAVE
//!   \param[in]  i32Handle  File Handle
//!
//!   \note        Can be overlaid
////////////////////////////////////////////////////////////////////////////////
int Decode_WAVEFORMATEXTENSIBLE_header(WAVDEC *ptrThis, CCI_Ctx *Ctx, int32_t *i32ckSize)
{
    uint8_t data[26];
    int i;
    GUID Wave_Data;
    Ctx->cci_dec_read(0, (uint8_t *)data, 26, 0, Ctx->user_data);
    ptrThis->wBitsPerSample      = (data[1] << 8) | data[0];
    ptrThis->wcbSize             = (data[3] << 8) | data[2];
    ptrThis->wValidBitsPerSample = (data[5] << 8) | data[4];
    ptrThis->dwChannelMask       = (data[9] << 24) | (data[8] << 16) | (data[7] << 8) | data[6];
    Wave_Data.Data1              = (data[13] << 24) | (data[12] << 16) | (data[11] << 8) | data[10];
    Wave_Data.Data2              = (data[15] << 8) | data[14];
    Wave_Data.Data3              = (data[17] << 8) | data[16];
    for (i = 0; i < 8; i++)
    {
        Wave_Data.Data4[i] = data[i + 18];
    }

    *i32ckSize -= 26;

    if (DffiCompareGUID(&PCMGUID, &Wave_Data) == 1)
    {
        ptrThis->gwFormatTag = WAVE_FORMAT_PCM;
    }
    else
    {
        return -1;
    }
    // Fudge the Block Align to process more samples, simultaneously, so rest of the processing is unchanged
    ptrThis->gwBlockAlign = 504; // This ensures that we can process both 24 bit and 16 bit sample blocks
    // Dummy variable below for processing blocks of data.
    if (ptrThis->wBitsPerSample == 16)
        ptrThis->wnSamplesPerBlock = (252 >> (ptrThis->gwChannels - 1));
    else if (ptrThis->wBitsPerSample == 24)
        ptrThis->wnSamplesPerBlock = (168 >> (ptrThis->gwChannels - 1));
    else if (ptrThis->wBitsPerSample == 32)
        ptrThis->wnSamplesPerBlock = (126 >> (ptrThis->gwChannels - 1));
    else
        ptrThis->wnSamplesPerBlock = (504 >> (ptrThis->gwChannels - 1)); // Got to be 8 bit PC

    return 0;
}

///////////////////////////////////////////////////////////////////////
//! \brief	Compares GUID.
//!
//!
//! \param[in]	    *pLeft
//! \param[in]	    *pRight
//!
//! \return			Compares 2 GUID objects.
//!	\i32ReturnValue
//!
///////////////////////////////////////////////////////////////////////
RtStatus_t DffiCompareGUID(GUID *pLeft, GUID *pRight)
{
    int32_t i;
    register int32_t *pLeftGUID  = (int32_t *)pLeft;
    register int32_t *pRightGUID = (int32_t *)pRight;
    RtStatus_t i32ReturnValue    = (RtStatus_t)1;

    for (i = 0; i < 4; i++)
    {
        if (*pLeftGUID++ != *pRightGUID++)
        {
            i32ReturnValue = (RtStatus_t)0;
            break;
        }
    }
    return i32ReturnValue;
}

/////////////////////////////////////////////////////////////////////////////////
//!
//!  \brief  Processes PCM Wave header
//!
//!
//!   \param[in]  i32Handle  File Handle
//!   \param[in]  ptrThis    Pointer to WAVE
//!
//!    \note      Can be overlaid
////////////////////////////////////////////////////////////////////////////////

void Decode_PCM_header(WAVDEC *ptrThis, CCI_Ctx *Ctx, int32_t *i32ckSize)
{
    Ctx->cci_dec_read(0, (uint8_t *)&ptrThis->wBitsPerSample, 2, 0, Ctx->user_data);
    *i32ckSize -= 2;
    // Fudge the Block Align to process more samples, simultaneously, so rest of the processing is unchanged
    ptrThis->gwBlockAlign = 504; // This ensures that we can process both 24 bit and 16 bit sample blocks
    // Dummy variable below for processing blocks of data.
    if (ptrThis->wBitsPerSample == 16)
        ptrThis->wnSamplesPerBlock = (252 >> (ptrThis->gwChannels - 1));
    else if (ptrThis->wBitsPerSample == 24)
        ptrThis->wnSamplesPerBlock = (168 >> (ptrThis->gwChannels - 1));
    else
        ptrThis->wnSamplesPerBlock = (504 >> (ptrThis->gwChannels - 1)); // Got to be 8 bit PCM
}

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//!
//!    \brief      Processes a chunk

//!
//!   \param[in]   i32ckIDval  chunk ID value.
//!   \param[in]   i32ckSize   pointer to chunk size variable.
//!   \param[in]   i32Handle   File Handle of newly opened file.
//!   \param[in]   *MetaData   pointer to file_meta_data_t
//!   \param[in]   *ptrThis    Pointer to WAVE
//!
//!   \retval       0  if there are no nested chunks.
//!   \retval       1 = if there are nested chunks
//!   \retval       2 = if it is a Data chunk
//!   \retval      -1 = FAILED
//!   \note        Can be overlaid
////////////////////////////////////////////////////////////////////////////////
int32_t DffiProcessChunk(
    int32_t i32ckIDval, int32_t *i32ckSize, CCI_Ctx *Ctx, file_meta_data_t *MetaData, WAVDEC *ptrThis)
{
    int32_t i32ret_val = 0;
    uint8_t Bufstring[INTERNAL_BUF];
    int32_t imax_length = (*i32ckSize >= 30) ? (30 - 1) : *i32ckSize;

    switch (i32ckIDval)
    {
        case RIFF_CKID:
            // Only for RIFF chunk, check if 'W','A','V','E' is present.
            // We suppport only wave files.
            i32ckIDval = DffiGetckID(ptrThis, Ctx);
            if (i32ckIDval == WAVE_CKID)
            {
                *i32ckSize -= 4;
                i32ret_val = 1;
            }
            else
                i32ret_val = -1;
            break;

        case FMT_CKID:
            DffiProcess_Common_Fmt_Header(ptrThis, Ctx, i32ckSize);
            MetaData->stream_type = STREAM_TYPE_UNKNOWN;

            if (ptrThis->gwFormatTag == WAVE_FORMAT_ADPCM)
            {
                Decode_MS_ADPCM_header(ptrThis, Ctx, i32ckSize);
                MetaData->stream_type = STREAM_TYPE_MSADPCM;
            }
            else if (ptrThis->gwFormatTag == WAVE_FORMAT_IMA_ADPCM)
            {
                Decode_IMA_ADPCM_header(ptrThis, Ctx, i32ckSize);
                MetaData->stream_type = STREAM_TYPE_IMAADPCM;
            }
            else if (ptrThis->gwFormatTag == WAVE_FORMAT_PCM)
            {
                Decode_PCM_header(ptrThis, Ctx, i32ckSize);
                MetaData->stream_type = STREAM_TYPE_PCM;
            }
            else if (ptrThis->gwFormatTag == WAVE_FORMAT_EXTENSIBLE)
            {
                i32ret_val = Decode_WAVEFORMATEXTENSIBLE_header(ptrThis, Ctx, i32ckSize);
                if (i32ret_val < 0)
                    break;
                MetaData->stream_type = STREAM_TYPE_PCM;
            }
            else
            {
                i32ret_val = -1; // Unknown algo.
                break;
            }

            if (*i32ckSize != 0)
                i32ret_val = 2; // To skip remaining data

            break;

        case FACT_CKID:
            Ctx->cci_dec_read(0, (uint8_t *)&ptrThis->gwWavDuration, 4, 0, Ctx->user_data);
            i32ret_val = 0;
            break;

        case DATA_CKID:
            // store_file_offset(Ftell(i32Handle)); // Store the offset to be passed to the algo.
            MetaData->start_pos    = Ctx->cci_dec_tell(0, Ctx->user_data);
            ptrThis->datachunksize = *i32ckSize;
            MetaData->num_bytes    = *i32ckSize;
            i32ret_val             = 2; // To skip this chunk
            break;

        case LIST_CKID:
            // For LIST chunk, check nested chunk is present.
            // Look for 'I','N','F','O' chunk.
            i32ckIDval = DffiGetckID(ptrThis, Ctx);
            if (i32ckIDval == INFO_CKID)
            {
                *i32ckSize -= 4;
                i32ret_val = 1;
            }
            else
                i32ret_val = -1;
            break;

        case ISFT_CKID:
            if (*i32ckSize <= INTERNAL_BUF)
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, *i32ckSize, 0, Ctx->user_data);
            }
            else
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, INTERNAL_BUF, 0, Ctx->user_data);
                Ctx->cci_dec_seek(0, (*i32ckSize - INTERNAL_BUF), SEEK_CUR, Ctx->user_data);
            }
            i32ret_val = 0;
            break;

        case INAM_CKID:
            if (*i32ckSize <= INTERNAL_BUF)
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, *i32ckSize, 0, Ctx->user_data);
            }
            else
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, INTERNAL_BUF, 0, Ctx->user_data);
                Ctx->cci_dec_seek(0, (*i32ckSize - INTERNAL_BUF), SEEK_CUR, Ctx->user_data);
            }
            i32ret_val = 0;
            break;

        case IART_CKID:
            // If we get this chunk, then read in the song artist.
            memset((void *)Bufstring, 0, 30);
            if (*i32ckSize <= INTERNAL_BUF)
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, *i32ckSize, 0, Ctx->user_data);
            }
            else
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, INTERNAL_BUF, 0, Ctx->user_data);
                Ctx->cci_dec_seek(0, (*i32ckSize - INTERNAL_BUF), SEEK_CUR, Ctx->user_data);
            }
            // changed from if(MetaData->xxx) to if(MetaData). It is struct, so MetaData->xxx is always nonzero
            if (MetaData)
            {
                memcpy((uint8_t *)MetaData->song_artist, Bufstring, imax_length);
            }
            i32ret_val = 0;
            break;

        case DISP_CKID:
            // If we get this chunk, then read in the song title.
            memset((void *)Bufstring, 0, 30);
            Ctx->cci_dec_read(0, (uint8_t *)Bufstring, 4, 0, Ctx->user_data);
            if (*i32ckSize <= INTERNAL_BUF)
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, *i32ckSize, 0, Ctx->user_data);
            }
            else
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, INTERNAL_BUF, 0, Ctx->user_data);
                Ctx->cci_dec_seek(0, (*i32ckSize - INTERNAL_BUF), SEEK_CUR, Ctx->user_data);
            }
            // changed from if(MetaData->xxx) to if(MetaData). It is struct, so MetaData->xxx is always nonzero
            if (MetaData)
            {
                memcpy(MetaData->song_title, Bufstring, imax_length);
            }
            i32ret_val = 0;
            break;

        case IGNR_CKID:
            // If we get this chunk, then read in the song genre.
            memset((void *)Bufstring, 0, 30);
            if (*i32ckSize <= INTERNAL_BUF)
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, *i32ckSize, 0, Ctx->user_data);
            }
            else
            {
                Ctx->cci_dec_read(0, (uint8_t *)Bufstring, INTERNAL_BUF, 0, Ctx->user_data);
                Ctx->cci_dec_seek(0, (*i32ckSize - INTERNAL_BUF), SEEK_CUR, Ctx->user_data);
            }
            // changed from if(MetaData->xxx) to if(MetaData). It is struct, so MetaData->xxx is always nonzero
            if (MetaData)
            {
                memcpy((uint8_t *)MetaData->song_genre, Bufstring, imax_length);
            }
            i32ret_val = 0;
            break;

        case ICRD_CKID:
            // If we get this chunk, then read in the song creation date, and store off the year.
            memset((void *)Bufstring, 0, 30);
            Ctx->cci_dec_read(0, (uint8_t *)Bufstring, *i32ckSize, 0, Ctx->user_data);
            // changed from if(MetaData->xxx) to if(MetaData). It is struct, so MetaData->xxx is always nonzero
            if (MetaData)
            {
                memcpy((uint8_t *)MetaData->song_year, Bufstring, YEAR_SIZE);
            }
            i32ret_val = 0;
            break;

        default:
            i32ret_val = 0;
            break;
    }
    return i32ret_val;
}

/////////////////////////////////////////////////////////////////////////////////
//!
//!   \brief     Find the chunk id

//!
//!   \param[in] i32Handle  File Handle of newly opened file.
//!   \param[in] ptrThis  Pointer to WAVE
//!
//!   \return  ckid
//!
////////////////////////////////////////////////////////////////////////////////

int32_t DffiGetckID(WAVDEC *ptrThis, CCI_Ctx *Ctx)
{
    int32_t i;
    uint32_t word;
    // Default to unknown
    int32_t ckid = UNKNOWN_CKID;

    Ctx->cci_dec_read(0, (uint8_t *)&word, 4, 0, Ctx->user_data);

    for (i = 0; i < TOTAL_CKID; i++)
    {
        if (word == ckidlist_dffi[i])
        {
            ckid = i;
            break;
        }
    }
    return ckid;
}

/////////////////////////////////////////////////////////////////////////////////
//!
//!  \brief      Processes Fmt header (common part)
//!
//!
//!   \param[in] i32Handle  File Handle of newly opened file.
//!   \param[in] ptrThis  Pointer to WAVE
//!
//!   \note       Can be overlaid
////////////////////////////////////////////////////////////////////////////////
//

void DffiProcess_Common_Fmt_Header(WAVDEC *ptrThis, CCI_Ctx *Ctx, int32_t *i32ckSize)
{
    unsigned short data[14];
    Ctx->cci_dec_read(0, (uint8_t *)data, 2 * 7, 0, Ctx->user_data);

    ptrThis->gwFormatTag      = data[0];
    ptrThis->gwChannels       = data[1];
    ptrThis->gwSamplesPerSec  = (((unsigned int)data[3]) << 16) | data[2];
    ptrThis->gwAvgBytesPerSec = (((unsigned int)data[5]) << 16) | data[4];
    ptrThis->gwBlockAlign     = data[6];
    *i32ckSize -= 14;
}

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}
