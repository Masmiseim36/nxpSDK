/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*! @file cci_metadata.h
 *  @brief Contains header data related to extracting metada for the codec interface subsystem.
 */
#ifndef _CCI_METADATA_H
#define _CCI_METADATA_H

/*
 * MetaData definitions
 */
#include "codec_interface_public_api.h"
#include "audio_cfg.h"
#include "errordefs.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Definitions
 */
#define MAX_PTP_STRING_CHARS       64 /*!< @brief Maximum ptp string chars */
#define MAX_PTP_STRING_CHARS_GENRE 34 /*!< @brief Maximum ptp string chars genre */
#define MAX_PTP_STRING_CHARS_YEAR  5  /*!< @brief Maximum ptp string chars year */

#define META_DATA_FOUND              (WORD)(0)     /*!< @brief Metadata was found.*/
#define META_DATA_NOT_FOUND          (WORD)(0x100) /*!< @brief Metadata was not found.*/
#define META_DATA_FILE_NOT_SUPPORTED (WORD)(0x101) /*!< @brief File type is not supported.*/
#define META_DATA_FILEOPEN_ERROR     (WORD)(0x200) /*!< @brief An error occurred while opening the file.*/
#define META_DATA_FILEREAD_ERROR     (WORD)(0x201) /*!< @brief An error occurred while reading the file.*/
#define META_DATA_FILECLOSE_ERROR    (WORD)(0x202) /*!< @brief An error occurred while closing the file.*/
#define META_DATA_FILESEEK_ERROR     (WORD)(0x203) /*!< @brief An error occurred while seeking the file.*/
#define META_DATA_NOT_VBR_FORMAT     (WORD)(0x102) /*!< @brief Data is not in VBR Format */
#define VBR_NOT_DETERMINED           0             /*!< @brief VBR Not determined */
#define VBR_TRUE                     1             /*!< @brief VBR True Value */
#define XING_TOC_SIZE                100           /*!< @brief Xing Size */
#define VBR_TOC_SIZE                 200           /*!< @brief VBR TOC Size */
#define VBR_TOC_SIZE_IN_BYTES        200 * 4       /*!< @brief VBR TOC Size in bytes */

#ifndef TITLE_SIZE
#define TITLE_SIZE       CASCFG_TRACK_TITLE_LEN /*!< @brief Title size */
#define TITLE_SIZE_BYTES (TITLE_SIZE * 2)       /*!< @brief Title size in bytes*/
#endif

#ifndef ARTIST_SIZE
#define ARTIST_SIZE       CASCFG_TRACK_ARTIST_LEN /*!< @brief Artist size */
#define ARTIST_SIZE_BYTES (ARTIST_SIZE * 2)       /*!< @brief Artist size in bytes*/
#endif

#ifndef ALBUM_SIZE
#define ALBUM_SIZE       CASCFG_TRACK_ALBUM_LEN /*!< @brief Album size */
#define ALBUM_SIZE_BYTES (ALBUM_SIZE * 2)       /*!< @brief Album size in bytes*/
#endif

#ifndef GENRE_SIZE
#define GENRE_SIZE       CASCFG_TRACK_GENRE_LEN /*!< @brief Genre size */
#define GENRE_SIZE_BYTES (GENRE_SIZE * 2)       /*!< @brief Genre size in bytes*/
#endif

#ifndef YEAR_SIZE
#define YEAR_SIZE       CASCFG_TRACK_YEAR_LEN /*!< @brief YYYY + NULL terminator */
#define YEAR_SIZE_BYTES (YEAR_SIZE * 2)       /*!< @brief Year size in bytes */
#endif

#ifndef TRACKNUM_SIZE
#define TRACKNUM_SIZE 5 /*!< @brief assume track number is smaller than 0xfff (12bit or 4095)*/
#endif

