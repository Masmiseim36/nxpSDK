/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
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
 * xa-class-mimo-proc.c
 *
 * Generic mimo process class
 ******************************************************************************/

#ifndef XA_DISABLE_CLASS_MIMO_PROC
#define MODULE_TAG                      MIMO

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"
#include "xa-class-base.h"
#include "audio/xa-mimo-proc-api.h"

/*******************************************************************************
 * External data structures
 ******************************************************************************/
extern const int xf_io_ports[XAF_MAX_COMPTYPE][2];

/*******************************************************************************
 * Constant definitions
 ******************************************************************************/

/* ...maximum in ports for mimo class */
#define XA_MIMO_PROC_MAX_IN_PORTS       XF_CFG_MAX_IN_PORTS

/* ...maximum out ports for mimo class */
#define XA_MIMO_PROC_MAX_OUT_PORTS      XF_CFG_MAX_OUT_PORTS

/*******************************************************************************
 * Data structures
 ******************************************************************************/

/* ...mimo-proc - input data */
typedef struct XAInTrack
{
    /* ...input port data */
    xf_input_port_t     input;

    /* ...current presentation timestamp (in samples; local to a mimo-proc state) */
    //UWORD32             pts;

    /* ...total amount of decoded frames since last synchronization point */
    //UWORD32           decoded;

    /* ...input port index */
    WORD32              idx;

    /* ...total amount of consumed frames since last synchronization point */
    UWORD32             consumed;

}   XAInTrack;

/* ...mimo-proc - output data */
typedef struct XAOutTrack
{
    /* ...output port data */
    xf_output_port_t     output;

    /* ...current presentation timestamp (in samples; local to a mimo-proc state) */
    //UWORD32             pts;

    /* ...total amount of decoded frames since last synchronization point */
    //UWORD32           decoded;

    /* ...output port index */
    WORD32              idx;

    /* ...total amount of produced frames since last synchronization point */
    UWORD32             produced;

}   XAOutTrack;

/*******************************************************************************
 * Helpers
 ******************************************************************************/

static inline UWORD32 xa_in_track_test_flags(XAInTrack *in_track, UWORD32 flags)
{
    return (in_track->input.flags & flags);
}

static inline UWORD32 xa_in_track_set_flags(XAInTrack *in_track, UWORD32 flags)
{
    return (in_track->input.flags |= flags);
}

static inline UWORD32 xa_in_track_clear_flags(XAInTrack *in_track, UWORD32 flags)
{
    return (in_track->input.flags &= ~flags);
}

static inline UWORD32 xa_in_track_toggle_flags(XAInTrack *in_track, UWORD32 flags)
{
    return (in_track->input.flags ^= flags);
}

static inline UWORD32 xa_out_track_test_flags(XAOutTrack *out_track, UWORD32 flags)
{
    return (out_track->output.flags & flags);
}

static inline UWORD32 xa_out_track_set_flags(XAOutTrack *out_track, UWORD32 flags)
{
    return (out_track->output.flags |= flags);
}

static inline UWORD32 xa_out_track_clear_flags(XAOutTrack *out_track, UWORD32 flags)
{
    return (out_track->output.flags &= ~flags);
}

static inline UWORD32 xa_out_track_toggle_flags(XAOutTrack *out_track, UWORD32 flags)
{
    return (out_track->output.flags ^= flags);
}

/*******************************************************************************
 * Mimo-proc data definitions
 ******************************************************************************/

/* ...mimo-proc data */
typedef struct XAMimoProc
{
    /***************************************************************************
     * Control data
     **************************************************************************/

    /* ...generic audio codec data */
    XACodecBase         base;

    /* ...input ports or tracks */
    XAInTrack             in_track[XA_MIMO_PROC_MAX_IN_PORTS];

    /* ...output ports or tracks */
    XAOutTrack            out_track[XA_MIMO_PROC_MAX_OUT_PORTS];

    /***************************************************************************
     * Run-time configuration parameters
     **************************************************************************/

    /* ...sample size in bytes */
    UWORD32                     sample_size;

    /* ...audio sample duration */
    UWORD64                     factor;

    /* ...audio frame size in samples */
    //UWORD32                 frame_size;

    /* ...audio frame duration */
    //UWORD32                 frame_duration;

    /* ...number of input porst or tracks */
    UWORD32                 num_in_ports;

    /* ...number of input porst or tracks */
    UWORD32                 num_out_ports;

    /* ...component schedule relaxation */
    UWORD32                 relax_sched;

    /* ...probe enabled flag */
    UWORD32                    probe_enabled;

    /* ...probe output port data */
    xf_output_port_t         probe;

    /* ...probe output buffer pointer */
    void                     *probe_output;

    /* ...mimo_proc output buffer pointer */
    void 					*out_ptr[XA_MIMO_PROC_MAX_OUT_PORTS];

    /***************************************************************************
     * response message pointer
     **************************************************************************/
    xf_message_t        *m_response;

}   XAMimoProc;

/*******************************************************************************
 * Output Track state flags
 ******************************************************************************/

/* ...output track is idle (will autostart as soon as output buffer is received) */
#define XA_OUT_TRACK_FLAG_IDLE              __XF_OUTPUT_FLAG(1 << 0)

/* ...output track is active */
#define XA_OUT_TRACK_FLAG_ACTIVE            __XF_OUTPUT_FLAG(1 << 1)

/* ...output track is active */
#define XA_OUT_TRACK_FLAG_PAUSED            __XF_OUTPUT_FLAG(1 << 2)

/* ...output track is routed */
#define XA_OUT_TRACK_FLAG_ROUTED            __XF_OUTPUT_FLAG(1 << 3)

/* ...output track setup completed */
#define XA_OUT_TRACK_FLAG_OUTPUT_SETUP      __XF_OUTPUT_FLAG(1 << 4)

/* ...output track flushing */
#define XA_OUT_TRACK_FLAG_FLUSHING          __XF_OUTPUT_FLAG(1 << 5)

/* ...output track setup flushing completed */
#define XA_OUT_TRACK_FLAG_FLUSHING_DONE     __XF_OUTPUT_FLAG(1 << 6)

/*******************************************************************************
 * Probe Output state flags
 ******************************************************************************/

/* ...probe port setup condition */
#define XA_MIMO_FLAG_PROBE_SETUP           __XF_OUTPUT_FLAG(1 << 7)

/* ...probe port is paused */
#define XA_MIMO_PROBE_PORT_PAUSED          __XF_OUTPUT_FLAG(1 << 8)

/*******************************************************************************
 * Input Track state flags
 ******************************************************************************/

/* ...input track is idle (will autostart as soon as input data received) */
#define XA_IN_TRACK_FLAG_IDLE              __XF_INPUT_FLAG(1 << 0)

/* ...input track is rendered */
#define XA_IN_TRACK_FLAG_ACTIVE            __XF_INPUT_FLAG(1 << 1)

/* ...input track is paused */
#define XA_IN_TRACK_FLAG_PAUSED            __XF_INPUT_FLAG(1 << 2)

/* ...input track input port is setup */
#define XA_IN_TRACK_FLAG_INPUT_SETUP       __XF_INPUT_FLAG(1 << 3)

/* ...input track has received data */
#define XA_IN_TRACK_FLAG_RECVD_DATA        __XF_INPUT_FLAG(1 << 4)

/*******************************************************************************
 * Helper functions
 ******************************************************************************/
/* ...Count the input tracks that have received data or are active*/
static inline UWORD32 xa_mimo_proc_check_active(XAMimoProc *mimo_proc)
{
    XAInTrack      *track;
    UWORD32        i;
    UWORD32        cnt = 0;

    for (track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, track++)
    {
        if (xa_in_track_test_flags(track, XA_IN_TRACK_FLAG_RECVD_DATA | XA_IN_TRACK_FLAG_ACTIVE))
            cnt++;
    }
    return cnt;
}

static inline UWORD32 xa_mimo_proc_input_port_ready(XAMimoProc *mimo_proc)
{
    XAInTrack      *track;
    UWORD32        i;
    UWORD32        ports_ready = 0;

    for (track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, track++)
    {
        /* ... skip port readiness check based on relax sched or if port is paused */
        if (XF_CHK_PORT_MASK(mimo_proc->relax_sched, i) || xa_in_track_test_flags(track, XA_IN_TRACK_FLAG_PAUSED))
        {
            ports_ready++;
            continue;
        }

        /* ...tbd - check only XA_IN_TRACK_FLAG_ACTIVE here? */
        if (xa_in_track_test_flags(track, XA_IN_TRACK_FLAG_RECVD_DATA | XA_IN_TRACK_FLAG_ACTIVE) && xf_input_port_ready(&track->input))
            ports_ready++;
    }

    return ports_ready;
}

