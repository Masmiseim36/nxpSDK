/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************
 * xa-class-mixer.c
 *
 * Generic mixer component class
 ******************************************************************************/

#define MODULE_TAG                      MIXER

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"
#include "xa-class-base.h"
#include "audio/xa-mixer-api.h"

/*******************************************************************************
 * Data structures
 ******************************************************************************/

/* ...mixed source - input data */
typedef struct XATrack
{
    /* ...input port data */
    xf_input_port_t     input;

    /* ...current presentation timestamp (in samples; local to a mixer state) */
    UWORD32                 pts;

    /* ...total amount of decoded frames since last synchronization point */
    UWORD32                 decoded;

    /* ...total amount of rendered frames (consumed) since last synchronization point */
    UWORD32                 rendered;

}   XATrack;

/*******************************************************************************
 * Helpers
 ******************************************************************************/

static inline UWORD32 xa_track_test_flags(XATrack *track, UWORD32 flags)
{
    return (track->input.flags & flags);
}

static inline UWORD32 xa_track_set_flags(XATrack *track, UWORD32 flags)
{
    return (track->input.flags |= flags);
}

static inline UWORD32 xa_track_clear_flags(XATrack *track, UWORD32 flags)
{
    return (track->input.flags &= ~flags);
}

static inline UWORD32 xa_track_toggle_flags(XATrack *track, UWORD32 flags)
{
    return (track->input.flags ^= flags);
}

/*******************************************************************************
 * Mixer data definitions
 ******************************************************************************/

/* ...mixer data */
typedef struct XAMixer
{
    /***************************************************************************
     * Control data
     **************************************************************************/

    /* ...generic audio codec data */
    XACodecBase         base;

    /* ...input tracks */
    XATrack             track[XA_MIXER_MAX_TRACK_NUMBER];
    
    /* ...output port */
    xf_output_port_t    output;

    /***************************************************************************
     * Run-time configuration parameters
     **************************************************************************/

    /* ...audio frame size in samples */
    UWORD32                 frame_size;

    /* ...audio frame duration */
    UWORD32                 frame_duration;
    
    /* ...sample size in bytes */
    UWORD32                     sample_size;

    /* ...audio byte duration */
    UWORD64                     factor;

    /* ...presentation timestamp (in samples; local mixer scope) */
    UWORD32                 pts;
 
    /* ...probe enabled flag */
    UWORD32                 probe_enabled;

    /* ...probe output port data */
    xf_output_port_t        probe;

    /* ...probe output buffer pointer */
    void                    *probe_output;
   
    /* ...mixer output buffer pointer */
    void                    *out_ptr;
    
    /***************************************************************************
     * response message pointer 
     **************************************************************************/
    xf_message_t        *m_response;

}   XAMixer;

/*******************************************************************************
 * Mixer flags
 ******************************************************************************/

/* ...output port setup completed */
#define XA_MIXER_FLAG_OUTPUT_SETUP      __XA_BASE_FLAG(1 << 0)

/*******************************************************************************
 * Track state flags
 ******************************************************************************/

/* ...track is idle (will autostart as soon as input data received) */
#define XA_TRACK_FLAG_IDLE              __XF_INPUT_FLAG(1 << 0)

/* ...track is rendered */
#define XA_TRACK_FLAG_ACTIVE            __XF_INPUT_FLAG(1 << 1)

/* ...track is paused */
#define XA_TRACK_FLAG_PAUSED            __XF_INPUT_FLAG(1 << 2)

/* ...track input port is setup */
#define XA_TRACK_FLAG_INPUT_SETUP       __XF_INPUT_FLAG(1 << 3)

/* ...track has received data */
#define XA_TRACK_FLAG_RECVD_DATA        __XF_INPUT_FLAG(1 << 4)

/*******************************************************************************
 * Output Track state flags
 ******************************************************************************/
/* ...output track is paused */
#define XA_OUT_TRACK_FLAG_PAUSED        __XF_OUTPUT_FLAG(1 << 0)

/*******************************************************************************
 * Probe Output state flags
 ******************************************************************************/

/* ...probe output port number */
#define XA_MIXER_PROBE_PORT_NUMBER      (XA_MIXER_MAX_TRACK_NUMBER + 1)

/* ...probe port setup condition */
#define XA_MIXER_FLAG_PROBE_SETUP       __XF_OUTPUT_FLAG(1 << 1)

/* ...probe port is paused */
#define XA_MIXER_PROBE_PORT_PAUSED      __XF_OUTPUT_FLAG(1 << 2)

/*******************************************************************************
 * Helper functions
 ******************************************************************************/
#if 0
/* ...Count the tracks that have received data or are active*/
static inline UWORD32 xa_mixer_check_active(XAMixer *mixer)
{
    XATrack        *track;
    UWORD32            i;
    UWORD32            cnt = 0;
    
    for (track = &mixer->track[i = 0]; i < XA_MIXER_MAX_TRACK_NUMBER; i++, track++)
    {
        if (xa_track_test_flags(track, XA_TRACK_FLAG_RECVD_DATA | XA_TRACK_FLAG_ACTIVE))
            cnt++;
    }
    return cnt;
}
#endif

