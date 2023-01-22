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
 * xa-mixer.c
 *
 * Sample mixer plugin
 ******************************************************************************/

#define MODULE_TAG                      MIXER

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdint.h>
#include <string.h>

/* ...debugging facility */
#include "xf-debug.h"
#include "audio/xa-mixer-api.h"
#include "xaf-api.h"


#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t mix_cycles;
#endif

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

/* ...API structure */
typedef struct XAPcmMixer
{
    /* ...mixer state */
    UWORD32                 state;
    
    /* ...number of samples in a frame */
    UWORD32                 frame_size;
    
    /* ...number of channels (channel mask?) */
    UWORD32                 channels;
    
    /* ...PCM sample width */
    UWORD32                 pcm_width;

    /* ...sampling rate */
    UWORD32                 sample_rate;

    /* ...number of bytes in input/output buffer */
    UWORD32                 buffer_size;
    
    /* ...master volume and individual track volumes*/
    UWORD32                 volume[XA_MIXER_MAX_TRACK_NUMBER + 1];
    
    /* ...input buffers */
    void               *input[XA_MIXER_MAX_TRACK_NUMBER];
    
    /* ...number of samples in individual buffers */
    UWORD32                 input_length[XA_MIXER_MAX_TRACK_NUMBER];

    /* ...output buffer */
    void               *output;

    /* ...number of produced bytes - do I need that? have buffer-size already - tbd */
    UWORD32                 produced;
    
    /* ...scratch buffer pointer */
    void               *scratch;
    
    /* ...input over flag */
    UWORD32             input_over[XA_MIXER_MAX_TRACK_NUMBER];

     /*...helper for large input buffer consume tracking */
    UWORD32     consumed[XA_MIXER_MAX_TRACK_NUMBER];

    /* ...input port bypass flag: 0 disabled (default), 1 enabled */
    UWORD32     inport_bypass;

}   XAPcmMixer;

/*******************************************************************************
 * Mixer state flags
 ******************************************************************************/

#define XA_MIXER_FLAG_PREINIT_DONE      (1 << 0)
#define XA_MIXER_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_MIXER_FLAG_RUNNING           (1 << 2)
#define XA_MIXER_FLAG_OUTPUT            (1 << 3)
#define XA_MIXER_FLAG_COMPLETE          (1 << 4)

/*******************************************************************************
 * Macro definitions
 ******************************************************************************/
/* mixer frame-size in samples/channel */
#define MIXER_FRAME_SIZE_MAX    4096
#define MIXER_FRAME_SIZE_MIN      32

/*******************************************************************************
 * DSP functions
 ******************************************************************************/

#define DSP_SATURATE_S16(WORD32)   \
    (WORD16)((WORD32) > 0x7fff ? 0x7fff : ((WORD32) < -0x8000 ? -0x8000 : (WORD32)))

/* ...mixer preinitialization (default parameters) */
static inline void xa_mixer_preinit(XAPcmMixer *d)
{
    UWORD32     i;
    
    /* ...pre-configuration initialization; reset internal data */
    memset(d, 0, sizeof(*d));
        
    /* ...set default parameters */
    d->pcm_width = 16, d->channels = 2, d->frame_size = 512;

    /* ...set default volumes (last index is a master volume)*/
    for (i = 0; i <= XA_MIXER_MAX_TRACK_NUMBER; i++)
    {
        d->volume[i] = ((1 << 12) << 16) | (1 << 12);
    }
#ifdef XA_INPORT_BYPASS_TEST
    /* ...enabled at init for testing. To be enabled by set-config to the plugin. */
    d->inport_bypass = 1;
#endif

}