static inline UWORD32 xa_mimo_proc_output_port_ready(XAMimoProc *mimo_proc)
{
    XAOutTrack  *track;
    UWORD32     i;

    for (track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, track++)
    {
        /* ... skip port readiness check based on relax sched */
        if ( XF_CHK_PORT_MASK(mimo_proc->relax_sched, (mimo_proc->num_in_ports+i)) )
        {
            /* ...skip relax_sched check during init (TENA-2999).
             * Else component is scheduled on the START buffer from 1st output port causing init not to complete */
            if (!(mimo_proc->base.state & XA_BASE_FLAG_RUNTIME_INIT))
            {
                continue;
            }
        }

        /* ...skip paused ports */
        if (xa_out_track_test_flags(track, XA_OUT_TRACK_FLAG_PAUSED))
            continue;
#if 1
        /* ...skip unrouted ports */
        if (!xa_out_track_test_flags(track, XA_OUT_TRACK_FLAG_ROUTED )
            && !(mimo_proc->base.state & XA_BASE_FLAG_RUNTIME_INIT) /* ...unrouted port readiness check to be post init. */
        )
        {
            if(mimo_proc->num_out_ports>1)
            continue;
        }
#endif
        /* ...TBD check if any output port is active (routed) */
        if (!xf_output_port_ready(&track->output))
            return 0;
    }
    return 1;
}

#ifdef MIMO_AVOID_EXCESS_SCHED
static inline UWORD32 xa_mimo_proc_input_port_reschedule_ready(XAMimoProc *mimo_proc)
{
    XAInTrack      *track;
    UWORD32        i;
    UWORD32        ports_ready = 0;

    for (track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, track++)
    {
        if (xf_input_port_ready(&track->input))
        {
            ++ports_ready;
        }
    }

    return ports_ready;
}

static inline UWORD32 xa_mimo_proc_output_port_reschedule_ready(XAMimoProc *mimo_proc)
{
    XAOutTrack  *track;
    UWORD32     i;
    UWORD32     ports_ready = 0;

    for (track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, track++)
    {
        if (xf_output_port_ready(&track->output))
        {
            ++ports_ready;
        }
    }
    return ports_ready;
}
#endif //MIMO_AVOID_EXCESS_SCHED

static inline UWORD32 xa_mimo_proc_output_port_flush_done(XAMimoProc *mimo_proc)
{
    XAOutTrack  *track;
    UWORD32     i;
    UWORD32        cnt = 0;

    for (track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, track++)
    {
        if (xa_out_track_test_flags(track, XA_OUT_TRACK_FLAG_FLUSHING_DONE))
            cnt++;
    }
    return (cnt == mimo_proc->num_out_ports);
}

/* ...prepare mimo_proc for steady operation */
static inline XA_ERRORCODE xa_mimo_proc_prepare_runtime(XAMimoProc *mimo_proc)
{
    XACodecBase    *base = (XACodecBase *) mimo_proc;
    UWORD32             frame_size;
    UWORD64             factor;
    UWORD32             i;
	xf_message_t   *m;
	xf_start_msg_t *msg;
	xf_start_msg_t msg_local;

	if ( mimo_proc->num_out_ports == 0)
	{
		msg = &msg_local;
	}
	else
	{
		m = xf_msg_dequeue(&mimo_proc->out_track[0].output.queue);
        XF_CHK_ERR(m != NULL, XAF_INVALIDVAL_ERR);
		msg = m->buffer;
	}

    /* ...memset the start-msg buffer to zero */
    memset(msg, 0, sizeof(xf_start_msg_t));

    /* ...query mimo_proc parameters */
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_SAMPLE_RATE, &msg->sample_rate);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_CHANNELS, &msg->channels);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PCM_WIDTH, &msg->pcm_width);
    TRACE(INIT, _b("mimo[%p]::runtime init: f=%u, c=%u, w=%u"), mimo_proc, msg->sample_rate, msg->channels, msg->pcm_width);

    for (i=0; i<mimo_proc->num_in_ports; i++)
    {
        XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, mimo_proc->in_track[i].idx, &msg->input_length[i]);
        TRACE(INIT, _b("mimo[%p]::runtime init: i[%d]=%u"), mimo_proc, i, msg->input_length[i]);
    }
    for (i=0; i<mimo_proc->num_out_ports; i++)
    {
        XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, mimo_proc->out_track[i].idx, &msg->output_length[i]);
        TRACE(INIT, _b("mimo[%p]::runtime init: o[%d]=%u"), mimo_proc, i, msg->output_length[i]);
    }

	if ( mimo_proc->num_out_ports == 0)
	{
		msg->output_length[0] = msg->input_length[0];
	}

    /* ...populate port buffer length */
    msg->probe_length = (mimo_proc->probe_enabled) ? mimo_proc->probe.length : 0;

    /* ...save sample size in bytes */
    mimo_proc->sample_size = msg->channels * ((msg->pcm_width == 8) ? 1 :((msg->pcm_width == 16) ? 2 : 4));

    /* ...sample size should be positive */
    XF_CHK_ERR(mimo_proc->sample_size > 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...calculate frame duration; get number of samples in the frame (don't like division here - tbd) */
    frame_size = msg->output_length[0] / mimo_proc->sample_size;

    /* ...it must be a multiple */
    XF_CHK_ERR(frame_size * mimo_proc->sample_size == msg->output_length[0], XA_API_FATAL_INVALID_CMD_TYPE);

    if(base->comp_type == XAF_MIMO_PROC_NN)
    {
        /* ...retrieve upsampling factor for given frame rate */
        XF_CHK_ERR(factor = xf_timebase_factor_fps(msg->sample_rate), XA_API_FATAL_INVALID_CMD_TYPE); /* ...for NNE components, refer timebase_factor w.r.t frames-per-second. */

        /* ...set frame duration factor (converts number of bytes into timebase units) */
        mimo_proc->factor = factor / mimo_proc->sample_size;

        /* ...factor must be a multiple for supported sample rates, can be off by value of 1 for fps (frames-per-second) components */
        XF_CHK_ERR((factor - mimo_proc->factor * mimo_proc->sample_size)<2, XA_MIMO_PROC_CONFIG_FATAL_RANGE);
    }
    else
    {
        /* ...calculate mimo-proc frame duration; get upsample factor */
        XF_CHK_ERR(factor = xf_timebase_factor(msg->sample_rate), XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...set frame duration factor (converts number of bytes into timebase units) */
        mimo_proc->factor = factor / mimo_proc->sample_size;

        /* ...factor must be a multiple */
        XF_CHK_ERR(mimo_proc->factor * mimo_proc->sample_size == factor, XA_MIMO_PROC_CONFIG_FATAL_RANGE);
    }

    TRACE(INIT, _b("ts-factor: %llu (%llu)"), mimo_proc->factor, factor);

    if(mimo_proc->num_out_ports)
    {
        WORD32 err, err_alloc;
        UWORD32 port_idx = mimo_proc->num_in_ports;

        /* ...allocate connect buffers. err value is 1 if buffer is already allocated */
        err = xf_output_port_route_alloc(&mimo_proc->out_track[0].output, msg->output_length[0], base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
        err_alloc = err;

        if(!err)
        {
            /* ...set routed flag */
            xa_out_track_set_flags(&mimo_proc->out_track[0], XA_OUT_TRACK_FLAG_ROUTED);
            /* ...indicate plugin of the output port state */
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_CONNECT, &port_idx);
        }
        else
        {
            /* ...clear routed flag */
            xa_out_track_clear_flags(&mimo_proc->out_track[0], XA_OUT_TRACK_FLAG_ROUTED);
            /* ...pass route info to component */
            m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &port_idx);

            TRACE(INFO, _b("mimo_proc[%p]:port %d marked unrouted before route in prepare runtime"), mimo_proc, port_idx);
        }

        port_idx++;

		/* ...pass response to caller */
		xf_response_data(m, sizeof(*msg));

		/* ...tbd - more checks here? */
		for (i=1; i<mimo_proc->num_out_ports; i++, port_idx++)
		{
			m = xf_msg_dequeue(&mimo_proc->out_track[i].output.queue);

			if (m != NULL)
			{
                /* ...allocate connect buffers */
                err = xf_output_port_route_alloc(&mimo_proc->out_track[i].output, msg->output_length[i], base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
                err_alloc |= err;

                if(!err)
                {
                    /* ...set routed flag */
                    xa_out_track_set_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_ROUTED);
                    /* ...indicate plugin of the output port state */
                    m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_CONNECT, &port_idx);
                }
                else
                {
                    /* ...clear routed flag */
                    xa_out_track_clear_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_ROUTED);
                    /* ...pass route info to component */
                    m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &port_idx);

                    TRACE(INFO, _b("mimo_proc[%p]:port %d marked unrouted before route in prepare runtime"), mimo_proc, port_idx);
                }

				/* ...pass response to caller */
				xf_response_data(m, sizeof(*msg));
			}
		}

        /* ...schedule for processing if i/o ports are ready */
        if (((err_alloc == 0) || (err_alloc ==1)) && xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
        {
            xa_base_schedule(base, 0);
        }
	}

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands handlers
 ******************************************************************************/

