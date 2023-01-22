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
 * xa-class-audio-codec.c
 *
 * Generic audio codec task implementation
 ******************************************************************************/

#define MODULE_TAG                      CODEC

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "xf-dp.h"
#include "xa-class-base.h"
#include "audio/xa-audio-decoder-api.h"

#define INIT_CHG

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

typedef struct XAAudioCodec
{
    /***************************************************************************
     * Control data
     **************************************************************************/

    /* ...generic audio codec data */
    XACodecBase             base;

    /* ...input port data */
    xf_input_port_t         input;

    /* ...output port data */
    xf_output_port_t        output;

    /* ...input port index */
    WORD32                  in_idx;

    /* ...output port index */
    WORD32                  out_idx;

    /***************************************************************************
     * Run-time configuration parameters
     **************************************************************************/

    /* ...sample size in bytes */
    UWORD32                     sample_size;

    /* ...audio sample duration */
    UWORD64                     factor;

    /* ...total number of produced audio frames since last reset */
    UWORD32                     produced;

    UWORD32                     consumed;

    UWORD32					probe_enabled;

    UWORD32					dec_init_without_inp;

    /* ...probe output port data */
    xf_output_port_t 		probe;

    /* ...probe output buffer pointer */
    void 					*probe_output;

    /* ...codec output buffer pointer */
    void 					*out_ptr;
    
    /* ...temporary output pointer for audio class component initialization */
    void                   *pinit_output;

    /* ...temporary output buffer size */
    UWORD32                 init_output_size;

    /***************************************************************************
     * response message pointer 
     **************************************************************************/
    xf_message_t        *m_response;

}   XAAudioCodec;

/*******************************************************************************
 * Auxiliary codec execution flags
 ******************************************************************************/

/* ...input port setup condition */
#define XA_CODEC_FLAG_INPUT_SETUP       __XA_BASE_FLAG(1 << 0)

/* ...output port setup condition */
#define XA_CODEC_FLAG_OUTPUT_SETUP      __XA_BASE_FLAG(1 << 1)

/* ...probe port setup condition */
#define XA_CODEC_FLAG_PROBE_SETUP		__XA_BASE_FLAG(1 << 2)

/* ...end-of-stream sequence complete condition */
#define XA_CODEC_FLAG_EOS_SEQ_DONE		__XA_BASE_FLAG(1 << 3)

/*******************************************************************************
 * Auxiliary port operation flags
 ******************************************************************************/

/* ...input port is paused */
#define XA_CODEC_INP_PORT_PAUSED        __XF_INPUT_FLAG(1 << 0)

/* ...output port is paused */
#define XA_CODEC_OUT_PORT_PAUSED        __XF_OUTPUT_FLAG(1 << 0)

/* ...probe port is paused */
#define XA_CODEC_PROBE_PORT_PAUSED      __XF_OUTPUT_FLAG(1 << 1)

/*******************************************************************************
 * Data processing scheduling
 ******************************************************************************/

/* ...prepare codec for steady operation (tbd - don't absolutely like it) */
static inline XA_ERRORCODE xa_codec_prepare_runtime(XAAudioCodec *codec)
{
    XACodecBase    *base = (XACodecBase *)codec;
    xf_message_t   *m = xf_msg_queue_head(&codec->output.queue);
    xf_start_msg_t *msg = m->buffer;
    UWORD32             frame_size, pcm_buffer_length;
    UWORD64             factor;
    UWORD32         core = XF_MSG_DST_CORE(m->id);

    /* ...fill-in buffer parameters */
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_CODEC_CONFIG_PARAM_SAMPLE_RATE, &msg->sample_rate);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_CODEC_CONFIG_PARAM_CHANNELS, &msg->channels);
    XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, XA_CODEC_CONFIG_PARAM_PCM_WIDTH, &msg->pcm_width);
    XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, codec->in_idx, &msg->input_length[0]);
    XA_API(base, XA_API_CMD_GET_MEM_INFO_SIZE, codec->out_idx, &msg->output_length[0]);

    /* ...populate port buffer length */
    msg->probe_length = (codec->probe_enabled) ? codec->probe.length : 0;

    TRACE(INIT, _b("codec[%p]::runtime init: f=%u, c=%u, w=%u, i=%u, o=%u"), codec, msg->sample_rate, msg->channels, msg->pcm_width, msg->input_length[0], msg->output_length[0]);

    /* ...reallocate input port buffer as needed - tbd */
    BUG(msg->input_length[0] > codec->input.length, _x("Input buffer reallocation required: %u to %u"), codec->input.length, msg->input_length[0]);

    /* ...save sample size in bytes */
    codec->sample_size = msg->channels * ((msg->pcm_width == 8) ? 1 :((msg->pcm_width == 16) ? 2 : 4));

    /* ...sample size should be positive */
    XF_CHK_ERR(codec->sample_size > 0, XA_API_FATAL_INVALID_CMD_TYPE);

     /* ...encoder should refer pcm length on input side, TENA-3025. */
    pcm_buffer_length = (base->comp_type == XAF_ENCODER) ? msg->input_length[0]:msg->output_length[0];

    /* ...calculate frame duration; get number of samples in the frame (don't like division here - tbd) */
    frame_size = pcm_buffer_length / codec->sample_size;

    /* ...it must be a multiple */
    XF_CHK_ERR(frame_size * codec->sample_size == pcm_buffer_length, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...retrieve upsampling factor for given sample rate */
    XF_CHK_ERR(factor = xf_timebase_factor(msg->sample_rate), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...set frame duration factor (converts number of bytes into timebase units) */
    codec->factor = factor / codec->sample_size;

    TRACE(INIT, _b("ts-factor: %llu (%llu)"), codec->factor, factor);

    /* ...factor must be a multiple */
    XF_CHK_ERR(codec->factor * codec->sample_size == factor, XA_CODEC_CONFIG_FATAL_RANGE);

    /* ...codec runtime initialization is completed */
    TRACE(INIT, _b("codec[%p] runtime initialized: i=%u, o=%u"), codec, msg->input_length[0], msg->output_length[0]);
    
    /* ...Free temporary output buffer */
    if ( codec->pinit_output != NULL)
    {
        xf_mem_free(codec->pinit_output, codec->init_output_size, core, 0 );
        codec->pinit_output  = NULL;
    }

    /* ...pass response to caller (push out of output port) */
    xf_output_port_produce(&codec->output, sizeof(*msg));

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

/* ...EMPTY-THIS-BUFFER command processing */
static XA_ERRORCODE xa_codec_empty_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;

    /* ...make sure the port is sane */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 0, XA_API_FATAL_INVALID_CMD);

    /* ...command is allowed only in post-init state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...put message into input queue */
    if (xf_input_port_put(&codec->input, m))
    {
        /* ...restart stream if it is in completed state */
        if (base->state & XA_BASE_FLAG_COMPLETED)
        {
            /* ...reset execution stage */
            base->state = XA_BASE_FLAG_POSTINIT | XA_BASE_FLAG_EXECUTION;

            /* ...reset execution runtime */
            XA_API(base, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_RUNTIME_INIT, NULL);

            /* ...reset produced samples counter */
            codec->produced = 0;
        }

        /* ...codec must be in one of these states */
        XF_CHK_ERR(base->state & (XA_BASE_FLAG_RUNTIME_INIT | XA_BASE_FLAG_EXECUTION), XA_API_FATAL_INVALID_CMD);
        
        /* ...schedule data processing if output is ready */
        if (xf_output_port_ready(&codec->output))
        {
            xa_base_schedule(base, 0);
        }
    }

    TRACE(INPUT, _b("Received buffer [%p]:%u"), m->buffer, m->length);

    return XA_NO_ERROR;
}

