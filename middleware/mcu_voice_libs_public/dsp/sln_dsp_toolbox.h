/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __SLN_DSP_TOOLBOX_H__
#define __SLN_DSP_TOOLBOX_H__

#include <stddef.h>
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum _sln_dsp_states
{
    kDspInvalidDevice	   = -8,
    kDspOutOfMemory        = -7,
    kDspStreamIDInUse      = -6,
    kDspNoStreamsAvailable = -5,
    kDspBitGrowthOverflow  = -4,
    kDspInvalidSteamID     = -3,
    kDspInvalidParam       = -2,
    kDspNullPointer        = -1,
    kDspSuccess            = 0
} sln_dsp_states_t;

typedef void *(*dsp_malloc_func_t)(size_t size);
typedef void (*free_func_t)(void* ptr);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/*!
 * @brief Initialization function for DSP toolbox; if NULL pointer allocates and configures memory pool:
 * @param **memPool [in/out]  Pointer to memory pool available for use by DSP toolbox
 * @param dsp_malloc_func         Function pointer to memory allocation function
 * @returns                   Status of initialization
*/
int32_t SLN_DSP_Init(uint8_t **memPool, dsp_malloc_func_t malloc_func);

/*!
 * @brief De-Initialization function for DSP toolbox; clears and de-allocates memory pool.
 * @param **memPool [in/out] Pointer to memory pool available for use by DSP toolbox
 * @param free_func          Function pointer to memory free function
 * @returns                  Status of initialization
*/
int32_t SLN_DSP_Deinit(uint8_t **memPool, free_func_t free_func);

/*!
 * @brief Set length in bytes to of PDM stream to process
 * @param **memPool [in/out] Pointer to memory pool available for use by DSP toolbox
 * @param lenBytes [in]      Number of PDM stream bytes to process
 * @returns                  Status of operation
 */
int32_t SLN_DSP_SetCaptureLength(uint8_t **memPool, uint32_t lenBytes);

/*!
 * @brief Get length of bytes that will be output from PDM to PCM conversion
 * @param **memPool [in/out] Pointer to memory pool available for use by DSP toolbox
 * @param *lenBytes [out]    Pointer to number of bytes that will be output
 * @returns                  Status
 */
int32_t SLN_DSP_GetConversionLength(uint8_t **memPool, uint32_t *lenBytes);

/*!
 * @brief Set gain factor to apply to PCM output signal
 * @param **memPool [in/out] Pointer to memory pool available for use by DSP toolbox
 * @param gainFactor [in]    Factor to multiply output signal (signal is left shifted by this ammount)
 * @returns                  Status
 */
int32_t SLN_DSP_SetGainFactor(uint8_t **memPool, int16_t gainFactor);

/*!
 * @brief Convert a buffer of PDM data into 16-bit data.
 * @param **memPool [in/out] Pointer to memory pool available for use by DSP toolbox
 * @param streamID [in]      Numeric ID for audio steam to perform conversion on
 * @param *in [in]           PDM data input as 8-bit pointer to allow filter changes
 * @param *out [out]         PCM output data
 * @returns                  Status of initialization
*/
int32_t SLN_DSP_pdm_to_pcm(uint8_t **memPool, uint32_t streamID, uint8_t *in, int16_t *out);

/*!
 * @brief Allocate a buffer for the PDM data that will be processed.
 * @param **memPool [in/out] Pointer to memory pool available for use by DSP toolbox
 * @param scratch [in/out]   A pointer to be allocated a scratch pad of data
 * @param malloc_func [in]   Pointer to the malloc function to allocate memory with
 * @returns                  Status of initialization
*/
int32_t SLN_DSP_create_scratch_buf(uint8_t **memPool, uint32_t **scratch, dsp_malloc_func_t malloc_func);

/*!
 * @brief Converts multiple audio stream from PDM to PCM using multiple channels.
 * @param **memPool [in/out] Pointer to memory pool available for use by DSP toolbox
 * @param firstStreamID [in] Stream ID used for the DC offset and downsampling parameters
 * @param numChannels [in]   Number of buffer channels to parse
 * @param *in [in]           Pointer to the PDM Input data
 * @param *out [out]         A pointer to the PCM buffer to convert the PDM into.
 * @returns                  Status of initialization
*/
int32_t SLN_DSP_pdm_to_pcm_multi_ch(uint8_t **memPool, uint32_t firstStreamID, uint32_t numChannels, uint32_t *in, int16_t *out, uint32_t *scratch);

/*!
 * @brief Down-sample PCM stream by factor of three.
 * @param **memPool [in/out]  Pointer to memory pool available for use by DSP toolbox
 * @param streamID [in]       Numeric ID for audio steam to perform conversion on
 * @param *in [in]            PCM input data
 * @param inSampleCount [in]  Number of samples to process from input buffer
 * @param *out [out]          PCM output data
 * @returns                   Status of initialization
*/
int32_t SLN_DSP_downsample_by_3(uint8_t **memPool, uint32_t streamID, int16_t *in, uint32_t inSampleCount, int16_t *out);


#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif // __SLN_DSP_TOOLBOX_H__