/* ...EMPTY-THIS-BUFFER command processing */
static XA_ERRORCODE xa_mimo_proc_empty_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XAMimoProc *mimo_proc = (XAMimoProc *) base;
    UWORD32    i          = XF_MSG_DST_PORT(m->id);
    XAInTrack  *track     = &mimo_proc->in_track[i];

    /* ...make sure the port is valid */
    XF_CHK_ERR(i < mimo_proc->num_in_ports, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    TRACE(INPUT, _b("in_track-%u: received buffer [%p]:%u"), i, m->buffer, m->length);

    /* ...update received data for the track */
    if (m->length)
        xa_in_track_set_flags(track, XA_IN_TRACK_FLAG_RECVD_DATA);
    else
        xa_in_track_clear_flags(track, XA_IN_TRACK_FLAG_RECVD_DATA);

    /* ...place received message into track input port */
    if (xf_input_port_put(&track->input, m))
    {
        /* ...restart stream if it is in completed state */
        if (base->state & XA_BASE_FLAG_COMPLETED)
        {
            /* ...reset execution stage */
            base->state = XA_BASE_FLAG_POSTINIT | XA_BASE_FLAG_EXECUTION;

            /* ...reset execution runtime */
            XA_API(base, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_RUNTIME_INIT, NULL);

            /* ...tbd - reset produced samples counter, required? */
        }

        /* ...process track autostart if needed */
        if (xa_in_track_test_flags(track, XA_IN_TRACK_FLAG_IDLE))
        {
            /* ...put track into active state */
            xa_in_track_toggle_flags(track, XA_IN_TRACK_FLAG_IDLE | XA_IN_TRACK_FLAG_ACTIVE);

            /* ...save track presentation timestamp */
            //track->pts = mimo_proc->pts;
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_CONNECT, &i);

            TRACE(INFO, _b("in_track-%u started"), i);
        }

        /* ...schedule data processing if all active (routed) output ports are available */
        if (xa_mimo_proc_output_port_ready(mimo_proc) &&
            (!(base->state & XA_BASE_FLAG_RUNTIME_INIT) || !mimo_proc->num_out_ports) /* ...allow sched if no outport */
        )
        {
            /* ...force data processing */
            xa_base_schedule(base, 0);
        }
    }

    return XA_NO_ERROR;
}

/* ...FILL-THIS-BUFFER command processing */
static XA_ERRORCODE xa_mimo_proc_fill_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XAMimoProc *mimo_proc = (XAMimoProc *) base;
    UWORD32    i          = XF_MSG_DST_PORT(m->id) - mimo_proc->num_in_ports;
    XAOutTrack  *track    = &mimo_proc->out_track[i];

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...handle probe setup if probe is enabled */
    if (mimo_proc->probe_enabled && i == mimo_proc->num_out_ports)
    {
        if (base->state & XA_BASE_FLAG_COMPLETED || xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
        {
            /* ...return message arrived from application immediately */
            xf_response_ok(m);

            return XA_NO_ERROR;
        }
        else
        {
            TRACE(PROBE, _b("Received probe output buffer [%p]:%u"), m->buffer, m->length);

            /* ...adjust message length (may be shorter than original) */
            m->length = mimo_proc->probe.length;
        }

        if (xf_output_port_put(&mimo_proc->probe, m) && xa_mimo_proc_output_port_ready(mimo_proc) && xa_mimo_proc_input_port_ready(mimo_proc))
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
    XF_CHK_ERR(i < mimo_proc->num_out_ports, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process runtime initialization explicitly */
    if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
    {
        /* ...message must be zero-length */
        XF_CHK_ERR(m->length == 0, XA_MIMO_PROC_EXEC_FATAL_STATE);
    }
    else if (m == xf_output_port_control_msg(&track->output))
    {
        /* ... mark flushing sequence is done */
        xf_output_port_flush_done(&track->output);

        /* ... mark flushing sequence is done locally also */
        xa_out_track_set_flags(track, XA_OUT_TRACK_FLAG_FLUSHING_DONE);

        TRACE(INFO, _b("out_track-%u flushed"), i);

        if (xa_mimo_proc_output_port_flush_done(mimo_proc))
        {
            XAInTrack *in_track;

            for (in_track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, in_track++)
            {
                /* ...input stream is over; return zero-length input back to caller */
                xf_input_port_purge(&in_track->input);
            }

            /* ...flush probe port */
            if (mimo_proc->probe_enabled)
            {
                if (!xf_output_port_flush(&mimo_proc->probe, XF_FLUSH))
                {
                    BUG(1, _x("probe port: invalid state"));
                }
            }
        }

#if 1   //TENA_2379
        if (xf_output_port_unrouting(&track->output))
        {
            /* ...flushing during port unrouting; complete unroute sequence */
            xf_output_port_unroute_done(&track->output, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
            TRACE(INFO, _b("port is unrouted"));
        }
#endif
        else if (m->length == XF_MSG_LENGTH_INVALID)
        {
            /* ...complete internal unrouting of the port whose dest no longer exists */
            xf_output_port_unroute(&track->output, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
            TRACE(INFO, _b("mimo_proc[%p] completed internal unroute of out_track-%u"), mimo_proc, i);
        }

        return XA_NO_ERROR;
    }
    else if ((base->state & XA_BASE_FLAG_COMPLETED) && !xf_output_port_routed(&track->output))
    {
        /* ...return message arrived from application immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }
    /* ...indicates that the downstream component no longer exists */
    else if ((m->length == XF_MSG_LENGTH_INVALID) && xf_output_port_routed(&track->output))
    {
         m->length = track->output.length; /* ...reset length for sanity */

        if (!xf_output_port_flushing(&track->output))
        {
            UWORD32 port_idx = XF_MSG_DST_PORT(m->id);

            /* ...cancel any pending processing */
            xa_base_cancel(base);

            /* ...output port is invalid; trigger port flush to collect all the buffers in transit */
            (void) xf_output_port_flush(&track->output, XF_FILL_THIS_BUFFER);

            /* ...clear output-port-setup condition */
            xa_out_track_clear_flags(track, XA_OUT_TRACK_FLAG_OUTPUT_SETUP);

            /* ...clear routed flag of mimo-class. Note: ROUTED flag of output is not touched here */
            xa_out_track_clear_flags(track, XA_OUT_TRACK_FLAG_ROUTED);

            /* ...pass resume info to component. Clear paused status at component, so that reconnect wont be affected with the previous pause */
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_RESUME, &port_idx);

            /* ...pass unroute info to component */
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &port_idx);

            /* ...flushing sequence is started; wait until flow-control message returns */
            xa_out_track_set_flags(track, XA_OUT_TRACK_FLAG_FLUSHING);

            if (xa_mimo_proc_output_port_ready(mimo_proc)
                && (xa_mimo_proc_input_port_ready(mimo_proc) || (base->state & XA_BASE_FLAG_RUNTIME_INIT))
            )
            {
                /* ...check probe port, if probe is enabled */
                if (mimo_proc->probe_enabled && !(base->state & XA_BASE_FLAG_RUNTIME_INIT))
                {
                    if (!xf_output_port_ready(&mimo_proc->probe) && !xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
                        return XA_NO_ERROR;
                }

                /* ...force data processing */
                xa_base_schedule(base, 0);
            }
            TRACE(INFO, _b("mimo_proc[%p] started internal unroute of port[%u]"), mimo_proc, i);
         }
         TRACE(INFO, _b("mimo_proc[%p] drop buffer port[%u]"), mimo_proc, i);

         return XA_NO_ERROR;
    }
    else
    {
        /* ...adjust message length (may be shorter than original) [TENA-2957] */
        m->length = track->output.length;
    }

    if (xf_output_port_put(&track->output, m))
    {
        /* ...process track autostart if needed */
        if (xa_out_track_test_flags(track, XA_OUT_TRACK_FLAG_IDLE))
        {
            /* ...put track into active state */
            xa_out_track_toggle_flags(track, XA_OUT_TRACK_FLAG_IDLE | XA_OUT_TRACK_FLAG_ACTIVE);

            /* ...save track presentation timestamp */
            //track->pts = mimo_proc->pts;

            TRACE(INFO, _b("out_track-%u started"), i);
        }

#if 1
        /* ...set route flag for sink components */
        if (!xa_out_track_test_flags(track, XA_OUT_TRACK_FLAG_ROUTED) && (m->length != 0) && (!xf_output_port_unrouting(&track->output)))
        {
            int port = XF_MSG_DST_PORT(m->id);

        	xa_out_track_set_flags(track, XA_OUT_TRACK_FLAG_ROUTED);

            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_CONNECT, &port);
        }
#endif
        TRACE(OUTPUT, _b("received output buffer track-%u [%p]:%u"), i, m->buffer, m->length);

        /* ...put message into output port */
        /* ...check for readiness of both ports to avoid over-scheduling and allow scheduling at init without input */
        if (xa_mimo_proc_output_port_ready(mimo_proc)
            && (xa_mimo_proc_input_port_ready(mimo_proc) || (base->state & XA_BASE_FLAG_RUNTIME_INIT))
        )
        {
            /* ...check probe port, if probe is enabled */
            if (mimo_proc->probe_enabled && !(base->state & XA_BASE_FLAG_RUNTIME_INIT))
            {
                if (!xf_output_port_ready(&mimo_proc->probe) && !xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
                    return XA_NO_ERROR;
            }

            /* ...force data processing */
            xa_base_schedule(base, 0);
        }
    }

    return XA_NO_ERROR;
}

/* ...output port routing */
static XA_ERRORCODE xa_mimo_proc_port_route(XACodecBase *base, xf_message_t *m)
{
    XAMimoProc             *mimo_proc = (XAMimoProc *) base;
    xf_route_port_msg_t    *cmd = m->buffer;
    UWORD32                 src = XF_MSG_DST(m->id);
    UWORD32                 dst = cmd->dst;
    UWORD32                 i   = XF_MSG_DST_PORT(m->id) - mimo_proc->num_in_ports;
    xf_output_port_t       *port = &mimo_proc->out_track[i].output;
    UWORD32                 port_idx = XF_MSG_DST_PORT(m->id);

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure sane output port is addressed */
    XF_CHK_ERR(i < mimo_proc->num_out_ports, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure port is not routed yet */
    XF_CHK_ERR(!xf_output_port_routed(port), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...reset mimo-specific output port state flags */
    mimo_proc->out_track[i].output.flags = 0;

    /* ...set output track as active */
    xa_out_track_set_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_ACTIVE);

    TRACE(INFO, _b("route: out_track-%u started"), i);

    /* ...route output port - allocate queue */
    XF_CHK_ERR(xf_output_port_route(port, __XF_MSG_ID(dst, src), cmd->alloc_number, cmd->alloc_size, cmd->alloc_align, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]) == 0, XA_API_FATAL_MEM_ALLOC);

    if(cmd->alloc_size)
    {
        /* ...set routed flag */
        xa_out_track_set_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_ROUTED);
        /* ...pass route info to component */
        m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_CONNECT, &port_idx);
    }
    else
    {
        /* ...clear routed flag */
        xa_out_track_clear_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_ROUTED);
        /* ...pass route info to component */
        m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &port_idx);

        TRACE(INFO, _b("mimo_proc[%p]:port %d marked unrouted before route"), mimo_proc, port_idx);
    }

    /* ...schedule processing instantly - tbd - check if we have anything pending on input */
    /* ...TBD - do we need to check if other output ports are ready? */
    if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
    {
        xa_base_schedule(base, 0);
    }

    /* ...pass success result to caller */
    xf_response_ok(m);

    return XA_NO_ERROR;
}