/* ...FILL-THIS-BUFFER command processing */
static XA_ERRORCODE xa_codec_fill_this_buffer(XACodecBase *base, xf_message_t *m)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;
    xaf_comp_type  comp_type = base->comp_type;

    /* ...command is allowed only in postinit state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...handle probe setup if probe is enabled */
    if (codec->probe_enabled && XF_MSG_DST_PORT(m->id) == 2)
    {
        /* ...command is allowed only in execution state */
        //XF_CHK_ERR(base->state & XA_BASE_FLAG_EXECUTION, XA_API_FATAL_INVALID_CMD);

        if (base->state & XA_BASE_FLAG_COMPLETED || xa_port_test_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED))
        {
            /* ...return message arrived from application immediately */
            xf_response_ok(m);

            return XA_NO_ERROR;
        }
        else
        {
        	TRACE(PROBE, _b("Received probe output buffer [%p]:%u"), m->buffer, m->length);

            /* ...adjust message length (may be shorter than original) */
            m->length = codec->probe.length;
        }

        if (xf_output_port_put(&codec->probe, m) && xf_output_port_ready(&codec->output) && xf_input_port_ready(&codec->input))
        {
            /* ...schedule data processing instantly */
            if (base->state & (XA_BASE_FLAG_EXECUTION))
            {
                xa_base_schedule(base, 0);
            }
        }

    	return XA_NO_ERROR;
    }

    /* ...make sure the port is sane */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 1, XA_API_FATAL_INVALID_CMD);

    /* ...special handling of zero-length buffer */
    if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
    {
        /* ...message must be zero-length */
        BUG(m->length != 0, _x("Invalid message length: %u"), m->length);
    }
    else if (m == xf_output_port_control_msg(&codec->output))
    {
        /* ... mark flushing sequence is done */
        xf_output_port_flush_done(&codec->output);

#if 1   //TENA_2379                                                                                                     
        if (xf_output_port_unrouting(&codec->output))
        {   
            /* ...flushing during port unrouting; complete unroute sequence */                                            
            xf_output_port_unroute_done(&codec->output);                                                                  
            TRACE(INFO, _b("port is unrouted"));
        }   
#endif
        else if (m->length == XF_MSG_LENGTH_INVALID)
        {
            /* ...complete flushing and unrouting of the outport whose dest no longer exists */
            xf_output_port_unroute(&codec->output);
            TRACE(INFO, _b("codec[%p] completed internal unroute of port"), codec);
        }

        /* ...complete pending zero-length input buffer */
        xf_input_port_purge(&codec->input);

        TRACE(INFO, _b("codec[%p] playback completed"), codec);

        /* ...playback is over */
        return XA_NO_ERROR;
    }
    else if ((base->state & XA_BASE_FLAG_COMPLETED) && !(base->state & XA_CODEC_FLAG_EOS_SEQ_DONE) && !xf_output_port_routed(&codec->output))
    {
        /* ...mark EOS sequence done */
        base->state |= XA_CODEC_FLAG_EOS_SEQ_DONE;

        /* ...return message arrived from application immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }
    /* ...indicates that the downstream component no longer exists */
    else if ((m->length == XF_MSG_LENGTH_INVALID) && xf_output_port_routed(&codec->output))
    {
         m->length = codec->output.length; /* ...reset length for sanity */

        if(!xf_output_port_flushing(&codec->output))
        {
            /* ...cancel any pending processing */
            xa_base_cancel(base);

            /* ...output port is invalid; trigger port flush to collect all the buffers in transit */
            (void)xf_output_port_flush(&codec->output, XF_FILL_THIS_BUFFER);

            /* ...clear output-port-setup condition */
            base->state &= ~XA_CODEC_FLAG_OUTPUT_SETUP;

            TRACE(INFO, _b("codec[%p] started internal unroute of port"), codec);
         }
         TRACE(INFO, _b("codec[%p] drop buffer"), codec);

         return XA_NO_ERROR;
    }
    else
    {
        if (base->state & XA_CODEC_FLAG_EOS_SEQ_DONE)
        {
            /* ...reset execution stage */
            base->state = XA_BASE_FLAG_POSTINIT | XA_BASE_FLAG_EXECUTION;
            
            /* ...reset execution runtime */
            XA_API(base, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_RUNTIME_INIT, NULL);

            /* ...reset produced samples counter */
            codec->produced = 0;
        
            TRACE(INFO, _b("codec[%p] COMPLETED to EXECUTION"), codec);
        }

        TRACE(OUTPUT, _b("Received output buffer [%p]:%u"), m->buffer, m->length);

        /* ...adjust message length (may be shorter than original) */
        m->length = codec->output.length;
    }

    /* ...place message into output port */
#if 1 //TENA_2198
#ifdef INIT_CHG
    /* ... allow processing when output bufer is received and any of the following conditions are met:
     * 1. input buffer is available
     * 2. init is pending and component is non-decoder
     * 3. init is pending and component is decoder and initialize_with_input is not required by application. */
#if 0
    if (xf_output_port_put(&codec->output, m) && (xf_input_port_ready(&codec->input) || (((base->state & XA_BASE_FLAG_RUNTIME_INIT) && (comp_type != XAF_DECODER)) || ((base->state & XA_BASE_FLAG_RUNTIME_INIT) && (comp_type == XAF_DECODER) && (codec->dec_init_without_inp == 1)))))
#else
if (xf_output_port_put(&codec->output, m) && 
    ( xf_input_port_ready(&codec->input) || 
     ((base->state & XA_BASE_FLAG_RUNTIME_INIT) && 
        ((comp_type != XAF_DECODER) || (codec->dec_init_without_inp == 1))
     )
    )
   )
#endif
#else
    if (xf_output_port_put(&codec->output, m) && (xf_input_port_ready(&codec->input) || ((base->state & XA_BASE_FLAG_RUNTIME_INIT) && comp_type == XAF_ENCODER)))
#endif
#else
    if (xf_output_port_put(&codec->output, m) && xf_input_port_ready(&codec->input))
#endif
    {
    	/* ...check probe port, if probe is enabled */
    	if (codec->probe_enabled && !(base->state & XA_BASE_FLAG_RUNTIME_INIT))
    	{
    		if (!xf_output_port_ready(&codec->probe) && !xa_port_test_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED))
    			return XA_NO_ERROR;
    	}

        /* ...schedule data processing instantly */
        if ((base->state & (XA_BASE_FLAG_RUNTIME_INIT | XA_BASE_FLAG_EXECUTION)) && (xf_output_port_ready(&codec->output)))
        {
            xa_base_schedule(base, 0);
        }
    }

    return XA_NO_ERROR;
}