/* ...do mixing of stereo PCM-16 streams */
static XA_ERRORCODE xa_mixer_do_execute_stereo_16bit(XAPcmMixer *d)
{
    WORD16  *output = d->output;
    WORD16  *b[XA_MIXER_MAX_TRACK_NUMBER]; 
    UWORD16 v_l[XA_MIXER_MAX_TRACK_NUMBER];
    UWORD16 v_r[XA_MIXER_MAX_TRACK_NUMBER];
    UWORD16 w_l, w_r;    
    UWORD32 t32;
    UWORD32 i, j;
    UWORD32 ports_inactive;
    UWORD32 ports_completed;
    UWORD32 frame_size, inlen, sample_size, nout_remaining;

    /* ...retrieve master volume - assume up to 24dB amplifying (4 bits) */
    t32 = d->volume[XA_MIXER_MAX_TRACK_NUMBER];
    w_l = (UWORD16)(t32 & 0xFFFF), w_r = (UWORD16)(t32 >> 16);

    /* ...reset produced bytes */
    d->produced = 0;
    
    for (j = 0, ports_inactive = 0, ports_completed = 0; j < XA_MIXER_MAX_TRACK_NUMBER; j++)
    {
        /* ...check if we have input buffer available */
        if (d->input_length[j] == 0)
        {
            ports_inactive += (d->input_over[j]);

            /* ...set complete flag saying we have no active input port */
            if(ports_inactive == XA_MIXER_MAX_TRACK_NUMBER)
            {
                d->state |= XA_MIXER_FLAG_COMPLETE;
                d->state &= ~XA_MIXER_FLAG_OUTPUT;

                TRACE(PROCESS, _b("exec-done"));

                return XA_NO_ERROR;
            }
        }
        /* if any port is active, break */
        ports_completed += (d->input_over[j]);
    }
    TRACE(PROCESS, _b("ports_inactive:%u completed:%d"), ports_inactive, ports_completed);
        
    /* ...copy of input-bytes, decremented on consumption over iterations */
    memset(d->consumed, 0, sizeof(d->consumed));
    nout_remaining = (d->buffer_size - d->produced);
    sample_size = ((d->pcm_width == 16) ? sizeof(WORD16) : sizeof(WORD32)) * d->channels;

    do
    {
        WORD32 inports_active = XA_MIXER_MAX_TRACK_NUMBER;

        /* ...process all tracks, iterate only on unequal samples at input ports */
        inlen = d->buffer_size;
        
        /* ...prepare individual tracks */
        for (j = 0; j < XA_MIXER_MAX_TRACK_NUMBER; j++)
        {
            UWORD32 n = d->input_length[j];
 
            /* ...check if we have input buffer available */
            if (n == 0)
            {
                /* ...output silence (multiply garbage in the scratch buffer by 0) */
                b[j] = d->scratch;
                v_l[j] = v_r[j] = 0;
                inports_active--;
            }
            else
            {
                /* ...assign input buffer pointer. */
                XF_CHK_ERR(b[j] = d->input[j], XA_MIXER_EXEC_FATAL_INPUT);

                /* ...set individual track volume/balance */
                t32 = d->volume[j];
                v_l[j] = (UWORD16)(t32 & 0xFFFF), v_r[j] = (UWORD16)(t32 >> 16);

                /* ...advance the input pointer by sample-offset, required for unequal length input */
                b[j] += (d->consumed[j] / ((d->pcm_width == 16) ? sizeof(WORD16) : sizeof(WORD32)));

                /* ...input_size: bytes to process is the minimum of of all input port bytes */
                inlen = (inlen < n) ? inlen : n;
                /* ...output_size: bytes to process is at-most output port bytes */
                inlen = (inlen < nout_remaining) ? inlen : nout_remaining;
            }
        
            TRACE(PROCESS, _b("b[%u] = %p%s"), j, b[j], (n == 0 ? " - scratch" : ""));
        }

        inlen = (inports_active)?inlen:0;
        
        if(inlen)
        {
            frame_size = inlen / sample_size;

            for (i=0; i < frame_size; i++)
            {
                WORD32     l32 = 0, r32 = 0;
            
                /* ...fill-in every channel in our map (unrolls loop here) */
                for (j = 0; j < XA_MIXER_MAX_TRACK_NUMBER; j++)
                {
                    /* ...left channel processing (no saturation here yet) */
                    l32 += *b[j]++ * v_l[j];
            
                    /* ...right channel processing */
                    r32 += *b[j]++ * v_r[j];
                }
            
                /* ...normalize (truncate towards -inf) and multiply by master volume */
                l32 = ((l32 >> 12) * w_l) >> 12;
                r32 = ((r32 >> 12) * w_r) >> 12;
            
                /* ...saturate and store in buffer */
                *output++ = DSP_SATURATE_S16(l32);
                *output++ = DSP_SATURATE_S16(r32);
            }

            for (j = 0; j < XA_MIXER_MAX_TRACK_NUMBER; j++)
            {
                if(d->input_length[j])
                {
                    d->input_length[j] -= frame_size * sample_size;
                    d->consumed[j] += frame_size * sample_size;

                    /* ...partial samples should be left unprocessed */
                    if((WORD32)d->input_length[j] < sample_size)
                    {
                        /* ...update partial bytes as consumed */
                        d->consumed[j] += d->input_length[j];
                        d->input_length[j] = 0;
                    }
                }
            }
            d->produced += frame_size * sample_size;
            nout_remaining = (d->buffer_size - d->produced);
        }//if(inlen)
    } while(inlen && nout_remaining);

    /* ...memset rest of the output buffer */
    memset(output, 0, d->buffer_size - d->produced);

    d->produced = d->buffer_size;

    /* ...put flag saying we have output buffer */
    d->state |= XA_MIXER_FLAG_OUTPUT;
 
    TRACE(PROCESS, _b("produced: %u bytes (%u samples)"), d->produced, d->frame_size);
    
    /* ...set complete flag saying we have consumed all available input and input is over */
    if(ports_completed == XA_MIXER_MAX_TRACK_NUMBER)
    {
        d->state |= XA_MIXER_FLAG_COMPLETE;

        TRACE(PROCESS, _b("exec-done"));
    }

    /* ...return success result code */
    return XA_NO_ERROR;
}

