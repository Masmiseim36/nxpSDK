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
 * xa-tflm-inference-api.c
 *
 * TFLM Inference API file (conforms to Cadence Audio Codec API)
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdint.h>
#include <string.h>

/* ...debugging facility */
#include "osal-timer.h"
#include "xf-debug.h"
#include "tflm-inference-api.h"
#include "microspeech/xa-microspeech-inference-api.h"


/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

/* ...API structure */
typedef struct XAInference
{
    /* ...Micro speech inference module state */
    UWORD32                 state;

    /* ...number of channels (channel mask?) */
    UWORD32                 channels;

    /* ...PCM sample width */
    UWORD32                 pcm_width;

    /* ...sampling rate */
    UWORD32                 sample_rate;

    /* ...input buffer */
    void               *input;

    /* ...output buffer */
    void               *output;

    /* ...scratch buffer pointer */
    void               *scratch;

    /* ...scratch buffer pointer */
    void               *persistent;

    /* ...sratch buffer size */
    UWORD32             scratch_size;

    /* ...persist buffer size */
    UWORD32             persist_size;

    /* ...number of available bytes in the input buffer */
    UWORD32                input_avail;

    /* ...number of bytes consumed from input buffer */
    UWORD32             consumed;

    /* ...number of produced bytes */
    UWORD32             produced;

    /* ...inference specification */
    xaf_tflm_inference_spec_t inf_spec;

}   XAInference;

/*******************************************************************************
 * TFLM inference state flags
 ******************************************************************************/

#define XA_INFERENCE_FLAG_PREINIT_DONE      (1 << 0)
#define XA_INFERENCE_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_INFERENCE_FLAG_RUNNING           (1 << 2)
#define XA_INFERENCE_FLAG_OUTPUT            (1 << 3)
#define XA_INFERENCE_FLAG_EOS_RECEIVED      (1 << 4)
#define XA_INFERENCE_FLAG_COMPLETE          (1 << 5)

/*******************************************************************************
 * DSP functions
 ******************************************************************************/

/* ...Micro speech inference component pre-initialization (default parameters) */
static inline void xa_inference_preinit(XAInference *d)
{
    /* ...pre-configuration initialization; reset internal data */
    memset(d, 0, sizeof(*d));

    /* ...set default parameters */
    d->channels = 1;
    d->pcm_width = 16;
    d->sample_rate = 48000;
}


/* ...apply gain to 16-bit PCM stream */
static XA_ERRORCODE xa_inference_do_execute(XAInference *d)
{
    WORD16   *pIn = (WORD16 *) d->input;
    WORD8    *pOut = (WORD8 *) d->output;

    /* ...check I/O buffer */
    XF_CHK_ERR(d->input, XA_INFERENCE_EXEC_FATAL_INPUT);
    XF_CHK_ERR(d->output, XA_INFERENCE_EXEC_FATAL_OUTPUT);

    /* ...Processing loop */
    d->consumed = 0;
    d->produced = 0;


    if ( d->input_avail >=  d->inf_spec.input_size)
    {
        void *output_tensor;
        int ret = inference_exec_process(pIn, d->inf_spec.input_size, pOut, (int *) &d->produced, &output_tensor, d->persistent);

        if ( ret != 0 )
        {
            return XA_FATAL_ERROR;
        }
        
        /* ...save total number of consumed bytes */
        d->consumed = d->inf_spec.input_stride;

        /* ...save total number of produced bytes */
        d->produced = d->inf_spec.output_size;
        
        if (d->inf_spec.inference_exec_postprocess)
        {
            void *p_op_resolver = (void *) ((unsigned int) d->persistent + inference_persistent_byte_size(d->inf_spec.tensor_arena_size));
            void *addl_persist = (void *) ((unsigned int) p_op_resolver + d->inf_spec.op_resolver_size);
            ret = (d->inf_spec.inference_exec_postprocess)(pOut, output_tensor, (int *) &d->produced, addl_persist);

            if ( ret != 0 )
            {
                return XA_FATAL_ERROR;
            }
        }
    }
    else if(d->state & XA_INFERENCE_FLAG_EOS_RECEIVED)
    {
        d->state |= XA_INFERENCE_FLAG_COMPLETE;
        d->state &= ~XA_INFERENCE_FLAG_EOS_RECEIVED;
    }

    /* ...put flag saying we have output buffer */
    d->state |= XA_INFERENCE_FLAG_OUTPUT;

    TRACE(PROCESS, _b("produced: %u bytes"), d->produced);

    /* ...return success result code */
    return XA_NO_ERROR;
}    