/* ...port unroute command */
static XA_ERRORCODE xa_mimo_proc_port_unroute(XACodecBase *base, xf_message_t *m)
{
    XAMimoProc         *mimo_proc = (XAMimoProc *) base;
    UWORD32             i = XF_MSG_DST_PORT(m->id) - mimo_proc->num_in_ports;
    xf_output_port_t   *port = &mimo_proc->out_track[i].output;
    UWORD32             port_idx = XF_MSG_DST_PORT(m->id);
    WORD32              error = 0;

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure sane output port is addressed */
    XF_CHK_ERR(i < mimo_proc->num_out_ports, XA_API_FATAL_INVALID_CMD_TYPE);

    if(!xf_output_port_routed(port))
    {
        /* ...if XF_MSG_LENGTH_INVALID triggered internal unroute is completed, send the response instantly */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    /* ...cancel any pending processing */
    xa_base_cancel(base);

    /* ...clear output-port-setup condition */
    xa_out_track_clear_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_OUTPUT_SETUP);

    /* ...clear routed flag */
    xa_out_track_clear_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_ROUTED);

    /* ...pass resume info to component. Clear paused status at component, so that reconnect wont be affected with the previous pause */
    m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_RESUME, &port_idx);

    /* ...pass unroute info to component */
    error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &port_idx);

    /* ...only the first error is captured*/
    if ((error) && (m->error == 0))
        m->error = error;

    /* ...pass flush command down the graph */
    if (xf_output_port_flush(port, XF_FLUSH))
    {
        TRACE(INFO, _b("port is idle; instantly unroute"));

        /* ...flushing sequence is not needed; command may be satisfied instantly */
        xf_output_port_unroute(port, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);

        /* ...schedule processing if other output ports are ready */
        if (xa_mimo_proc_output_port_ready(mimo_proc))
        {
            xa_base_schedule(base, 0);
        }

        /* ...pass response to the proxy */
        xf_response_ok(m);
    }
    else
    {
        TRACE(INFO, _b("port is busy; propagate unroute command"));

        xa_out_track_set_flags(&mimo_proc->out_track[i], XA_OUT_TRACK_FLAG_FLUSHING);

        /* ...flushing sequence is started; save flow-control message */
        xf_output_port_unroute_start(port, m);
    }

    return XA_NO_ERROR;
}

/* ...FLUSH command processing */
static XA_ERRORCODE xa_mimo_proc_flush(XACodecBase *base, xf_message_t *m)
{
    XAMimoProc *mimo_proc = (XAMimoProc *) base;
    UWORD32     i = XF_MSG_DST_PORT(m->id);

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...ensure input parameter length is zero or XF_MSG_LENGTH_INVALID */
    XF_CHK_ERR((m->length == 0) || (m->length == XF_MSG_LENGTH_INVALID), XA_API_FATAL_INVALID_CMD_TYPE);

    TRACE(INFO, _b("flush command received"));

    /* ...if flush command is addressed to input port */
    if (i < mimo_proc->num_in_ports)
    {
        XAInTrack  *in_track = &mimo_proc->in_track[i];

        /* ...input port flushing; check the track state is valid */
        if (xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_ACTIVE | XA_IN_TRACK_FLAG_PAUSED))
        {
            /* ...purge input port */
            xf_input_port_purge(&in_track->input);

            /* ...force clearing of ACTIVE and INPUT_SETUP condition */
            xa_in_track_clear_flags(in_track, XA_IN_TRACK_FLAG_ACTIVE | XA_IN_TRACK_FLAG_PAUSED | XA_IN_TRACK_FLAG_INPUT_SETUP | XA_IN_TRACK_FLAG_RECVD_DATA);

            /* ...and enter into idle state */
            xa_in_track_set_flags(in_track, XA_IN_TRACK_FLAG_IDLE);

            /* ...pass resume info to component. Clear paused status at component, so that reconnect wont be affected with the previous pause */
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_RESUME, &i);

            /* ...pass idle state (disconnect) info to component */
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &i);

            /* ...other tracks may be waiting for this track, so force data processing */
            if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
            {
                xa_base_schedule(base, 0);
            }

            TRACE(INFO, _b("mimo_proc[%p]::in_track[%u] flushed"), mimo_proc, i);
        }

        /* ...complete message instantly (no propagation to output port) */
        xf_response(m);
    }
    else
    {
        XAOutTrack  *out_track;

        i = XF_MSG_DST_PORT(m->id) - mimo_proc->num_in_ports;
        out_track = &mimo_proc->out_track[i];

        /* ...make sure the port is valid */
        XF_CHK_ERR(i < mimo_proc->num_out_ports, XA_API_FATAL_INVALID_CMD_TYPE);

        if (xf_output_port_unrouting(&out_track->output))
        {
            if (xa_mimo_proc_output_port_ready(mimo_proc))
            {
                xa_base_schedule(base, 0);
            }

            /* ... mark flushing sequence is done locally also */
            xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_FLUSHING_DONE);

            /* ...complete original flow-control command */
            if ((base->state & XA_BASE_FLAG_COMPLETED) && (xa_mimo_proc_output_port_flush_done(mimo_proc)))
            {
                XAInTrack *in_track;

                for (in_track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, in_track++)
                {
                    /* ...input stream is over; return zero-length input back to caller */
                    xf_input_port_purge(&in_track->input);
                }
            }

            /* ...flushing during port unrouting; complete unroute sequence */
            xf_output_port_unroute_done(&out_track->output, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);

            TRACE(INFO, _b("port is unrouted"));
        }
        else
        {
            /* ...output port flush command/response; check if the port is routed */
            if (!xf_output_port_routed(&out_track->output))
            {
                /* ...complete all queued messages */
                xf_output_port_flush(&out_track->output, XF_FLUSH);

                /* ...and pass response to flushing command */
                xf_response(m);
            }
            else
            {
                /* ...response to flushing command received */
                BUG(m != xf_output_port_control_msg(&out_track->output), _x("invalid message: %p"), m);

                /* ...mark flushing sequence is completed */
                xf_output_port_flush_done(&out_track->output);

                /* ... mark flushing sequence is done locally also */
                xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_FLUSHING_DONE);

            }

            /* ...complete original flow-control command */
            if (xa_mimo_proc_output_port_flush_done(mimo_proc))
            {
                XAInTrack *in_track;

                for (in_track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, in_track++)
                {
                    /* ...input stream is over; return zero-length input back to caller */
                    xf_input_port_purge(&in_track->input);
                }
            }

            /* ...clear output-setup condition */
            xa_out_track_clear_flags(out_track, XA_OUT_TRACK_FLAG_PAUSED | XA_OUT_TRACK_FLAG_OUTPUT_SETUP);
        }
    }

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Codec API implementation
 ******************************************************************************/

