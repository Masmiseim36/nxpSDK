/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CEI_ENCTYPES_H__
#define __CEI_ENCTYPES_H__

/**
 * @file    cei_enctypes.h
 * @brief   Basic common encoder interface types, information, configuration structures
 *
 */

/**
 * @brief Encoder types
 *
 */
typedef enum
{
    CEIENC_OPUS,
    CEIENC_LAST
} CeiEncoderType;

/**
 * @brief Bitstream info
 *
 */
typedef struct
{
    uint32_t sample_rate; /*!< @brief Bitstream sample rate */
    uint8_t num_channels; /*!< @brief Bitstream number of channels */
    uint8_t endian;       /*!< @brief Bitstream endianness */
    uint8_t sign;         /*!< @brief Bitstream sign */
    uint8_t sample_size;  /*!< @brief Bitstream sample size */
    uint8_t interleaved;  /*!< @brief Bitstream interleaved */
} CeiBitstreamInfo;

/**
 * @brief Opus configuration
 *
 */
typedef struct
{
    int32_t complexity;          /*!< @brief Opus complexity */
    int32_t bitrate;             /*!< @brief Opus bitrate */
    int32_t vbr;                 /*!< @brief Opus variable bit rate */
    int32_t vbrConstraint;       /*!< @brief Opus VBR constraint */
    int32_t forceChannels;       /*!< @brief Opus force channels */
    int32_t maxBandwidth;        /*!< @brief Opus maximum bandwidth */
    int32_t bandwidth;           /*!< @brief Opus bandwidth */
    int32_t application;         /*!< @brief Opus application */
    int32_t inbandFec;           /*!< @brief Opus inband FEC */
    int32_t packetLossPerc;      /*!< @brief Opus packet loss percent for FEC */
    int32_t dtx;                 /*!< @brief Opus discontinuous transmission */
    int32_t lsbDepth;            /*!< @brief Opus depth of signal being encoded */
    int32_t expertFrameDuration; /*!< @brief Opus variable duration frames */
    int32_t predictionDisabled;  /*!< @brief Opus prediction disabled */
} CeiOpusConfig;

#endif
