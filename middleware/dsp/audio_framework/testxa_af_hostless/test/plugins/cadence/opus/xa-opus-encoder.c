/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
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
 * xa-opus-encoder.c
 *
 * OPUS decoder plugin - thin wrapper around OPUS-ENC library
 ******************************************************************************/
#define MODULE_TAG                      OPUS_ENC

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdint.h>
#include <string.h>

#include "xa_type_def.h"
/* ...debugging facility */
#include "xf-debug.h"
#include "audio/xa-audio-decoder-api.h"
#include "audio/xa-opus-encoder-api.h"

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t enc_cycles;
#endif

typedef struct XA_OPUS_Encoder
{
	   /* ... module state */
	    UWORD32            state;

	    /* ...number of channels (channel mask?) */
	    UWORD32            channels;

	    /* ...PCM sample width */
	    UWORD32            pcm_width;

	    /* ...sampling rate */
	    UWORD32            sample_rate;

	    /* ...number of bytes in input/output buffer */
	    UWORD32            buffer_size;

	    /* ...input buffer */
	    void               *input;

	    /* ...output buffer */
	    void               *output;

	    /* ...scratch buffer pointer */
	    void               *scratch;

	    /* ...number of available bytes in the input buffer */
	    UWORD32            input_avail;

	    /* ...number of bytes consumed from input buffer */
	    UWORD32            consumed;

	    /* ...number of produced bytes */
	    UWORD32            produced;

	    /* ...control structure for opus encoder */
	    xa_opus_enc_control_t enc_control;

	    /* ...opus encoder frame size */
	    UWORD32 			frame_size;

	    /* ...opus encoder max frames per packet */
	    UWORD32 			max_frames_per_packet;

	    /* ...Opus enc handle */
	    UWORD32            internal_data __attribute__((aligned(8)));

}XA_OPUS_Encoder;


/*******************************************************************************
 * OPU-Enc state flags
 ******************************************************************************/

#define XA_OPUS_ENC_FLAG_PREINIT_DONE      (1 << 0)
#define XA_OPUS_ENC_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_OPUS_ENC_FLAG_RUNNING           (1 << 2)
#define XA_OPUS_ENC_FLAG_OUTPUT            (1 << 3)
#define XA_OPUS_ENC_FLAG_EOS_RECEIVED      (1 << 4)
#define XA_OPUS_ENC_FLAG_COMPLETE          (1 << 5)

#define MAX_INP_CHANNELS 2 //TBD: BHANU_TEST max channels, need to modify argument for xa_opus_enc_get_handle_byte_size
#define _MIN(a, b)		(((a)<(b))?(a):(b))