/* ...buffers handling */
static XA_ERRORCODE xa_mimo_proc_memtab(XACodecBase *base, WORD32 idx, WORD32 type, WORD32 size, WORD32 align, UWORD32 core)
{
    XAMimoProc    *mimo_proc = (XAMimoProc *)base;
    UWORD32        i;

    if (type == XA_MEMTYPE_INPUT)
    {
        XAInTrack    *track;

        for (track = &mimo_proc->in_track[i = 0]; i < XA_MIMO_PROC_MAX_IN_PORTS; i++, track++)
        {
            /* ...get next available track */
            if (!xa_in_track_test_flags(track, XA_IN_TRACK_FLAG_IDLE))
                break;
        }

        /* ...input buffer allocation; check track number is sane */
        XF_CHK_ERR(i < mimo_proc->num_in_ports, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...create input port for a track */
        XF_CHK_ERR(xf_input_port_init(&track->input, size, align, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_INPUT]) == 0, XA_API_FATAL_MEM_ALLOC);

        if(size)
        {
            /* ...set input port buffer */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, idx, track->input.buffer);
        }

        /* ...store idx in port state */
        track->idx = idx;

        /* ...put track into idle state (will start as soon as we receive data) */
        xa_in_track_set_flags(track, XA_IN_TRACK_FLAG_IDLE);

        XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &i);

        TRACE(INIT, _b("mimo_proc[%p]::in_track[%u] input port created - size=%u"), mimo_proc, i, size);
    }
    else
    {
        XAOutTrack    *track;

        /* ...output buffer allocation */
        XF_CHK_ERR(type == XA_MEMTYPE_OUTPUT, XA_API_FATAL_INVALID_CMD_TYPE);

        for (track = &mimo_proc->out_track[i = 0]; i < XA_MIMO_PROC_MAX_OUT_PORTS; i++, track++)
        {
            /* ...get next available track */
            if (!xa_out_track_test_flags(track, XA_OUT_TRACK_FLAG_IDLE))
                break;
        }

        /* ...check output track number is sane */
        XF_CHK_ERR(i < mimo_proc->num_out_ports, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...create output port for a track */
        XF_CHK_ERR(xf_output_port_init(&track->output, size) == 0, XA_API_FATAL_MEM_ALLOC);

        /* ...store idx in port state */
        track->idx = idx;

        /* ...put track into idle state (will start as soon as we receive output buffer) */
        xa_out_track_set_flags(track, XA_OUT_TRACK_FLAG_IDLE);

        /* ...set routed flag */
        xa_out_track_set_flags(track, XA_OUT_TRACK_FLAG_ROUTED);

        TRACE(INIT, _b("mimo_proc[%p]::out_track[%u] output port created - size=%u"), mimo_proc, i, size);
    }

    if (mimo_proc->probe_enabled)
    {
        XAInTrack     *inp_track;
        XAOutTrack    *out_track;
        UWORD32          io_ports_created = 1;
    	UWORD32       probe_size = 0;

        for (inp_track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, inp_track++)
        {
            if ((io_ports_created &= xf_input_port_created(&inp_track->input)) == 0)
                break;

            /*... return error if probe is enabled for input port with input bypass enabled. */
            if((xf_input_port_bypass(&inp_track->input) && (XF_CHK_PORT_MASK(mimo_proc->probe_enabled, i))))
            {
                TRACE(ERROR, _x("Probe buffer-length error on port[%d] with input bypass"), i);
                return XAF_INVALIDVAL_ERR;
            }

            probe_size += XF_CHK_PORT_MASK(mimo_proc->probe_enabled, i) ?  XF_ALIGNED_PROBE_SIZE(mimo_proc->in_track[i].input.length) : 0;
        }

        for (out_track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, out_track++)
        {
            if ((io_ports_created &= xf_output_port_created(&out_track->output)) == 0)
                break;

            probe_size += XF_CHK_PORT_MASK(mimo_proc->probe_enabled, (mimo_proc->num_in_ports + i)) ?  XF_ALIGNED_PROBE_SIZE(mimo_proc->out_track[i].output.length) : 0;
        }

        if (io_ports_created)
        {
            /* ...initialize probe output port queue (no allocation here yet) */
            XF_CHK_ERR(xf_output_port_init(&mimo_proc->probe, probe_size) == 0, XA_API_FATAL_MEM_ALLOC);

            /* ...put probe port into pause state */
            xa_port_set_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED);

            TRACE(INIT, _b("mimo_proc[%p]::probe output port created - size=%u"), mimo_proc, probe_size);
        }
    }

    return XA_NO_ERROR;
}

/* ...preprocessing function */
static XA_ERRORCODE xa_mimo_proc_preprocess(XACodecBase *base)
{
    XAMimoProc     *mimo_proc = (XAMimoProc *) base;
    XAInTrack      *in_track;
    XAOutTrack     *out_track;
    UWORD32         i;
    XA_ERRORCODE    e = XA_MIMO_PROC_EXEC_NONFATAL_NO_DATA;
    UWORD32         inport_nodata = 0;

    /* ...prepare probe buffer if required */
    if (mimo_proc->probe_enabled && !xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
    {
        if (!(mimo_proc->probe.flags & XA_MIMO_FLAG_PROBE_SETUP))
        {
            void *probe_output;

            /* ...get probe buffer from port, if possible */
            if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
            {
                /* ...run-time is not initialized yet; probe buffer not required */
            }
            else if ((probe_output = xf_output_port_data(&mimo_proc->probe)) == NULL)
            {
                /* ...no probe buffer available */
                return XA_MIMO_PROC_EXEC_NONFATAL_NO_DATA;
            }
            else
            {
                /* ...record probe buffer for producing probe output */
                mimo_proc->probe_output = probe_output;

                /* ...mark probe output port is setup */
                mimo_proc->probe.flags ^= XA_MIMO_FLAG_PROBE_SETUP;
            }
        }
    }

    /* ...setup output buffer pointers */
    for (out_track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, out_track++)
    {
        /* ...skip idle output ports */
        if (!xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_ACTIVE))  continue;

        if (base->state & XA_BASE_FLAG_RUNTIME_INIT) return XA_NO_ERROR;

        /* ...skip paused output ports */
        if (xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_PAUSED))  continue;

        /* ...skip unrouted output ports */
        if (!xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_ROUTED))  continue;

        /* ...prepare output buffer */
        if (!xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_OUTPUT_SETUP))
        {
            void   *output;

            /* ...set output buffer pointer */
            if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
            {
                /* ...no actual data processing during initialization */
                return XA_NO_ERROR;
            }
            else if ((output = xf_output_port_data(&out_track->output)) == NULL)
            {
                /* ...no output buffer available */
                if ( !XF_CHK_PORT_MASK(mimo_proc->relax_sched, (mimo_proc->num_in_ports+i)) )
                    return e;
            }

            /* ...copy mimo_proc output buffer pointer for probe */
            mimo_proc->out_ptr[i] = output;

            /* ...set output buffer pointer */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, out_track->idx, output);

            /* ...mark output port is setup */
            xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_OUTPUT_SETUP);
        }
    }

    /* ...setup input buffer pointers and length */
    for (in_track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, in_track++)
    {
        /* ...skip tracks that are not active */
        if (!xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_ACTIVE))  continue;

        /* ...set temporary mimo_proc request */
        e = XA_NO_ERROR;

        /* ...skip the tracks that has been setup already */
        if (xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_INPUT_SETUP))  continue;

        UWORD32 filled = 0;
        if (xf_input_port_bypass(&in_track->input))
        {
            void *input;

            /* ...port is in bypass mode; try to update the buffer pointer, remaining bytes if necessary */
            xf_input_port_fill(&in_track->input);

            /* ...use input buffer directly; check if there is data available */
            if ((input = xf_input_port_data(&in_track->input)) != NULL)
            {
                /* ...set input data buffer pointer */
                XA_API(base, XA_API_CMD_SET_MEM_PTR, i, input);

                /* ...retrieve number of input bytes */
                filled = xf_input_port_length(&in_track->input);
            }
        }
        else
        {
            if (!xf_input_port_done(&in_track->input))
            {
                /* ...take actual data from input port (mimo_proc is always using internal buffer) */
                xf_input_port_fill(&in_track->input);
            }

            /* ...retrieve number of bytes available */
            filled = xf_input_port_level(&in_track->input);
        }

            /* ...allow partially filled inputs to components */
            if (!xf_input_port_done(&in_track->input) && !filled && !xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_PAUSED))
            {
                if ( !XF_CHK_PORT_MASK(mimo_proc->relax_sched, i) )
                {
                    inport_nodata++;
                    if (inport_nodata == xa_mimo_proc_check_active(mimo_proc))
                    {
                        return XA_MIMO_PROC_EXEC_NONFATAL_NO_DATA;
                    }
                    continue;
                }
            }

        /* ...set total number of bytes we have in buffer */
        XA_API(base, XA_API_CMD_SET_INPUT_BYTES, in_track->idx, &filled);

        /* ...actual data is to be played */
        TRACE(INPUT, _b("in_track-%u: filled %u bytes"), i, filled);

        /* ...check if input stream is over */
        if (xf_input_port_done(&in_track->input))
        {
            /* ...pass input-over command to the codec to indicate the final buffer */
            XA_API(base, XA_API_CMD_INPUT_OVER, in_track->idx, NULL);

            TRACE(INFO, _b("mimo_proc[%p]:in_track[%u] signal input-over (filled: %u)"), mimo_proc, i, filled);
        }

        /* ...mark the track input is setup */
        xa_in_track_set_flags(in_track, XA_IN_TRACK_FLAG_INPUT_SETUP);
    }

    /* ...do mimo_proc operation only when all active tracks are setup */
    return e;
}

