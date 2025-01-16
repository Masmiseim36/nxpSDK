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
 * xa-class-renderer.c
 *
 * Generic audio renderer component class
 ******************************************************************************/

#ifndef XA_DISABLE_CLASS_RENDERER
 #define MODULE_TAG                      RENDERER


/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "xf-dp.h"
#include "xa-class-base.h"
#include "audio/xa-renderer-api.h"

/*******************************************************************************
 * Data structures
 ******************************************************************************/

/* ...renderer data */
typedef struct XARenderer
{
    /***************************************************************************
     * Control data
     **************************************************************************/

    /* ...generic audio codec data */
    XACodecBase         base;

    /* ...input port */
    xf_input_port_t     input;

    /* ...input port index */
    WORD32                  in_idx;

    /* ...output port data */
    xf_output_port_t        output;

    /* ...output port index */
    WORD32                  out_idx;

    /* ...buffer completion hook */
    xa_renderer_cb_t    cdata;

    /* ...output ready message */
    xf_message_t        msg;

    /***************************************************************************
     * Run-time configuration parameters
     **************************************************************************/

    /* ...sample size in bytes */
    UWORD32                     sample_size;

    /* ...time conversion factor (input byte "duration" in timebase units) */
    UWORD64                 factor;

    /* ...internal message scheduling flag (shared with interrupt) */
    UWORD32                 schedule;

    /***************************************************************************
     * response message pointer
     **************************************************************************/
    xf_message_t        *m_response;

}   XARenderer;

/*******************************************************************************
 * Renderer flags
 ******************************************************************************/

/* ...rendering is performed */
#define XA_RENDERER_FLAG_RUNNING        __XA_BASE_FLAG(1 << 0)

/* ...ouput data is ready */
#define XA_RENDERER_FLAG_OUTPUT_READY   __XA_BASE_FLAG(1 << 1)

/* ...output port setup condition */
#define XA_RENDERER_FLAG_OUTPUT_SETUP   __XA_BASE_FLAG(1 << 2)

/*******************************************************************************
 * Internal helpers
 ******************************************************************************/

/* ...prepare renderer for steady operation */
static inline XA_ERRORCODE xa_renderer_prepare_runtime(XARenderer *renderer)
{
    XACodecBase    *base = (XACodecBase *) renderer;
    xf_message_t   *m = xf_msg_queue_head(&renderer->output.queue);
    xf_start_msg_t *msg = m->buffer;
    UWORD64             factor;

    /* ...query renderer parameters */
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE, &msg->sample_rate);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_RENDERER_CONFIG_PARAM_CHANNELS, &msg->channels);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_RENDERER_CONFIG_PARAM_PCM_WIDTH, &msg->pcm_width);

    /* ...calculate sample size */
    renderer->sample_size = msg->channels * ((msg->pcm_width == 8) ? 1 :((msg->pcm_width == 16) ? 2 : 4));

    /* ...calculate output audio frame duration; get upsample factor */
    XF_CHK_ERR(factor = xf_timebase_factor(msg->sample_rate), XA_RENDERER_CONFIG_FATAL_RANGE);

    /* ...sample size should be positive */
    XF_CHK_ERR(renderer->sample_size > 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...set renderer timestamp factor (converts input bytes into timebase units) */
    renderer->factor = factor / renderer->sample_size;

    TRACE(INIT, _b("ts-factor: %llu (%llu)"), renderer->factor, factor);

    /* ...it must be a multiple */
    XF_CHK_ERR(renderer->factor * renderer->sample_size == factor, XA_RENDERER_CONFIG_FATAL_RANGE);

    XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, 0, &msg->input_length[0]);

    XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, renderer->out_idx, &msg->output_length[0]);

    /* ...allocate connect buffers */
    xf_output_port_route_alloc(&renderer->output, msg->output_length[0], base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);

    TRACE(INFO, _b("renderer[%p]::runtime init: f=%u, c=%u, w=%u i=%u o=%u"), renderer, msg->sample_rate, msg->channels, msg->pcm_width, msg->input_length[0], msg->output_length[0]);

    /* ...pass response to caller (push out of output port) */
    xf_output_port_produce(&renderer->output, sizeof(*msg));

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands handlers
 ******************************************************************************/