static XA_ERRORCODE xa_opus_encoder_get_api_size(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    *(WORD32 *)pv_value = xa_opus_enc_get_handle_byte_size(MAX_INP_CHANNELS);
    *(WORD32 *)pv_value += sizeof(XA_OPUS_Encoder);
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_init(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
        {
            memset(d, 0, sizeof(*d));
            
            /* ...init defaults */
            d->frame_size = 320;
            d->sample_rate = 16000;
            d->channels = 1;
            d->pcm_width = 16;
            d->max_frames_per_packet = 1;

            /* Mark Opus Enc component Pre-Initialization is DONE */
            d->state |= XA_OPUS_ENC_FLAG_PREINIT_DONE;
            return XA_NO_ERROR;
        }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
        {
            /* ...post-configuration initialization (all parameters are set) */
            XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

            /* Mark post-initialization is complete */
            d->state |= XA_OPUS_ENC_FLAG_POSTINIT_DONE;
            return XA_NO_ERROR;
        }

    case XA_CMD_TYPE_INIT_PROCESS:
        {
            XA_ERRORCODE err_code;
            /* ...kick run-time initialization process; make sure AMR dec component is setup */
            XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

            /* ...Call the core library initialization function */
            err_code = xa_opus_enc_init((xa_codec_handle_t)(&d->internal_data), &d->enc_control);

            XF_CHK_ERR(err_code == XA_NO_ERROR, XA_API_FATAL_INVALID_CMD_TYPE);

            /* ...enter into execution stage */
            d->state |= XA_OPUS_ENC_FLAG_RUNNING;

            return XA_NO_ERROR;
        }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
        {
            /* ...check if initialization is done; make sure pointer is sane */
            XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

            /* ...put current status */
            *(WORD32 *)pv_value = (d->state & XA_OPUS_ENC_FLAG_RUNNING ? 1 : 0);
            return XA_NO_ERROR;
        }

    default:
        /* ...unrecognised command type */
        TRACE(ERROR, _x("Unrecognised command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

static XA_ERRORCODE xa_opus_encoder_set_config_param(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case XA_CODEC_CONFIG_PARAM_CHANNELS:
    case XA_OPUS_ENC_CONFIG_PARAM_CHANNELS:
        d->enc_control.API_numChannels = *(WORD32 *)pv_value;
        d->channels		= *(WORD32 *)pv_value;
        break;

    case XA_CODEC_CONFIG_PARAM_SAMPLE_RATE:
    case XA_OPUS_ENC_CONFIG_PARAM_SAMPLE_RATE:
        d->enc_control.API_sampleRate = *(WORD32 *)pv_value;
        d->sample_rate	= *(WORD32 *)pv_value;
        break;

    case XA_CODEC_CONFIG_PARAM_PCM_WIDTH:
    case XA_OPUS_ENC_CONFIG_PARAM_PCM_WIDTH:
        d->pcm_width	= *(WORD32 *)pv_value;
        break;

    case XA_OPUS_ENC_CONFIG_PARAM_APPLICATION:
        d->enc_control.application = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_BITRATE:
    	d->enc_control.bitRate = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_CBR:
    	d->enc_control.cbr = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_CVBR:
    	d->enc_control.cvbr = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_BW:
    	d->enc_control.bandwidth = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_MAX_BW:
    	d->enc_control.max_bandwidth = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_MAX_PAYLOAD:
    	d->enc_control.max_payload = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_COMPLEXITY:
    	d->enc_control.complexity = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_SILK_INBANDFEC_ENABLE:
    	d->enc_control.SILK_InBandFEC_enabled = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_FORCE_NUMCHANNELS:
    	d->enc_control.force_numChannels = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_SILK_DTX_ENABLE:
    	d->enc_control.SILK_DTX_enabled = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_PACKET_LOSS_PERCENTAGE:
    	d->enc_control.packetLossPercentage = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_FORCE_MODE:
    	d->enc_control.force_mode = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_FRAME_SIZE: /* deprecated */
    case XA_OPUS_ENC_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
    	d->frame_size = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_MAX_FRAMES_PER_PACKET:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 6) && (*(WORD32 *)pv_value > 0), XA_OPUS_ENC_CONFIG_NONFATAL_RANGE);
    	d->max_frames_per_packet = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_SIGNAL_TYPE:
    	d->enc_control.signal_type = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_ENC_CONFIG_PARAM_RESET_STATE:
    	d->enc_control.reset_state = *(WORD32 *)pv_value;
    	break;

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_config_param(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    pUWORD32    pui_value    = (pUWORD32) pv_value;
    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* Since for AMR-WB these config parameters fixed we are using the hard coded values */
    switch (i_idx)
    {
    case XA_CODEC_CONFIG_PARAM_PCM_WIDTH:
    case XA_OPUS_ENC_CONFIG_PARAM_PCM_WIDTH:
        /* ...return sample bit-width */
        *pui_value = d->pcm_width;
        break;

    case XA_CODEC_CONFIG_PARAM_SAMPLE_RATE:
    case XA_OPUS_ENC_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return output sampling frequency */
        *pui_value = d->sample_rate;
        break;

    case XA_CODEC_CONFIG_PARAM_CHANNELS:
    case XA_OPUS_ENC_CONFIG_PARAM_CHANNELS:
        /* ...return number of output channels */
        *pui_value = d->channels;
        break;

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_execute(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
    {
    	WORD16 out_samples = 0;
        WORD32 frame_bytes = d->channels * d->frame_size * (d->pcm_width/8);

    	d->produced = 0;
    	d->consumed = 0; 
#if 1
        if(d->input_avail < frame_bytes)
        {
            if(d->state & XA_OPUS_ENC_FLAG_EOS_RECEIVED)
            {
              memset( (char*)d->input + d->input_avail, 0, (frame_bytes - d->input_avail));
            }
            else
            {
              return XA_OPUS_EXECUTE_NONFATAL_INSUFFICIENT_DATA;
            }
        }
#endif
    	xa_opus_enc((xa_codec_handle_t)(&d->internal_data), (pWORD16)d->input, (pUWORD8)d->output, d->frame_size, &d->enc_control, &out_samples, (pVOID)d->scratch);

    	d->produced = out_samples;
    	d->consumed = _MIN(frame_bytes, d->input_avail);

        d->state |= XA_OPUS_ENC_FLAG_OUTPUT;
        if ((d->input_avail == d->consumed) && (d->state & XA_OPUS_ENC_FLAG_EOS_RECEIVED)) /* Signal done */
        {
             d->state |= XA_OPUS_ENC_FLAG_COMPLETE;
             d->state &= ~XA_OPUS_ENC_FLAG_EOS_RECEIVED; /* TENA-2544 */
        }
        return XA_NO_ERROR;
    }
    case XA_CMD_TYPE_DONE_QUERY:
        *(WORD32 *)pv_value = (d->state & XA_OPUS_ENC_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        /* ...reset opus_enc component operation */
        return xa_opus_enc_init((xa_codec_handle_t)(&d->internal_data), &d->enc_control);

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

static XA_ERRORCODE xa_opus_encoder_set_input_bytes(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;

    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_OPUS_ENC_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_avail = size;

    /* ... reset exec-done state of the plugin to enable processing input. TENA-2544 */
    d->state &= ~XA_OPUS_ENC_FLAG_COMPLETE;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_output_bytes(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = d->produced;
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_curidx_input_buf(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return number of bytes consumed */
    *(WORD32 *)pv_value = d->consumed;
    d->consumed = 0;
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_input_over(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...put end-of-stream flag */
    d->state |= XA_OPUS_ENC_FLAG_EOS_RECEIVED;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_memtabs_size(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check AMR dec component is pre-initialized */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have all our tables inside API handle - good? tbd */
    *(WORD32 *)pv_value = 0;
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_set_memtabs_ptr(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check AMR dec component is pre-initialized */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_n_memtabs(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer, 1 output buffer and 1 scratch buffer*/
    *(WORD32 *)pv_value = 3;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_mem_info_size(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
#if 0
    	*(WORD32 *)pv_value = XA_OPUS_MAX_BYTES_CHANNEL_PACKET*(d->channels);
#else
        /* ...input buffer size according to the frame_size  */
    	*(WORD32 *)pv_value = d->frame_size * sizeof(WORD16) * d->max_frames_per_packet * (d->channels);
#endif
    	return XA_NO_ERROR;

    case 1:
        /* ...output buffer */
        *(WORD32 *)pv_value = (XA_OPUS_MAX_BYTES_ENCODED_PACKET);
        return XA_NO_ERROR;

    case 2:
        /* ...scratch buffer */
        *(WORD32 *)pv_value = xa_opus_enc_get_scratch_byte_size();
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_mem_info_alignment(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 8-bytes aligned */
    *(WORD32 *)pv_value = 8;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_encoder_get_mem_info_type(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
        *(WORD32 *)pv_value = XA_MEMTYPE_INPUT;
        return XA_NO_ERROR;

    case 1:
        /* ...output buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_OUTPUT;
        return XA_NO_ERROR;

    case 2:
        /* ...scratch buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_SCRATCH;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

static XA_ERRORCODE xa_opus_encoder_set_mem_ptr(XA_OPUS_Encoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_ENC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...select memory buffer */
    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
        d->input = pv_value;
        return XA_NO_ERROR;

    case 1:
        /* ...output buffer */
        d->output = pv_value;
        return XA_NO_ERROR;

    case 2:
        /* ...scratch buffer */
        d->scratch = pv_value;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }

    return XA_NO_ERROR;
}

static XA_ERRORCODE (* const xa_opus_encoder_api[])(XA_OPUS_Encoder *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE]           = xa_opus_encoder_get_api_size,

    [XA_API_CMD_INIT]                   = xa_opus_encoder_init,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_opus_encoder_set_config_param,

    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_opus_encoder_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_opus_encoder_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_opus_encoder_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_opus_encoder_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_opus_encoder_get_curidx_input_buf,
    [XA_API_CMD_INPUT_OVER]             = xa_opus_encoder_input_over,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_opus_encoder_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_opus_encoder_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_opus_encoder_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_opus_encoder_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_opus_encoder_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_opus_encoder_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_opus_encoder_set_mem_ptr,
};

/* ...total numer of commands supported */
#define XA_OPUS_ENC_API_COMMANDS_NUM   (sizeof(xa_opus_encoder_api) / sizeof(xa_opus_encoder_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_opus_encoder(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif
    XA_OPUS_Encoder    *d = (XA_OPUS_Encoder *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_OPUS_ENC_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_opus_encoder_api[i_cmd], XA_API_FATAL_INVALID_CMD);

#ifdef XAF_PROFILE
    comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
    
    /* ...execute requested command */
    ret = xa_opus_encoder_api[i_cmd](d, i_idx, pv_value);

#ifdef XAF_PROFILE
    comp_stop = clk_read_stop(CLK_SELN_THREAD);
    enc_cycles += clk_diff(comp_stop, comp_start);
#endif
    
    return ret;
}
