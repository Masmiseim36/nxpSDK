/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "dsp_xaf.h"
#include "session.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

bool session_init(session_t *session)
{
    int status;

    status = xos_mutex_create(&session->lock, XOS_MUTEX_WAIT_PRIORITY, 0);
    if (status != XOS_OK)
    {
        DSP_PRINTF("[session] failed to create mutex\r\n");
        return false;
    }

    session->frame_list = frame_list_create(FRAME_LIST_SIZE);
    if (!session->frame_list)
    {
        DSP_PRINTF("[session] buffer list allocation failed\r\n");
        xos_mutex_delete(&session->lock);
        return false;
    }

    session->frames_dropped = 0U;
    session->frames_played  = 0U;
    session->last_law       = XA_G711_LAW_A;
    session->underrun_count = 0U;

    session_reset(session);

    return true;
}

void session_reset(session_t *session)
{
    session->ssrc_id             = 0U;
    session->buffering_countdown = BUFFERING_FRAMES;
}

static void frame_list_clear(dsp_handle_t *dsp, frame_list_t *list)
{
    rtp_header_t *dealloc_packet;
    rtp_frame_t *frame;

    while (!frame_list_is_empty(list))
    {
        frame          = frame_list_first(list);
        dealloc_packet = frame->dealloc_packet;
        frame_list_remove_first(list);

        if (dealloc_packet != NULL)
        {
            dsp_xaf_packet_consumed(dsp, dealloc_packet);
        }
    }

    list->last_played_timestamp = 0U;
    list->last_played_set       = false;
}

session_t *session_get(dsp_handle_t *dsp, uint32_t ssrc_id)
{
    static uint32_t next = 0U;

    session_t *session = NULL;
    int i;

    /* Find if session with given ssrc_id is already allocated */
    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        if (ssrc_id == dsp->sessions[i].ssrc_id)
        {
            return &dsp->sessions[i];
        }
    }

    /* If not find free session */
    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        if (dsp->sessions[i].ssrc_id == 0U)
        {
            session          = &dsp->sessions[i];
            session->ssrc_id = ssrc_id;
            return session;
        }
    }

    /* If no free session, find the one allocated for longest time and reuse it */
    session = &dsp->sessions[next];
    next    = ((next + 1U) % MAX_INPUT_STREAMS);

    session_lock(session);
    frame_list_clear(dsp, session->frame_list);
    session_reset(session);
    session_unlock(session);

    return session;
}

void session_add_packet(dsp_handle_t *dsp, session_t *session, rtp_header_t *packet, uint32_t size)
{
    bool added;

    session_lock(session);
    added = frame_list_add(session->frame_list, packet, size);
    if (session->buffering_countdown > 0U)
    {
        session->buffering_countdown--;
    }
    session_unlock(session);

    if (!added)
    {
        /*
         * buffer_list is full, will not fit all frames from the packet
         * or frame timestamp is from already played - release packet
         */
        dsp_xaf_packet_consumed(dsp, packet);
    }
}

void session_lock(session_t *session)
{
    xos_mutex_lock(&session->lock);
}

void session_unlock(session_t *session)
{
    xos_mutex_unlock(&session->lock);
}
