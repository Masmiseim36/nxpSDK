/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SESSION_H__
#define __SESSION_H__

#include <stdint.h>

#include "dsp_xaf.h"
#include "frame_list.h"
#include "rtp.h"
#include "xa_g711_codec_api.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief RTP session context */
typedef struct _session
{
    /**
     * @brief Lock
     */
    XosMutex lock;

    /**
     * @brief Synchronization source identifier
     */
    volatile uint32_t ssrc_id;

    /**
     * @brief List of buffered frames for playback
     */
    frame_list_t *frame_list;

    /**
     * @brief Countdown of received frames before playback can start
     */
    volatile uint32_t buffering_countdown;

    /**
     * @brief Number of dropped frames.
     * Frames dropped after buffer is completely empty are not counted.
     * This does not get reset - it is counted for all sessions this session object ever pointed to.
     */
    uint32_t frames_dropped;

    /**
     * @brief Number of played frames.
     * This does not get reset - it is counted for all sessions this session object ever pointed to.
     */
    uint32_t frames_played;

    /**
     * @brief G.711 of the last played frame of the last sesion.
     * This does not get reset.
     */
    xa_g711_law_t last_law;

    /**
     * @brief Number of times session object frame list was empty during playback.
     * This happens either at larger drop of data or at the end of session stream (where it is ok).
     * This does not get reset - it is counted for all sessions this session object ever pointed to.
     */
    uint32_t underrun_count;
} session_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/**
 * @brief    Initializes session object
 *
 * @param    session pointer to session object to initialize
 *
 * @returns  if succeeded or failed
 */
bool session_init(session_t *session);

/**
 * @brief    Resets session object
 *
 * @param    session pointer to session object to reset
 */
void session_reset(session_t *session);

/**
 * @brief Returns session with the given SSRC ID or (re)initializes old one
 *
 * @param dsp DSP context handle, containing list of sessions
 * @param ssrc_id RTP SSRC ID
 *
 * @returns session object pointer
 */
session_t *session_get(dsp_handle_t *dsp, uint32_t ssrc_id);

/**
 * @brief Adds frames from the given RTP packet to the session buffer
 *
 * @param dsp DSP context handle
 * @param session session object pointer
 * @param packet RTP packet
 *
 * @returns size size of packet including RTP header
 */
void session_add_packet(dsp_handle_t *dsp, session_t *session, rtp_header_t *packet, uint32_t size);

/*!
 * @brief    Locks the session from concurrent access
 *
 * @param    session pointer to session object to lock
 */
void session_lock(session_t *session);

/*!
 * @brief    Unocks the session for concurrent access
 *
 * @param    session pointer to session object to unlock
 */
void session_unlock(session_t *session);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __SESSION_H__ */
