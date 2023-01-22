/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

////////////////////////////////////////////////////////////////////////////////
//! \addtogroup asi_dffi
//! @{
//
// Copyright (c)
//
//! \file asi_dffi_flac_extractmetadata.c
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
#include <error.h>
#include "codec_interface.h"
#include "cci_metadata.h"
#include <stdlib.h> // atoi()

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////
#define FLAC_FILE_IDENTIFIER 0x43614C66 // "fLaC"<--
#define MAX_CHANNELS         2
#if !defined(ERROR)
#define ERROR -1
#endif

extern int8_t txt_ConvertUtf8ToUtf16c(char *ptr, uint16_t *pUnicodeValue);

#define MAX_STRING_LEN 255
#define TOTAL_COMMENTS 15

////////////////////////////////////////////////////////////////////////////////
//    local function prototypes
int CheckFlacId(CCI_Ctx *Ctx, file_meta_data_t *MetaData);
int FLACReadMetadata(CCI_Ctx *Ctx, file_meta_data_t *MetaData);
int FLACReadStreamInfoMetadata(CCI_Ctx *Ctx, file_meta_data_t *MetaData, uint32_t u32Length);
int FLACReadVorbisCommentMetadata(CCI_Ctx *Ctx, file_meta_data_t *MetaData, uint32_t u32Length);
int32_t GetFlacBytes(CCI_Ctx *Ctx, int32_t i32BytesToRead, uint8_t *pBuffer)
{
    int32_t i32ReturnValue = 0;
    if (Ctx->cci_dec_read(0, pBuffer, i32BytesToRead, 0, Ctx->user_data) == i32BytesToRead)
    {
        switch (i32BytesToRead)
        {
            case 4:
                i32ReturnValue |= (*pBuffer++) << 24;
            case 3:
                i32ReturnValue |= (*pBuffer++) << 16;
            case 2:
                i32ReturnValue |= (*pBuffer++) << 8;
            case 1:
                i32ReturnValue |= (*pBuffer++);
                break;
        };
    }
    else
    {
        i32ReturnValue = -1;
    }

    return i32ReturnValue;
}

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
uint8_t flac_copy_one_byte(char *ptr, char *ptr1)
{
    *ptr1 = *ptr;
    return 1;
}

uint64_t BigNum_Divide_64_by_32_flac(uint64_t n, uint32_t base)
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