/* ...EMPTY-THIS-BUFFER command processing */
static XA_ERRORCODE xa_renderer_empty_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XARenderer *renderer = (XARenderer *) base;

    /* ...make sure the port is valid (what about multi-channel renderer?) */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    TRACE(INPUT, _b("received buffer [%p]:%u"), m->buffer, m->length);

    /* ...put message into input port */
    if (xf_input_port_put(&renderer->input, m))
    {
        /* ...init requires start cmd on output port */
        if ((base->state & XA_BASE_FLAG_RUNTIME_INIT) && ((base->state & XA_RENDERER_FLAG_OUTPUT_READY) == 0))
        {
            return XA_NO_ERROR;
        }

        /* ...force data processing instantly */
        xa_base_schedule(base, 0);
    }

    return XA_NO_ERROR;
}

/* ...FILL-THIS-BUFFER command processing */
static XA_ERRORCODE xa_renderer_fill_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XARenderer *renderer = (XARenderer *) base;

    if(m != &renderer->msg)
    {
        /* ...make sure the port is sane and is output port */
        XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 1, XA_API_FATAL_INVALID_CMD);

        /* ...special handling of zero-length buffer */
        if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
        {
            /* ...message must be zero-length */
            BUG(m->length != 0, _x("Invalid message length: %u"), m->length);

            /* ...place message into output port, for init response */
            xf_output_port_put(&renderer->output, m);
        }
        else if (m == xf_output_port_control_msg(&renderer->output))
        {
            /* ... mark flushing sequence is done */
            xf_output_port_flush_done(&renderer->output);

#if 1       //TENA_2379
            if (xf_output_port_unrouting(&renderer->output))
            {
                /* ...flushing during port unrouting; complete unroute sequence */
                xf_output_port_unroute_done(&renderer->output, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
                TRACE(INFO, _b("port is unrouted"));
            }
#endif
            else if (m->length == XF_MSG_LENGTH_INVALID)
            {
                /* ...complete flushing and unrouting of the outport whose dest no longer exists */
                xf_output_port_unroute(&renderer->output, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);
                TRACE(INFO, _b("renderer[%p] completed internal unroute of port"), renderer);
            }

            /* ...complete pending zero-length input buffer */
            xf_input_port_purge(&renderer->input);

            TRACE(INFO, _b("renderer[%p] playback completed"), renderer);

            /* ...playback is over */
            return XA_NO_ERROR;
        }
        /* ...indicates that the downstream component no longer exists */
        else if ((m->length == XF_MSG_LENGTH_INVALID) && xf_output_port_routed(&renderer->output))
        {
             m->length = renderer->output.length; /* ...reset length for sanity */

            if(!xf_output_port_flushing(&renderer->output))
            {
                /* ...cancel any pending processing */
                //xa_base_cancel(base); /* NOTE: renderer outport is optional, hence no base_cancel */

                /* ...output port is invalid; trigger port flush to collect all the buffers in transit */
                (void)xf_output_port_flush(&renderer->output, XF_FILL_THIS_BUFFER);

                /* ...clear output-port-setup condition */
                base->state &= ~XA_RENDERER_FLAG_OUTPUT_SETUP;

                TRACE(INFO, _b("renderer[%p] started internal unroute of port"), renderer);
            }
            TRACE(INFO, _b("renderer[%p] drop buffer"), renderer);

            return XA_NO_ERROR;
        }
        else
        {
            TRACE(INFO, _b("Received output buffer [%p]:%u"), m->buffer, m->length);

            /* ...adjust message length (may be shorter than original) */
            m->length = renderer->output.length;

            /* ...place message into output port, for init response */
            xf_output_port_put(&renderer->output, m);

            return XA_NO_ERROR;
        }
    }
    else
    {
        /* ...make sure message is our internal one */
        XF_CHK_ERR(m == &renderer->msg, XA_API_FATAL_INVALID_CMD_TYPE);

        if ((base->state & XA_RENDERER_FLAG_RUNNING) == 0)
        {
            /* ...mark renderer as running */
            base->state ^= XA_RENDERER_FLAG_RUNNING;
        }
    }

    /* ...atomically clear callback message scheduling flag */
    xf_atomic_clear(&renderer->schedule, 1);

    /* ...check if output port flag was not set */
    if ((base->state & XA_RENDERER_FLAG_OUTPUT_READY) == 0)
    {
        /* ...indicate ouput is ready */
        base->state ^= XA_RENDERER_FLAG_OUTPUT_READY;

        /* ...force data processing instantly */
        xa_base_schedule(base, 0);
    }
    else
    {
        /* ...should never come here! */
    }

    return XA_NO_ERROR;
}

