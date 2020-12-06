/*
 * Copyright 2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef SLN_AFE_H
#define SLN_AFE_H

#include <stddef.h>
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AFE_MEM_SIZE_2MICS	(134144U)
#define AFE_MEM_SIZE_3MICS	(172032U)

typedef enum _sln_afe_states
{
    kAfeUnsupported        = -5,
    kAfeMemError           = -4,
    kAfeOutOfMemory        = -3,
    kAfeNullPointer        = -2,
    kAfeFail               = -1,
    kAfeSuccess            = 0
} sln_afe_states_t;

typedef struct _sln_afe_configuration_params
{
    uint16_t postProcessedGain;     // The amount of dynamic gain after processing. This will not causing clipping */
    uint8_t numberOfMics;           // The number of microphones. Only 2/3 microphones are supported */
    uint8_t *afeMemBlock;           // Private heap for the AFE */
    uint32_t afeMemBlockSize;       // Size of the private heap */
}sln_afe_configuration_params_t;

typedef void *(*afe_malloc_func_t)(size_t size);

/*******************************************************************************
 * Declarations
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/*!
 * @brief Initialize audio front end engine
 * @param memPool     Application layer reference to AFE memory
 * @param malloc_func Application specific memory allocation function pointer
 * @param afeConfig   Reference to afe configuration used by application
 * @returns           Initialization status
 */
int32_t SLN_AFE_Init(uint8_t **memPool, afe_malloc_func_t malloc_func, sln_afe_configuration_params_t *afeConfig);

/*!
 * @brief AFE Audio processing function
 * @param memPool        Application layer reference to AFE memory
 * @param audioBUff      Input audio capture buffer for audio front end processing
 * @param refSignal      Reference signal buffer containing audio playback signal for barge-in
 * @param processedAudio Audio output signal processed by audio font end
 * @returns              Processing status
 */
int32_t SLN_AFE_Process_Audio(uint8_t **memPool, int16_t *audioBuff, int16_t *refSignal, uint8_t *processedAudio);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /* SLN_AFE_H */