#ifndef DATE_SIZE
#define DATE_SIZE       MAX_PTP_SHORT_STRING_CHARS /*!< @brief Date size */
#define DATE_SIZE_BYTES (DATE_SIZE * 2)            /*!< @brief Date size in bytes*/
#endif

#ifndef ENCODINGPROFILE_SIZE
#define ENCODINGPROFILE_SIZE       MAX_PTP_SHORT_STRING_CHARS /*!< @brief Encoding profile size */
#define ENCODINGPROFILE_SIZE_BYTES (ENCODINGPROFILE_SIZE * 2) /*!< @brief Encoding profile size in bytes*/
#endif

/*
 * Structure
 */

/*!
 * @brief Enum Codec bitrate type
 */
typedef enum _codec_bitrate_type
{
    CONSTANT_BITRATE = 0, /*!< @brief 0 = Constant BitRate*/
    VARIABLE_BITRATE,     /*!< @brief 1 = Variable BitRate*/
    BITRATE_TYPE_UNKNOWN, /*!< @brief 2 = Unknown BitRate Type*/
} codec_bitrate_type_t;

/*!
 * @brief CCI Context Structure
 */
typedef struct
{
    void *user_data; /*!< @brief Holds file pointer (in case of direct file interface)
                      or CCIDecInfo context (in case of streamer interface)*/
    int32_t (*cci_dec_read)(
        int32_t, uint8_t *, int32_t, int32_t, void *); /*!< @brief CCI interface callback API for read operation*/
    int32_t (*cci_dec_seek)(int32_t,
                            int32_t,
                            int32_t,
                            void *);          /*!< @brief CCI interface callback API for seek operation*/
    int32_t (*cci_dec_tell)(int32_t, void *); /*!< @brief CCI interface callback API for file position operation*/
} CCI_Ctx;

/*!
 * @brief File Metadata Structure
 */
typedef struct
{
    audio_stream_type_t stream_type;   /*!< @brief Mp3, Wma, etc.*/
    uint8_t song_title[TITLE_SIZE];    /*!< @brief Title of Song */
    uint8_t song_artist[ARTIST_SIZE];  /*!< @brief Artist of Song */
    uint8_t song_album[ALBUM_SIZE];    /*!< @brief Album of Song */
    uint8_t song_genre[GENRE_SIZE];    /*!< @brief ptr to buffer to store genre string */
    uint8_t song_year[YEAR_SIZE];      /*!< @brief ptr to buffer to store album year, in
                                            UTF16 as yyyy (ex. 1969) */
    uint8_t genre;                     /*!< @brief Genre of audio file -- if MP3, use
                                                 the ID3v1 tag byte */
    uint32_t track_number;             /*!< @brief Track number of album/playlist */
    uint32_t sample_rate;              /*!< @brief Sample Rate of song (Hz) */
    int32_t num_channels;              /*!< @brief Number of channels */
    uint32_t bit_rate;                 /*!< @brief Bitrate of song (for MP3 VBR, this
                                                  will be first bitrate found) */
    uint32_t avg_bit_rate;             /*!< @brief Avg bitrate in VBR song (if 0,
                                            then it was not known) */
    uint32_t total_time;               /*!< @brief Length of song in seconds */
    uint32_t start_pos;                /*!< @brief Position where audio starts.
                                                 (if required by the decoder) */
    uint32_t num_bytes;                /*!< @brief Total Bytes in audio part of song */
    codec_bitrate_type_t bitrate_type; /*!< @brief Bitrate Type */
    bool valid_vbr_header;             /*!< @brief Is the VBR header valid */
    int32_t file_handle;               /*!< @brief Fast open key */
    int32_t bits_per_sample;           /*!< @brief AIFF decoder */
    uint32_t audio_sub_type;           /*!< @brief Audio Subtype */
    bool has_janus_drm;                /*!< @brief indicates whether wma song is
                                                 DRM or not */
    int32_t mpeg_layer;                /*!< @brief Mpeg Layer */
    bool no_audio;                     /*!< @brief No Audio */
    int32_t is_ogg_flac;               /*!< @brief flag to identify OGG FLAC file */
} file_meta_data_t;