/* ...FLUSH command processing */
static XA_ERRORCODE xa_renderer_flush(XACodecBase *base, xf_message_t *m)
{
    XARenderer *renderer = (XARenderer *) base;

    /* ...command is allowed only in "execution" state - not necessarily - tbd*/
    XF_CHK_ERR(base->state & XA_BASE_FLAG_EXECUTION, XA_API_FATAL_INVALID_CMD);

    /* ...ensure input parameter length is zero or XF_MSG_LENGTH_INVALID */
    XF_CHK_ERR((m->length == 0) || (m->length == XF_MSG_LENGTH_INVALID), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...flush command must be addressed to input port */
    //XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    if (XF_MSG_DST_PORT(m->id) == 0)
    {
        /* ...cancel data processing if needed */
        xa_base_cancel(base);

        /* ...input port flushing; purge content of input buffer */
        xf_input_port_purge(&renderer->input);

        /* ...pass response to caller */
        xf_response(m);
    }
    else if (xf_output_port_unrouting(&renderer->output))
    {
         /* ...flushing during port unrouting; complete unroute sequence */
        xf_output_port_unroute_done(&renderer->output, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);

        /* ...clear output-setup condition */
        base->state &= ~XA_RENDERER_FLAG_OUTPUT_SETUP;

        TRACE(INFO, _b("port is unrouted"));
    }
    else
    {
        /* ...output port flush command/response; check if the port is routed */
        if (!xf_output_port_routed(&renderer->output))
        {
            /* ...complete all queued messages */
            xf_output_port_flush(&renderer->output, XF_FLUSH);

            /* ...and pass response to flushing command */
            xf_response(m);
        }
        else
        {
            /* ...response to flushing command received */
            BUG(m != xf_output_port_control_msg(&renderer->output), _x("invalid message: %p"), m);

            /* ...mark flushing sequence is completed */
            xf_output_port_flush_done(&renderer->output);

            /* ...complete original flow-control command */
            xf_input_port_purge_done(&renderer->input);
        }

        /* ...clear output-setup condition */
        base->state &= ~XA_RENDERER_FLAG_OUTPUT_SETUP;
    }

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Completion callback - shall be a separate IRQ-safe code
 ******************************************************************************/

/* ...this code runs from interrupt handler; we need to protect data somehow */
static void xa_renderer_callback(xa_renderer_cb_t *cdata, WORD32 i_idx)
{
    XARenderer     *renderer = container_of(cdata, XARenderer, cdata);

    /* ...schedule component execution if needed */
    if (xf_atomic_test_and_set(&renderer->schedule, 1))
    {
        /* ...pass fake fill-this-buffer command */
        xf_msg_schedule_isr(&renderer->msg);
    }
}

/*******************************************************************************
 * Codec API implementation
 ******************************************************************************/

/* ...buffers handling */
static XA_ERRORCODE xa_renderer_memtab(XACodecBase *base, WORD32 idx, WORD32 type, WORD32 size, WORD32 align, UWORD32 core)
{
    XARenderer *renderer = (XARenderer *)base;

    if (type == XA_MEMTYPE_INPUT)
    {
        /* ...input buffer allocation; make sure input port index is sane */
        XF_CHK_ERR(idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...create input port for a track */
        XF_CHK_ERR(xf_input_port_init(&renderer->input, size, align, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_INPUT]) == 0, XA_API_FATAL_MEM_ALLOC);

        /* ...well, we want to use buffers without copying them into interim buffer */
        TRACE(INIT, _b("renderer input port created - size=%u"), size);

        /* ...set input port buffer if needed */
        if(size)
        {
            XA_API(base, XA_API_CMD_SET_MEM_PTR, idx, renderer->input.buffer);
        }

        renderer->in_idx = idx;
    }
    else
    {
        /* ...output buffer specification */
        XF_CHK_ERR(type == XA_MEMTYPE_OUTPUT, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...output buffer allocation; make sure output port index is sane */
        XF_CHK_ERR(idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...initialize output port queue (no allocation here yet) */
        XF_CHK_ERR(xf_output_port_init(&renderer->output, size) == 0, XA_API_FATAL_MEM_ALLOC);

        TRACE(INIT, _b("renderer output port created - size=%u"), size);

        /* ...save output port index */
        renderer->out_idx = idx;
    }

    /* ...set internal scheduling message */
    renderer->msg.id = __XF_MSG_ID(0, ((xf_component_t *)renderer)->id);
    renderer->msg.opcode = XF_FILL_THIS_BUFFER;
    renderer->msg.length = 0;
    renderer->msg.buffer = NULL;

    /* ...mark renderer output buffer as not-ready to enable scheduling on interrupt */
    base->state &= ~XA_RENDERER_FLAG_OUTPUT_READY;

    return XA_NO_ERROR;
}

/* ...preprocessing function */
static XA_ERRORCODE xa_renderer_preprocess(XACodecBase *base)
{
    XARenderer     *renderer = (XARenderer *) base;
    UWORD32        filled = 0;

    /* ...check current execution stage */
    if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
    {
        /* ...no special processing in runtime initialization stage */
        return XA_NO_ERROR;
    }

    /* ...submit input buffer to the renderer */
    if (xf_input_port_bypass(&renderer->input))
    {
        void   *input;

        /* ...port is in bypass mode; try to update the buffer pointer, remaining bytes if necessary */
        xf_input_port_fill(&renderer->input);

        /* ...in-place buffers used */
        if ((input = xf_input_port_data(&renderer->input)) != NULL)
        {
            /* ...set input buffer pointer */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, 0, input);

            /* ..retrieve number of bytes */
            filled = xf_input_port_length(&renderer->input);
        }
        else if (!xf_input_port_done(&renderer->input))
        {
            /* ...continue with partial input if output port is ready */
            if (!(base->state & XA_RENDERER_FLAG_OUTPUT_READY))
            {
                /* ...insufficient input data */
                return XA_RENDERER_EXEC_NONFATAL_INPUT;
            }
        }
        else /* ..filled=0, input_port_done=1 */
        {
            if(!(base->state & XA_RENDERER_FLAG_OUTPUT_READY) && (base->state & XA_RENDERER_FLAG_RUNNING))
            {
                /* ..return if output is not ready, even if input buffer is full */
                return XA_RENDERER_EXEC_NONFATAL_OUTPUT;
            }
        }
    }
    else
    {
        /* ...port is in non-bypass mode; try to fill internal buffer */
        if (xf_input_port_done(&renderer->input) || xf_input_port_fill(&renderer->input))
        {
            if(!(base->state & XA_RENDERER_FLAG_OUTPUT_READY) && (base->state & XA_RENDERER_FLAG_RUNNING))
            {
                /* ..return if output is not ready, even if input buffer is full */
                return XA_RENDERER_EXEC_NONFATAL_OUTPUT;
            }
        }
        else
        {
            /* ...continue with partial input if output port is ready */
            if (!(base->state & XA_RENDERER_FLAG_OUTPUT_READY))
            {
                /* ...insufficient input data */
                return XA_RENDERER_EXEC_NONFATAL_INPUT;
            }
        }

        /* ...retrieve number of bytes put in buffer */
        filled = xf_input_port_level(&renderer->input);
    }

    /* ...set total number of bytes we have in buffer */
    XA_API(base, XA_API_CMD_SET_INPUT_BYTES, 0, &filled);

    /* ...check if input stream is over */
    if (xf_input_port_done(&renderer->input))
    {
        /* ...pass input-over command to plugin */
        XA_API(base, XA_API_CMD_INPUT_OVER, 0, NULL);
    }

    /* ...prepare output buffer if needed */
    if (!(base->state & XA_RENDERER_FLAG_OUTPUT_SETUP) && (xf_output_port_routed(&renderer->output)))
    {
        void   *output;
        if ((output = xf_output_port_data(&renderer->output)) != NULL)
        {
            /* ...set the output buffer pointer */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, renderer->out_idx, output);

            TRACE(OUTPUT, _x("set output ptr: %p"), output);

            /* ...mark output port is setup */
            base->state ^= XA_RENDERER_FLAG_OUTPUT_SETUP;
        }
        else
        {
            TRACE(OUTPUT, _x("set output ptr: NULL"));

            /* ...no output buffer available, set NULL for plugin */
            XA_API(base, XA_API_CMD_SET_MEM_PTR, renderer->out_idx, NULL);
        }
    }

    return XA_NO_ERROR;
}

/* ...postprocessing function */
static XA_ERRORCODE xa_renderer_postprocess(XACodecBase *base, int done)
{
    XARenderer     *renderer = (XARenderer *) base;
    UWORD32             consumed = 0;
    UWORD32             produced = 0;
    UWORD32             i = 0;

    /* ...get total amount of consumed bytes */
    if(!xf_input_port_bypass(&renderer->input) ||
        !(done && (base->state & XA_BASE_FLAG_EXECUTION)) /* ...skip for input_port_bypass at init */
    )
    {
        XA_API(base, XA_API_CMD_GET_CURIDX_INPUT_BUF, i, &consumed);
        XA_API(base, XA_API_CMD_GET_OUTPUT_BYTES, renderer->out_idx, &produced);
    }

    /* ...get number of produced bytes only if runtime is initialized (sample size is known) */
    if (renderer->sample_size && (base->state & XA_RENDERER_FLAG_OUTPUT_SETUP))
    {
        /* ...output buffer maintenance; check if we have produced anything */
        if (produced)
        {
            /* ...immediately complete output buffer (don't wait until it gets filled) */
            xf_output_port_produce(&renderer->output, produced);

            /* ...clear output port setup flag */
            base->state ^= XA_RENDERER_FLAG_OUTPUT_SETUP;
        }
    }

    TRACE(INPUT, _b("renderer[%p]::postprocess(c=%u, p=%u, d=%u)"), renderer, consumed, produced, done);

    /* ...input buffer maintenance; consume that amount from input port */
    if (consumed)
    {
        /* ...consume bytes from input buffer */
        xf_input_port_consume(&renderer->input, consumed);
    }

    /* ...reset output-ready state */
    if(produced)
    {
        base->state &= ~XA_RENDERER_FLAG_OUTPUT_READY;
    }

    if (done)
    {
        if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
        {
            /* ...processing is done while in runtime initialization state (can't be - tbd) */
            BUG(1, _x("breakpoint"));
        }
        else if (base->state & XA_BASE_FLAG_EXECUTION)
        {
            /* ...runtime initialization is done */
            XA_CHK(xa_renderer_prepare_runtime(renderer));

            /* ...clear output port ready flag to enable scheduling on interrupt */
            base->state &= ~XA_RENDERER_FLAG_OUTPUT_READY;

            /* ...force data processing instantly to process input, if any */
            xa_base_schedule(base, 0);
        }
        else
        {
            /* ...output stream is over; propagate condition to sink port */
            if (xf_output_port_flush(&renderer->output, XF_FILL_THIS_BUFFER))
            {
                /* ...input stream is over; complete pending zero-length message */
                xf_input_port_purge(&renderer->input);

                /* ...clear renderer running flag */
                base->state &= ~XA_RENDERER_FLAG_RUNNING;

                /* ...no propagation to output port */
                TRACE(INFO, _b("renderer[%p] playback completed"), renderer);
            }
            else
            {
                /* ...flushing sequence is started; wait until flow-control message returns */
                TRACE(INFO, _b("propagate end-of-stream condition"));
            }
        }

        /* ...return early to prevent task rescheduling */
        return XA_NO_ERROR;
    }

    /* ...reschedule execution if we have pending input */
    if (xf_input_port_ready(&renderer->input))
    {
        if (consumed)
        {
            /* ...schedule execution with respect to urgency  */
            xa_base_schedule(base, consumed * renderer->factor);
        }
    }

    return XA_NO_ERROR;
}

/* ...output port routing */
static XA_ERRORCODE xa_renderer_port_route(XACodecBase *base, xf_message_t *m)
{
    XARenderer     *renderer = (XARenderer *) base;
    xf_route_port_msg_t    *cmd = m->buffer;
    xf_output_port_t       *port = &renderer->output;
    UWORD32                     src = XF_MSG_DST(m->id);
    UWORD32                     dst = cmd->dst;

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure output port is addressed */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure port is not routed yet */
    XF_CHK_ERR(!xf_output_port_routed(port), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...route output port - allocate queue */
    XF_CHK_ERR(xf_output_port_route(port, __XF_MSG_ID(dst, src), cmd->alloc_number, cmd->alloc_size, cmd->alloc_align, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]) == 0, XA_API_FATAL_MEM_ALLOC);
#if 0
    /* ...schedule processing instantly */
    xa_base_schedule(base, 0);
#endif
    /* ...pass success result to caller */
    xf_response_ok(m);

    return XA_NO_ERROR;
}

/* ...port unroute command */
static XA_ERRORCODE xa_renderer_port_unroute(XACodecBase *base, xf_message_t *m)
{
    XARenderer     *renderer = (XARenderer *) base;

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure output port is addressed */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 1, XA_API_FATAL_INVALID_CMD_TYPE);

#if 0
    /* ...cancel any pending processing */
    xa_base_cancel(base);
#endif

    if(!xf_output_port_routed(&renderer->output))
    {
        /* ...if XF_MSG_LENGTH_INVALID triggered internal unroute is completed, send response instantly */
        xf_response_ok(m);
        return XA_NO_ERROR;
    }

    /* ...clear output-port-setup condition */
    base->state &= ~XA_RENDERER_FLAG_OUTPUT_SETUP;

    /* ...optional output port is being unrouted, set NULL for plugin */
    XA_API(base, XA_API_CMD_SET_MEM_PTR, renderer->out_idx, NULL);

    /* ...pass flush command down the graph */
    if (xf_output_port_flush(&renderer->output, XF_FLUSH))
    {
        TRACE(INFO, _b("port is idle; instantly unroute"));

        /* ...flushing sequence is not needed; command may be satisfied instantly */
        xf_output_port_unroute(&renderer->output, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_OUTPUT]);

        /* ...pass response to the proxy */
        xf_response_ok(m);
    }
    else
    {
        TRACE(INFO, _b("port is busy; propagate unroute command"));

        /* ...flushing sequence is started; save flow-control message */
        xf_output_port_unroute_start(&renderer->output, m);
    }

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Command-processing function
 ******************************************************************************/

/* ...command hooks */
static XA_ERRORCODE (* const xa_renderer_cmd[])(XACodecBase *, xf_message_t *) =
{
    /* ...set-parameter - actually, same as in generic case */
    [XF_OPCODE_TYPE(XF_SET_PARAM)] = xa_base_set_param,
    [XF_OPCODE_TYPE(XF_GET_PARAM)] = xa_base_get_param,

    /* ...extended set-get-config parameter */
    [XF_OPCODE_TYPE(XF_SET_PARAM_EXT)] = xa_base_set_param_ext,
    [XF_OPCODE_TYPE(XF_GET_PARAM_EXT)] = xa_base_get_param_ext,

    /* ...input buffers processing */
    [XF_OPCODE_TYPE(XF_EMPTY_THIS_BUFFER)] = xa_renderer_empty_this_buffer,
    [XF_OPCODE_TYPE(XF_FILL_THIS_BUFFER)] = xa_renderer_fill_this_buffer,
    [XF_OPCODE_TYPE(XF_FLUSH)]  = xa_renderer_flush,

    [XF_OPCODE_TYPE(XF_ROUTE)]  = xa_renderer_port_route,
    [XF_OPCODE_TYPE(XF_UNROUTE)]  = xa_renderer_port_unroute,
};

/* ...total number of commands supported */
#define XA_RENDERER_CMD_NUM         (sizeof(xa_renderer_cmd) / sizeof(xa_renderer_cmd[0]))

/*******************************************************************************
 * Entry points
 ******************************************************************************/

/* ...renderer termination-state command processor */
static int xa_renderer_terminate(xf_component_t *component, xf_message_t *m)
{
    XARenderer *renderer = (XARenderer *) component;

    if (m == NULL)
    {
        /* ...ignore component processing during component termination(rare case) */
        TRACE(INFO, _b("component processing ignored.."));
        return 0;
    }

    /* ...check if we received internal message */
    if (m == &renderer->msg)
    {
        /* ...callback execution completed; complete operation */
#ifdef XF_MSG_ERR_HANDLING
        return XAF_UNREGISTER;
#else
        return -1;
#endif
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
            TRACE(OUTPUT, _b("renderer[%p] response_failure in terminate"), renderer);
        }
        return 0;
    }
}