/* ...prepare mixer for steady operation */
static inline XA_ERRORCODE xa_mixer_prepare_runtime(XAMixer *mixer)
{
    XACodecBase    *base = (XACodecBase *) mixer;
    xf_message_t   *m = xf_msg_queue_head(&mixer->output.queue);
    xf_start_msg_t *msg = m->buffer;
    UWORD32             frame_size;
    UWORD64             factor;
    
    /* ...query mixer parameters */
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIXER_CONFIG_PARAM_SAMPLE_RATE, &msg->sample_rate);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIXER_CONFIG_PARAM_CHANNELS, &msg->channels);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIXER_CONFIG_PARAM_PCM_WIDTH, &msg->pcm_width);
    XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, 0, &msg->input_length[0]);
    XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, XA_MIXER_MAX_TRACK_NUMBER, &msg->output_length[0]);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIXER_CONFIG_PARAM_FRAME_SIZE, &frame_size);

    /* ...populate port buffer length */
    msg->probe_length = (mixer->probe_enabled) ? mixer->probe.length : 0;

    TRACE(INIT, _b("mixer[%p]::runtime init: f=%u, c=%u, w=%u, i=%u, o=%u"), mixer, msg->sample_rate, msg->channels, msg->pcm_width, msg->input_length[0], msg->output_length[0]);

    /* ...save sample size in bytes */
    mixer->sample_size = msg->channels * ((msg->pcm_width == 8) ? 1 :((msg->pcm_width == 16) ? 2 : 4));

    /* ...calculate mixer frame duration; get upsample factor */
    XF_CHK_ERR(factor = xf_timebase_factor(msg->sample_rate), XA_MIXER_CONFIG_FATAL_RANGE);

    /* ...sample size should be positive */
    XF_CHK_ERR(mixer->sample_size > 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...set frame duration factor (converts number of bytes into timebase units) */
    mixer->factor = factor / mixer->sample_size;

    TRACE(INIT, _b("ts-factor: %llu (%llu)"), mixer->factor, factor);

    /* ...factor must be a multiple */
    XF_CHK_ERR(mixer->factor * mixer->sample_size == factor, XA_MIXER_CONFIG_FATAL_RANGE);

    /* ...set mixer frame duration */
    mixer->frame_duration = frame_size * factor; /* Note: mixer->factor, factor is for samples */
    
    /* ...pass response to caller (push out of output port) */
    xf_output_port_produce(&mixer->output, sizeof(*msg));

    /* ...codec runtime initialization is completed */
    TRACE(INIT, _b("mixer[%p] runtime initialized"), mixer);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands handlers
 ******************************************************************************/

/* ...EMPTY-THIS-BUFFER command processing */
static XA_ERRORCODE xa_mixer_empty_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) base;
    UWORD32         i = XF_MSG_DST_PORT(m->id);
    XATrack    *track = &mixer->track[i];

    /* ...make sure the port is valid */
    XF_CHK_ERR(i < XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    TRACE(INPUT, _b("track-%u: received buffer [%p]:%u"), i, m->buffer, m->length);

    /* ...update received data for the track */
    if (m->length)
        xa_track_set_flags(track, XA_TRACK_FLAG_RECVD_DATA);
    else
        xa_track_clear_flags(track, XA_TRACK_FLAG_RECVD_DATA);
    
    /* ...place received message into track input port */
    if (xf_input_port_put(&track->input, m))
    {
        /* ...restart mixer if it is in completed state */
        if (base->state & XA_BASE_FLAG_COMPLETED)
        {
            /* ...reset execution stage */
            base->state = XA_BASE_FLAG_POSTINIT | XA_BASE_FLAG_EXECUTION;

            /* ...reset execution runtime */
            XA_API(base, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_RUNTIME_INIT, NULL);
        }

        /* ...process track autostart if needed */
        if (xa_track_test_flags(track, XA_TRACK_FLAG_IDLE))
        {
            /* ...put track into active state */
            xa_track_toggle_flags(track, XA_TRACK_FLAG_IDLE | XA_TRACK_FLAG_ACTIVE);
            
            /* ...save track presentation timestamp */
            track->pts = mixer->pts;

            TRACE(INFO, _b("track-%u started (pts=%08x)"), i, track->pts);
        }
        
        /* ...schedule data processing if there is output port available */
        if (xf_output_port_ready(&mixer->output))
        {
            /* ...force data processing */
            xa_base_schedule(base, 0);
        }
    }
    
    return XA_NO_ERROR;
}