////////////////////////////////////////////////////////////////////////////////
//! \brief get file size Callback API for all metadata extraction.
//!
//! \fntype Function
//!
//! \param[in] *Ctx         Pointer to CCI Context structure
//!
//! \retval File Size.
////////////////////////////////////////////////////////////////////////////////
int32_t flac_GetFileSize(CCI_Ctx *Ctx)
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
//! \fntype Reentrant Function
//!
//! Extract the meta data from the file.
//!
//! \param[in]  file_handle Fast open key; this key allows the file to be quickly found and opened
//! \param[out]  *MetaData Metadata extracted from the file.
//!
//! \return Status of the call.
//! \retval SUCCESS
//! \retval ERROR_ASI_DFFI_EXTRACTMETADATA
//!
////////////////////////////////////////////////////////////////////////////////
int32_t codec_extract_metadata_flac(file_meta_data_t *MetaData, CCI_Ctx *Ctx)
{
    RtStatus_t Retcode = SUCCESS;

    if (Ctx->user_data != NULL)
        Ctx->cci_dec_seek(0, 0, SEEK_SET, Ctx->user_data);
    else
        return (CODEC_METADATA_NOT_FOUND);

    // Set some defaults
    MetaData->start_pos        = 0;
    MetaData->total_time       = 0;
    MetaData->valid_vbr_header = 0;
    MetaData->avg_bit_rate     = 0;
    MetaData->bit_rate         = 0;

    // check flac file id (native FLAC file type)
    if (SUCCESS == (Retcode = (RtStatus_t)CheckFlacId(Ctx, MetaData)))
    {
        MetaData->is_ogg_flac = 0; // FALSE;  /* native FLAC file */
        // read stream info metadata block
        if (SUCCESS == (Retcode = (RtStatus_t)FLACReadMetadata(Ctx, MetaData)))
        {
            MetaData->stream_type = STREAM_TYPE_FLAC;
            MetaData->start_pos   = Ctx->cci_dec_tell(0, Ctx->user_data);
            MetaData->num_bytes   = flac_GetFileSize(Ctx);
            MetaData->bit_rate    = (uint32_t)BigNum_Divide_64_by_32_flac(
                ((uint64_t)MetaData->num_bytes * 8 * 1000),
                MetaData->total_time); //(((uint64_t)MetaData->num_bytes*8*1000)/MetaData->total_time);
            MetaData->avg_bit_rate = MetaData->bit_rate;
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
        }
    }
    else
        Retcode = CODEC_METADATA_FILETYPE_FAILED;

    return (RtStatus_t)(Retcode);
}
////////////////////////////////////////////////////////////////////////////////
//! \brief Checks the headers for flac ID.
//!
//! \fntype internal Function
//!
//! Checks the headers for FLAC ID.
//!
//! \param[in]  iHandle: File handle that we're checking
//! \param[in]  *MetaData -- this structure will be filled in by the function
//! \retval SUCCESS or ERROR
//!
////////////////////////////////////////////////////////////////////////////////
int CheckFlacId(CCI_Ctx *Ctx, file_meta_data_t *MetaData)
{
    uint32_t u32Temp    = 0;
    int32_t uCurrentPos = Ctx->cci_dec_tell(0, Ctx->user_data);
    // file pointer is at valid position
    if (uCurrentPos != -1)
    {
        // read four bytes for identifier
        if (Ctx->cci_dec_read(0, (uint8_t *)&u32Temp, 4, 0, Ctx->user_data) == 4)
        {
            if (FLAC_FILE_IDENTIFIER == u32Temp)
            {
                // valid flac file
                return SUCCESS;
            }
            else if ((u32Temp & 0x00FFFFFF) == 0x334449) /* ID3 */
            {
                unsigned i, skip;
                /* skip the version and flags bytes */
                Ctx->cci_dec_seek(0, 2, SEEK_CUR, Ctx->user_data);
                /* get the size (in bytes) to skip */
                skip = 0;
                for (i = 0; i < 4; i++)
                {
                    if (Ctx->cci_dec_read(0, (uint8_t *)&u32Temp, 1, 0, Ctx->user_data) != 1)
                        return 0;
                    skip <<= 7;
                    skip |= (u32Temp & 0x7f);
                }
                /* skip the rest of the tag */
                Ctx->cci_dec_seek(0, skip, SEEK_CUR, Ctx->user_data);
                u32Temp = 0;
                Ctx->cci_dec_read(0, (uint8_t *)&u32Temp, 4, 0, Ctx->user_data);
                if (FLAC_FILE_IDENTIFIER == u32Temp)
                {
                    // valid flac file
                    return SUCCESS;
                }
            }
        }
    }
    return ERROR;
}
////////////////////////////////////////////////////////////////////////////////
//! \brief Read FLAC Metadata block header and calls metadata block data reader.
//!
//! \fntype internal Function
//!
//! Read FLAC Metadata block header and calls metadata block data reader.
//!
//! \param[in]  iHandle File handle that we're checking
//! \param[out]  *pMetaData Metadata extracted from the file.
//! \retval SUCCESS or !SUCCESS
//!
////////////////////////////////////////////////////////////////////////////////
int FLACReadMetadata(CCI_Ctx *Ctx, file_meta_data_t *pMetaData)
{
    int Retcode  = SUCCESS;
    bool is_last = 1;
    int32_t type, length = 0;
    int32_t u32Temp     = 0;
    int32_t uCurrentPos = Ctx->cci_dec_tell(0, Ctx->user_data);
    // file pointer is at valid position
    if (uCurrentPos != -1)
    {
        do
        {
            // read four bytes
            if ((u32Temp = GetFlacBytes(Ctx, 4, (uint8_t *)&uCurrentPos)) != -1)
            {
                // is this metadata block is last?
                is_last = (0x80000000 & u32Temp) >> 31;

                // metadata block type.
                type = (0x7F000000 & u32Temp) >> 24;

                // length
                length = 0x00FFFFFF & u32Temp;

                switch (type)
                {
                    case 0:
                        // stream info block
                        // stream metadata information must present.
                        Retcode = FLACReadStreamInfoMetadata(Ctx, pMetaData, length);
                        break;
                    case 4: // vorbis comment metadata block (or  flac Tag)
                        // No need to check Retcode for Comment metadata. comment metadata are optional.
                        FLACReadVorbisCommentMetadata(Ctx, pMetaData, length);
                        break;
                    // add more case here to support other metadata blocks of flac
                    default:
                        // we currently not reading other metadata blocks so skip till metadata blocks are finished
                        Ctx->cci_dec_seek(0, length, SEEK_CUR, Ctx->user_data);
                        break;
                };

                // No need to check Retcode for Comment metadata. comment metadata are optional.
                // stream metadata information must present.
                if (Retcode == ERROR)
                    return Retcode;
            }
            else
            {
                Retcode = ERROR;
                break;
            }

        } while (is_last == 0);
    }
    else
    {
        Retcode = ERROR;
    }

    return (Retcode);
}
////////////////////////////////////////////////////////////////////////////////
//! \brief Reads stream info metadata block
//!
//! \fntype internal Function
//!
//! Reads stream info metadata block
//!
//! \param[in]  iHandle: File handle that we're checking
//! \param[in/out]  *MetaData -- this structure will be filled in by the function
//! \param[in]  u32Length -- length of metadata block, to be read.
//! \retval SUCCESS or ERROR
//!
////////////////////////////////////////////////////////////////////////////////
int FLACReadStreamInfoMetadata(CCI_Ctx *Ctx, file_meta_data_t *MetaData, uint32_t u32Length)
{
    int32_t sample_rate;
    int32_t channels;
    int32_t bits_per_sample;
    int64_t total_samples, u64Temp, i32Temp2;
    int8_t md5sum[16];

    // skipping these fields as not used now
    if (GetFlacBytes(Ctx, 2, (uint8_t *)&u64Temp) == -1)
        return ERROR;
    if (GetFlacBytes(Ctx, 2, (uint8_t *)&u64Temp) == -1)
        return ERROR;
    if (GetFlacBytes(Ctx, 3, (uint8_t *)&u64Temp) == -1)
        return ERROR;
    if (GetFlacBytes(Ctx, 3, (uint8_t *)&u64Temp) == -1)
        return ERROR;

    //
    if ((total_samples = GetFlacBytes(Ctx, 4, (uint8_t *)&u64Temp)) == -1)
        return ERROR;

    u64Temp = total_samples;

    // sample rate is of 20 bits
    sample_rate = (0xFFFFF000 & u64Temp) >> 12;

    // channel is of 3 bits
    channels = (0x7 & (u64Temp >> 9)) + 1;

    // currently only 2 channels are supported.
    if (channels > MAX_CHANNELS)
        return ERROR;

    // bits per sample is of 5 bits
    bits_per_sample = (0x1F & (u64Temp >> 4)) + 1;

    total_samples = 0xF & u64Temp;

    if ((i32Temp2 = GetFlacBytes(Ctx, 4, (uint8_t *)&u64Temp)) == -1)
        return ERROR;
    // total samples are 36 bits wide
    total_samples = total_samples + i32Temp2;

    // read md5 sum
    if (Ctx->cci_dec_read(0, (uint8_t *)&md5sum, 16, 0, Ctx->user_data) != 16)
        return ERROR;

    // Update file metadata
    MetaData->sample_rate     = sample_rate;
    MetaData->num_channels    = channels;
    MetaData->bits_per_sample = bits_per_sample;
    MetaData->total_time      = (uint32_t)BigNum_Divide_64_by_32_flac(
        ((uint64_t)total_samples * 1000),
        sample_rate); //(((uint64_t)total_samples*1000) / sample_rate);// convert to mSec

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Reads vorbis comment metadata block
//!
//! \fntype internal Function
//!
//! Reads vorbis comment metadata block ( flac tag information )
//!
//! \param[in]  iHandle: File handle that we're checking
//! \param[in/out]  *MetaData -- this structure will be filled in by the function
//! \param[in]  u32Length -- length of metadata block, to be read.
//! \retval SUCCESS or ERROR
//!
////////////////////////////////////////////////////////////////////////////////
int FLACReadVorbisCommentMetadata(CCI_Ctx *Ctx, file_meta_data_t *pMetaData, uint32_t uLength)
{
    uint32_t u32LoopCounter, u32Length, u32CommentCount;
    // int64_t u64Temp;
    int shift_comments = 0;
    char *destLocal    = NULL;
    int *destPtr;
    int *ptr;
    char *p;
    int8_t i8LoopCnt;
    char strCommentString[MAX_STRING_LEN + 1];
    char *CmntTag[TOTAL_COMMENTS] = {"TITLE=", "TRACKNUMBER=", "ARTIST=", "ALBUM=", "GENRE=", "DATE="};

    /* read vendor string length*/
    if (GetFlacBytes(Ctx, 4, (uint8_t *)&u32Length) == -1)
        return ERROR;

    if (u32Length > 0)
    {
        if (u32Length < MAX_STRING_LEN)
        {
            // read vendor string text
            if (Ctx->cci_dec_read(0, (uint8_t *)&strCommentString, u32Length, 0, Ctx->user_data) != u32Length)
                return ERROR;

            strCommentString[u32Length] = '\0';
        }
        else
        {
            // read vendor string text
            if (Ctx->cci_dec_read(0, (uint8_t *)&strCommentString, MAX_STRING_LEN, 0, Ctx->user_data) != MAX_STRING_LEN)
                return ERROR;

            strCommentString[MAX_STRING_LEN] = '\0';

            // skip remaining data
            if (Ctx->cci_dec_seek(0, u32Length - MAX_STRING_LEN, SEEK_CUR, Ctx->user_data) != SUCCESS)
            {
                return ERROR;
            }
        }
    }
    else
        *strCommentString = 0;

    /* read num comments */
    if (GetFlacBytes(Ctx, 4, (uint8_t *)&u32CommentCount) == -1)
        return ERROR;

    /* read comments */
    if (u32CommentCount > 0)
    {
        for (u32LoopCounter = 0; u32LoopCounter < u32CommentCount; u32LoopCounter++)
        {
            // read comment length
            if (GetFlacBytes(Ctx, 4, (uint8_t *)&u32Length) == -1)
                return ERROR;

            if (u32Length > 0)
            {
                if (u32Length < MAX_STRING_LEN)
                {
                    // read comment string
                    if (Ctx->cci_dec_read(0, (uint8_t *)&strCommentString, u32Length, 0, Ctx->user_data) != u32Length)
                        return ERROR;

                    strCommentString[u32Length] = '\0';
                }
                else
                {
                    // read comment string text
                    if (Ctx->cci_dec_read(0, (uint8_t *)&strCommentString, MAX_STRING_LEN, 0, Ctx->user_data) !=
                        MAX_STRING_LEN)
                        return ERROR;

                    strCommentString[MAX_STRING_LEN] = '\0';

                    // skip remaining data
                    if (Ctx->cci_dec_seek(0, u32Length - MAX_STRING_LEN, SEEK_CUR, Ctx->user_data) != SUCCESS)
                    {
                        return ERROR;
                    }

                    u32Length = MAX_STRING_LEN;
                }

                for (i8LoopCnt = 0; i8LoopCnt < u32Length; i8LoopCnt++)
                {
                    if (strCommentString[i8LoopCnt] == '=')
                        break;

                    // strCommentString[i8LoopCnt] = (char)toupper(strCommentString[i8LoopCnt]);
                    p = (char *)(&strCommentString[i8LoopCnt]);
                    if ((*p >= 'a') && (*p <= 'z'))
                    {
                        *p -= ('a' - 'A');
                    }
                }

                // process vorbis comment read
                for (i8LoopCnt = 0; i8LoopCnt < TOTAL_COMMENTS; i8LoopCnt++)
                {
                    if ((char *)CmntTag[i8LoopCnt] == NULL)
                        continue;

                    ptr = (int *)strstr(strCommentString, (char *)CmntTag[i8LoopCnt]);

                    // check if we have an entry
                    if ((char *)ptr == strCommentString)
                    {
                        switch (i8LoopCnt)
                        {
                            case 0: // title
                            {
                                destPtr        = (int *)&(pMetaData->song_title);
                                destLocal      = (char *)destPtr;
                                shift_comments = 6;
                            }
                            break;
                            case 1:
                            {
                                shift_comments          = 12;
                                pMetaData->track_number = atoi(strCommentString + shift_comments);
                                destLocal               = NULL;
                            }
                            break;
                            case 2:
                            {
                                destPtr        = (int *)&(pMetaData->song_artist);
                                destLocal      = (char *)destPtr;
                                shift_comments = 7;
                            }
                            break;
                            case 3:
                            {
                                destPtr        = (int *)&(pMetaData->song_album);
                                destLocal      = (char *)destPtr;
                                shift_comments = 6;
                            }
                            break;
                            case 4:
                            {
                                destPtr        = (int *)&(pMetaData->song_genre);
                                destLocal      = (char *)destPtr;
                                shift_comments = 6;
                            }
                            break;
                            case 5:
                            {
                                shift_comments = 5;
                                if ((u32Length - shift_comments) > YEAR_SIZE)
                                    u32Length = YEAR_SIZE + shift_comments;
                                destPtr   = (int *)&(pMetaData->song_year);
                                destLocal = (char *)destPtr;
                            }
                            break;

                            default:
                                break;
                        } // end switch

                        if (destLocal) // test for null pointer
                        {
                            int jj = 0;

                            int comment_lengths8    = u32Length - shift_comments;
                            int uBytesConsumed      = 0;
                            int uBytesConsumedTotal = 0;
                            while (comment_lengths8)
                            {
                                uBytesConsumed = flac_copy_one_byte(
                                    (char *)((strCommentString + shift_comments) + uBytesConsumedTotal),
                                    (char *)(destLocal + jj));
                                jj += 1;
                                uBytesConsumedTotal += uBytesConsumed;
                                comment_lengths8 -= uBytesConsumed;
                            }
                            /* Null terminate the strings */
                            *((char *)(destLocal + jj)) = 0;

                        } // if(destLocal)
                        break;
                    } // if(ptr)
                }     // for(p=0;p<5;p++)
            }
            else
                *strCommentString = 0;
        }
    }

    return SUCCESS;
}