/* ...renderer class destructor */
static int xa_renderer_destroy(xf_component_t *component, xf_message_t *m)
{
    XARenderer *renderer = (XARenderer *) component;
    UWORD32         core = xf_component_core(component);
    XACodecBase *base = &renderer->base;

    /* ...get the saved command message pointer before the component memory is freed */
    xf_message_t *m_resp = renderer->m_response;

    /* ...destroy input port */
    xf_input_port_destroy(&renderer->input, core, base->component.mem_pool_type[XAF_MEM_POOL_TYPE_COMP_INPUT]);

    /* ...destroy base object */
    xa_base_destroy(&renderer->base, XF_MM(sizeof(*renderer)), core);

    if (m_resp != NULL)
    {
        /* ...complete the command with response */
        xf_response_err(m_resp);
    }

    TRACE(INIT, _b("renderer[%p] destroyed"), renderer);

    /* ...indicate the component is destroyed */
    return 0;
}

/* ...renderer class first-stage destructor */
static int xa_renderer_cleanup(xf_component_t *component, xf_message_t *m)
{
    XARenderer     *renderer = (XARenderer *) component;
    XACodecBase    *base = (XACodecBase *) renderer;
    UWORD32             state = XA_RENDERER_STATE_IDLE;

    /* ...cancel component task execution if needed */
    xa_base_cancel(base);

    /* ...stop hardware renderer if it's running */
    XA_API_NORET(base, XA_API_CMD_SET_CONFIG_PARAM, XA_RENDERER_CONFIG_PARAM_STATE, &state);

    /* ...purge input port */
    xf_input_port_purge(&renderer->input);

    /* ...save command message to send response after flush completes */
    renderer->m_response = m;

    /* ...check if we have internal message scheduled */
    if (xf_atomic_test_and_clear(&renderer->schedule, 1))
    {
        /* ...wait until callback message is returned */
        component->entry = xa_renderer_terminate;
        component->exit = xa_renderer_destroy;

        TRACE(INIT, _b("renderer[%p] cleanup sequence started"), renderer);

        /* ...indicate that second stage is required */
        return 1;
    }
    else
    {
        /* ...callback is not scheduled; destroy renderer */
        return xa_renderer_destroy(component, NULL);
    }
}