/* ...FILL-THIS-BUFFER command processing */
static XA_ERRORCODE xa_mixer_fill_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) base;
    UWORD32         i = XF_MSG_DST_PORT(m->id);
 
    /* ...command is allowed only in postinit state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...handle probe setup if probe is enabled */
    if (mixer->probe_enabled && i == XA_MIXER_PROBE_PORT_NUMBER)
    {
        if (base->state & XA_BASE_FLAG_COMPLETED || xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED))
        {
            /* ...return message arrived from application immediately */
            xf_response_ok(m);

            return XA_NO_ERROR;
        }
        else
        {
            TRACE(PROBE, _b("Received probe output buffer [%p]:%u"), m->buffer, m->length);

            /* ...adjust message length (may be shorter than original) */
            m->length = mixer->probe.length;
        }

        /* ...tbd - check input ports as well? */
        if (xf_output_port_put(&mixer->probe, m) && xf_output_port_ready(&mixer->output))
        {
            /* ...schedule data processing instantly */
            if (base->state & (XA_BASE_FLAG_EXECUTION))
            {
                xa_base_schedule(base, 0);
            }
        }

        return XA_NO_ERROR;
    }
   
    /* ...make sure the port is valid */
    XF_CHK_ERR(i == XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...process runtime initialization explicitly */
    if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
    {
        /* ...message must be zero-length */
        XF_CHK_ERR(m->length == 0, XA_MIXER_EXEC_FATAL_STATE);
    }
#if 1
    else if (m == xf_output_port_control_msg(&mixer->output))
    {
        int i;

        /* ... mark flushing sequence is done */
        xf_output_port_flush_done(&mixer->output);

#if 1   //TENA_2379                                                                                                     
        if (xf_output_port_unrouting(&mixer->output))
        {   
            /* ...flushing during port unrouting; complete unroute sequence */                                            
            xf_output_port_unroute_done(&mixer->output);                                                                  
            TRACE(INFO, _b("port is unrouted"));
        }   
#endif
        else if (m->length == XF_MSG_LENGTH_INVALID)
        {
            /* ...complete flushing and unrouting of the outport whose dest no longer exists */
            xf_output_port_unroute(&mixer->output);
            TRACE(INFO, _b("mixer[%p] completed internal unroute of port"), mixer);
        }

        /* ...complete pending zero-length input buffer */
        for (i = 0; i < XA_MIXER_MAX_TRACK_NUMBER; i++)
        {
            xf_input_port_purge(&mixer->track[i].input);
        }

        TRACE(INFO, _b("mixer[%p] playback completed"), mixer);

        /* ...playback is over */
        return XA_NO_ERROR;
    }
#endif
    /* ...indicates that the downstream component no longer exists */
    else if ((m->length == XF_MSG_LENGTH_INVALID) && xf_output_port_routed(&mixer->output))
    {
         m->length = mixer->output.length; /* ...reset length for sanity */

        if(!xf_output_port_flushing(&mixer->output))
        {
            /* ...cancel any pending processing */
            xa_base_cancel(base);

            /* ...output port is invalid; trigger port flush to collect all the buffers in transit */
            (void)xf_output_port_flush(&mixer->output, XF_FILL_THIS_BUFFER);

            /* ...clear output-port-setup condition */
            base->state &= ~XA_MIXER_FLAG_OUTPUT_SETUP;

            TRACE(INFO, _b("mixer[%p] started internal unroute of port"), mixer);
         }
         TRACE(INFO, _b("mixer[%p] drop buffer"), mixer);

         return XA_NO_ERROR;
    }
    else
    {
        if ((base->state & XA_BASE_FLAG_COMPLETED) && !xf_output_port_routed(&mixer->output))
        {

            /* ...message must have exactly expected size (there is no ordered abortion) */
            XF_CHK_ERR(m->length == mixer->output.length, XA_MIXER_EXEC_FATAL_STATE);

            /* ...return message arrived from application immediately */
            xf_response_ok(m);
        
            TRACE(INFO, _b("mixer[%p]::EOS generated"), mixer);
        
            return XA_NO_ERROR;
        }
        else
        {
            /* ...adjust message length (may be shorter than original) [TENA-2957] */
            m->length = mixer->output.length;
        }

    }

    TRACE(OUTPUT, _b("received output buffer [%p]:%u"), m->buffer, m->length);
    
    /* ...put message into output port */
    if (xf_output_port_put(&mixer->output, m))
    {
        /* ...check probe port, if probe is enabled */
        if (mixer->probe_enabled && !(base->state & XA_BASE_FLAG_RUNTIME_INIT))
        {
            if (!xf_output_port_ready(&mixer->probe) && !xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED))
                return XA_NO_ERROR;
        }

        /* ...force data processing */
        xa_base_schedule(base, 0);
    }

    return XA_NO_ERROR;
}

/* ...output port routing */
static XA_ERRORCODE xa_mixer_port_route(XACodecBase *base, xf_message_t *m)
{
    XAMixer                *mixer = (XAMixer *) base;
    xf_route_port_msg_t    *cmd = m->buffer;
    xf_output_port_t       *port = &mixer->output;
    UWORD32                     src = XF_MSG_DST(m->id);
    UWORD32                     dst = cmd->dst;
    
    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure output port is addressed */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure port is not routed yet */
    XF_CHK_ERR(!xf_output_port_routed(port), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...route output port - allocate queue */
    XF_CHK_ERR(xf_output_port_route(port, __XF_MSG_ID(dst, src), cmd->alloc_number, cmd->alloc_size, cmd->alloc_align) == 0, XA_API_FATAL_MEM_ALLOC);

    /* ...schedule processing instantly - tbd - check if we have anything pending on input */
    xa_base_schedule(base, 0);
    
    /* ...pass success result to caller */
    xf_response_ok(m);
    
    return XA_NO_ERROR;
}

/* ...port unroute command */
static XA_ERRORCODE xa_mixer_port_unroute(XACodecBase *base, xf_message_t *m)
{
    XAMixer            *mixer = (XAMixer *) base;
    xf_output_port_t   *port = &mixer->output;
    
    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure output port is addressed */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);

    if(!xf_output_port_routed(port))
    {
        /* ...if XF_MSG_LENGTH_INVALID triggered internal unroute is completed, send response instantly */
        xf_response_ok(m);
        return XA_NO_ERROR;
    }

    /* ...cancel any pending processing */
    xa_base_cancel(base);

    /* ...clear output-port-setup condition */
    base->state &= ~XA_MIXER_FLAG_OUTPUT_SETUP;

    /* ...pass flush command down the graph */
    if (xf_output_port_flush(port, XF_FLUSH))
    {
        TRACE(INFO, _b("port is idle; instantly unroute"));

        /* ...flushing sequence is not needed; command may be satisfied instantly */
        xf_output_port_unroute(port);

        /* ...pass response to the proxy */
        xf_response_ok(m);
    }
    else
    {
        TRACE(INFO, _b("port is busy; propagate unroute command"));

        /* ...flushing sequence is started; save flow-control message */
        xf_output_port_unroute_start(port, m);
    }

    return XA_NO_ERROR;
}