/* ...postprocessing function */
static XA_ERRORCODE xa_mimo_proc_postprocess(XACodecBase *base, int done)
{
    XAMimoProc     *mimo_proc = (XAMimoProc *) base;
    XAInTrack      *in_track;
    XAOutTrack     *out_track;
    UWORD32         produced = 0;
    UWORD32         consumed = 0;
    UWORD32         output_produced = 0;
    UWORD32         input_consumed = 0;
    UWORD32         i;
    UWORD32         probe_length = 0;
    void           *probe_outptr = mimo_proc->probe_output;

    /* ...input ports maintenance; process all tracks */
    for (in_track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, in_track++)
    {
        /* ...skip the tracks that are not runing */
        if (!xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_ACTIVE))  continue;

        if (!(done && (base->state & XA_BASE_FLAG_EXECUTION)))
        {
            /* ...get total amount of consumed bytes */
            XA_API(base, XA_API_CMD_GET_CURIDX_INPUT_BUF, in_track->idx, &consumed);
        }

        TRACE(INPUT, _b("in_track-%u::postprocess(c=%u)"), i, consumed);

        /* ...produce probe output if required (and if runtime is initialized) */
        if (mimo_proc->sample_size && mimo_proc->probe_enabled && !xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
        {
            if (XF_CHK_PORT_MASK(mimo_proc->probe_enabled, i))
            {
                /* ...copy input port data onto probe port */
                probe_outptr = xf_copy_probe_data(probe_outptr, i, consumed, in_track->input.buffer);

                /* ...compute probe data length locally */
                probe_length += consumed;
            }
        }

        if (consumed)
        {
            input_consumed = consumed;
            /* ...consume that amount from input port */
            xf_input_port_consume(&in_track->input, consumed);

            /* ...clear input setup flag */
            xa_in_track_clear_flags(in_track, XA_IN_TRACK_FLAG_INPUT_SETUP);
        }
        /* ...check if port is not completely filled - insufficient data */
        else if (in_track->input.length != xf_input_port_level(&in_track->input))
        {
            /* ...clear input setup flag */
            xa_in_track_clear_flags(in_track, XA_IN_TRACK_FLAG_INPUT_SETUP);
        }

        /* ...check if input port is done */
        if (xf_input_port_done(&in_track->input) && !xf_input_port_level(&in_track->input))
        {
            /* ...switch to idle state */
            xa_in_track_toggle_flags(in_track, XA_IN_TRACK_FLAG_ACTIVE | XA_IN_TRACK_FLAG_IDLE);

            /* ...pass input port disconnect to component */
            XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &i);

            TRACE(INFO, _b("mimo_proc[%p]::in_track[%u] completed, exec_done:%d"), mimo_proc, i, done);

            /* ...check if this is not last active input port */
            if (xa_mimo_proc_check_active(mimo_proc)
                /* ...also skip if other unfinished ports exist - TENA_2441. */
                || (!done)
            )
            {
                /* ...not last active input port - no need to wait for output flush, purge instantly */
                xf_input_port_purge(&in_track->input);
            }
        }
    }

    /* ...output ports maintenance; process all tracks */
    for (out_track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, out_track++)
    {
        /* ...skip idle output ports */
        if (!xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_ACTIVE))  continue;

        /* ...skip unrouted output ports */
        if (!xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_ROUTED))  continue;

        /* ...check if we have produced anything, only if runtime is initialized */
        if(mimo_proc->sample_size)
        {
            XA_API(base, XA_API_CMD_GET_OUTPUT_BYTES, out_track->idx, &produced);
        }

        TRACE(OUTPUT, _b("mimo_proc[%p]::out_track[%u] postprocess(p=%u, done=%u)"), mimo_proc, i, produced, done);

        /* ...produce probe output if required (and if runtime is initialized) */
        if (mimo_proc->sample_size && mimo_proc->probe_enabled && !xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
        {
            if (XF_CHK_PORT_MASK(mimo_proc->probe_enabled, (mimo_proc->num_in_ports + i)))
            {
                /* ...copy output port data onto probe port */
                probe_outptr = xf_copy_probe_data(probe_outptr, (mimo_proc->num_in_ports + i), produced, mimo_proc->out_ptr[i]);

                /* ...compute probe data length locally */
                probe_length += produced;
            }
        }

        if (produced)
        {
            /* ...all ports need not produce same length - tbd */
            if (output_produced)
            {
                /* ...make sure we have produced same length on all output ports */
                BUG((produced != output_produced), _x("Invalid output lengths across ports: %u != %u"), produced, output_produced);
            }
            else
            {
                output_produced = produced;
            }

            /* ...push data from output port */
            xf_output_port_produce(&out_track->output, produced);

            /* ...clear output-setup and flush-done condition */
            xa_out_track_clear_flags(out_track, XA_OUT_TRACK_FLAG_OUTPUT_SETUP | XA_OUT_TRACK_FLAG_FLUSHING_DONE);
        }
        else if (mimo_proc->out_ptr[i] == NULL)
        {
            /* ...clear output-setup condition with relax schedule on output port. TENA-2543 */
            xa_out_track_clear_flags(out_track, XA_OUT_TRACK_FLAG_OUTPUT_SETUP);
        }
    }

    if (probe_length)
    {
        /* ...compute total probe output length (including length variables and alignment) */
        probe_length = probe_outptr - mimo_proc->probe_output;

        /* ...immediately complete probe output buffer (don't wait until it gets filled) */
        xf_output_port_produce(&mimo_proc->probe, probe_length);

        /* ...clear probe port setup flag */
        mimo_proc->probe.flags ^= XA_MIMO_FLAG_PROBE_SETUP;
    }

    /* ...process execution stage transitions */
    if (done)
    {
        if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
        {
            /* ...failed to initialize runtime (can't be? - tbd)*/
            BUG(1, _x("breakpoint"));
        }
        else if (base->state & XA_BASE_FLAG_EXECUTION)
        {
#if 1
            /* ...shifted into prepare_runtime after CONNECT_NOALLOC changes. */
#else
            for (out_track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, out_track++)
            {
                int port;

                /* ...reset routed flag */
                xa_out_track_clear_flags(out_track, XA_OUT_TRACK_FLAG_ROUTED);

                port = i + mimo_proc->num_in_ports;

                XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT, &port);

                TRACE(INFO, _b("mimo_proc[%p]:port %d marked unrouted before route"), mimo_proc, port);
            }
#endif
            /* ...enter into execution state; initialize runtime */
			XA_ERRORCODE err= XA_CHK(xa_mimo_proc_prepare_runtime(mimo_proc));
			if ( mimo_proc->num_out_ports == 0 )
			{
				xa_base_schedule(base,0);
			}

			return err;
		}
		else
		{

            for (out_track = &mimo_proc->out_track[i = 0]; i < mimo_proc->num_out_ports; i++, out_track++)
            {
                /* ...output stream is over; propagate condition to sink port */
                if (xf_output_port_flush(&out_track->output, XF_FILL_THIS_BUFFER))
                {
                    /* ...flushing sequence is not needed; complete pending zero-length input */
                    xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_FLUSHING_DONE);

                    /* ...no propagation to output port */
                    TRACE(INFO, _b("mimo_proc[%p]:out_track[%u] playback completed"), mimo_proc, i);
                }
                else
                {
                    /* ...flushing sequence is started; wait until flow-control message returns */
                    xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_FLUSHING);
                    TRACE(INFO, _b("mimo_proc[%p]:out_track[%u] propagate end-of-stream condition"), mimo_proc, i);
                }

            }

            if (xa_mimo_proc_output_port_flush_done(mimo_proc))
            {
                for (in_track = &mimo_proc->in_track[i = 0]; i < mimo_proc->num_in_ports; i++, in_track++)
                {
                    /* ...input stream is over; return zero-length input back to caller */
                    xf_input_port_purge(&in_track->input);
                }

                /* ...flush probe port */
                if (mimo_proc->probe_enabled)
                {
                    if (!xf_output_port_flush(&mimo_proc->probe, XF_FLUSH))
                    {
                        BUG(1, _x("probe port: invalid state"));
                    }
                }

            }
		}

        /* ...return early to prevent task rescheduling - TENA 2553. */
        return XA_NO_ERROR;
    }

    /* ...reschedule data processing if there is a pending output message */