/* ...renderer class factory */
xf_component_t * xa_renderer_factory(UWORD32 core, xa_codec_func_t process,xaf_comp_type comp_type)
{
    XARenderer *renderer;

    /* ...construct generic audio component */
    XF_CHK_ERR(renderer = (XARenderer *)xa_base_factory(core, XF_MM(sizeof(*renderer)), process), NULL);

    /* ...set generic codec API */
    renderer->base.memtab = xa_renderer_memtab;
    renderer->base.preprocess = xa_renderer_preprocess;
    renderer->base.postprocess = xa_renderer_postprocess;

    /* ...set message-processing table */
    renderer->base.command = xa_renderer_cmd;
    renderer->base.command_num = XA_RENDERER_CMD_NUM;

    /* ...set component destructor hook */
    renderer->base.component.exit = xa_renderer_cleanup;

    /* ...set notification callback data */
    renderer->cdata.cb = xa_renderer_callback;
    renderer->base.comp_type = comp_type;
    /* ...pass buffer completion callback to the component */
    XA_API_NORET(&renderer->base, XA_API_CMD_SET_CONFIG_PARAM, XA_RENDERER_CONFIG_PARAM_CB, &renderer->cdata);

    TRACE(INIT, _b("Renderer[%p] created"), renderer);

    /* ...return handle to component */
    return (xf_component_t *) renderer;
}

#endif  //XA_DISABLE_CLASS_RENDERER