/* ...PAUSE message processing */
static XA_ERRORCODE xa_mixer_pause(XACodecBase *base, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) base;
    UWORD32         i = XF_MSG_DST_PORT(m->id);
    XATrack    *track = &mixer->track[i];
    
    /* ...make sure the buffer is empty */
    XF_CHK_ERR(m->length == 0, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...check destination port is valid */
    XF_CHK_ERR(i <= XA_MIXER_PROBE_PORT_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check if mixer is running */
    if (base->state & XA_BASE_FLAG_COMPLETED)
    {
        TRACE(WARNING, _b("mixer[%p] completed, ignore pause command"), mixer);
    
        /* ...complete message immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    /* ...check for actual track flags */
    if (i < XA_MIXER_MAX_TRACK_NUMBER && xa_track_test_flags(track, XA_TRACK_FLAG_ACTIVE))
    {
        /* ...switch to paused state */
        xa_track_toggle_flags(track, XA_TRACK_FLAG_ACTIVE | XA_TRACK_FLAG_PAUSED);

        /* ...other tracks may be waiting for this one, so force data processing */
        if (xf_output_port_ready(&mixer->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("mixer[%p]::track[%u] paused"), mixer, i);
    }
    else if (i == XA_MIXER_MAX_TRACK_NUMBER && !xa_port_test_flags(&mixer->output.flags, XA_OUT_TRACK_FLAG_PAUSED))
    {
        xa_port_set_flags(&mixer->output.flags, XA_OUT_TRACK_FLAG_PAUSED);

        TRACE(INFO, _b("mixer[%p]::port[%u] (out port) paused"), mixer, i);

    }
    else if (mixer->probe_enabled && i == XA_MIXER_PROBE_PORT_NUMBER && !xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED))
    {
        /* ...flush probe port */
        if (!xf_output_port_flush(&mixer->probe, XF_FLUSH))
        {
            BUG(1, _x("probe port: invalid state"));
        }

        /* ...pause probe port */
        xa_port_set_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED);

        /* ...other tracks may be waiting for this one, so force data processing */
        if (xf_output_port_ready(&mixer->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("mixer[%p]::probe-port[%u] paused"), mixer, i);
    }
    else
    {
        /* ...track is in idle state and pausing here means suspending */
        TRACE(INFO, _b("mixer[%p]::port[%u] is not active or already paused"), mixer, i);
    }

    /* ...complete message immediately */
    xf_response(m);
    
    return XA_NO_ERROR;
}

/* ...RESUME command processing */
static XA_ERRORCODE xa_mixer_resume(XACodecBase *base, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) base;
    UWORD32         i = XF_MSG_DST_PORT(m->id);
    XATrack    *track = &mixer->track[i];
    
    /* ...make sure the buffer is empty */
    XF_CHK_ERR(m->length == 0, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...check destination port is valid */
    XF_CHK_ERR(i <= XA_MIXER_PROBE_PORT_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...check if mixer is running */
    if (base->state & XA_BASE_FLAG_COMPLETED)
    {
        TRACE(WARNING, _b("mixer[%p] completed, ignore resume command"), mixer);
    
        /* ...complete message immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    /* ...check for actual track state */
    if (i < XA_MIXER_MAX_TRACK_NUMBER && xa_track_test_flags(track, XA_TRACK_FLAG_PAUSED))
    {
        /* ...switch track to active state */
        xa_track_toggle_flags(track, XA_TRACK_FLAG_ACTIVE | XA_TRACK_FLAG_PAUSED);

        /* ...reset track presentation timestamp - tbd */
        track->pts = mixer->pts;

        /* ...force data processing if there is an output buffer */
        if (xf_output_port_ready(&mixer->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("mixer[%p]::track[%u] resumed"), mixer, i);
    }
    else if (i == XA_MIXER_MAX_TRACK_NUMBER && xa_port_test_flags(&mixer->output.flags, XA_OUT_TRACK_FLAG_PAUSED))
    {
        xa_port_clear_flags(&mixer->output.flags, XA_OUT_TRACK_FLAG_PAUSED);

        /* ...force data processing if there is an output buffer */
        if (xf_output_port_ready(&mixer->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("mixer[%p]::port[%u] (out-port) resumed"), mixer, i);
    }    
    else if (mixer->probe_enabled && i == XA_MIXER_PROBE_PORT_NUMBER && xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED))
    {
        /* ...resume probe port */
        xa_port_clear_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED);

        /* ...other tracks may be waiting for this one, so force data processing */
        if (xf_output_port_ready(&mixer->output) && xf_output_port_ready(&mixer->probe))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("mixer[%p]::probe-port[%u] resumed"), mixer, i);
    }
    else
    {
        /* ...track is in idle state; do nothing */
        TRACE(INFO, _b("mixer[%p]::port[%u] is not paused"), mixer, i);
    }
    
    /* ...complete message */
    xf_response(m);
    
    return XA_NO_ERROR;
}

/* ...FLUSH command processing */
static XA_ERRORCODE xa_mixer_flush(XACodecBase *base, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) base;
    UWORD32         i = XF_MSG_DST_PORT(m->id);
    XATrack    *track = &mixer->track[i];

    /* ...ensure input parameter length is zero or XF_MSG_LENGTH_INVALID */
    XF_CHK_ERR((m->length == 0) || (m->length == XF_MSG_LENGTH_INVALID), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check destination port index */
    if (i == XA_MIXER_MAX_TRACK_NUMBER)
    {
        /* ...flushing response received; that is a port unrouting sequence */
        XF_CHK_ERR(xf_output_port_unrouting(&mixer->output), XA_API_FATAL_INVALID_CMD_TYPE);
        
        /* ...complete unroute sequence */
        xf_output_port_unroute_done(&mixer->output);

        TRACE(INFO, _b("port is unrouted"));

        return XA_NO_ERROR;
    }

    /* ...check destination port index is valid */
    XF_CHK_ERR(i < XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input port flushing; check the track state is valid */
    if (xa_track_test_flags(track, XA_TRACK_FLAG_ACTIVE | XA_TRACK_FLAG_PAUSED))
    {
        /* ...purge input port */
        xf_input_port_purge(&track->input);

        /* ...force clearing of ACTIVE and INPUT_SETUP condition */
        xa_track_clear_flags(track, XA_TRACK_FLAG_ACTIVE | XA_TRACK_FLAG_PAUSED | XA_TRACK_FLAG_INPUT_SETUP | XA_TRACK_FLAG_RECVD_DATA);

        /* ...and enter into idle state */
        xa_track_set_flags(track, XA_TRACK_FLAG_IDLE);

        /* ...other tracks may be waiting for this track, so force data processing */
        if (xf_output_port_ready(&mixer->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("mixer[%p]::track[%u] flushed"), mixer, i);
    }

    /* ...complete message instantly (no propagation to output port) */
    xf_response(m);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Codec API implementation
 ******************************************************************************/

/* ...buffers handling */
static XA_ERRORCODE xa_mixer_memtab(XACodecBase *base, WORD32 idx, WORD32 type, WORD32 size, WORD32 align, UWORD32 core)
{
    XAMixer    *mixer = (XAMixer *)base;
    UWORD32        i;
    
    if (type == XA_MEMTYPE_INPUT)
    {
        XATrack    *track = &mixer->track[idx];

        /* ...input buffer allocation; check track number is sane */
        XF_CHK_ERR(idx < XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...create input port for a track */
        XF_CHK_ERR(xf_input_port_init(&track->input, size, align, core) == 0, XA_API_FATAL_MEM_ALLOC);

        if(size)
        {
            /* ...set input port buffer */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, idx, track->input.buffer);
        }

        /* ...put track into idle state (will start as soon as we receive data) */
        xa_track_set_flags(track, XA_TRACK_FLAG_IDLE);
        
        TRACE(INIT, _b("mixer[%p]::track[%u] input port created - size=%u"), mixer, idx, size);
    }
    else
    {
        /* ...output buffer allocation */
        XF_CHK_ERR(type == XA_MEMTYPE_OUTPUT, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...check port number is what we expect */
        XF_CHK_ERR(idx == XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...set mixer frame-size (in samples - for timestamp tracking) */
        XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIXER_CONFIG_PARAM_FRAME_SIZE, &mixer->frame_size);
        
        /* ...create output port for a track */
        XF_CHK_ERR(xf_output_port_init(&mixer->output, size) == 0, XA_API_FATAL_MEM_ALLOC);
        
        TRACE(INIT, _b("mixer[%p] output port created; size=%u"), mixer, size);
    }

    if (mixer->probe_enabled)
    {
        XATrack     *track;
        UWORD32      probe_size = 0;
        UWORD32      io_ports_created;

        io_ports_created = xf_output_port_created(&mixer->output);

        if (io_ports_created)
        {
            probe_size += XF_CHK_PORT_MASK(mixer->probe_enabled, XA_MIXER_MAX_TRACK_NUMBER) ?  (XF_ALIGNED_PROBE_SIZE(mixer->output.length)) : 0;
        }

        for (track = &mixer->track[i = 0]; i < XA_MIXER_MAX_TRACK_NUMBER; i++, track++)
        {
            if ((io_ports_created &= xf_input_port_created(&track->input)) == 0)
                break;

            /*... return error if probe is enabled for input port with input bypass enabled. */
            if((xf_input_port_bypass(&track->input) && (XF_CHK_PORT_MASK(mixer->probe_enabled, i))))
            {
                TRACE(ERROR, _x("Probe buffer-length error on port[%d] with input bypass"), i);
                return XAF_INVALIDVAL_ERR;
            }

            probe_size += XF_CHK_PORT_MASK(mixer->probe_enabled, i) ?  (XF_ALIGNED_PROBE_SIZE(mixer->track[i].input.length)) : 0;
        }

        if (io_ports_created)
        {
            /* ...initialize probe output port queue (no allocation here yet) */
            XF_CHK_ERR(xf_output_port_init(&mixer->probe, probe_size) == 0, XA_API_FATAL_MEM_ALLOC);

            /* ...put probe port into pause state */
            xa_port_set_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED);

            TRACE(INIT, _b("mixer[%p]::probe output port created - size=%u"), mixer, probe_size);
        }
    }

    return XA_NO_ERROR;
}

/* ...preprocessing function */
static XA_ERRORCODE xa_mixer_preprocess(XACodecBase *base)
{
    XAMixer        *mixer = (XAMixer *) base;
    XATrack        *track;
    UWORD8              i;
    XA_ERRORCODE    e = XA_MIXER_EXEC_NONFATAL_NO_DATA;
    int inport_nodata_flag = 0;

    /* ...check if output port is paused */
    if (xa_port_test_flags(&mixer->output.flags, XA_OUT_TRACK_FLAG_PAUSED))
    {
        return XA_MIXER_EXEC_NONFATAL_NO_DATA;
    }

    /* ...prepare probe buffer if required */
    if (mixer->probe_enabled && !xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED))
    {
        if (!(mixer->probe.flags & XA_MIXER_FLAG_PROBE_SETUP))
        {
            void *probe_output;

            /* ...get probe buffer from port, if possible */
            if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
            {
                /* ...run-time is not initialized yet; probe buffer not required */
            }
            else if ((probe_output = xf_output_port_data(&mixer->probe)) == NULL)
            {
                /* ...no probe buffer available */
                return XA_MIXER_EXEC_NONFATAL_NO_DATA;
            }
            else
            {
                /* ...record probe buffer for producing probe output */
                mixer->probe_output = probe_output;

                /* ...mark probe output port is setup */
                mixer->probe.flags ^= XA_MIXER_FLAG_PROBE_SETUP;
            }
        }
    }

    /* ...prepare output buffer */
    if (!(base->state & XA_MIXER_FLAG_OUTPUT_SETUP))
    {
        void   *output;

        /* ...set output buffer pointer */
        if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
        {
            /* ...no actual data processing during initialization */
            return XA_NO_ERROR;
        }
        else if ((output = xf_output_port_data(&mixer->output)) == NULL)
        {
            /* ...no output buffer available */
            return e;
        }
            
        /* ...copy mixer output buffer pointer for probe */
        mixer->out_ptr = output;

        /* ...set output buffer pointer */
        XA_API(base, XA_API_CMD_SET_MEM_PTR, XA_MIXER_MAX_TRACK_NUMBER, output);

        /* ...mark output port is setup */
        base->state ^= XA_MIXER_FLAG_OUTPUT_SETUP;
    }
    
    /* ...setup input buffer pointers and length */
    for (track = &mixer->track[i = 0]; i < XA_MIXER_MAX_TRACK_NUMBER; i++, track++)
    {
        /* ...skip tracks that are not played */
        if (!xa_track_test_flags(track, XA_TRACK_FLAG_ACTIVE))
        {
            if (!xa_track_test_flags(track, XA_TRACK_FLAG_PAUSED))
            {
                /* ...pass input-over command to the codec */
                XA_API(base, XA_API_CMD_INPUT_OVER, i, NULL);
            }
            continue;
        }

        /* ...set temporary mixing request */
        e = XA_NO_ERROR;

        /* ...skip the tracks that has been setup already */
        if (xa_track_test_flags(track, XA_TRACK_FLAG_INPUT_SETUP))  continue;

        /* ...found active track that hasn't been setup yet */
        TRACE(INPUT, _b("track-%u: ts=%08x vs mts=%08x"), i, track->pts, mixer->pts);

        /* ...if track presentation timestamp is in the future, do nothing yet really */
        if (!xf_time_after(track->pts, mixer->pts))
        {
            UWORD32     filled = 0;
            
            if (xf_input_port_bypass(&track->input))
            {
                void *input;

                /* ...port is in bypass mode; try to update the buffer pointer, remaining bytes if necessary */
                xf_input_port_fill(&track->input);

                /* ...use input buffer directly; check if there is data available */
                if ((input = xf_input_port_data(&track->input)) != NULL)
                {
                    /* ...set input data buffer pointer */
                    XA_API(base, XA_API_CMD_SET_MEM_PTR, i, input);
            
                    /* ...retrieve number of input bytes */
                    filled = xf_input_port_length(&track->input);
                }
                else if (!xf_input_port_done(&track->input))
                {
                    /* ...failed to prefill input buffer - no sufficient data yet */
                    inport_nodata_flag = 1; 
                    continue;
                }
            }
            else
            {
                /* ...take actual data from input port (mixer is always using internal buffer) */
                if (!xf_input_port_fill(&track->input))
                {
                    /* ...failed to prefill input buffer - no sufficient data yet */
                    inport_nodata_flag = 1; 
                    continue;
                }
                else
                {
                    /* ...retrieve number of bytes available */
                    filled = xf_input_port_level(&track->input);
                }
            }

            /* ...check if input stream is over */
            if (xf_input_port_done(&track->input))
            {
                /* ...pass input-over command to the codec to indicate the final buffer */
                XA_API(base, XA_API_CMD_INPUT_OVER, i, NULL);
            
                TRACE(INFO, _b("mixer[%p]:track[%u] signal input-over (filled: %u)"), mixer, i, filled);
            }

            /* ...set total number of bytes we have in buffer */
            XA_API(base, XA_API_CMD_SET_INPUT_BYTES, i, &filled);

            /* ...actual data is to be played */
            TRACE(INPUT, _b("track-%u: filled %u bytes"), i, filled);
        }
        
        /* ...mark the track input is setup (emit silence or actual data) */
        xa_track_set_flags(track, XA_TRACK_FLAG_INPUT_SETUP);
    }

    if (inport_nodata_flag)
        return XA_MIXER_EXEC_NONFATAL_NO_DATA;

    /* ...do mixing operation only when all active tracks are setup */
    return e;
}

/* ...postprocessing function */
static XA_ERRORCODE xa_mixer_postprocess(XACodecBase *base, int done)
{
    XAMixer        *mixer = (XAMixer *) base;
    XATrack        *track;
    UWORD32             produced=0;
    UWORD32             consumed;
    UWORD8              i;
    UWORD32         probe_length = 0;
    void           *probe_outptr = mixer->probe_output;

    if (done)
    {
        if (base->state & XA_BASE_FLAG_EXECUTION)
        {
            /* ...enter into execution state; initialize runtime */
            return XA_CHK(xa_mixer_prepare_runtime(mixer));
        }
    }

    /* ...input ports maintenance; process all tracks */
    for (track = &mixer->track[i = 0]; i < XA_MIXER_MAX_TRACK_NUMBER; i++, track++)
    {
        /* ...skip the tracks that are not running */
        if (!xa_track_test_flags(track, XA_TRACK_FLAG_ACTIVE))
        {
#if 1 //TENA_2365
            /* ...to allow update of input buffer if it was setup for exec. */
            if (!xa_track_test_flags(track, XA_TRACK_FLAG_INPUT_SETUP)) 
#endif
            {
                continue;
            }
        }

        /* ...clear input setup flag */
        xa_track_clear_flags(track, XA_TRACK_FLAG_INPUT_SETUP);
        
        /* ...advance track presentation timestamp */
        track->pts += mixer->frame_size;
        
        /* ...get total amount of consumed bytes */
        XA_API(base, XA_API_CMD_GET_CURIDX_INPUT_BUF, i, &consumed);

        TRACE(INPUT, _b("track-%u::postprocess(c=%u, ts=%08x)"), i, consumed, track->pts);

        /* ...produce probe output if required */
        if (mixer->probe_enabled && !xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED))
        {
            if (XF_CHK_PORT_MASK(mixer->probe_enabled, i))
            {
                /* ...copy input port data onto probe port */
                probe_outptr = xf_copy_probe_data(probe_outptr, i, consumed, track->input.buffer);
       
                /* ...compute probe data length locally */
                probe_length += consumed;
            }
        }

        /* ...consume that amount from input port (may be zero) */
        xf_input_port_consume(&track->input, consumed);
        
        /* ...check if input port is done */
        if (xf_input_port_done(&track->input))
        {
            /* ...input stream is over; return zero-length input back to caller */
            xf_input_port_purge(&track->input);

            /* ...switch to idle state */
            xa_track_toggle_flags(track, XA_TRACK_FLAG_ACTIVE | XA_TRACK_FLAG_IDLE);

            TRACE(INFO, _b("mixer[%p]::track[%u] completed"), mixer, i);
        }
    }

    if(mixer->sample_size)
    {
        /* ...check if we have produced anything during execution */
        XA_API(base, XA_API_CMD_GET_OUTPUT_BYTES, XA_MIXER_MAX_TRACK_NUMBER, &produced);
    }

    TRACE(OUTPUT, _b("mixer[%p]::postprocess(p=%u, ts=%08x, done=%u)"), mixer, produced, mixer->pts, done);

    /* ...probe port maintenance */
    if (mixer->probe_enabled && !xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED))
    {
        if (XF_CHK_PORT_MASK(mixer->probe_enabled, XA_MIXER_MAX_TRACK_NUMBER))
        {
            /* ...copy output port data onto probe port */
            probe_outptr = xf_copy_probe_data(probe_outptr, XA_MIXER_MAX_TRACK_NUMBER, produced, mixer->out_ptr);

            /* ...compute probe data length locally */
            probe_length += produced;
        }
    }

    /* ...output port maintenance */
    if (produced)
    {
        /* ...make sure we have produced exactly single frame */
        BUG(produced != mixer->output.length, _x("Invalid length: %u != %u"), produced, mixer->output.length);
        
        /* ...steady mixing process; advance mixer presentation timestamp */
        mixer->pts += mixer->frame_size;

        /* ...push data from output port */
        xf_output_port_produce(&mixer->output, produced);

        /* ...clear output-setup condition */
        base->state &= ~XA_MIXER_FLAG_OUTPUT_SETUP;
    }    

    if (probe_length)
    {
        /* ...compute total probe output length (including length variables and alignment) */
        probe_length = probe_outptr - mixer->probe_output;

        /* ...make sure probe port is enabled and active */
        BUG((!mixer->probe_enabled || xa_port_test_flags(&mixer->probe.flags, XA_MIXER_PROBE_PORT_PAUSED)), _x("Probe output produced in invalid probe state"));

        /* ...immediately complete probe output buffer (don't wait until it gets filled) */
        xf_output_port_produce(&mixer->probe, probe_length);

        /* ...clear probe port setup flag */
        mixer->probe.flags ^= XA_MIXER_FLAG_PROBE_SETUP;
    }
   
    /* ...process execution stage transitions */
    if (done)
    {
        if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
        {
            /* ...failed to initialize runtime (can't be? - tbd)*/
            BUG(1, _x("breakpoint"));
        }
        else
        {
            /* ...output stream is over; propagate condition to sink port */
            if (xf_output_port_flush(&mixer->output, XF_FILL_THIS_BUFFER))
            {
                /* ...flushing sequence is not needed; complete pending zero-length input */
                //xf_input_port_purge(&mixer->input);
                
                /* ...no propagation to output port */
                TRACE(INFO, _b("mixer[%p] playback completed. No need to propagate EoS."), mixer);
            }
            else
            {
                /* ...flushing sequence is started; wait until flow-control message returns */
                TRACE(INFO, _b("propagate end-of-stream condition"));
            }

            /* ...flush probe port */
            if (mixer->probe_enabled)
            {
                if (!xf_output_port_flush(&mixer->probe, XF_FLUSH))
                {
                    BUG(1, _x("probe port: invalid state"));
                }
            }
            return XA_NO_ERROR;
        }
    }

    /* ...reschedule data processing if there is a pending output message */
    if (xf_output_port_ready(&mixer->output))
    {
        /* ...schedule execution with respect to urgency */
        xa_base_schedule(base, (produced ? mixer->frame_duration : 0));
    }

    return XA_NO_ERROR;
}

/* ...set configuration parameter */
static XA_ERRORCODE xa_mixer_setparam(XACodecBase *base, WORD32 id, pVOID value)
{
    XAMixer     *mixer = (XAMixer *) base;

    if (id == XAF_COMP_CONFIG_PARAM_PROBE_ENABLE)
    {
        /* ... probe enabling allowed only before postinit */
        XF_CHK_ERR((base->state & XA_BASE_FLAG_POSTINIT) == 0, XA_API_FATAL_INVALID_CMD_TYPE);

        mixer->probe_enabled = *(WORD32 *) value;
        return XA_NO_ERROR;
    }
    else
    {
        /* ...pass command to underlying codec plugin */
        return XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, id, value);
    }
}

/*******************************************************************************
 * Command-processing function
 ******************************************************************************/

/* ...command hooks */
static XA_ERRORCODE (* const xa_mixer_cmd[])(XACodecBase *, xf_message_t *) =
{
    /* ...set-parameter - actually, same as in generic case */
    [XF_OPCODE_TYPE(XF_SET_PARAM)] = xa_base_set_param,
    [XF_OPCODE_TYPE(XF_GET_PARAM)] = xa_base_get_param,

    /* ...extended set-get-config parameter */
    [XF_OPCODE_TYPE(XF_SET_PARAM_EXT)] = xa_base_set_param_ext,
    [XF_OPCODE_TYPE(XF_GET_PARAM_EXT)] = xa_base_get_param_ext,

    /* ...output port routing/unrouting */
    [XF_OPCODE_TYPE(XF_ROUTE)] = xa_mixer_port_route,
    [XF_OPCODE_TYPE(XF_UNROUTE)] = xa_mixer_port_unroute,

    /* ...input/output buffers processing */
    [XF_OPCODE_TYPE(XF_EMPTY_THIS_BUFFER)] = xa_mixer_empty_this_buffer,
    [XF_OPCODE_TYPE(XF_FILL_THIS_BUFFER)] = xa_mixer_fill_this_buffer,
    [XF_OPCODE_TYPE(XF_FLUSH)] = xa_mixer_flush,

    /* ...track control */
    [XF_OPCODE_TYPE(XF_PAUSE)] = xa_mixer_pause,
    [XF_OPCODE_TYPE(XF_RESUME)] = xa_mixer_resume,
};

/* ...total number of commands supported */
#define XA_MIXER_CMD_NUM        (sizeof(xa_mixer_cmd) / sizeof(xa_mixer_cmd[0]))

/*******************************************************************************
 * Entry points
 ******************************************************************************/

/* ...mixer termination-state command processor */
static int xa_mixer_terminate(xf_component_t *component, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) component;

    if (m == NULL)
    {
        /* ...ignore component processing during component termination(rare case) */
        TRACE(OUTPUT, _b("component processing ignored.."));
        return 0;
    }

    if (m == xf_output_port_control_msg(&mixer->output))
    {
        /* ...output port flushing complete; mark port is idle and terminate */
        xf_output_port_flush_done(&mixer->output);
        TRACE(OUTPUT, _b("mixer[%p] flush completed in terminate"), mixer);
#ifdef XF_MSG_ERR_HANDLING
        return XAF_UNREGISTER;
#else
        return -1;
#endif
    }
    else if (m->opcode == XF_FILL_THIS_BUFFER && xf_output_port_routed(&mixer->output))
    {
        /* ...output buffer returned by the sink component; ignore and keep waiting */
        TRACE(OUTPUT, _b("collect output buffer"));
        return 0;
    }
    else
    {
        /* ...everything else is responded with generic failure */
        if (XF_MSG_SRC_PROXY(m->id))
        {
            xf_response_err(m);
        }
        else
        {
            xf_response_failure(m);
            TRACE(OUTPUT, _b("mixer[%p] response_failure in terminate"), mixer);
        }
        return 0;
    }
}

/* ...mixer class destructor */
static int xa_mixer_destroy(xf_component_t *component, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) component;
    UWORD32         core = xf_component_core(component);
    UWORD32         i;
    
    /* ...get the saved command message pointer before the component memory is freed */
    xf_message_t *m_resp = mixer->m_response;

    /* ...destroy all inputs */
    for (i = 0; i < XA_MIXER_MAX_TRACK_NUMBER; i++)
    {
        xf_input_port_destroy(&mixer->track[i].input, core);
    }

    /* ...destroy output port */
    xf_output_port_destroy(&mixer->output, core);

    /* ...destroy probe output port */
    if (mixer->probe_enabled)
    {
        xf_output_port_destroy(&mixer->probe, core);
    }

    /* ...destroy base object */
    xa_base_destroy(&mixer->base, XF_MM(sizeof(*mixer)), core);

    if (m_resp != NULL)
    {
        /* ...complete the command with response */
        xf_response_err(m_resp);
    }

    TRACE(INIT, _b("mixer[%p] destroyed"), mixer);

    return 0;
}