/* ...runtime reset */
static XA_ERRORCODE xa_inference_do_runtime_init(XAInference *d)
{
    /* ...no special processing is needed here */
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

/* ...codec API size query */
static XA_ERRORCODE xa_inference_get_api_size(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);

    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_inference_init(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - Micro speech inference component must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
    {
        /* ...pre-configuration initialization; reset internal data */
        xa_inference_preinit(d);

        /* ...and mark Micro speech inference component has been created */
        d->state = XA_INFERENCE_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        d->scratch_size = d->inf_spec.addl_scratch_size;
        
        d->persist_size  = inference_persistent_byte_size(d->inf_spec.tensor_arena_size);
        d->persist_size += d->inf_spec.op_resolver_size;
        d->persist_size += d->inf_spec.addl_persist_size;

        /* ...mark post-initialization is complete */
        d->state |= XA_INFERENCE_FLAG_POSTINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_PROCESS:
    {
        /* ...kick run-time initialization process; make sure Micro speech inference component is setup */
        XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        void *p_op_resolver = (void *) ((unsigned int) d->persistent + inference_persistent_byte_size(d->inf_spec.tensor_arena_size));
        void *addl_persist = (void *) ((unsigned int) d->persistent 
                                   + inference_persistent_byte_size(d->inf_spec.tensor_arena_size)
                                   + d->inf_spec.op_resolver_size);
        
        int ret = (d->inf_spec.inference_init_ops)(p_op_resolver, addl_persist);
            
        if ( ret != 0 )
            return XA_FATAL_ERROR;
    
        ret = inference_init(d->persistent, d->inf_spec.model, d->inf_spec.tensor_arena_size, p_op_resolver);
    
        if ( ret != 0 )
            return XA_FATAL_ERROR;

        /* ...enter into execution stage */
        d->state |= XA_INFERENCE_FLAG_RUNNING;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_INFERENCE_FLAG_RUNNING ? 1 : 0);

        return XA_NO_ERROR;
    }

    default:
        /* ...unrecognised command type */
        TRACE(ERROR, _x("Unrecognised command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set Micro speech inference component configuration parameter */
static XA_ERRORCODE xa_inference_set_config_param(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;

    /* ...sanity check - Micro speech inference component pointer must be sane */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...pre-initialization must be completed, Micro speech inference component must be idle */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...get parameter value  */
    i_value = (UWORD32) *(WORD32 *)pv_value;

    /* ...process individual configuration parameter */
    switch (i_idx & 0xF)
    {
    case XA_INFERENCE_CONFIG_PARAM_PCM_WIDTH:
        /* ...check value is permitted (16 bits only) */
        XF_CHK_ERR(i_value == 16, XA_INFERENCE_CONFIG_NONFATAL_RANGE);
        d->pcm_width = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_INFERENCE_CONFIG_PARAM_CHANNELS:
        /* ...allow mono/stereo only */
        XF_CHK_ERR(((i_value == 1)||(i_value == 2)), XA_INFERENCE_CONFIG_NONFATAL_RANGE);
        d->channels = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_INFERENCE_CONFIG_PARAM_SAMPLE_RATE:
    {
        /* ...set Micro speech inference component sample rate */
        switch ((UWORD32)i_value)
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
            XF_CHK_ERR(0, XA_INFERENCE_CONFIG_NONFATAL_RANGE);
        }
        return XA_NO_ERROR;
    }

    case XA_INFERENCE_CONFIG_PARAM_FRAME_SIZE:
        return XA_INFERENCE_CONFIG_NONFATAL_READONLY;

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_inference_get_config_param(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - Micro speech inference component must be initialized */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual configuration parameter */
    switch (i_idx & 0xF)
    {
    case XA_INFERENCE_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return sample rate */
        *(WORD32 *)pv_value = d->sample_rate;
        return XA_NO_ERROR;

    case XA_INFERENCE_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_INFERENCE_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

    case XA_INFERENCE_CONFIG_PARAM_PRODUCED:
        /* ...return no.of produced bytes */
        *(WORD32 *)pv_value = d->produced;
        return XA_NO_ERROR;

    case XA_INFERENCE_CONFIG_PARAM_FRAME_SIZE:
        /* ...return Micro speech inference component buffer size */
        *(WORD32 *)pv_value = d->inf_spec.input_size;
        return XA_NO_ERROR;

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...execution command */
static XA_ERRORCODE xa_inference_execute(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;

    /* ...sanity check - Micro speech inference component must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...Micro speech inference component must be in running state */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        ret = xa_inference_do_execute(d);
        return ret;

    case XA_CMD_TYPE_DONE_QUERY:
        /* ...check if processing is complete */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        *(WORD32 *)pv_value = (d->state & XA_INFERENCE_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        /* ...reset Micro speech inference component operation */
        return xa_inference_do_runtime_init(d);

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_inference_set_input_bytes(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;

    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...Micro speech inference component must be initialized */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_INFERENCE_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_avail = size;

    d->state &= ~XA_INFERENCE_FLAG_COMPLETE;

    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_inference_get_output_bytes(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be zero */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...Micro speech inference component must be running */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_INFERENCE_EXEC_NONFATAL_OUTPUT);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = ((d->state & XA_INFERENCE_FLAG_OUTPUT) ? d->produced : 0);

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_inference_get_curidx_input_buf(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...Micro speech inference component must be running */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_RUNNING, XA_INFERENCE_EXEC_FATAL_STATE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_INFERENCE_EXEC_FATAL_INPUT);

    /* ...return number of bytes consumed (always consume fixed-length chunk) */
    *(WORD32 *)pv_value = d->consumed;

    /* ...and reset internally managed number of consumed bytes */
    d->consumed = 0;

    return XA_NO_ERROR;
}

/* ...end-of-stream processing */
static XA_ERRORCODE xa_inference_input_over(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...put end-of-stream flag */
    d->state |= XA_INFERENCE_FLAG_EOS_RECEIVED;

    TRACE(PROCESS, _b("Input-over-condition signalled"));

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/
/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_inference_get_memtabs_size(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check Micro speech inference component is pre-initialized */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have all our tables inside API structure - good? tbd */
    *(WORD32 *)pv_value = 0;

    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_inference_set_memtabs_ptr(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check Micro speech inference component is pre-initialized */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_inference_get_n_memtabs(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer, 1 output buffer, 1 persist buffer and 1 scratch buffer*/
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_inference_get_mem_info_size(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    /* ...using intput, output and scratch buffers are of the same length */
    case 0: /* ...input buffers */
        *(WORD32 *)pv_value = (WORD32) d->inf_spec.input_size;
        return XA_NO_ERROR;
    case 1: /* ...output buffers */
        *(WORD32 *)pv_value = (WORD32) d->inf_spec.output_size;
        return XA_NO_ERROR;
    case 2: /* ...scratch buffers */
        *(WORD32 *)pv_value = (WORD32) d->scratch_size;
        return XA_NO_ERROR;
    case 3: /* ...persistent buffers */
        *(WORD32 *)pv_value = (WORD32) d->persist_size;
        return XA_NO_ERROR;
    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_inference_get_mem_info_alignment(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 4-bytes aligned */
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_inference_get_mem_info_type(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffer */
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

    case 3:
        /* ...persistent buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_PERSIST;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set memory pointer */
static XA_ERRORCODE xa_inference_set_mem_ptr(XAInference *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_INFERENCE_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

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

    case 3:
        /* ...persistent buffers */
        d->persistent = pv_value;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}


/*******************************************************************************
 * API command hooks
 ******************************************************************************/

static XA_ERRORCODE (* const xa_inference_api[])(XAInference *, WORD32, pVOID) = 
{
        [XA_API_CMD_GET_API_SIZE]           = xa_inference_get_api_size,

        [XA_API_CMD_INIT]                   = xa_inference_init,
        [XA_API_CMD_SET_CONFIG_PARAM]       = xa_inference_set_config_param,
        [XA_API_CMD_GET_CONFIG_PARAM]       = xa_inference_get_config_param,

        [XA_API_CMD_EXECUTE]                = xa_inference_execute,
        [XA_API_CMD_SET_INPUT_BYTES]        = xa_inference_set_input_bytes,
        [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_inference_get_output_bytes,
        [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_inference_get_curidx_input_buf,
        [XA_API_CMD_INPUT_OVER]             = xa_inference_input_over,

        [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_inference_get_memtabs_size,
        [XA_API_CMD_SET_MEMTABS_PTR]        = xa_inference_set_memtabs_ptr,
        [XA_API_CMD_GET_N_MEMTABS]          = xa_inference_get_n_memtabs,
        [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_inference_get_mem_info_size,
        [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_inference_get_mem_info_alignment,
        [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_inference_get_mem_info_type,
        [XA_API_CMD_SET_MEM_PTR]            = xa_inference_set_mem_ptr,
};

/* ...total number of commands supported */
#define XA_INFERENCE_API_COMMANDS_NUM   (sizeof(xa_inference_api) / sizeof(xa_inference_api[0]))

/*******************************************************************************
 * TFLM Inference Generic API entry point
 ******************************************************************************/

XA_ERRORCODE xa_tflm_inference_init_spec(xa_codec_handle_t p_xa_module_obj, 
                                           xaf_tflm_inference_spec_t *inference_spec)
{
    XAInference *d = (XAInference *) p_xa_module_obj;

    memcpy(&d->inf_spec, inference_spec, sizeof(xaf_tflm_inference_spec_t));

    return XA_NO_ERROR;
}

XA_ERRORCODE xa_tflm_inference_api(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XAInference *d = (XAInference *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_INFERENCE_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_inference_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_inference_api[i_cmd](d, i_idx, pv_value);
}