#ifdef MIMO_AVOID_EXCESS_SCHED
    if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
    {
        /* ...schedule execution with respect to urgency */
        if ( mimo_proc->num_out_ports == 0 )
        {
            if (input_consumed && xa_mimo_proc_input_port_reschedule_ready(mimo_proc))
            {
                xa_base_schedule(base, input_consumed * mimo_proc->factor);
            }
        }
        else
        {
            //if ((input_consumed && xa_mimo_proc_input_port_reschedule_ready(mimo_proc) ) || (output_produced && xa_mimo_proc_output_port_reschedule_ready(mimo_proc)))
            if ((input_consumed || output_produced ) && (xa_mimo_proc_input_port_reschedule_ready(mimo_proc) || xa_mimo_proc_output_port_reschedule_ready(mimo_proc)))
            {
                xa_base_schedule(base, output_produced * mimo_proc->factor);
            }
        }
    }
#else //MIMO_AVOID_EXCESS_SCHED
    if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
    {
        /* ...schedule execution with respect to urgency */
        if ( mimo_proc->num_out_ports == 0 )
        {
            if (input_consumed) xa_base_schedule(base, input_consumed * mimo_proc->factor);
        }
        else
        {
            if (input_consumed || output_produced) xa_base_schedule(base, output_produced * mimo_proc->factor);
        }
    }
#endif //MIMO_AVOID_EXCESS_SCHED

    return XA_NO_ERROR;
}

/* ...set configuration parameter */
static XA_ERRORCODE xa_mimo_proc_setparam(XACodecBase *base, WORD32 id, pVOID value)
{
    XAMimoProc     *mimo_proc = (XAMimoProc *) base;

    if (id == XAF_COMP_CONFIG_PARAM_PROBE_ENABLE)
    {
        /* ... probe enabling allowed only before postinit */
        XF_CHK_ERR((base->state & XA_BASE_FLAG_POSTINIT) == 0, XA_API_FATAL_INVALID_CMD_TYPE);

        mimo_proc->probe_enabled = *(WORD32 *) value;
        return XA_NO_ERROR;
    }
    else if (id == XAF_COMP_CONFIG_PARAM_RELAX_SCHED)
    {
        mimo_proc->relax_sched = *(UWORD32 *) value;
        return XA_NO_ERROR;
    }
    else
    {
        /* ...pass command to underlying codec plugin */
        return XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, id, value);
    }
}

/* ...port pause function */
static XA_ERRORCODE xa_mimo_proc_port_pause(XACodecBase *base, xf_message_t *m)
{
    XAMimoProc     *mimo_proc = (XAMimoProc *) base;
    UWORD32         i         = XF_MSG_DST_PORT(m->id);

    /* ...make sure the buffer is empty */
    XF_CHK_ERR(m->length == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check destination port is valid */
    XF_CHK_ERR((i <= (mimo_proc->num_in_ports + mimo_proc->num_out_ports)), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check if mimo-proc component is running */
    if (base->state & XA_BASE_FLAG_COMPLETED)
    {
        TRACE(WARNING, _b("mimo-proc[%p] completed, ignore pause command"), mimo_proc);

        /* ...complete message immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    if (i < mimo_proc->num_in_ports)
    {
        XAInTrack *in_track = &mimo_proc->in_track[i];

        if (xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_ACTIVE) && !xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_PAUSED))
        {
            /* ...mark the port as paused */
            xa_in_track_set_flags(in_track, XA_IN_TRACK_FLAG_PAUSED);

            /* ... pass port pause info to component */
            m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_PAUSE, &i);

            /* ...other tracks may be waiting for this track, so force data processing */
            if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
            {
                xa_base_schedule(base, 0);
            }

            TRACE(INFO, _b("mimo_proc[%p]::port[%u] paused (inp-port)"), mimo_proc, i);
        }
        else
        {
            TRACE(INFO, _b("mimo_proc[%p]::port[%u] not active or already paused"), mimo_proc, i);
        }
    }
    else if (i < (mimo_proc->num_in_ports + mimo_proc->num_out_ports))
    {
        UWORD32     out_port  = i - mimo_proc->num_in_ports;
        XAOutTrack *out_track = &mimo_proc->out_track[out_port];

        if (xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_ACTIVE) && !xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_PAUSED))
        {
            /* ...mark the port as paused */
            xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_PAUSED);

            /* ... pass port pause info to component */
            m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_PAUSE, &i);

            /* ...other tracks may be waiting for this track, so force data processing */
            if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
            {
                xa_base_schedule(base, 0);
            }

            TRACE(INFO, _b("mimo_proc[%p]::port[%u] paused (out-port)"), mimo_proc, i);
        }
        else
        {
            TRACE(INFO, _b("mimo_proc[%p]::port[%u] not active or already paused"), mimo_proc, i);
        }
    }
    else
    {
        if (mimo_proc->probe_enabled && !xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
        {
            /* ...flush probe port */
            if (!xf_output_port_flush(&mimo_proc->probe, XF_FLUSH))
            {
                BUG(1, _x("probe port: invalid state"));
            }

            /* ...pause probe port */
            xa_port_set_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED);

            /* ...other tracks may be waiting for this track, so force data processing */
            if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
            {
                xa_base_schedule(base, 0);
            }
        }
        else
        {
            TRACE(INFO, _b("mimo_proc[%p]::probe-port[%u] not active or already paused"), mimo_proc, i);
        }
    }

    /* ...complete message immediately */
    xf_response_ok(m);

    return XA_NO_ERROR;
}

/* ...port resume function */
static XA_ERRORCODE xa_mimo_proc_port_resume(XACodecBase *base, xf_message_t *m)
{
    XAMimoProc     *mimo_proc = (XAMimoProc *) base;
    UWORD32         i         = XF_MSG_DST_PORT(m->id);

    /* ...make sure the buffer is empty */
    XF_CHK_ERR(m->length == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check destination port is valid */
    XF_CHK_ERR((i <= (mimo_proc->num_in_ports + mimo_proc->num_out_ports)), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check if mimo-proc component is running */
    if (base->state & XA_BASE_FLAG_COMPLETED)
    {
        TRACE(WARNING, _b("mimo-proc[%p] completed, ignore resume command"), mimo_proc);

        /* ...complete message immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    if (i < mimo_proc->num_in_ports)
    {
        XAInTrack *in_track = &mimo_proc->in_track[i];

        if (xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_ACTIVE) && xa_in_track_test_flags(in_track, XA_IN_TRACK_FLAG_PAUSED))
        {
            /* ...mark the port as resumed */
            xa_in_track_clear_flags(in_track, XA_IN_TRACK_FLAG_PAUSED);

            /* ... pass port resume info to component */
            m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_RESUME, &i);

            /* ...other tracks may be waiting for this track, so force data processing */
            if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
            {
                xa_base_schedule(base, 0);
            }

            TRACE(INFO, _b("mimo_proc[%p]::port[%u] resumed (inp-port)"), mimo_proc, i);
        }
        else
        {
            TRACE(INFO, _b("mimo_proc[%p]::port[%u] not active or already resumed"), mimo_proc, i);
        }
    }
    else if (i < (mimo_proc->num_in_ports + mimo_proc->num_out_ports))
    {
        UWORD32     out_port  = i - mimo_proc->num_in_ports;
        XAOutTrack *out_track = &mimo_proc->out_track[out_port];

        if (xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_ACTIVE) && xa_out_track_test_flags(out_track, XA_OUT_TRACK_FLAG_PAUSED))
        {
            /* ...mark the port as resumed */
            xa_out_track_clear_flags(out_track, XA_OUT_TRACK_FLAG_PAUSED);

            /* ... pass port resume info to component */
            m->error = XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, XA_MIMO_PROC_CONFIG_PARAM_PORT_RESUME, &i);

            /* ...other tracks may be waiting for this track, so force data processing */
            if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc))
            {
                xa_base_schedule(base, 0);
            }

            TRACE(INFO, _b("mimo_proc[%p]::port[%u] resumed (out-port)"), mimo_proc, i);
        }
        else
        {
            TRACE(INFO, _b("mimo_proc[%p]::port[%u] not active or already resumed"), mimo_proc, i);
        }
    }
    else
    {
        if (mimo_proc->probe_enabled && xa_port_test_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED))
        {
            /* ...resume probe port */
            xa_port_clear_flags(&mimo_proc->probe.flags, XA_MIMO_PROBE_PORT_PAUSED);

            /* ...other tracks may be waiting for this track, so force data processing */
            if (xa_mimo_proc_input_port_ready(mimo_proc) && xa_mimo_proc_output_port_ready(mimo_proc) && xf_output_port_ready(&mimo_proc->probe))
            {
                xa_base_schedule(base, 0);
            }
        }
        else
        {
            TRACE(INFO, _b("mimo_proc[%p]::probe-port[%u] not active or already resumed"), mimo_proc, i);
        }
    }

    /* ...complete message immediately */
    xf_response_ok(m);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Command-processing function
 ******************************************************************************/