/* ...mixer class first-stage destructor */
static int xa_mixer_cleanup(xf_component_t *component, xf_message_t *m)
{
    XAMixer    *mixer = (XAMixer *) component;
    UWORD32         i;

    /* ...cancel internal scheduling message if needed */
    xa_base_cancel(&mixer->base);    
    
    /* ...purge all input ports (specify "unregister"? - don't know yet - tbd) */
    for (i = 0; i < XA_MIXER_MAX_TRACK_NUMBER; i++)
    {
        xf_input_port_purge(&mixer->track[i].input);
    }

    if (mixer->probe_enabled)
    {
        if (!xf_output_port_flush(&mixer->probe, XF_FLUSH))
        {
            BUG(1, _x("probe port: invalid state"));
        }
    }

    /* ...save command message to send response after flush completes */
    mixer->m_response = m;

    /* ...flush output port */
    if (xf_output_port_flush(&mixer->output, XF_FLUSH))
    {
        /* ...flushing sequence is not needed; destroy mixer */
        return xa_mixer_destroy(component, NULL);
    }
    else
    {
        /* ...wait until output port is cleaned; adjust component hooks */
        component->entry = xa_mixer_terminate;
        component->exit = xa_mixer_destroy;
        
        TRACE(INIT, _b("mixer[%p] cleanup sequence started"), mixer);

        /* ...indicate that second stage is required */
        return 1;
    }
}

/* ...mixer class factory */
xf_component_t * xa_mixer_factory(UWORD32 core, xa_codec_func_t process, xaf_comp_type comp_type)
{
    XAMixer    *mixer;

    /* ...construct generic audio component */
    XF_CHK_ERR(mixer = (XAMixer *)xa_base_factory(core, XF_MM(sizeof(*mixer)), process), NULL);

    /* ...set generic codec API */
    mixer->base.memtab = xa_mixer_memtab;
    mixer->base.preprocess = xa_mixer_preprocess;
    mixer->base.postprocess = xa_mixer_postprocess;
    mixer->base.setparam = xa_mixer_setparam;

    /* ...set message-processing table */
    mixer->base.command = xa_mixer_cmd;
    mixer->base.command_num = XA_MIXER_CMD_NUM;

    /* ...set component destructor hook */
    mixer->base.component.exit = xa_mixer_cleanup;

    /* ...set component type */
    mixer->base.comp_type = comp_type;
    
    TRACE(INIT, _b("Mixer[%p] created"), mixer);

    /* ...return handle to component */
    return (xf_component_t *) mixer;
}