/* ...output port routing */
static XA_ERRORCODE xa_codec_port_route(XACodecBase *base, xf_message_t *m)
{
    XAAudioCodec           *codec = (XAAudioCodec *) base;
    xf_route_port_msg_t    *cmd = m->buffer;
    xf_output_port_t       *port = &codec->output;
    UWORD32                     src = XF_MSG_DST(m->id);
    UWORD32                     dst = cmd->dst;
    
    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure output port is addressed */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure port is not routed yet */
    XF_CHK_ERR(!xf_output_port_routed(port), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...route output port - allocate queue */
    XF_CHK_ERR(xf_output_port_route(port, __XF_MSG_ID(dst, src), cmd->alloc_number, cmd->alloc_size, cmd->alloc_align) == 0, XA_API_FATAL_MEM_ALLOC);

    /* ...schedule processing instantly */
    xa_base_schedule(base, 0);
    
    /* ...pass success result to caller */
    xf_response_ok(m);
    
    return XA_NO_ERROR;
}

/* ...port unroute command */
static XA_ERRORCODE xa_codec_port_unroute(XACodecBase *base, xf_message_t *m)
{
    XAAudioCodec           *codec = (XAAudioCodec *) base;
    
    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...make sure output port is addressed */
    XF_CHK_ERR(XF_MSG_DST_PORT(m->id) == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    if(!xf_output_port_routed(&codec->output))
    {
        /* ...if XF_MSG_LENGTH_INVALID triggered internal unroute is completed, send response instantly */
        xf_response_ok(m);
        return XA_NO_ERROR;
    }

    /* ...cancel any pending processing */
    xa_base_cancel(base);

    /* ...clear output-port-setup condition */
    base->state &= ~XA_CODEC_FLAG_OUTPUT_SETUP;

    /* ...pass flush command down the graph */
    if (xf_output_port_flush(&codec->output, XF_FLUSH))
    {
        TRACE(INFO, _b("port is idle; instantly unroute"));

        /* ...flushing sequence is not needed; command may be satisfied instantly */
        xf_output_port_unroute(&codec->output);

        /* ...pass response to the proxy */
        xf_response_ok(m);
    }
    else
    {
        TRACE(INFO, _b("port is busy; propagate unroute command"));

        /* ...flushing sequence is started; save flow-control message */
        xf_output_port_unroute_start(&codec->output, m);
    }

    return XA_NO_ERROR;
}

/* ...FLUSH command processing */
static XA_ERRORCODE xa_codec_flush(XACodecBase *base, xf_message_t *m)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;

    /* ...command is allowed only in "postinit" state */
    XF_CHK_ERR(base->state & XA_BASE_FLAG_POSTINIT, XA_API_FATAL_INVALID_CMD);

    /* ...ensure input parameter length is zero or XF_MSG_LENGTH_INVALID */
    XF_CHK_ERR((m->length == 0) || (m->length == XF_MSG_LENGTH_INVALID), XA_API_FATAL_INVALID_CMD_TYPE);

    TRACE(INFO, _b("flush command received"));
    
    /* ...flush command must be addressed to input port */
    if (XF_MSG_DST_PORT(m->id) == 0)
    {
        int outbuf_available_flag;

        /* ...cancel data processing message if needed */
        xa_base_cancel(base);

        /* ...input port flushing; purge content of input buffer */
        xf_input_port_purge(&codec->input);

        /* ...clear input-ready condition */
        base->state &= ~XA_CODEC_FLAG_INPUT_SETUP;

        /* ...reset execution runtime */
        XA_API(base, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_RUNTIME_INIT, NULL);

        /* ...reset produced samples counter */
        codec->produced = 0;

        outbuf_available_flag = xf_output_port_ready(&codec->output);
        /* ...propagate flushing command to output port */
        if (xf_output_port_flush(&codec->output, XF_FLUSH))
        {
            /* ...set the component state to completed for unrouted port */
            if (!xf_output_port_routed(&codec->output))
            {
                base->state ^= XA_BASE_FLAG_EXECUTION | XA_BASE_FLAG_COMPLETED;

                if(outbuf_available_flag)
                {
                    /* ...mark EOS sequence done, after flushing with 0 length output */
                    base->state |= XA_CODEC_FLAG_EOS_SEQ_DONE;
                }
            }
            /* ...flushing sequence is not needed; satisfy command instantly */
            xf_response(m);
        }
        else
        {
            /* ...flushing sequence is started; save flow-control message at input port */
            xf_input_port_control_save(&codec->input, m);
        }
    }
    else if (xf_output_port_unrouting(&codec->output))
    {
        /* ...flushing during port unrouting; complete unroute sequence */
        xf_output_port_unroute_done(&codec->output);

        TRACE(INFO, _b("port is unrouted"));
    }
    else
    {
        /* ...output port flush command/response; check if the port is routed */
        if (!xf_output_port_routed(&codec->output))
        {
            /* ...complete all queued messages */
            xf_output_port_flush(&codec->output, XF_FLUSH);

            /* ...and pass response to flushing command */
            xf_response(m);
        }
        else
        {            
            /* ...response to flushing command received */
            BUG(m != xf_output_port_control_msg(&codec->output), _x("invalid message: %p"), m);

            /* ...mark flushing sequence is completed */
            xf_output_port_flush_done(&codec->output);

            /* ...complete original flow-control command */
            xf_input_port_purge_done(&codec->input);
        }

        /* ...clear output-setup condition */
        base->state &= ~XA_CODEC_FLAG_OUTPUT_SETUP;
    }

    return XA_NO_ERROR;
}

/* ...port pause function */
static XA_ERRORCODE xa_codec_port_pause(XACodecBase *base, xf_message_t *m)
{
    XAAudioCodec *codec = (XAAudioCodec *) base;
    UWORD32           i = XF_MSG_DST_PORT(m->id);
    
    /* ...make sure the buffer is empty */
    XF_CHK_ERR(m->length == 0, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...check destination port is valid */
    XF_CHK_ERR(i <= 2, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ... if pause has come for probe port(probe-stop), handle it first */
    if (codec->probe_enabled && (i == 2) && !xa_port_test_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED))
    {
        /* ...flush probe port */
        if (!xf_output_port_flush(&codec->probe, XF_FLUSH))
        {
            BUG(1, _x("probe port: invalid state"));
        }

        /* ...pause probe port */
        xa_port_set_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED);

        /* ...other ports may be waiting, so force data processing */
        if (xf_input_port_ready(&codec->input) && xf_output_port_ready(&codec->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("codec[%p]-probe port[%u] paused"), codec, i);

        /* ...complete message immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    /* ...check if codec is running */
    if (base->state & XA_BASE_FLAG_COMPLETED)
    {
        TRACE(WARNING, _b("codec[%p] completed, ignore pause command"), codec);
    
        /* ...complete message immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    /* ...mark the port as paused */
    if (i == 0 && !xa_port_test_flags(&codec->input.flags,  XA_CODEC_INP_PORT_PAUSED))
    {
        xa_port_set_flags(&codec->input.flags,  XA_CODEC_INP_PORT_PAUSED);

        TRACE(INFO, _b("codec[%p]-port[%u] paused"), codec, i);
    }
    else if (i == 1 && !xa_port_test_flags(&codec->output.flags, XA_CODEC_OUT_PORT_PAUSED))
    {
        xa_port_set_flags(&codec->output.flags, XA_CODEC_OUT_PORT_PAUSED);

        TRACE(INFO, _b("codec[%p]-port[%u] paused"), codec, i);
    }
    else
    {
        TRACE(INFO, _b("codec[%p]-port[%u] is not active or already paused"), codec, i);
    }
    
    /* ...complete message immediately */
    xf_response_ok(m);
    
    return XA_NO_ERROR;
}

/* ...port resume function */
static XA_ERRORCODE xa_codec_port_resume(XACodecBase *base, xf_message_t *m)
{
    XAAudioCodec *codec = (XAAudioCodec *) base;
    UWORD32           i = XF_MSG_DST_PORT(m->id);
    
    /* ...make sure the buffer is empty */
    XF_CHK_ERR(m->length == 0, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...check destination port is valid */
    XF_CHK_ERR(i <= 2, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check if codec is running */
    if (base->state & XA_BASE_FLAG_COMPLETED)
    {
        TRACE(WARNING, _b("codec[%p] completed, ignore resume command"), codec);
    
        /* ...complete message immediately */
        xf_response_ok(m);

        return XA_NO_ERROR;
    }

    /* ...mark the port as resumed */
    if (i == 0 && xa_port_test_flags(&codec->input.flags,  XA_CODEC_INP_PORT_PAUSED))
    {
        xa_port_clear_flags(&codec->input.flags,  XA_CODEC_INP_PORT_PAUSED);

        /* ...other ports may be waiting, so force data processing */
        if (xf_input_port_ready(&codec->input) && xf_output_port_ready(&codec->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("codec[%p]-port[%u] resumed"), codec, i);
    }
    else if (i == 1 && xa_port_test_flags(&codec->output.flags, XA_CODEC_OUT_PORT_PAUSED))
    {
        xa_port_clear_flags(&codec->output.flags, XA_CODEC_OUT_PORT_PAUSED);

        /* ...other ports may be waiting, so force data processing */
        if (xf_input_port_ready(&codec->input) && xf_output_port_ready(&codec->output))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("codec[%p]-port[%u] resumed"), codec, i);
    }
    else if (i == 2 && codec->probe_enabled && xa_port_test_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED))
    {
        xa_port_clear_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED);

        /* ...other ports may be waiting, so force data processing */
        if (xf_input_port_ready(&codec->input) && xf_output_port_ready(&codec->output) && xf_output_port_ready(&codec->probe))
        {
            xa_base_schedule(base, 0);
        }

        TRACE(INFO, _b("codec[%p]-probe port[%u] resumed"), codec, i);
    }
    else
    {
        TRACE(INFO, _b("codec[%p]-port[%u] is not active or already resumed"), codec, i);
    }

    /* ...complete message immediately */
    xf_response_ok(m);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Generic codec API
 ******************************************************************************/

/* ...memory buffer handling */
static XA_ERRORCODE xa_codec_memtab(XACodecBase *base, WORD32 idx, WORD32 type, WORD32 size, WORD32 align, UWORD32 core)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;

    if (type == XA_MEMTYPE_INPUT)
    {
        /* ...input port specification; allocate internal buffer */
        XF_CHK_ERR(xf_input_port_init(&codec->input, size, align, core) == 0, XA_API_FATAL_MEM_ALLOC);

        /* ...save input port index */
        codec->in_idx = idx;

        /* ...set input buffer pointer as needed */
        if(size)
        {
            XA_API(base, XA_API_CMD_SET_MEM_PTR, idx, codec->input.buffer);
        }

        (size ? TRACE(INPUT, _x("set input ptr: %p"), codec->input.buffer) : 0);
        
        /* ...put input port into running state */
        xa_port_clear_flags(&codec->input.flags, XA_CODEC_INP_PORT_PAUSED);
    }
    else
    {
        /* ...output buffer specification */
        XF_CHK_ERR(type == XA_MEMTYPE_OUTPUT, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...initialize output port queue (no allocation here yet) */
        XF_CHK_ERR(xf_output_port_init(&codec->output, size) == 0, XA_API_FATAL_MEM_ALLOC);

        /* ...save output port index */
        codec->out_idx = idx;
        
        /* ...allocate this output buffer only for the codec initialization. This buffer will be freed when initialization is done */
        XF_CHK_ERR(codec->pinit_output = xf_mem_alloc(size, align, core, 0), XAF_MEMORY_ERR);

        codec->init_output_size = size;

        /* ...put output port into running state */
        xa_port_clear_flags(&codec->output.flags, XA_CODEC_OUT_PORT_PAUSED);
    }

    if (codec->probe_enabled)
    {
    	if (xf_input_port_created(&codec->input) && xf_output_port_created(&codec->output))
    	{
    		WORD32 probe_size;

            /*... returrn error if probe is enabled for input port with input bypass enabled */
            if((xf_input_port_bypass(&codec->input) && (XF_CHK_PORT_MASK(codec->probe_enabled, 0))))
            {
                TRACE(ERROR, _x("Probe buffer-length error on port[0] with input bypass"));
                return XAF_INVALIDVAL_ERR;
            }

            probe_size  = XF_CHK_PORT_MASK(codec->probe_enabled, 0) ? XF_ALIGNED_PROBE_SIZE(codec->input.length)  : 0;
            probe_size += XF_CHK_PORT_MASK(codec->probe_enabled, 1) ? XF_ALIGNED_PROBE_SIZE(codec->output.length) : 0;

    		/* ...initialize probe output port queue (no allocation here yet) */
    		XF_CHK_ERR(xf_output_port_init(&codec->probe, probe_size) == 0, XA_API_FATAL_MEM_ALLOC);

            /* ...put probe port into pause state */
            xa_port_set_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED);
    	}
    }

    return XA_NO_ERROR;
}

/* ...prepare input/output buffers */
static XA_ERRORCODE xa_codec_preprocess(XACodecBase *base)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;
    xaf_comp_type comp_type = base->comp_type;

    /* ...skip processing if I/O ports are paused */
    if (xa_port_test_flags(&codec->output.flags, XA_CODEC_OUT_PORT_PAUSED) || 
        xa_port_test_flags(&codec->input.flags, XA_CODEC_INP_PORT_PAUSED))
    {
        //return XA_CODEC_EXEC_PORT_PAUSED;
        return XA_CODEC_EXEC_NO_DATA;
    }

    /* ...prepare probe buffer if required */
    if (codec->probe_enabled && !xa_port_test_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED))
    {
    	if (!(base->state & XA_CODEC_FLAG_PROBE_SETUP))
    	{
    		void *probe_output;

            /* ...get probe buffer from port, if possible */
            if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
            {
                /* ...run-time is not initialized yet; probe buffer is not required */
            }
            else if ((probe_output = xf_output_port_data(&codec->probe)) == NULL)
            {
                /* ...no probe buffer available */
                return XA_CODEC_EXEC_NO_DATA;
            }
            else
            {
                /* ...record probe buffer for producing probe output */
                codec->probe_output = probe_output;

                /* ...mark probe output port is setup */
                base->state ^= XA_CODEC_FLAG_PROBE_SETUP;
            }
    	}
    }

    /* ...prepare output buffer if needed */
    if (!(base->state & XA_CODEC_FLAG_OUTPUT_SETUP))
    {
        void   *output;

        /* ...get output buffer from port, if possible */
        if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
        {
#ifndef INIT_CHG
            if (comp_type == XAF_ENCODER)
            {
                /* ...no actual data processing during encoder initialization. TENA_2198 */
                return XA_NO_ERROR;
            }
#endif
            /* ...run-time is not initialized yet; use temporary output buffer */
             output = (char *)codec->pinit_output;
        }
        else if ((output = xf_output_port_data(&codec->output)) == NULL)
        {
            /* ...no output buffer available */
            return XA_CODEC_EXEC_NO_DATA;
        }

        /* ...copy codec output buffer pointer for probe */
        codec->out_ptr = output;

        /* ...set the output buffer pointer */
        XA_API(base, XA_API_CMD_SET_MEM_PTR, codec->out_idx, output);

        TRACE(OUTPUT, _x("set output ptr: %p"), output);
        
        /* ...mark output port is setup */
        base->state ^= XA_CODEC_FLAG_OUTPUT_SETUP;
    }

#ifdef INIT_CHG
    if ((base->state & XA_BASE_FLAG_RUNTIME_INIT) && (comp_type != XAF_DECODER))
    {
        /* ...no actual data processing during initialization except for decoder */
        return XA_NO_ERROR;
    }
#endif

    /* ...prepare input data if needed */
    if (!(base->state & XA_CODEC_FLAG_INPUT_SETUP))
    {
        UWORD32 filled = 0;
        /* ...fill input buffer */
        if (xf_input_port_bypass(&codec->input))
        {
            void   *input;

            /* ...port is in bypass mode; try to update the buffer pointer, remaining bytes if necessary */
            xf_input_port_fill(&codec->input);
            
            /* ...use input buffer directly; check if there is data available */
            if ((input = xf_input_port_data(&codec->input)) != NULL)
            {
                /* ...set input data buffer pointer */
                XA_API(base, XA_API_CMD_SET_MEM_PTR, codec->in_idx, input);

                /* ...retrieve number of input bytes */
                filled = xf_input_port_length(&codec->input);
            }
            else if (!xf_input_port_done(&codec->input))
            {
                /* ...return non-fatal indication to prevent further processing, unless initialization is pending */
                if (!(base->state & XA_BASE_FLAG_RUNTIME_INIT))
                    return XA_CODEC_EXEC_NO_DATA;
            }
        }
        else
        {
            /* ...port is in non-bypass mode; try to fill internal buffer */
            if (xf_input_port_done(&codec->input) || xf_input_port_fill(&codec->input))
            {
                /* ...retrieve number of bytes in input buffer (not really - tbd) */
                filled = xf_input_port_level(&codec->input);
            }
            else
            {
                /* ...return non-fatal indication to prevent further processing, unless initialization is pending */
                if (!(base->state & XA_BASE_FLAG_RUNTIME_INIT))
                    return XA_CODEC_EXEC_NO_DATA;
                
                filled = xf_input_port_level(&codec->input);
            }
        }

        /* ...check if input stream is over */
        if (xf_input_port_done(&codec->input))
        {
            /* ...pass input-over command to the codec to indicate the final buffer */
            XA_API(base, XA_API_CMD_INPUT_OVER, codec->in_idx, NULL);

            TRACE(INFO, _b("codec[%p]: signal input-over (filled: %u)"), codec, filled);
        }

        TRACE(INPUT, _b("input-buffer fill-level: %u bytes"), filled);

        /* ...specify number of bytes available in the input buffer */
        XA_API(base, XA_API_CMD_SET_INPUT_BYTES, codec->in_idx, &filled);

        /* ...mark input port is setup */
        base->state ^= XA_CODEC_FLAG_INPUT_SETUP;
    }

    return XA_NO_ERROR;
}

/* ...post-processing operation; input/output ports maintenance */
static XA_ERRORCODE xa_codec_postprocess(XACodecBase *base, int done)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;
    WORD32          consumed = 0;
    WORD32          produced = 0;
    xaf_comp_type    comp_type = base->comp_type;
    UWORD32 probe_length = 0;
    void   *probe_outptr = codec->probe_output;

    /* ...get number of consumed / produced bytes */
    if(!xf_input_port_bypass(&codec->input) ||
        !(done && (base->state & XA_BASE_FLAG_EXECUTION) && (comp_type != XAF_DECODER)) /* ...skip for input_port_bypass if non-decoder type at init */
    )
    {
        XA_API(base, XA_API_CMD_GET_CURIDX_INPUT_BUF, codec->in_idx, &consumed);
    }

    /* ...get number of produced bytes only if runtime is initialized (sample size is known) */
    if(codec->sample_size)
    {
        XA_API(base, XA_API_CMD_GET_OUTPUT_BYTES, codec->out_idx, &produced);
    }

    TRACE(DECODE, _b("codec[%p]::postprocess(c=%u, p=%u, d=%u)"), codec, consumed, produced, done);

    /* ...produce probe output if required (and if runtime is initialized) */
    if (codec->sample_size && codec->probe_enabled && !xa_port_test_flags(&codec->probe.flags, XA_CODEC_PROBE_PORT_PAUSED))
    {
        if (XF_CHK_PORT_MASK(codec->probe_enabled, 0))
        {
            /* ...copy input port data onto probe port */
            probe_outptr = xf_copy_probe_data(probe_outptr, 0, consumed, codec->input.buffer);

            /* ...compute probe data length locally */
            probe_length += consumed;
        }

        if (XF_CHK_PORT_MASK(codec->probe_enabled, 1))
        {
            /* ...copy output port data onto probe port */
            probe_outptr = xf_copy_probe_data(probe_outptr, 1, produced, codec->out_ptr);
        
            /* ...compute probe data length locally */
            probe_length += produced;
        }
    }

    /* ...input buffer maintenance; check if we consumed anything */
    if (consumed)
    {
        if(comp_type == XAF_ENCODER)
        {
            codec->consumed += consumed / codec->sample_size;
        }
        /* ...consume specified number of bytes from input port */
        xf_input_port_consume(&codec->input, consumed);

        /* ...clear input-setup flag */
        base->state ^= XA_CODEC_FLAG_INPUT_SETUP;
    }

    /* ...output buffer maintenance; check if we have produced anything */
    if (produced)
    {
        /* ...increment total number of produced samples (really don't like division here - tbd) */
        codec->produced += produced / codec->sample_size;

        /* ...immediately complete output buffer (don't wait until it gets filled) */
        xf_output_port_produce(&codec->output, produced);

        /* ...clear output port setup flag */
        base->state ^= XA_CODEC_FLAG_OUTPUT_SETUP;
    }

    if (probe_length)
    {
        /* ...compute total probe output length (including length variables and alignment) */
        probe_length = probe_outptr - codec->probe_output;

        /* ...immediately complete probe output buffer (don't wait until it gets filled) */
        xf_output_port_produce(&codec->probe, probe_length);

        /* ...clear probe port setup flag */
        base->state ^= XA_CODEC_FLAG_PROBE_SETUP;
    }

    /* ...process execution stage transition */
    if (done)
    {
        if (base->state & XA_BASE_FLAG_RUNTIME_INIT)
        {
            /* ...stream is completed while codec is in runtime initialization stage */
            BUG(1, _x("breakpoint"));
        }
        else if (base->state & XA_BASE_FLAG_EXECUTION)
        {
            /* ...runtime initialization done */
            XA_CHK(xa_codec_prepare_runtime(codec));

            /* ...clear output port setup flag as we were using scratch buffer;
             * technically, no need to repeat setup of input buffer, but some codecs require
             * it as well
             */
            base->state &= ~(XA_CODEC_FLAG_INPUT_SETUP | XA_CODEC_FLAG_OUTPUT_SETUP);
        }
        else
        {
            /* ...output stream is over; propagate condition to sink port */
            if (xf_output_port_flush(&codec->output, XF_FILL_THIS_BUFFER))
            {
                /* ...flushing sequence is not needed; complete pending zero-length input */
                xf_input_port_purge(&codec->input);
                
                /* ...no propagation to output port */
                TRACE(INFO, _b("codec[%p] playback completed"), codec);
            }
            else
            {
                /* ...flushing sequence is started; wait until flow-control message returns */
                TRACE(INFO, _b("codec[%p] propagate end-of-stream condition"), codec);
            }

            /* ...output stream is over; flush probe port */
            if (codec->probe_enabled)
            {
            	if (!xf_output_port_flush(&codec->probe, XF_FLUSH))
            	{
            		BUG(1, _x("probe port: invalid state"));
            	}

            	TRACE(INFO, _b("probe port completed"));
            }

            if (!produced && !xf_output_port_routed(&codec->output))
            {
                /* ...mark EOS sequence done */
                base->state |= XA_CODEC_FLAG_EOS_SEQ_DONE;
            }
        }

        /* ...return early to prevent task rescheduling */
        return XA_NO_ERROR;
    }
    else if ((base->state & XA_BASE_FLAG_RUNTIME_INIT) && (consumed == 0))
    {
        /* ...clear input-setup flag */
        base->state &= ~(XA_CODEC_FLAG_INPUT_SETUP);

        /* ...send output buffer back if initialization was attempted without input */
        if ((codec->dec_init_without_inp) && !xf_input_port_level(&codec->input))
        {
            xf_message_t *msg = xf_msg_dequeue(&codec->output.queue);   

            /* ...complete message with 0 length to indicate initialization is not complete */
            xf_response_data(msg, 0);
        
            /* ...clear output port setup flag */
            base->state &= ~(XA_CODEC_FLAG_OUTPUT_SETUP);
        }
    }

    /* ...reschedule processing if needed */
    if (xf_input_port_ready(&codec->input) && xf_output_port_ready(&codec->output))
    {
        /* ...schedule data processing with respect to its urgency */
        if(comp_type == XAF_ENCODER)
        {
            xa_base_schedule(base, consumed * codec->factor);
        }
        else
        {
            xa_base_schedule(base, produced * codec->factor);
        }
    }

    return XA_NO_ERROR;
}

/* ...configuration parameter retrieval */
static XA_ERRORCODE xa_codec_getparam(XACodecBase *base, WORD32 id, pVOID value)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;

    if (id == XA_CODEC_CONFIG_PARAM_PRODUCED)
    {
        /* ...retrieve number of produced samples since last reset */
        *(UWORD32 *)value = codec->produced;

        return XA_NO_ERROR;
    }
    else
    {
        /* ...pass command to underlying codec plugin */
        return XA_API(base, XA_API_CMD_GET_CONFIG_PARAM, id, value);
    }
}