/* ...command hooks */
static XA_ERRORCODE (* const xa_mimo_proc_cmd[])(XACodecBase *, xf_message_t *) =
{
    /* ...set-parameter - actually, same as in generic case */
    [XF_OPCODE_TYPE(XF_SET_PARAM)] = xa_base_set_param,
    [XF_OPCODE_TYPE(XF_GET_PARAM)] = xa_base_get_param,

    /* ...extended set-get-config parameter */
    [XF_OPCODE_TYPE(XF_SET_PARAM_EXT)] = xa_base_set_param_ext,
    [XF_OPCODE_TYPE(XF_GET_PARAM_EXT)] = xa_base_get_param_ext,

    /* ...output port routing/unrouting */
    [XF_OPCODE_TYPE(XF_ROUTE)] = xa_mimo_proc_port_route,
    [XF_OPCODE_TYPE(XF_UNROUTE)] = xa_mimo_proc_port_unroute,

    /* ...input/output buffers processing */
    [XF_OPCODE_TYPE(XF_EMPTY_THIS_BUFFER)] = xa_mimo_proc_empty_this_buffer,
    [XF_OPCODE_TYPE(XF_FILL_THIS_BUFFER)] = xa_mimo_proc_fill_this_buffer,
    [XF_OPCODE_TYPE(XF_FLUSH)] = xa_mimo_proc_flush,

    [XF_OPCODE_TYPE(XF_PAUSE)] = xa_mimo_proc_port_pause,
    [XF_OPCODE_TYPE(XF_RESUME)] = xa_mimo_proc_port_resume,

};

/* ...total number of commands supported */
#define XA_MIMO_PROC_CMD_NUM        (sizeof(xa_mimo_proc_cmd) / sizeof(xa_mimo_proc_cmd[0]))

/*******************************************************************************
 * Entry points
 ******************************************************************************/

/* ...mimo_proc termination-state command processor */
static int xa_mimo_proc_terminate(xf_component_t *component, xf_message_t *m)
{
    XAMimoProc  *mimo_proc = (XAMimoProc *) component;

    if (m == NULL)
    {
        /* ...ignore component processing during component termination(rare case) */
        TRACE(OUTPUT, _b("component processing ignored.."));
        return 0;
    }

    if(XF_MSG_DST_PORT(m->id) < mimo_proc->num_in_ports)
    {
        /* ...XF_EMPTY_THIS_BUFFER is responded with generic failure */
        if (XF_MSG_SRC_PROXY(m->id))
        {
            xf_response_err(m);
        }
        else
        {
            xf_response_failure(m);
            TRACE(OUTPUT, _b("mimo_proc[%p] response_failure for inport[%d] in terminate"), mimo_proc, XF_MSG_DST_PORT(m->id));
        }
        return 0;
    }

    UWORD32      i = XF_MSG_DST_PORT(m->id) - mimo_proc->num_in_ports;
    XAOutTrack  *track = &mimo_proc->out_track[i];

    if (m == xf_output_port_control_msg(&track->output))
    {
        /* ...output port flushing complete; mark port is idle and terminate */
        xf_output_port_flush_done(&track->output);
        xa_out_track_set_flags(track, XA_OUT_TRACK_FLAG_FLUSHING_DONE);
        TRACE(OUTPUT, _b("mimo_proc[%p] flush completed for port[%d] in terminate"), mimo_proc, i);
        if (xa_mimo_proc_output_port_flush_done(mimo_proc))
#ifdef XF_MSG_ERR_HANDLING
            return XAF_UNREGISTER;
#else
            return -1;
#endif

        return 0;
    }
    else if (m->opcode == XF_FILL_THIS_BUFFER && xf_output_port_routed(&track->output))
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
            TRACE(OUTPUT, _b("mimo_proc[%p] response_failure for outport[%d] in terminate"), mimo_proc, i);
        }

        return 0;
    }
}

/* ...mimo_proc class destructor */
static int xa_mimo_proc_destroy(xf_component_t *component, xf_message_t *m)
{
    XAMimoProc *mimo_proc = (XAMimoProc *) component;
    UWORD32     core = xf_component_core(component);
    UWORD32     i;
    XACodecBase *base = &mimo_proc->base;

    /* ...get the saved command message pointer before the component memory is freed */
    xf_message_t *m_resp = mimo_proc->m_response;

    /* ...destroy all input ports */
    for (i = 0; i < XA_MIMO_PROC_MAX_IN_PORTS; i++)
    {
        xf_input_port_destroy(&mimo_proc->in_track[i].input, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_INPUT]);
    }

    /* ...destroy all output ports */
    for (i = 0; i < XA_MIMO_PROC_MAX_OUT_PORTS; i++)
    {
        xf_output_port_destroy(&mimo_proc->out_track[i].output, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
    }

    /* ...destroy probe output port */
    if (mimo_proc->probe_enabled)
    {
        xf_output_port_destroy(&mimo_proc->probe, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
    }

    /* ...destroy base object */
    xa_base_destroy(&mimo_proc->base, XF_MM(sizeof(*mimo_proc)), core);

    if (m_resp != NULL)
    {
        /* ...complete the command with response */
        xf_response_err(m_resp);
    }

    TRACE(INIT, _b("mimo_proc[%p] destroyed"), mimo_proc);

    return 0;
}

/* ...mimo_proc class first-stage destructor */
static int xa_mimo_proc_cleanup(xf_component_t *component, xf_message_t *m)
{
    XAMimoProc    *mimo_proc = (XAMimoProc *) component;
    UWORD32        i;
    XAOutTrack    *out_track;

    /* ...cancel internal scheduling message if needed */
    xa_base_cancel(&mimo_proc->base);

    /* ...purge all input ports (specify "unregister"? - don't know yet - tbd) */
    for (i = 0; i < mimo_proc->num_in_ports; i++)
    {
        xf_input_port_purge(&mimo_proc->in_track[i].input);
    }

    /* ...flush all output ports */
    for (i = 0; i < mimo_proc->num_out_ports; i++)
    {
        out_track = &mimo_proc->out_track[i];

        if (xf_output_port_flush(&out_track->output, XF_FLUSH))
        {
            /* ... mark flushing sequence is done */
            xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_FLUSHING_DONE);
        }
        else
        {
            /* ...flushing sequence is started; wait until flow-control message returns */
            xa_out_track_set_flags(out_track, XA_OUT_TRACK_FLAG_FLUSHING);
            TRACE(INFO, _b("mimo_proc[%p]:out_track[%u] cleanup - propagate end-of-stream condition"), mimo_proc, i);
        }
    }

    /* ...save command message to send response after flush completes */
    mimo_proc->m_response = m;

    if (xa_mimo_proc_output_port_flush_done(mimo_proc))
    {
        /* ...flush probe port */
        if (mimo_proc->probe_enabled)
        {
            if (!xf_output_port_flush(&mimo_proc->probe, XF_FLUSH))
            {
                BUG(1, _x("probe port: invalid state"));
            }
        }

        /* ...flushing sequence is complete; destroy mimo_proc */
        return xa_mimo_proc_destroy(component, NULL);
    }
    else
    {
        /* ...wait until output port is cleaned; adjust component hooks */
        component->entry = xa_mimo_proc_terminate;
        component->exit = xa_mimo_proc_destroy;

        TRACE(INIT, _b("mimo_proc[%p] cleanup sequence started"), mimo_proc);

        /* ...indicate that second stage is required */
        return 1;
    }
}

/* ...mimo_proc class factory */
xf_component_t * xa_mimo_proc_factory(UWORD32 core, xa_codec_func_t process, xaf_comp_type comp_type)
{
    XAMimoProc    *mimo_proc;

    /* ...construct generic audio component */
    XF_CHK_ERR(mimo_proc = (XAMimoProc *)xa_base_factory(core, XF_MM(sizeof(*mimo_proc)), process), NULL);

    /* ...set generic codec API */
    mimo_proc->base.memtab = xa_mimo_proc_memtab;
    mimo_proc->base.preprocess = xa_mimo_proc_preprocess;
    mimo_proc->base.postprocess = xa_mimo_proc_postprocess;
    mimo_proc->base.setparam = xa_mimo_proc_setparam;

    /* ...set message-processing table */
    mimo_proc->base.command = xa_mimo_proc_cmd;
    mimo_proc->base.command_num = XA_MIMO_PROC_CMD_NUM;

    /* ...set component destructor hook */
    mimo_proc->base.component.exit = xa_mimo_proc_cleanup;

    /* ...set component type */
    mimo_proc->base.comp_type = comp_type;

    /* ...initialize relax_sched */
    mimo_proc->relax_sched = 0;

    /* ...set num IO ports */
    mimo_proc->num_in_ports  = xf_io_ports[comp_type][0];
    mimo_proc->num_out_ports = xf_io_ports[comp_type][1];

    TRACE(INIT, _b("MimoProc[%p] with %d input and %d output ports is created"), mimo_proc, mimo_proc->num_in_ports, mimo_proc->num_out_ports);

    /* ...return handle to component */
    return (xf_component_t *) mimo_proc;
}

#endif  //XA_DISABLE_CLASS_MIMO_PROC