/* ...runtime reset */
static XA_ERRORCODE xa_mixer_do_runtime_init(XAPcmMixer *d)
{
    d->state = XA_MIXER_FLAG_RUNNING | XA_MIXER_FLAG_POSTINIT_DONE;

    /* ...no special processing is needed here */
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

/* ...codec API size query */
static XA_ERRORCODE xa_mixer_get_api_size(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);
    
    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_mixer_init(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - mixer must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
    {
        /* ...pre-configuration initialization; reset internal data */
        xa_mixer_preinit(d);

        /* ...and mark mixer has been created */
        d->state = XA_MIXER_FLAG_PREINIT_DONE;
        
        return XA_NO_ERROR;
    }
    
    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_MIXER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
        /* ...calculate input/output buffer size in bytes */
        d->buffer_size = d->channels * d->frame_size * (d->pcm_width == 16 ? sizeof(WORD16) : sizeof(WORD32));
        
        /* ...mark post-initialization is complete */
        d->state |= XA_MIXER_FLAG_POSTINIT_DONE;
        
        return XA_NO_ERROR;
    }
    
    case XA_CMD_TYPE_INIT_PROCESS:
    {
        /* ...kick run-time initialization process; make sure mixer is setup */
        XF_CHK_ERR(d->state & XA_MIXER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...enter into execution stage */
        d->state |= XA_MIXER_FLAG_RUNNING;
        
        return XA_NO_ERROR;
    }
    
    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        
        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_MIXER_FLAG_RUNNING ? 1 : 0);

        return XA_NO_ERROR;
    }
    
    default:
        /* ...unrecognized command type */
        TRACE(ERROR, _x("Unrecognized command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set mixer configuration parameter */
static XA_ERRORCODE xa_mixer_set_config_param(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;
    
    /* ...sanity check - mixer pointer must be sane */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...pre-initialization must be completed, mixer must be idle */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...get parameter value  */
    i_value = (UWORD32) *(WORD32 *)pv_value;
    
    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_MIXER_CONFIG_PARAM_PCM_WIDTH:
        /* ...check value is permitted (16 bits only) */
        XF_CHK_ERR(i_value == 16, XA_MIXER_CONFIG_NONFATAL_RANGE);
        d->pcm_width = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_MIXER_CONFIG_PARAM_CHANNELS:
        /* ...allow stereo only */
        XF_CHK_ERR(i_value == 2, XA_MIXER_CONFIG_NONFATAL_RANGE);
        d->channels = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_MIXER_CONFIG_PARAM_SAMPLE_RATE:      
         {
            /* ...set mixer sample rate */
            switch((UWORD32)i_value)
            {
                case 4000:
                case 8000:
                case 11025:
                case 12000:
                case 16000:
                case 22050:
                case 24000:
                case 32000:
                case 44100:
                case 48000:
                case 64000:
                case 88200:
                case 96000:
                case 128000:
                case 176400:
                case 192000:
                    d->sample_rate = (UWORD32)i_value;
                    break;
                default:
                    XF_CHK_ERR(0, XA_MIXER_CONFIG_NONFATAL_RANGE);
            }
            return XA_NO_ERROR;
        }
        
    case XA_MIXER_CONFIG_PARAM_FRAME_SIZE: /* ...deprecated */
    case XA_MIXER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
        XF_CHK_ERR(((i_value <= MIXER_FRAME_SIZE_MAX) && (i_value >= MIXER_FRAME_SIZE_MIN)), XA_MIXER_CONFIG_NONFATAL_RANGE);
        /* ...set frame length (in samples) */
        d->frame_size = *(WORD32 *)pv_value;
        return XA_NO_ERROR;

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_mixer_get_config_param(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - mixer must be initialized */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_MIXER_CONFIG_PARAM_INPUT_TRACKS:
        /* ...return maximal number of input tracks supported */
        *(WORD32 *)pv_value = XA_MIXER_MAX_TRACK_NUMBER;
        return XA_NO_ERROR;
        
    case XA_MIXER_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return mixer sample rate */
        *(WORD32 *)pv_value = d->sample_rate;
        return XA_NO_ERROR;
        
    case XA_MIXER_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_MIXER_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

    case XA_MIXER_CONFIG_PARAM_FRAME_SIZE: /* ...deprecated */
    case XA_MIXER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
        /* ...return current in/out frame length (in samples) */
        *(WORD32 *)pv_value = d->frame_size;
        return XA_NO_ERROR;

#ifdef XA_EXT_CONFIG_TEST
    case XA_MIXER_CONFIG_PARAM_BUFFER_SIZE:
    {
        /* ...return current in/out frame length (in bytes) */
        xaf_ext_buffer_t *ext_buf = (xaf_ext_buffer_t *) pv_value;
        memcpy(ext_buf->data, &d->buffer_size, sizeof(d->buffer_size));
        ext_buf->valid_data_size = sizeof(d->buffer_size);
        return XA_NO_ERROR;
    }
#else
    case XA_MIXER_CONFIG_PARAM_BUFFER_SIZE:
        /* ...return current in/out frame length (in bytes) */
        *(WORD32 *)pv_value = d->buffer_size;
        return XA_NO_ERROR;
#endif

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...execution command */
static XA_ERRORCODE xa_mixer_execute(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t mix_start, mix_stop;
#endif

    /* ...sanity check - mixer must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...mixer must be in running state */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        /* ...perform mixing of the channels */
#ifdef XAF_PROFILE
        mix_start = clk_read_start(CLK_SELN_THREAD);
#endif
        ret = xa_mixer_do_execute_stereo_16bit(d);
#ifdef XAF_PROFILE
        mix_stop = clk_read_stop(CLK_SELN_THREAD);
        mix_cycles += clk_diff(mix_stop, mix_start);
#endif
        return ret;
        
    case XA_CMD_TYPE_DONE_QUERY:
        /* ...check if processing is complete */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        *(WORD32 *)pv_value = (d->state & XA_MIXER_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;
        
    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        /* ...reset mixer operation */
        return xa_mixer_do_runtime_init(d);
        
    default:
        /* ...unrecognized command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_mixer_set_input_bytes(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;
    
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx >= 0 && i_idx < XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...mixer must be initialized */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...input buffer must exist */
    XF_CHK_ERR(d->input[i_idx], XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_MIXER_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_length[i_idx] = size;
    
    if(size >= d->buffer_size)
    {
        /* ...reset input_over flag only when input-buffer worth of data is available, else retain the earlier state. */
        d->input_over[i_idx] = 0;
    }

    if(size && d->inport_bypass)
    {
        /* ...input length should be multiple of buffer-size */
        XF_CHK_ERR(!(size % d->buffer_size), XA_MIXER_EXEC_NONFATAL_INPUT);
    }

    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_mixer_get_output_bytes(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be zero */
    XF_CHK_ERR(i_idx == XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...mixer must be running */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_MIXER_EXEC_NONFATAL_OUTPUT);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = (d->state & XA_MIXER_FLAG_OUTPUT ? d->produced : 0);

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_mixer_get_curidx_input_buf(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx >= 0 && i_idx < XA_MIXER_MAX_TRACK_NUMBER, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...mixer must be running */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_RUNNING, XA_MIXER_EXEC_FATAL_STATE);
    
    /* ...input buffer must exist */
    XF_CHK_ERR(d->input[i_idx], XA_MIXER_EXEC_FATAL_INPUT);

    /* ...return number of bytes consumed (always consume fixed-length chunk) */
    *(WORD32 *)pv_value = d->consumed[i_idx], d->consumed[i_idx] = 0;
    
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/

/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_mixer_get_memtabs_size(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check mixer is pre-initialized */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...we have all our tables inside API structure - good? tbd */
    *(WORD32 *)pv_value = 0;
    
    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_mixer_set_memtabs_ptr(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check mixer is pre-initialized */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_mixer_get_n_memtabs(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...we have N input buffers, 1 output buffer and 1 scratch buffer */
    *(WORD32 *)pv_value = XA_MIXER_MAX_TRACK_NUMBER + 1 + 1;
    
    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_mixer_get_mem_info_size(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    if(d->inport_bypass && (i_idx < XA_MIXER_MAX_TRACK_NUMBER))
    {
        /* ...input buffer length should be zero, if input-port bypass is enabled. */
        *(WORD32 *)pv_value = 0;
    }
    else
    {
        /* ...all buffers are of the same length */
        *(WORD32 *)pv_value = (WORD32) d->buffer_size;
    }

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_mixer_get_mem_info_alignment(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 4-bytes aligned */
    *(WORD32 *)pv_value = 4;
        
    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_mixer_get_mem_info_type(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0 ... XA_MIXER_MAX_TRACK_NUMBER - 1:
        /* ...input buffers */
        *(WORD32 *)pv_value = XA_MEMTYPE_INPUT;
        return XA_NO_ERROR;
        
    case XA_MIXER_MAX_TRACK_NUMBER:
        /* ...output buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_OUTPUT;
        return XA_NO_ERROR;
        
    case XA_MIXER_MAX_TRACK_NUMBER + 1:
        /* ...scratch buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_SCRATCH;
        return XA_NO_ERROR;
        
    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set memory pointer */
static XA_ERRORCODE xa_mixer_set_mem_ptr(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_MIXER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...select memory buffer */
    switch (i_idx)
    {
    case 0 ... XA_MIXER_MAX_TRACK_NUMBER - 1:
        /* ...input buffers */
        d->input[i_idx] = pv_value;
        return XA_NO_ERROR;
        
    case XA_MIXER_MAX_TRACK_NUMBER:
        /* ...output buffer */
        d->output = pv_value;
        return XA_NO_ERROR;
        
    case XA_MIXER_MAX_TRACK_NUMBER + 1:
        /* ...scratch buffer */
        d->scratch = pv_value;
        return XA_NO_ERROR;
        
    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set input over */
static XA_ERRORCODE xa_mixer_input_over(XAPcmMixer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    d->input_over[i_idx] = 1;

    return XA_NO_ERROR;
}

/*******************************************************************************
 * API command hooks
 ******************************************************************************/

static XA_ERRORCODE (* const xa_mixer_api[])(XAPcmMixer *, WORD32, pVOID) = 
{
    [XA_API_CMD_GET_API_SIZE]           = xa_mixer_get_api_size,

    [XA_API_CMD_INIT]                   = xa_mixer_init, 
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_mixer_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_mixer_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_mixer_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_mixer_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_mixer_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_mixer_get_curidx_input_buf,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_mixer_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_mixer_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_mixer_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_mixer_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_mixer_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_mixer_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_mixer_set_mem_ptr,
    [XA_API_CMD_INPUT_OVER]             = xa_mixer_input_over,
};

/* ...total numer of commands supported */
#define XA_MIXER_API_COMMANDS_NUM   (sizeof(xa_mixer_api) / sizeof(xa_mixer_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_mixer(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XAPcmMixer    *d = (XAPcmMixer *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_MIXER_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);
    
    /* ...see if command is defined */
    XF_CHK_ERR(xa_mixer_api[i_cmd], XA_API_FATAL_INVALID_CMD);
    
    /* ...execute requested command */
    return xa_mixer_api[i_cmd](d, i_idx, pv_value);
}
