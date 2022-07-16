/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CEI_H__
#define __CEI_H__

#include <stdint.h>
#include <stdio.h>

/**
 * @file    cei.h
 * @brief   Common encoder interface API prototypes
 *
 */

/**
 * @brief Common encoder error
 *
 */
#define CEI_ENCODER_ENCODEERR -1

/**
 * @brief Holds buffer information,
 */
typedef struct
{
    void *ptr;     /*!< @brief Buffer pointer */
    uint32_t size; /*!< @brief Buffer length */
} BufferInfo;

/**
 * @brief Defines a function returning a memory size to be allocated.
 *
 * @param channels Number of channels, which the encoder will encode.
 * @return The number of bytes to be allocated.
 */
typedef int (*CeiFnGetMemorySize)(uint8_t channels);

/**
 * @brief Defines a function initializing an encoder.
 *
 * @param memory Pointer to memory previously allocated to hold encoder structures.
 * @param sampleRate The source sample rate.
 * @param channels Number of channels which the encoder will encode.
 *
 * @returns 0 if encoder was initialized successfully, anything other otherwise.
 */
typedef int (*CeiFnEncoderInit)(void *memory, uint32_t sampleRate, uint8_t channels);

/**
 * @brief Defines a function retrieving encoder configuration.
 *
 * @param memory Pointer to previously allocated memory with encoder structures. Can be NULL if default configuration is
 * desired.
 * @param config Pointer to an encoder-specific configuration structure.
 *
 * @returns 0 if configuration gathered successfully, anything other otherwise.
 */
typedef int (*CeiFnEncoderGetConfig)(void *memory, void *config);

/**
 * @brief Defines a function applying configuration to an encoder.
 *
 * @param memory Pointer to previously allocated memory with encoder structures.
 * @param config Pointer to an encoder-specific configuration structure.
 *
 * @returns 0 if configuration applied successfully, anything other otherwise.
 */
typedef int (*CeiFnEncoderSetConfig)(void *memory, void *config);

/**
 * @brief Defines a function which encodes data in a given input buffer to a given output buffer. Input buffer will
 * contain 16-bit signed interleaved little-endian samples (s16le).
 *
 * @param memory Pointer to previously allocated memory with encoder structures.
 * @param source Contains information about the input buffer.
 * @param dest Contains information about the output buffer.
 *
 * @returns Number of produced bytes or CEI_ENCODER_ENCODEERR. 0 constitutes an EOS condition.
 */
typedef int (*CeiFnEncode)(void *memory, BufferInfo *source, BufferInfo *dest);

/**
 * @brief Common encoder interface function pointers
 *
 */
typedef struct
{
    CeiFnGetMemorySize fnGetMemorySize; /*!< @brief Get memory size function pointer */
    CeiFnEncoderInit fnInit;            /*!< @brief Init function pointer */
    CeiFnEncoderGetConfig fnGetConfig;  /*!< @brief Get config function pointer */
    CeiFnEncoderSetConfig fnSetConfig;  /*!< @brief Set config function pointer */
    CeiFnEncode fnEncode;               /*!< @brief Encode function pointer */
} CeiEncoderFunctions;

#endif
