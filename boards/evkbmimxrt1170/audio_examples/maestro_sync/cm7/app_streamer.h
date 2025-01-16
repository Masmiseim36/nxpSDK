/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_STREAMER_H_
#define _APP_STREAMER_H_

/* streamer library includes. */
#include "streamer_api.h"
#include "streamer_element_properties.h"

#ifdef VIT_PROC
#include "vit_proc.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Maestro network audio buffer size */
#define AUDIO_BUFFER_SIZE (30 * 1024)

/*! @brief Maestro Streamer interface structure */
typedef struct _streamer_handle_t
{
    STREAMER_T *streamer;
    volatile bool audioPlaying;
} streamer_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the Maestro streamer interface
 *
 * This function initializes the Maestro streamer library, and initializes the PCM
 * output interface.  This function should be called before creating a streamer
 * handle.
 */
void STREAMER_Init(void);

/*!
 * @brief Create a Maestro streamer interface handle
 *
 * This function creates a Maestro streamer interface and starts a task.
 *
 * @param handle Pointer to input handle.
 * @param noSDCard Determines whether the VoiceSeeker output will be saved to the SD card or not.
 * @return kStatus_Success on success, otherwise an error.
 */
status_t STREAMER_Create(streamer_handle_t *handle, bool noSDCard);

status_t STREAMER_SetDebug(streamer_handle_t *handle, bool set_debug);

/*!
 * @brief Destroy an Maestro streamer interface handle
 *
 * This function destroys an Maestro streamer interface and frees associated memory.
 *
 * @param handle Pointer to input handle
 */
void STREAMER_Destroy(streamer_handle_t *handle);

/*!
 * @brief Start audio playback for the streamer interface
 *
 * This function puts the streamer in a playing state, and begins pulling data
 * from the internal ring buffer, filled with calls to STREAMER_Start.
 *
 * @param handle Pointer to input handle
 */
void STREAMER_Start(streamer_handle_t *handle);

/*!
 * @brief Stop audio playback for the streamer interface
 *
 * This function puts the streamer in a stopped state, and ends playback from
 * the audio buffer.  The internal audio buffer is cleared of any data.
 *
 * @param handle Pointer to input handle
 */
void STREAMER_Stop(streamer_handle_t *handle);

/*!
 * @brief Check if streamer interface is playing
 *
 * This function returns true/false of the playing state for the interface
 *
 * @param handle Pointer to input handle
 * @return true if playing, false if not
 */
bool STREAMER_IsPlaying(streamer_handle_t *handle);

#if defined(__cplusplus)
}
#endif

#endif