/* ...set configuration parameter */
static XA_ERRORCODE xa_codec_setparam(XACodecBase *base, WORD32 id, pVOID value)
{
    XAAudioCodec   *codec = (XAAudioCodec *) base;

    if (id == XAF_COMP_CONFIG_PARAM_PROBE_ENABLE)
    {
    	/* ... probe enabling allowed only before postinit */
    	XF_CHK_ERR((base->state & XA_BASE_FLAG_POSTINIT) == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    	codec->probe_enabled = *(WORD32 *) value;
        return XA_NO_ERROR;
    }
    else if (id == XAF_COMP_CONFIG_PARAM_DEC_INIT_WO_INP)
    {
        codec->dec_init_without_inp = *(UWORD32 *) value;
        return XA_NO_ERROR;
    }
    else
    {
        /* ...pass command to underlying codec plugin */
        return XA_API(base, XA_API_CMD_SET_CONFIG_PARAM, id, value);
    }
}

/*******************************************************************************
 * Component entry point
 ******************************************************************************/

/* ...command hooks */
static XA_ERRORCODE (* const xa_codec_cmd[])(XACodecBase *, xf_message_t *) =
{
    [XF_OPCODE_TYPE(XF_SET_PARAM)] = xa_base_set_param,
    [XF_OPCODE_TYPE(XF_GET_PARAM)] = xa_base_get_param,
    [XF_OPCODE_TYPE(XF_ROUTE)] = xa_codec_port_route,
    [XF_OPCODE_TYPE(XF_UNROUTE)] = xa_codec_port_unroute,
    [XF_OPCODE_TYPE(XF_EMPTY_THIS_BUFFER)] = xa_codec_empty_this_buffer,
    [XF_OPCODE_TYPE(XF_FILL_THIS_BUFFER)] = xa_codec_fill_this_buffer,
    [XF_OPCODE_TYPE(XF_FLUSH)] = xa_codec_flush,
    [XF_OPCODE_TYPE(XF_SET_PARAM_EXT)] = xa_base_set_param_ext,
    [XF_OPCODE_TYPE(XF_GET_PARAM_EXT)] = xa_base_get_param_ext,
    
    [XF_OPCODE_TYPE(XF_PAUSE)] = xa_codec_port_pause,
    [XF_OPCODE_TYPE(XF_RESUME)] = xa_codec_port_resume,
};

/* ...total number of commands supported */
#define XA_CODEC_CMD_NUM        (sizeof(xa_codec_cmd) / sizeof(xa_codec_cmd[0]))

/* ...command processor for termination state (only for routed port case) */
static int xa_audio_codec_terminate(xf_component_t *component, xf_message_t *m)
{
    XAAudioCodec   *codec = (XAAudioCodec *) component;

    if (m == NULL)
    {
        /* ...ignore component processing during component termination(rare case) */
        TRACE(OUTPUT, _b("component processing ignored.."));
        return 0;
    }
    /* ...check if we received output port control message */
    else if (m == xf_output_port_control_msg(&codec->output))
    {
        /* ...output port flushing complete; mark port is idle and terminate */
        xf_output_port_flush_done(&codec->output);
        TRACE(OUTPUT, _b("codec[%p] flush completed in terminate"), codec);
#ifdef XF_MSG_ERR_HANDLING
        return XAF_UNREGISTER;
#else
        return -1;
#endif
    }
    else if (m->opcode == XF_FILL_THIS_BUFFER)
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
            TRACE(OUTPUT, _b("codec[%p] response_failure in terminate"), codec);
        }
        return 0;
    }
}