/*! @brief CCI extract metadata top level function for direct file interface.
 *
 *
 * @param[in] file_extension             File Extension.
 * @param[in] meta_data                 meta_data extracted from the file.
 * @param[in] ctx           	        Pointer to CCI Context structure.
 *
 * @retval CODEC_SUCCESS                if meta_data found successfully.
 * @retval CODEC_METADATA_FILETYPE_FAILED if meta_data not found
 */
int32_t cci_extract_meta_data(uint32_t file_extension, file_meta_data_t *meta_data, CCI_Ctx *ctx);

/*! @brief CCI extract metadata top level function for streamer interface.
 *
 *
 * @param[in] streamType                Audio Stream Type.
 * @param[in] meta_data                 meta_data extracted from the file.
 * @param[in] ctx           	        Pointer to CCI Context structure.
 *
 * @retval CODEC_SUCCESS                if meta_data found successfully.
 * @retval CODEC_METADATA_FILETYPE_FAILED if meta_data not found
 */
int32_t ccidec_extract_meta_data(audio_stream_type_t streamType, file_meta_data_t *meta_data, CCI_Ctx *ctx);

/**
 * @brief Extract the meta data from the mp3 file.
 *
 * @param meta_data Metadata extracted from the file.
 * @param Ctx       Pointer to CCI Context structure.
 * @return int32_t
 */
extern int32_t codec_extract_metadata_mp3(file_meta_data_t *meta_data, CCI_Ctx *Ctx);
/**
 * @brief Extract the meta data from the wave file.
 *
 * @param meta_data Metadata extracted from the file.
 * @param Ctx       Pointer to CCI Context structure.
 * @return int32_t
 */
extern int32_t codec_extract_metadata_wave(file_meta_data_t *meta_data, CCI_Ctx *Ctx);
/**
 * @brief Extract the meta data from the aac file.
 *
 * @param meta_data Metadata extracted from the file.
 * @param Ctx       Pointer to CCI Context structure.
 * @return int32_t
 */
extern int32_t codec_extract_metadata_aac(file_meta_data_t *meta_data, CCI_Ctx *Ctx);
/**
 * @brief Extract the meta data from the flac file.
 *
 * @param meta_data Metadata extracted from the file.
 * @param Ctx       Pointer to CCI Context structure.
 * @return int32_t
 */
extern int32_t codec_extract_metadata_flac(file_meta_data_t *MetaData, CCI_Ctx *Ctx);
/**
 * @brief Extract the meta data from the wma file.
 *
 * @param meta_data Metadata extracted from the file.
 * @param Ctx       Pointer to CCI Context structure.
 * @return int32_t
 */
extern int32_t codec_extract_metadata_wma(file_meta_data_t *meta_data, CCI_Ctx *Ctx);
/**
 * @brief Extract the meta data from the ogg file.
 *
 * @param meta_data Metadata extracted from the file.
 * @param Ctx       Pointer to CCI Context structure.
 * @return int32_t
 */
extern int32_t codec_extract_metadata_ogg(file_meta_data_t *meta_data, CCI_Ctx *Ctx);
extern int32_t codec_extract_metadata_opus(file_meta_data_t *meta_data, CCI_Ctx *ctx);
#ifdef OGG_OPUS_DEC
/**
 * @brief Extract the meta data from the ogg opus file.
 *
 * @param meta_data Metadata extracted from the file.
 * @param Ctx       Pointer to CCI Context structure.
 * @return int32_t
 */
extern int32_t codec_extract_metadata_ogg_opus(file_meta_data_t *meta_data, CCI_Ctx *Ctx);
#endif
#if defined(__cplusplus)
}
#endif

#endif /* _CCI_METADATA_H */