/* ...audio codec destructor */
static int xa_audio_codec_destroy(xf_component_t *component, xf_message_t *m)
{
    XAAudioCodec   *codec = (XAAudioCodec *) component;
    UWORD32             core = xf_component_core(component);

    /* ...get the saved command message pointer before the component memory is freed */
    xf_message_t *m_resp = codec->m_response;

    /* ...destroy input port */
    xf_input_port_destroy(&codec->input, core);

    /* ...destroy output port */
    xf_output_port_destroy(&codec->output, core);

    if (codec->probe_enabled)
    {
    	xf_output_port_destroy(&codec->probe, core);
    }

    /* ...deallocate all resources */
    xa_base_destroy(&codec->base, XF_MM(sizeof(*codec)), core);

    if (m_resp != NULL)
    {
        /* ...complete the command with response */
        xf_response_err(m_resp);
    }

    TRACE(INIT, _b("audio-codec[%p@%u] destroyed"), codec, core);

    /* ...indicate the client has been destroyed */
    return 0;
}

/* ...audio codec destructor - first stage (ports unrouting) */
static int xa_audio_codec_cleanup(xf_component_t *component, xf_message_t *m)
{
    XAAudioCodec *codec = (XAAudioCodec *) component;

    /* ...cancel internal scheduling message if needed */
    xa_base_cancel(&codec->base);

    /* ...purge input port (returns OK? pretty strange at this point - tbd) */
    xf_input_port_purge(&codec->input);

    /* ...save command message to send response after flush completes */
    codec->m_response = m;

    if (codec->probe_enabled)
    {
    	if (!xf_output_port_flush(&codec->probe, XF_FLUSH))
    	{
    		BUG(1, _x("probe port: invalid state"));
    	}
    }

    /* ...propagate unregister command to connected component */
    if (xf_output_port_flush(&codec->output, XF_FLUSH))
    {
        /* ...flushing sequence is not needed; destroy audio codec */
        return xa_audio_codec_destroy(component, NULL);
    }
    else
    {
        /* ...wait until output port is cleaned; adjust component hooks */
        component->entry = xa_audio_codec_terminate;
        component->exit = xa_audio_codec_destroy;

        TRACE(INIT, _b("codec[%p] cleanup sequence started"), codec);

        /* ...indicate that second stage is required */
        return 1;
    }
}

/*******************************************************************************
 * Audio codec component factory
 ******************************************************************************/

xf_component_t * xa_audio_codec_factory(UWORD32 core, xa_codec_func_t process, xaf_comp_type comp_type)
{
    XAAudioCodec   *codec;

    /* ...allocate local memory for codec structure */
    XF_CHK_ERR(codec = (XAAudioCodec *) xa_base_factory(core, XF_MM(sizeof(*codec)), process), NULL);

    /* ...set base codec API methods */
    codec->base.memtab = xa_codec_memtab;
    codec->base.preprocess = xa_codec_preprocess;
    codec->base.postprocess = xa_codec_postprocess;
    codec->base.getparam = xa_codec_getparam;
    codec->base.setparam = xa_codec_setparam;

    /* ...set message commands processing table */
    codec->base.command = xa_codec_cmd;
    codec->base.command_num = XA_CODEC_CMD_NUM;

    /* ...set component destructor hook */
    codec->base.component.exit = xa_audio_codec_cleanup;

    /* ...set component type */
    codec->base.comp_type = comp_type;

    codec->dec_init_without_inp = 0;

    TRACE(INIT, _b("Codec[%p] initialized"), codec);

    return (xf_component_t *) codec;
}
